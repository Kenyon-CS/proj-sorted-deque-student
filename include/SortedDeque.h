#pragma once
#include <vector>
#include <stdexcept>

class SortedDeque {
public:
    SortedDeque();
    ~SortedDeque();
    SortedDeque(const SortedDeque& other);
    SortedDeque& operator=(const SortedDeque& other);

    // ---- Subproblem 1 ----
    void insert(int x); // TODO

    // ---- Subproblem 2 ----
    bool erase(int x);  // TODO

    // ---- Subproblem 3 ----
    void merge(const SortedDeque& other); // TODO

    // ---- Subproblem 4 ----
    int kth_smallest(std::size_t k) const; // TODO

    // ---- Subproblem 5 ----
    std::vector<int> to_vector() const; // TODO

    // Provided utilities
    std::size_t size() const { return sz; }
    bool empty() const { return sz == 0; }
    int front() const;
    int back() const;

private:
    struct Node {
        int val;
        Node* prev;
        Node* next;
        Node(int v): val(v), prev(nullptr), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    std::size_t sz;

    // helpers
    void clear();
    void copy_from(const SortedDeque& other);

    // link helpers
    void insert_before(Node* pos, Node* nd);
    void insert_after(Node* pos, Node* nd);
    void push_back_node(Node* nd);
    void push_front_node(Node* nd);
    void unlink(Node* nd);
};
