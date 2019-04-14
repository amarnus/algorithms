#include <iostream>
#include <vector>
#include <map>

using namespace std;

int n, nop, j = 0, N = 100000, price, min_price;
char op;
string item_name;
map <string, int> prices, stock;
int BIT[100000];

int get_stock(int max_price) {
    int ix = max_price;
    int sum = 0;
    while (ix > 0) {
        sum += BIT[ix];
        ix = ix - (ix & -ix);
    }
    return sum;
}

void update_stock(int i, int incr_by) {
    int ix = i;
    while (ix <= N) {
        BIT[ix] += incr_by;
        ix = ix + (ix & -ix);
    }
}

int main() {
    cin >> n;
    while (j < n) {
        cin >> item_name >> price;
        prices[item_name] = price;
        stock[item_name] = 0;
        j++;
    }

    cin >> nop;
    while (nop-- > 0) {
        cin >> op;
        if (op == '+') {
            cin >> item_name;
            stock[item_name]++;
            update_stock(prices[item_name], 1);
        } else if (op == '-') {
            cin >> item_name;
            if (stock[item_name] > 0) {
                stock[item_name]--;
                update_stock(prices[item_name], -1);
            }
        } else if (op == '?') {
            cin >> min_price;
            cout << get_stock(N) - get_stock(min_price) << endl;
        }
    }

    return 0;
}