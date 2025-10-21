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
    // Rebuild from other's vector (preserves order).
    // NOTE: to_vector() is TODO for students; copy_from is only used by
    // copy ctor / assignment in our tests after you implement to_vector().
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

// ------------------------
// STUDENT TODO FUNCTIONS
// ------------------------

// Subproblem 1: insert
void SortedDeque::insert(int x){
    // TODO: Insert x to keep non-decreasing order.
    // Placeholder so it compiles; this does NOT maintain sorted order.
    push_back_node(new Node(x));
}

// Subproblem 2: erase
bool SortedDeque::erase(int x){
    // TODO: Remove first occurrence of x in O(n). Return true if removed.
    (void)x;
    return false;
}

// Subproblem 3: merge
void SortedDeque::merge(const SortedDeque& other){
    // TODO: Merge *copy* of nodes from 'other' into *this* in O(n+m).
    // 'other' must remain unchanged afterwards.
    (void)other;
    // no-op
}

// Subproblem 4: kth_smallest (1-based)
int SortedDeque::kth_smallest(std::size_t k) const{
    // TODO: Throw std::out_of_range if k==0 or k>size().
    // Otherwise, return the k-th smallest (k-th node from head).
    (void)k;
    throw std::logic_error("kth_smallest: TODO");
}

// Subproblem 5: to_vector
std::vector<int> SortedDeque::to_vector() const{
    // TODO: return elements in order [front..back].
    return {};
}
