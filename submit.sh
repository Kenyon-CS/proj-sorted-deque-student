#!/usr/bin/env bash
set -euo pipefail

# Load .env if present
if [ -f .env ]; then
  set -a
  . ./.env
  set +a
fi

: "${PROJECT_NAME:=unknown-project}"
: "${SUBMIT_URL:?Set SUBMIT_URL in .env or environment}"
SUBMIT_KEY="${SUBMIT_KEY:-}"
EXPECTED_TOTAL="${EXPECTED_TOTAL:-}"   # e.g., 4 (optional but recommended)

# Ensure test binary exists
if [ ! -x bin/tests ]; then
  echo "No test binary at bin/tests; did you run 'make'?" >&2
  exit 2
fi

echo "Running tests..."
tmp_out="$(mktemp -t submit_tests.XXXXXX)"
set +e
bin/tests >"$tmp_out" 2>&1
rc=$?
set -e

# Count PASS/FAIL lines
passed=$(grep -cE '^[[:space:]]*PASS:' "$tmp_out" || true)
failed=$(grep -cE '^[[:space:]]*FAIL:' "$tmp_out" || true)

# Derive total
if [ -n "$EXPECTED_TOTAL" ]; then
  total="$EXPECTED_TOTAL"
else
  total=$((passed + failed))
  # Fallback if framework doesn't print FAIL lines and nothing failed explicitly:
  if [ "$total" -eq 0 ]; then
    # Support "ALL TESTS PASSED" banner as 1/1
    if grep -q "ALL TESTS PASSED" "$tmp_out"; then
      passed=1; total=1
    fi
  fi
fi

# Determine status & message
status=""
msg=""
if [ "$rc" -ne 0 ]; then
  # crashed or returned non-zero
  if [ -n "$EXPECTED_TOTAL" ] && [ "$passed" -lt "$EXPECTED_TOTAL" ]; then
    status="crashed"
    msg="Program crashed. Some tests passed (${passed}/${EXPECTED_TOTAL})."
  else
    status="crashed"
    msg="Program crashed. Some tests passed (${passed}/${total})."
  fi
else
  # exit code 0
  if [ -n "$EXPECTED_TOTAL" ]; then
    if [ "$passed" -eq "$EXPECTED_TOTAL" ]; then
      status="passed"
      msg="All tests passed (${passed}/${EXPECTED_TOTAL})."
    elif [ "$passed" -gt 0 ]; then
      status="partial"
      msg="Some tests passed (${passed}/${EXPECTED_TOTAL}). Submitting as partial credit."
    else
      status="failed"
      msg="No tests passed (0/${EXPECTED_TOTAL})."
    fi
  else
    # no expected total; use observed
    total_observed=$((passed + failed))
    if [ "$failed" -eq 0 ] && [ "$passed" -gt 0 ]; then
      status="passed"
      msg="All observed tests passed (${passed}/${total_observed})."
    elif [ "$passed" -gt 0 ]; then
      status="partial"
      msg="Some tests passed (${passed}/${total_observed}). Submitting as partial credit."
    else
      status="failed"
      msg="No tests passed (0/${total_observed})."
    fi
  fi
fi

# If EXPECTED_TOTAL set, prefer that in the result display
display_total="${EXPECTED_TOTAL:-$((passed + failed))}"
result="${passed}/${display_total}"

echo "$msg"
echo "Posting result: status=$status, result=$result"

# Collect metadata
name="$(git config --get user.name || echo "")"
email="$(git config --get user.email || echo "")"
commit="$(git rev-parse --short HEAD || echo "")"
repo_url="$(git config --get remote.origin.url || echo "")"
host="$(hostname || echo "")"
ts="$(date -u +"%Y-%m-%dT%H:%M:%SZ")"

# Include only the tail of output (useful for debugging without spamming)
tail_out="$(tail -n 40 "$tmp_out" | sed 's/\"/\\\"/g')"

read -r -d '' payload <<JSON || true
{
  "project": "${PROJECT_NAME}",
  "status": "${status}",
  "result": "${result}",
  "tests_passed": ${passed},
  "tests_total": ${display_total},
  "user_name": "${name}",
  "user_email": "${email}",
  "commit": "${commit}",
  "repo_url": "${repo_url}",
  "host": "${host}",
  "timestamp": "${ts}",
  "key": "${SUBMIT_KEY}",
  "output_tail": "${tail_out}"
}
JSON

# POST first; fallback to GET (?data=) for Apps Script redirects
if ! curl -sS -L --post301 --post302 \
  -H "Content-Type: application/json" \
  -X POST "$SUBMIT_URL" \
  --data "$payload" >/dev/null; then
  curl -sS --get "$SUBMIT_URL" --data-urlencode "data=${payload}" >/dev/null
fi

rm -f "$tmp_out"
echo "Submitted ✅ (${result}; status=${status})"
