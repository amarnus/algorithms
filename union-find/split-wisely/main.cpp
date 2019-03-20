#include <iostream>
#include <vector>

using namespace std;

struct UnionFind {

    vector<int> parents;
    vector<int> sizes;
    vector<int> sums;
    int size;

    UnionFind() {
        size = 0;
    }

    int find_root(int x) {
        if (x >= size) {
            throw "index " + to_string(x) + " is more than size " + to_string(size);
        }
        int y = x;
        while (parents[y] != y) {
            parents[y] = parents[parents[y]];
            y = parents[y];
        }
        return parents[y];
    }

    bool connected(int a, int b) {
        return find_root(a) == find_root(b);
    }

    void insert(int sum) {
        parents.push_back(size);
        sizes.push_back(1);
        sums.push_back(sum);
        size++;
    }

    void set_union(int a, int b) {
        int rootA = find_root(a);
        int rootB = find_root(b);
        // cout << "Root of " << a << " is " << rootA << " and sum is " << sums[rootA] << endl;
        // cout << "Root of " << b << " is " << rootB << " and sum is " << sums[rootB] << endl;
        if (rootA == rootB) {
            return;
        } else {
            if (sizes[rootA] <= sizes[rootB]) {
                parents[rootA] = rootB;
                sums[rootB] += sums[rootA];
                // cout << "Sum of " << rootB << " is " << sums[rootB] << endl;
                sizes[rootB]++;
            } else {
                parents[rootB] = rootA;
                sums[rootA] += sums[rootB];
                // cout << "Sum of " << rootA << " is " << sums[rootA] << endl;
                sizes[rootA]++;
            }
        }
    }

    int get_size() {
        return size;
    }

    int get_sum(int x) {
        return sums[x];
    }

    void print() {
        for(int i = 0; i < size; i++) {
            cout << parents[i] << " ";
        }
        cout << endl;
    }

};

int main() {
    int T;
    cin >> T;
    int i = 0;
    // cout << "T = " << T << endl;
    while (i < T) {
        int n, m;
        cin >> n >> m;
        int rootSum[n];
        int owed[n];
        UnionFind uf;
        // cout << "n = " << n << endl;
        for (int j = 0; j < n; j++) {
            int owe;
            cin >> owe;
            owed[j] = owe;
            rootSum[j] = owe;
            uf.insert(owe);
        }
        for (int k = 0; k < m; k++) {
            int x, y;
            cin >> x >> y;
            uf.set_union(x, y);
        }
        bool settled;
        for (int l = 0; l < n; l++) {
            int root = uf.find_root(l);
            // cout << "Root of " << l << " is " << root << " and sum is " << uf.get_sum(root) << endl;
            if (uf.get_sum(root) == 0) {
                settled = true;
            } else {
                settled = false;
                break;
            }
        }
        if (settled) {
            cout << "HOUDU" << endl;
        } else {
            cout << "ILLA" << endl;
        }
        i++;
    }

    return 0;
}