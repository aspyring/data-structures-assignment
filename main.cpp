#include <iostream>
#include "../data-structures-assignment/common/clean_data.hpp"
#include "../data-structures-assignment/common/load_data.hpp"
using namespace std;

// Forward declarations of your two modes:
void run_array_mode();
void run_list_mode();

int main() {
    while (true) {
        cout << "\n=== Data Structures Assignment ===\n"
             << "1) Clean data only\n"
             << "2) Array implementation (Q1-Q3)\n"
             << "3) Linked-list implementation (Q1-Q3)\n"
             << "4) Exit\n"
             << "Select an option: ";

        int choice;
        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            return 0;
        }

        switch (choice) {
            case 1:
                // Just invoke the cleaning routines:
                clean_transactions("../data/transactions.csv",
                                   "../data/transactions_cleaned_v2.csv");
                clean_reviews   ("../data/transactions_cleaned_v2.csv",
                                 "../data/reviews.csv",
                                 "../data/reviews_cleaned_v2.csv");
                cout << "Data cleaning complete.\n";
                break;
            case 2:
                run_array_mode();
                break;
            case 3:
                run_list_mode();
                break;
            case 4:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Please choose 1–4.\n";
        }
    }
}

