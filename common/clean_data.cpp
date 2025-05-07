#include "clean_data.hpp"
#include "load_data.hpp"
#include <fstream>
#include <cctype>
#include <cmath>
using namespace std;

//string manipulation
static string to_lower(string s) {
    for (char& c : s) c = tolower(c);
    return s;
}

// detect special tokens
static bool is_special(const string& s) {
    string t = to_lower(s);
    return (t == "nan" || t == "invalid" || t == "unknown");
}

// full-field duplicates
static bool txn_equal(const Transaction& a, const Transaction& b) {
    return a.customer_id    == b.customer_id
        && a.product        == b.product
        && a.category       == b.category
        && a.date           == b.date
        && a.payment_method == b.payment_method
        && a.price          == b.price;
}

static bool rev_equal(const Review& a, const Review& b) {
    return a.product_id   == b.product_id
        && a.customer_id  == b.customer_id
        && a.rating       == b.rating
        && a.review_text  == b.review_text;
}
static bool valid_date(const string& s) {
    // Must be exactly 10 chars: "DD/MM/YYYY"
    if (s.size() != 10) return false;
    // Slashes at positions 2 and 5
    if (s[2] != '/' || s[5] != '/') return false;
    // All other positions must be digits
    for (size_t i : {0,1,3,4,6,7,8,9}) {
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    // Parse day, month, year
    int day   = stoi(s.substr(0,2));
    int month = stoi(s.substr(3,2));
    int year  = stoi(s.substr(6,4));
    if (month < 1 || month > 12) return false;
    if (day   < 1 || day   > 31) return false;
    return true;
}
void clean_transactions(
    const string& in_file,
    const string& out_file)
{
    DynamicArray<Transaction> raw, clean;
    load_transactions_da(in_file, raw);

    for (size_t i = 0; i < raw.size(); ++i) {
        const auto& t = raw[i];

        // 1) drop empty or special in any text field
        if (t.customer_id.empty()   || is_special(t.customer_id)
         || t.product.empty()       || is_special(t.product)
         || t.category.empty()      || is_special(t.category)
         || t.date.empty()          || is_special(t.date)
         || t.payment_method.empty()|| is_special(t.payment_method))
            continue;

        // 1.5) drop if date not valid
        if (!valid_date(t.date))
            continue;

        // 2) drop invalid price (<=0 or NaN)
        if (t.price <= 0 || std::isnan(t.price))
            continue;

        // 3) dedupe exact match
        bool dup = false;
        for (size_t j = 0; j < clean.size(); ++j) {
            if (txn_equal(clean[j], t)) { dup = true; break; }
        }
        if (dup) continue;

        clean.push(t);
    }

    ofstream out(out_file);
    out << "Customer ID,Product,Category,Price,Date,Payment Method\n";
    for (size_t i = 0; i < clean.size(); ++i) {
        const auto& t = clean[i];
        out
          << t.customer_id    << ','
          << t.product        << ','
          << t.category       << ','
          << t.price          << ','
          << t.date           << ','
          << t.payment_method << '\n';
    }
}

void clean_reviews(
    const string& txn_clean_file,
    const string& rev_in_file,
    const string& out_file)
{
    // 1) load CLEANED transactions for referential integrity
    DynamicArray<Transaction> txns;
    load_transactions_da(txn_clean_file, txns);

    // 2) load RAW reviews
    DynamicArray<Review> raw, clean;
    load_reviews_da(rev_in_file, raw);

    for (size_t i = 0; i < raw.size(); ++i) {
        const auto& r = raw[i];

        // a) drop empty or special tokens
        if (r.product_id.empty()   || is_special(r.product_id)  ||
            r.customer_id.empty()  || is_special(r.customer_id) ||
            r.review_text.empty()  || is_special(r.review_text))
            continue;

        // b) drop invalid rating
        if (r.rating < 1 || r.rating > 5)
            continue;

        // c) referential integrity: customer must exist in txns
        bool has_customer = false;
        for (size_t j = 0; j < txns.size(); ++j) {
            if (txns[j].customer_id == r.customer_id) {
                has_customer = true;
                break;
            }
        }
        if (!has_customer) continue;

        // d) dedupe exact match
        bool dup = false;
        for (size_t k = 0; k < clean.size(); ++k) {
            if (rev_equal(clean[k], r)) {
                dup = true;
                break;
            }
        }
        if (dup) continue;

        clean.push(r);
    }

    // 3) write out
    ofstream out(out_file);
    out << "Product ID,Customer ID,Rating,Review Text\n";
    for (size_t i = 0; i < clean.size(); ++i) {
        const auto& r = clean[i];
        out
          << r.product_id   << ','
          << r.customer_id  << ','
          << r.rating       << ','
          << r.review_text  << '\n';
    }
}

