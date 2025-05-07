
#include "../common/load_data.hpp"
#include "../linkedlist_version/linked_list.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <sstream>
#include <cctype>

using namespace std;


// Q1 dependencies: Merge-Sort & Quick-Sort on LinkedList by date
//------------------------------------------------------------------------------



template<typename T>
typename LinkedList<T>::Node* split_list(typename LinkedList<T>::Node* head) {
    auto slow = head, fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    auto back = slow->next;
    slow->next = nullptr;
    return back;
}

template<typename T>
typename LinkedList<T>::Node* merge_sorted(typename LinkedList<T>::Node* a,
                                           typename LinkedList<T>::Node* b) {
    using Node = typename LinkedList<T>::Node;
    Node dummy(T{}), *tail = &dummy;
    while (a && b) {
        if (list_date_less(a->data.date, b->data.date)) {
            tail->next = a; a = a->next;
        } else {
            tail->next = b; b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

template<typename T>
typename LinkedList<T>::Node* merge_sort_list(typename LinkedList<T>::Node* head) {
    if (!head || !head->next) return head;
    auto back = split_list<T>(head);
    auto left  = merge_sort_list<T>(head);
    auto right = merge_sort_list<T>(back);
    return merge_sorted<T>(left, right);
}

template<typename T>
void linked_quick_sort(typename LinkedList<T>::Node*& head) {
    // three-way partition quicksort on singly list
    if (!head || !head->next) return;
    string pivot = head->data.date;
    typename LinkedList<T>::Node *lH=nullptr,*lT=nullptr,
                                 *eH=nullptr,*eT=nullptr,
                                 *gH=nullptr,*gT=nullptr;
    for (auto cur = head; cur; ) {
        auto nxt = cur->next;
        cur->next = nullptr;
        if (cur->data.date == pivot) {
            if (!eH) eH = eT = cur;
            else     { eT->next = cur; eT = cur; }
        }
        else if (list_date_less(cur->data.date, pivot)) {
            if (!lH) lH = lT = cur;
            else     { lT->next = cur; lT = cur; }
        } else {
            if (!gH) gH = gT = cur;
            else     { gT->next = cur; gT = cur; }
        }
        cur = nxt;
    }
    linked_quick_sort<T>(lH);
    linked_quick_sort<T>(gH);

    // concatenate lH->eH->gH
    typename LinkedList<T>::Node* newHead = nullptr;
    typename LinkedList<T>::Node* newTail = nullptr;
    if (lH) {
        newHead = lH;
        for (newTail = lH; newTail->next; newTail = newTail->next);
        newTail->next = eH;
    } else {
        newHead = eH;
        newTail = eT;
    }
    newTail->next = gH;
    head = newHead;
}


// Q2 dependencies: lower_bound/upper_bound on LinkedList by (category, payment)
//------------------------------------------------------------------------------

static bool key_ll_less(const Transaction& a, const Transaction& b) {
    if (a.category != b.category) return a.category < b.category;
    return a.payment_method < b.payment_method;
}

static LinkedList<Transaction>::Node* node_at(
    LinkedList<Transaction>::Node* head, size_t idx) {
    auto cur = head;
    for (size_t i = 0; i < idx && cur; ++i)
        cur = cur->next;
    return cur;
}

static size_t lower_bound_ll(
    LinkedList<Transaction>::Node* head, size_t N,
    const string& cat, const string& pay)
{
    size_t lo = 0, hi = N;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        auto* node = node_at(head, mid);
        const auto& t = node->data;
        if (t.category < cat ||
           (t.category == cat && t.payment_method < pay)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

static size_t upper_bound_ll(
    LinkedList<Transaction>::Node* head, size_t N,
    const string& cat, const string& pay)
{
    size_t lo = 0, hi = N;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        auto* node = node_at(head, mid);
        const auto& t = node->data;
        if (t.category < cat ||
           (t.category == cat && t.payment_method <= pay)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

static LinkedList<Transaction>::Node* merge_sorted_key(
    LinkedList<Transaction>::Node* a,
    LinkedList<Transaction>::Node* b)
{
    using Node = LinkedList<Transaction>::Node;
    Node dummy(Transaction{}), *tail = &dummy;
    while (a && b) {
        if (key_ll_less(a->data, b->data)) {
            tail->next = a; a = a->next;
        } else {
            tail->next = b; b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

static LinkedList<Transaction>::Node* merge_sort_list_key(
    LinkedList<Transaction>::Node* head)
{
    using Node = LinkedList<Transaction>::Node;
    if (!head || !head->next) return head;
    // split
    Node *slow = head, *fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next; // typo? Should be fast = fast->next->next;
    }
    Node* back = slow->next;
    slow->next = nullptr;
    // sort halves
    Node* left  = merge_sort_list_key(head);
    Node* right = merge_sort_list_key(back);
    return merge_sorted_key(left, right);
}

// Q3 dependencies: word-frequency on LinkedList<Review>
//------------------------------------------------------------------------------

struct WordCount {
    string word;
    size_t count;
};

static const char* STOP_WORDS[] = {
    "the","and","for","with","not","you","this","that","but","was","are",
    "have","has","had","they","from","your","all","too","its","it's",
    "very","would","took","expected","definitely", nullptr
};

static bool is_stop_word(const string& w) {
    for (const char** p = STOP_WORDS; *p; ++p)
        if (w == *p) return true;
    return false;
}

static string normalize(const string& s) {
    string out; out.reserve(s.size());
    for (char c : s) {
        if (isalnum((unsigned char)c))
            out += tolower((unsigned char)c);
        else
            out += ' ';
    }
    return out;
}

static void add_or_increment_ll(LinkedList<WordCount>& freq, const string& w) {
    for (auto node = freq.get_head(); node; node = node->next) {
        if (node->data.word == w) {
            ++node->data.count;
            return;
        }
    }
    freq.push_back(WordCount{w,1});
}

static LinkedList<WordCount>::Node* merge_sorted_wc(
    LinkedList<WordCount>::Node* a,
    LinkedList<WordCount>::Node* b)
{
    using Node = LinkedList<WordCount>::Node;
    Node dummy(WordCount{}), *tail = &dummy;
    while (a && b) {
        if (a->data.count > b->data.count) {
            tail->next = a; a = a->next;
        } else {
            tail->next = b; b = b->next;
        }
        tail = tail->next;
    }
    tail->next = a ? a : b;
    return dummy.next;
}

static LinkedList<WordCount>::Node* merge_sort_list_wc(
    LinkedList<WordCount>::Node* head)
{
    if (!head || !head->next) return head;
    auto back = split_list<WordCount>(head);
    auto left  = merge_sort_list_wc(head);
    auto right = merge_sort_list_wc(back);
    return merge_sorted_wc(left, right);
}

//------------------------------------------------------------------------------
// Q1, Q2, Q3 functions
//------------------------------------------------------------------------------

void list_q1(LinkedList<Transaction>& transactions) {
    size_t N = transactions.size();
    const string out_merge = "../data/transactions_sorted_ll_merge_v2.csv";
    const string out_quick = "../data/transactions_sorted_ll_quick_v2.csv";

    // Merge-Sort
    {
        // sort
        auto t0 = chrono::high_resolution_clock::now();
        transactions.set_head( merge_sort_list<Transaction>(transactions.get_head()) );
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        size_t mem = (size_t)ceil(log2(N)) * sizeof(void*);
        cout << "Q1 Merge-Sort time:   " << dt << " microseconds\n";
        cout << "Q1 Merge-Sort memory: ~" << mem << " bytes\n";

        // write CSV
        ofstream out(out_merge);
        out << "Customer ID,Product,Category,Price,Date,Payment Method\n";
        for (auto node = transactions.get_head(); node; node = node->next) {
            auto& t = node->data;
            out << t.customer_id << ',' << t.product << ','
                << t.category << ',' << t.price << ','
                << t.date << ',' << t.payment_method << '\n';
        }
        cout << "  Wrote Merge-Sorted CSV to " << out_merge << "\n\n";
    }

    // Quick-Sort
    {
        // copy
        LinkedList<Transaction> qlist;
        for (auto node = transactions.get_head(); node; node = node->next)
            qlist.push_back(node->data);

        // sort
        auto t0 = chrono::high_resolution_clock::now();
        qlist.quick_sort_by_date();
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        size_t mem = (size_t)ceil(log2(N)) * sizeof(void*);
        cout << "Q1 Quick-Sort time:   " << dt << " microseconds\n";
        cout << "Q1 Quick-Sort memory: ~" << mem << " bytes\n";

        // write CSV
        ofstream out(out_quick);
        out << "Customer ID,Product,Category,Price,Date,Payment Method\n";
        for (auto node = qlist.get_head(); node; node = node->next) {
            auto& t = node->data;
            out << t.customer_id << ',' << t.product << ','
                << t.category << ',' << t.price << ','
                << t.date << ',' << t.payment_method << '\n';
        }
        cout << "  Wrote Quick-Sorted CSV to " << out_quick << "\n\n";
    }
}

void list_q2(LinkedList<Transaction>& transactions) {
    size_t N = transactions.size();
    cout << "Q2: Electronics purchases via Credit Card\n\n";

    // a) Linear scan
    {
        auto t0 = chrono::high_resolution_clock::now();
        size_t count = 0;
        for (auto node = transactions.get_head(); node; node = node->next) {
            if (node->data.category == "Electronics" &&
                node->data.payment_method == "Credit Card") {
                ++count;
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        double pct = N ? (100.0 * count / N) : 0.0;
        cout << "  Linear scan count:      " << count << "/" << N << "\n";
        cout << "  Linear scan percentage: " << pct << "%\n";
        cout << "  Linear scan time:       " << dt << " microseconds\n";
        cout << "  Linear scan memory:     0 bytes\n\n";
    }

    // b) Binary-search
    {
        // build key list
        LinkedList<Transaction> key_list;
        for (auto node = transactions.get_head(); node; node = node->next)
            key_list.push_back(node->data);

        auto t0 = chrono::high_resolution_clock::now();
        key_list.set_head(merge_sort_list_key(key_list.get_head()));
        auto t1 = chrono::high_resolution_clock::now();
        auto dt_sort = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();


        auto bs0 = chrono::high_resolution_clock::now();
        size_t lo = lower_bound_ll(key_list.get_head(), N, "Electronics", "Credit Card");
        size_t hi = upper_bound_ll(key_list.get_head(), N, "Electronics", "Credit Card");
        auto bs1 = chrono::high_resolution_clock::now();
        auto dt_bs = chrono::duration_cast<chrono::microseconds>(bs1 - bs0).count();

        size_t count = (hi >= lo ? hi - lo : 0);
        double pct = N ? (100.0 * count / N) : 0.0;
        size_t mem = (size_t)ceil(log2(N)) * sizeof(void*);

        cout << "  Binary Search count:      " << count << "/" << N << "\n";
        cout << "  Binary Search percentage: " << pct << "%\n";
        cout << "  Sort time:                " << dt_sort << " microseconds\n";
        cout << "  Search time:              " << dt_bs   << " microseconds\n";
        cout << "  Sort memory (stack):      ~" << mem     << " bytes\n\n";
    }
}

void list_q3(LinkedList<Review>& reviews) {
    cout << "Q3: Top 10 words in 1-star reviews\n\n";

    auto t0 = chrono::high_resolution_clock::now();
    LinkedList<WordCount> freq;
    size_t totalOneStar = 0;
    for (auto node = reviews.get_head(); node; node = node->next) {
        if (node->data.rating != 1) continue;
        ++totalOneStar;
        string norm = normalize(node->data.review_text);
        istringstream iss(norm);
        string token;
        while (iss >> token) {
            if (token.size() < 3)    continue;
            if (is_stop_word(token)) continue;
            add_or_increment_ll(freq, token);
        }
    }
    cout << "Total 1-star reviews scanned: " << totalOneStar << "\n";

    auto head_wc = merge_sort_list_wc(freq.get_head());
    freq.set_head(head_wc);
    auto t1 = chrono::high_resolution_clock::now();
    auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

    size_t M = freq.size();
    size_t mem = M * (sizeof(WordCount) + sizeof(void*));
    size_t topN = M < 10 ? M : 10;

    cout << "  Time taken:   " << dt  << " microseconds\n";
    cout << "  Memory used:  " << mem << " bytes\n";
    cout << "  Top " << topN << " words:\n";
    size_t cnt = 0;
    for (auto node = freq.get_head(); node && cnt < topN; node = node->next, ++cnt) {
        cout << "    " << node->data.word
             << " (count=" << node->data.count << ")\n";
    }
    cout << endl;
}


// run_list_mode(): submenu
//------------------------------------------------------------------------------

int run_list_mode() {
    const string txn_clean = "../data/transactions_cleaned_v2.csv";
    const string rev_clean = "../data/reviews_cleaned_v2.csv";

    LinkedList<Transaction> transactions;
    if (!load_transactions_ll(txn_clean, transactions)) {
        cerr << "Error loading transactions\n";
        return 1;
    }
    LinkedList<Review> reviews;
    if (!load_reviews_ll(rev_clean, reviews)) {
        cerr << "Error loading reviews\n";
        return 1;
    }

    while (true) {
        cout << "\n-- Linked-List Mode --\n"
             << "1) Q1: Sort transactions by date\n"
             << "2) Q2: % Electronics via Credit Card\n"
             << "3) Q3: Top-10 words in 1-star reviews\n"
             << "4) Back to main menu\n"
             << "Choose (1-4): ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return 0;
        }
        switch (choice) {
            case 1: list_q1(transactions);    break;
            case 2: list_q2(transactions);    break;
            case 3: list_q3(reviews);         break;
            case 4: return 0;
            default: cout << "Please select 1–4.\n";
        }
    }
}
