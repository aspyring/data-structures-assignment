#include <string>

using namespace std;

// cleans raw CSV into cleaned_v2 CSV
void clean_transactions(
    const string& in_file,
    const string& out_file);

void clean_reviews(
    const string& txn_clean_file,  // <— changed
    const string& rev_in_file,
    const string& out_file);
