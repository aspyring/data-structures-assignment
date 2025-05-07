#include "../common/load_data.hpp"
#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <sstream>
#include <cctype>

using namespace std;


// Q1 dependencies: Quick- & Merge-Sort by date (DD/MM/YYYY)
//------------------------------------------------------------------------------

static bool date_less(const string& a, const string& b) {
    int dA = stoi(a.substr(0,2)), mA = stoi(a.substr(3,2)), yA = stoi(a.substr(6,4));
    int dB = stoi(b.substr(0,2)), mB = stoi(b.substr(3,2)), yB = stoi(b.substr(6,4));
    if (yA != yB) return yA < yB;
    if (mA != mB) return mA < mB;
    return dA < dB;
}

static void swap_txn(DynamicArray<Transaction>& A, int i, int j) {
    Transaction tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

static void quick_sort_rec(DynamicArray<Transaction>& A, int left, int right) {
    int i = left, j = right;
    string pivot = A[(left + right) / 2].date;
    while (i <= j) {
        while (date_less(A[i].date, pivot)) ++i;
        while (date_less(pivot, A[j].date)) --j;
        if (i <= j) {
            swap_txn(A, i, j);
            ++i; --j;
        }
    }
    if (left < j)  quick_sort_rec(A, left,  j);
    if (i < right) quick_sort_rec(A, i,      right);
}

static void sort_transactions_quick(DynamicArray<Transaction>& A) {
    int n = (int)A.size();
    if (n > 1) quick_sort_rec(A, 0, n - 1);
}

static void merge_txn(DynamicArray<Transaction>& A, int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    Transaction* L = new Transaction[n1];
    Transaction* R = new Transaction[n2];
    for (int i = 0; i < n1; ++i) L[i] = A[l + i];
    for (int j = 0; j < n2; ++j) R[j] = A[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (date_less(L[i].date, R[j].date)) A[k++] = L[i++];
        else                                 A[k++] = R[j++];
    }
    while (i < n1) A[k++] = L[i++];
    while (j < n2) A[k++] = R[j++];
    delete[] L;
    delete[] R;
}

static void merge_sort_rec(DynamicArray<Transaction>& A, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    merge_sort_rec(A, l,     m);
    merge_sort_rec(A, m + 1, r);
    merge_txn(A, l, m, r);
}

static void sort_transactions_merge(DynamicArray<Transaction>& A) {
    int n = (int)A.size();
    if (n > 1) merge_sort_rec(A, 0, n - 1);
}


// Q2 dependencies: Binary-Search on (category, payment_method)
//------------------------------------------------------------------------------

static bool key_less(const Transaction& a, const Transaction& b) {
    if (a.category != b.category) return a.category < b.category;
    return a.payment_method < b.payment_method;
}

static void quick_sort_key_rec(DynamicArray<Transaction>& A, int left, int right) {
    int i = left, j = right;
    Transaction pivot = A[(left + right) / 2];
    while (i <= j) {
        while (key_less(A[i], pivot)) ++i;
        while (key_less(pivot, A[j])) --j;
        if (i <= j) {
            swap_txn(A, i, j);
            ++i; --j;
        }
    }
    if (left < j)  quick_sort_key_rec(A, left,  j);
    if (i < right) quick_sort_key_rec(A, i,      right);
}

static void sort_transactions_by_category_payment(DynamicArray<Transaction>& A) {
    int n = (int)A.size();
    if (n > 1) quick_sort_key_rec(A, 0, n - 1);
}

static size_t lower_bound_key(const DynamicArray<Transaction>& A,
                              const string& cat,
                              const string& pay) {
    size_t lo = 0, hi = A.size();
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        const auto& t = A[mid];
        if (t.category < cat ||
           (t.category == cat && t.payment_method < pay)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}

static size_t upper_bound_key(const DynamicArray<Transaction>& A,
                              const string& cat,
                              const string& pay) {
    size_t lo = 0, hi = A.size();
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        const auto& t = A[mid];
        if (t.category < cat ||
           (t.category == cat && t.payment_method <= pay)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo;
}


// Q3 dependencies: One-star review word-frequency
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
        if (isalnum(static_cast<unsigned char>(c)))
            out += tolower(static_cast<unsigned char>(c));
        else
            out += ' ';
    }
    return out;
}

static void add_or_increment(DynamicArray<WordCount>& freq, const string& w) {
    for (size_t i = 0; i < freq.size(); ++i) {
        if (freq[i].word == w) {
            ++freq[i].count;
            return;
        }
    }
    freq.push(WordCount{w,1});
}

static void swap_wc(DynamicArray<WordCount>& A, int i, int j) {
    WordCount tmp = A[i];
    A[i] = A[j];
    A[j] = tmp;
}

static void quick_sort_wc(DynamicArray<WordCount>& A, int left, int right) {
    int i = left, j = right;
    size_t pivot = A[(left + right)/2].count;
    while (i <= j) {
        while (A[i].count > pivot)  ++i;
        while (A[j].count < pivot)  --j;
        if (i <= j) {
            swap_wc(A,i,j);
            ++i; --j;
        }
    }
    if (left < j)  quick_sort_wc(A, left,  j);
    if (i < right) quick_sort_wc(A, i,      right);
}

static void sort_wordcount_desc(DynamicArray<WordCount>& A) {
    if (A.size() > 1) quick_sort_wc(A, 0, (int)A.size()-1);
}


// Q1, Q2, Q3 functions
//------------------------------------------------------------------------------

void array_q1(const DynamicArray<Transaction>& transactions) {
    size_t N = transactions.size();
    const string out_quick = "../data/transactions_sorted_quick_v2A.csv";
    const string out_merge = "../data/transactions_sorted_merge_v2A.csv";

    // prepare copies
    DynamicArray<Transaction> quick_arr, merge_arr;
    for (size_t i = 0; i < N; ++i) {
        quick_arr.push(transactions[i]);
        merge_arr.push(transactions[i]);
    }

    // Quick-Sort
    {
        auto t0 = chrono::high_resolution_clock::now();
        sort_transactions_quick(quick_arr);
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        size_t mem = (size_t)ceil(log2(N)) * sizeof(void*);
        cout << "Q1 Quick-Sort time:   " << dt << " microseconds\n";
        cout << "Q1 Quick-Sort memory: ~" << mem << " bytes (stack)\n";
        ofstream out(out_quick);
        out << "Customer ID,Product,Category,Price,Date,Payment Method\n";
        for (size_t i = 0; i < quick_arr.size(); ++i) {
            auto& t = quick_arr[i];
            out << t.customer_id << ',' << t.product << ',' << t.category << ','
                << t.price << ',' << t.date << ',' << t.payment_method << '\n';
        }
        cout << "  Wrote Quick-Sorted CSV to " << out_quick << "\n\n";
    }

    // Merge-Sort
    {
        auto t0 = chrono::high_resolution_clock::now();
        sort_transactions_merge(merge_arr);
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        size_t mem = N * sizeof(Transaction);
        cout << "Q1 Merge-Sort time:   " << dt << " microseconds\n";
        cout << "Q1 Merge-Sort memory: " << mem << " bytes (temp arrays)\n";
        ofstream out(out_merge);
        out << "Customer ID,Product,Category,Price,Date,Payment Method\n";
        for (size_t i = 0; i < merge_arr.size(); ++i) {
            auto& t = merge_arr[i];
            out << t.customer_id << ',' << t.product << ',' << t.category << ','
                << t.price << ',' << t.date << ',' << t.payment_method << '\n';
        }
        cout << "  Wrote Merge-Sorted CSV to " << out_merge << "\n\n";
    }
}

void array_q2(const DynamicArray<Transaction>& transactions,
              const DynamicArray<Review>&      reviews) {
    size_t N = transactions.size();
    cout << "Q2: Electronics purchases with Credit Card\n\n";

    // a) Linear search
    {
        auto t0 = chrono::high_resolution_clock::now();
        size_t count = 0;
        for (size_t i = 0; i < N; ++i) {
            auto& t = transactions[i];
            if (t.category == "Electronics" &&
                t.payment_method == "Credit Card") {
                ++count;
            }
        }
        auto t1 = chrono::high_resolution_clock::now();
        auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
        double pct = N ? (100.0 * count / N) : 0.0;
        cout << "  Linear search count:      " << count << "/" << N << "\n";
        cout << "  Linear search percentage: " << pct << "%\n";
        cout << "  Linear search time:       " << dt << " microseconds\n";
        cout << "  Linear search memory:     0 bytes\n\n";
    }

    // b) Binary-search approach
    {
        // prepare key array
        DynamicArray<Transaction> key_arr;
        for (size_t i = 0; i < transactions.size(); ++i)
            key_arr.push(transactions[i]);

        auto t0 = chrono::high_resolution_clock::now();
        sort_transactions_by_category_payment(key_arr);
        auto t1 = chrono::high_resolution_clock::now();
        auto dt_sort = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();

        auto bs0 = chrono::high_resolution_clock::now();
        size_t lo = lower_bound_key(key_arr, "Electronics", "Credit Card");
        size_t hi = upper_bound_key(key_arr, "Electronics", "Credit Card");
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

void array_q3(const DynamicArray<Review>& reviews) {
    cout << "Q3: Top 10 words in 1-star reviews\n";

    // 1) Count total 1-star reviews
    size_t totalOneStar = 0;
    for (size_t i = 0; i < reviews.size(); ++i) {
        if (reviews[i].rating == 1)
            ++totalOneStar;
    }
    cout << "Total 1-star reviews scanned: " << totalOneStar << "\n";

    // 2) Build frequency table for only those 1-star reviews
    auto t0 = chrono::high_resolution_clock::now();
    DynamicArray<WordCount> freq;
    for (size_t i = 0; i < reviews.size(); ++i) {
        const auto& r = reviews[i];
        if (r.rating != 1)
            continue;                // skip non-1-star

        string norm = normalize(r.review_text);
        istringstream iss(norm);
        string token;
        while (iss >> token) {
            if (token.size() < 3)        continue;
            if (is_stop_word(token))     continue;
            add_or_increment(freq, token);
        }
    }
    auto t1 = chrono::high_resolution_clock::now();

    // 3) Sort and report
    sort_wordcount_desc(freq);
    auto dt = chrono::duration_cast<chrono::microseconds>(t1 - t0).count();
    size_t mem  = freq.size() * sizeof(WordCount);
    size_t topN = freq.size() < 10 ? freq.size() : 10;

    cout << "  Time taken:   " << dt   << " microseconds\n";
    cout << "  Memory used:  " << mem  << " bytes (approx)\n";
    cout << "  Top " << topN << " words:\n";
    for (size_t i = 0; i < topN; ++i) {
        cout << "    " << freq[i].word
             << " (count=" << freq[i].count << ")\n";
    }
    cout << endl;
}


// run_array_mode(): submenu
//------------------------------------------------------------------------------

int run_array_mode() {
    const string txn_clean = "../data/transactions_cleaned_v2.csv";
    const string rev_clean = "../data/reviews_cleaned_v2.csv";

    DynamicArray<Transaction> transactions;
    if (!load_transactions_da(txn_clean, transactions)) {
        cerr << "Error loading transactions\n";
        return 1;
    }
    DynamicArray<Review> reviews;
    if (!load_reviews_da(rev_clean, reviews)) {
        cerr << "Error loading reviews\n";
        return 1;
    }

    while (true) {
        cout << "\n-- Array Mode --\n"
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
            case 1: array_q1(transactions);                 break;
            case 2: array_q2(transactions, reviews);        break;
            case 3: array_q3(reviews);                      break;
            case 4: return 0;
            default: cout << "Please select 1–4.\n";
        }
    }
}
