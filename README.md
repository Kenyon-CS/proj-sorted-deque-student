# proj-sorted-deque (STUDENT VERSION)

You will implement a **sorted double-ended queue** backed by a doubly-linked list.
There are **5 subproblems**, each graded with **5+ tests**. The current code **does not** contain solutions.

Implement the following in `src/SortedDeque.cpp` where marked **TODO**:
1. `insert(int x)` — insert while keeping **non-decreasing** order.
2. `erase(int x)` — remove **first** occurrence, return `true` if removed.
3. `merge(const SortedDeque& other)` — destructive O(n+m) merge *without modifying `other`*.
4. `kth_smallest(size_t k) const` — 1-based; throw `std::out_of_range` if invalid.
5. `to_vector() const` — return all elements in order.

### Ground rules
- Use the provided doubly-linked `Node` (no `std::list`, `std::deque`, etc.).
- You may use `<vector>` **only** to produce the output in `to_vector()`.
- You can use the provided private helpers (`insert_before`, `unlink`, etc.).

### Commands
```bash
make
make test
make submit   # prints one-line JSON; posts to SUBMIT_URL if set
```

Good luck!
# proj-sorted-deque-student
