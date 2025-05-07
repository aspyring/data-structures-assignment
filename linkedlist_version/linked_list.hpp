#pragma once
#include <cstddef>
#include <string>
using namespace std;

// forward-declare list_date_less
static bool list_date_less(const string& a, const string& b) {
    int dA = stoi(a.substr(0,2)), mA = stoi(a.substr(3,2)), yA = stoi(a.substr(6,4));
    int dB = stoi(b.substr(0,2)), mB = stoi(b.substr(3,2)), yB = stoi(b.substr(6,4));
    if (yA != yB) return yA < yB;
    if (mA != mB) return mA < mB;
    return dA < dB;
}

template<typename T>
class LinkedList {
public:

    struct Node {
        T      data;
        Node*  next;
        Node(const T& d): data(d), next(nullptr) {}
    };

private:
    Node*   head;
    Node*   tail;
    size_t  length;

public:
    LinkedList(): head(nullptr), tail(nullptr), length(0) {}
    ~LinkedList() {
        Node* cur = head;
        while (cur) {
            Node* nx = cur->next;
            delete cur;
            cur = nx;
        }
    }

    void push_back(const T& item) {
        Node* n = new Node(item);
        if (!head) head = tail = n;
        else {
            tail->next = n;
            tail = n;
        }
        ++length;
    }

    void pop_back() {
        if (!head) return;
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            Node* cur = head;
            while (cur->next != tail) {
                cur = cur->next;
            }
            delete tail;
            tail = cur;
            tail->next = nullptr;
        }
        if (length > 0) --length;
    }


    size_t size() const { return length; }
    Node*  get_head() const { return head; }
    void set_head(Node* new_head) {
        head = new_head;
        tail = nullptr;
        length = 0;
        for (Node* cur = head; cur; cur = cur->next) {
            tail = cur;
            ++length;
        }
    }
    Node*  get_tail() const { return tail; }
    void set_tail(Node* new_tail) {
        tail = new_tail;
        head = nullptr;
        length = 0;
        for (Node* cur = tail; cur; cur = cur->next) {
            head = cur;
        }
    }
    // --- Q1: Merge Sort by date ---
    void sort_by_date() {
        head = merge_sort_list(head);
        // recompute tail & length
        tail = nullptr; length = 0;
        for (Node* p = head; p; p = p->next) {
            tail = p;
            ++length;
        }
    }

    // Q1 Quick-Sort  by date
    void quick_sort_by_date() {
        head = quick_sort_list(head);
        // recompute tail & length
        tail = nullptr; length = 0;
        for (Node* p = head; p; p = p->next) {
            tail = p;
            ++length;
        }
    }

private:
    // Partition around pivot into <, =, > lists, then recurse+concat
    static Node* quick_sort_list(Node* start) {
        if (!start || !start->next) return start;

        // Choose pivot = start’s date
        string pivot = start->data.date;

        // Dummy heads/tails for <, =, >
        Node *lH=nullptr,*lT=nullptr,
             *eH=nullptr,*eT=nullptr,
             *gH=nullptr,*gT=nullptr;

        // Partition
        for (Node* cur = start; cur; ) {
            Node* nxt = cur->next;
            cur->next = nullptr;
            if (cur->data.date == pivot) {
                if (!eH) eH = eT = cur;
                else      { eT->next = cur; eT = cur; }
            }
            else if (list_date_less(cur->data.date, pivot)) {
                if (!lH) lH = lT = cur;
                else     { lT->next = cur; lT = cur; }
            }
            else {
                if (!gH) gH = gT = cur;
                else     { gT->next = cur; gT = cur; }
            }
            cur = nxt;
        }

        // Recursively sort < and >
        lH = quick_sort_list(lH);
        gH = quick_sort_list(gH);

        // Concatenate: lH -> eH -> gH
        Node* newHead = nullptr;
        Node* newTail = nullptr;

        // attach lH
        if (lH) {
            newHead = lH;
            for (newTail = lH; newTail->next; newTail = newTail->next);
            newTail->next = eH;
        } else {
            newHead = eH;
            newTail = eT;
        }
        // attach gH
        if (gH) {
            if (!newHead) newHead = gH;
            newTail->next = gH;
            for (newTail = gH; newTail->next; newTail = newTail->next);
        }

        return newHead;
    }


private:
    // split into two halves
    static Node* split_list(Node* start) {
        if (!start) return nullptr;
        Node *slow = start, *fast = start->next;
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        Node* back = slow->next;
        slow->next = nullptr;
        return back;
    }

    // merge two sorted lists
    static Node* merge_sorted(Node* a, Node* b) {
        Node dummy(T{}), *tail = &dummy;
        while (a && b) {
            if (list_date_less(a->data.date, b->data.date)) {
                tail->next = a; a = a->next;
            } else {
                tail->next = b; b = b->next;
            }
            tail = tail->next;
        }
        tail->next = (a ? a : b);
        return dummy.next;
    }

    // recursive merge sort
    static Node* merge_sort_list(Node* start) {
        if (!start || !start->next) return start;
        Node* back = split_list(start);
        Node* left = merge_sort_list(start);
        Node* right= merge_sort_list(back);
        return merge_sorted(left, right);
    }
};
