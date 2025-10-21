#include "SortedDeque.h"
#include <cassert>

SortedDeque::SortedDeque(): head(nullptr), tail(nullptr), sz(0) {}
SortedDeque::~SortedDeque(){ clear(); }

SortedDeque::SortedDeque(const SortedDeque& other): head(nullptr), tail(nullptr), sz(0) {
    copy_from(other);
}

SortedDeque& SortedDeque::operator=(const SortedDeque& other){
    if(this != &other){
        clear();
        copy_from(other);
    }
    return *this;
}

void SortedDeque::clear(){
    Node* cur = head;
    while(cur){
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = tail = nullptr;
    sz = 0;
}

void SortedDeque::copy_from(const SortedDeque& other){
    // Rebuild from other's vector (preserves order)
    for(int x: other.to_vector()){
        push_back_node(new Node(x));
    }
}

void SortedDeque::insert_before(Node* pos, Node* nd){
    if(!pos){ // insert at end
        push_back_node(nd);
        return;
    }
    nd->prev = pos->prev;
    nd->next = pos;
    if(pos->prev) pos->prev->next = nd; else head = nd;
    pos->prev = nd;
    ++sz;
}

void SortedDeque::insert_after(Node* pos, Node* nd){
    if(!pos){ // insert at front
        push_front_node(nd);
        return;
    }
    nd->next = pos->next;
    nd->prev = pos;
    if(pos->next) pos->next->prev = nd; else tail = nd;
    pos->next = nd;
    ++sz;
}

void SortedDeque::push_back_node(Node* nd){
    nd->prev = tail;
    nd->next = nullptr;
    if(tail) tail->next = nd; else head = nd;
    tail = nd;
    ++sz;
}

void SortedDeque::push_front_node(Node* nd){
    nd->next = head;
    nd->prev = nullptr;
    if(head) head->prev = nd; else tail = nd;
    head = nd;
    ++sz;
}

void SortedDeque::unlink(Node* nd){
    if(nd->prev) nd->prev->next = nd->next; else head = nd->next;
    if(nd->next) nd->next->prev = nd->prev; else tail = nd->prev;
    --sz;
}

int SortedDeque::front() const{
    if(empty()) throw std::out_of_range("front on empty");
    return head->val;
}

int SortedDeque::back() const{
    if(empty()) throw std::out_of_range("back on empty");
    return tail->val;
}

// ---- Subproblem 1: insert(int x) ----
void SortedDeque::insert(int x){
    // TODO: insert x in non-decreasing order
    // Strategy: find first node with val > x and insert_before.
    Node* nd = new Node(x);
    Node* cur = head;
    while(cur && cur->val <= x){
        cur = cur->next;
    }
    // cur is first > x (or null to append at end)
    if(cur){
        insert_before(cur, nd);
    } else {
        push_back_node(nd);
    }
}

// ---- Subproblem 2: erase(int x) ----
bool SortedDeque::erase(int x){
    // TODO: remove first occurrence of x if present
    Node* cur = head;
    while(cur){
        if(cur->val == x){
            unlink(cur);
            delete cur;
            return true;
        }
        if(cur->val > x) break; // early exit since sorted
        cur = cur->next;
    }
    return false;
}

// ---- Subproblem 3: merge(const SortedDeque& other) ----
void SortedDeque::merge(const SortedDeque& other){
    // TODO: O(n+m) merge (copy nodes from 'other' so other remains unchanged)
    Node* a = head;
    Node* b = other.head; // read-only walk; copy values

    SortedDeque result;
    while(a && b){
        if(a->val <= b->val){
            result.push_back_node(new Node(a->val));
            a = a->next;
        } else {
            result.push_back_node(new Node(b->val));
            b = b->next;
        }
    }
    while(a){
        result.push_back_node(new Node(a->val));
        a = a->next;
    }
    while(b){
        result.push_back_node(new Node(b->val));
        b = b->next;
    }
    // swap result into *this
    clear();
    copy_from(result);
}

// ---- Subproblem 4: kth_smallest(size_t k) const ----
int SortedDeque::kth_smallest(std::size_t k) const{
    if(k == 0 || k > sz) throw std::out_of_range("k out of range");
    Node* cur = head;
    while(--k){
        cur = cur->next;
    }
    return cur->val;
}

// ---- Subproblem 5: to_vector() const ----
std::vector<int> SortedDeque::to_vector() const{
    std::vector<int> out;
    out.reserve(sz);
    for(Node* cur=head; cur; cur=cur->next){
        out.push_back(cur->val);
    }
    return out;
}
