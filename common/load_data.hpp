#include <string>
#include "../array_version/array_data_structure.hpp"
#include "../linkedlist_version/linked_list.hpp"
using namespace std;

struct Transaction {
    string customer_id;
    string product;
    string category;
    double price;
    string date;
    string payment_method;
};

struct Review {
    string product_id;
    string customer_id;
    int    rating;
    string review_text;
};

bool load_transactions_da(const string& filename,
                       DynamicArray<Transaction>& transactions);

bool load_reviews_da(const string& filename,
                  DynamicArray<Review>& reviews);


bool load_transactions_ll(const string& filename,
                          LinkedList<Transaction>& list);

bool load_reviews_ll(const string& filename,
                     LinkedList<Review>& list);


