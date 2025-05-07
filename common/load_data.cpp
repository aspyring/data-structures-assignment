#include "load_data.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

// trim whitespace
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n"),
           b = s.find_last_not_of (" \t\r\n");
    return (a == string::npos) ? "" : s.substr(a, b - a + 1);
}

bool load_transactions_da(const string& filename,
                       DynamicArray<Transaction>& transactions) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    string line;
    getline(in, line);  // skip header
    while (getline(in, line)) {
        // split into fields
        DynamicArray<string> flds;
        string field;
        istringstream ss(line);
        while (getline(ss, field, ',')) flds.push(trim(field));
        if (flds.size() != 6) continue;

        // parse price
        double price;
        try { price = stod(flds[3]); }
        catch (...) { continue; }

        Transaction t {
            flds[0], flds[1], flds[2],
            price,   flds[4], flds[5]
        };
        transactions.push(t);
    }
    return true;
}

bool load_reviews_da(const string& filename,
                  DynamicArray<Review>& reviews) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    string line;
    getline(in, line);  // skip header

    while (getline(in, line)) {
        // find the first three commas
        size_t p1 = line.find(',');
        if (p1 == string::npos) continue;
        size_t p2 = line.find(',', p1 + 1);
        if (p2 == string::npos) continue;
        size_t p3 = line.find(',', p2 + 1);
        if (p3 == string::npos) continue;

        // extract fields
        string prodId   = trim(line.substr(0,        p1));
        string custId   = trim(line.substr(p1 + 1,  p2 - p1 - 1));
        string ratingS  = trim(line.substr(p2 + 1,  p3 - p2 - 1));
        string reviewT  = trim(line.substr(p3 + 1));  // rest of line

        // validate rating
        int rating;
        try {
            rating = stoi(ratingS);
        } catch (...) {
            continue;
        }

        // push into array
        reviews.push(Review{prodId, custId, rating, reviewT});
    }

    return true;
}

bool load_transactions_ll(const string& filename,
                          LinkedList<Transaction>& list) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    string line;
    getline(in, line);  // skip header

    while (getline(in, line)) {
        // split into fields
        DynamicArray<string> flds;
        string field;
        istringstream ss(line);
        while (getline(ss, field, ',')) {
            flds.push( trim(field) );
        }
        if (flds.size() != 6) continue;

        // parse price
        double price;
        try {
            price = stod(flds[3]);
        } catch (...) {
            continue;
        }

        Transaction t {
            flds[0],  // customer_id
            flds[1],  // product
            flds[2],  // category
            price,    // price
            flds[4],  // date
            flds[5]   // payment_method
        };
        list.push_back(t);
    }
    return true;
}

bool load_reviews_ll(const string& filename,
                     LinkedList<Review>& list) {
    ifstream in(filename);
    if (!in.is_open()) return false;
    string line;
    getline(in, line);  // skip header

    while (getline(in, line)) {
        // handling commas in review text, split first 3 fields and treat rest as text
        size_t p1 = line.find(',');
        if (p1 == string::npos) continue;
        size_t p2 = line.find(',', p1 + 1);
        if (p2 == string::npos) continue;
        size_t p3 = line.find(',', p2 + 1);
        if (p3 == string::npos) continue;

        string prodId  = trim(line.substr(0, p1));
        string custId  = trim(line.substr(p1 + 1, p2 - p1 - 1));
        string ratingS = trim(line.substr(p2 + 1, p3 - p2 - 1));
        string revText = trim(line.substr(p3 + 1));  // rest of line

        // parse rating
        int rating;
        try {
            rating = stoi(ratingS);
        } catch (...) {
            continue;
        }

        Review r {
            prodId,
            custId,
            rating,
            revText
        };
        list.push_back(r);
    }
    return true;
}

