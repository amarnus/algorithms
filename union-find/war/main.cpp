#include <iostream>
#include <vector>

using namespace std;

struct UnionFind {

    vector<int> parents;
    vector<int> sizes;
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

    void insert() {
        parents.push_back(size);
        sizes.push_back(1);
        size++;
    }

    void set_union(int a, int b) {
        int rootA = find_root(a);
        int rootB = find_root(b);
        if (rootA == rootB) {
            return;
        } else {
            if (sizes[rootA] <= sizes[rootB]) {
                parents[rootA] = rootB;
                sizes[rootB]++;
            } else {
                parents[rootB] = rootA;
                sizes[rootA]++;
            }
        }
    }

    int get_size() {
        return size;
    }

    void print() {
        for(int i = 0; i < size; i++) {
            cout << parents[i] << " ";
        }
        cout << endl;
    }

};

void add_friends(UnionFind *uf, int enemies[], int x, int y) {
    int rootX, rootY, enemyX, enemyY, newRoot, rootEnemy;
    rootX = uf->find_root(x);
    rootY = uf->find_root(y);
    enemyX = enemies[rootX];
    enemyY = enemies[rootY];

    uf->set_union(x, y);
    newRoot = uf->find_root(x);
    
    if (enemyX != -1 && enemyY == -1) {
        enemies[newRoot] = enemyX;
    }
    else if (enemyX == -1 && enemyY != -1) {
        enemies[newRoot] = enemyY;
    }
    else if (enemyX != -1 && enemyY != -1) {
        uf->set_union(enemyX, enemyY);
        rootEnemy = uf->find_root(enemyX);
        enemies[rootEnemy] = newRoot;
        enemies[newRoot] = rootEnemy;
    }
}

void add_enemies(UnionFind *uf, int enemies[], int x, int y) {
    int rootX, rootY, enemyX, enemyY, newRootX, newRootY;
    rootX = uf->find_root(x);
    rootY = uf->find_root(y);
    newRootX = rootX;
    newRootY = rootY;
    enemyX = enemies[rootX];
    enemyY = enemies[rootY];
    
    if (enemyX != -1 && enemyX != rootY) {
        uf->set_union(enemyX, rootY);
        newRootY = uf->find_root(rootY);
    }
    
    if (enemyY != -1 && enemyY != rootX) {
        uf->set_union(enemyY, rootX);
        newRootX = uf->find_root(rootX);
    }

    enemies[newRootX] = newRootY;
    enemies[newRootY] = newRootX;
}

int main() {
    UnionFind uf;
    int n, op, x, y;
    cin >> n;
    int enemies[n];
    
    for (int i = 0; i < n; i++) {
        uf.insert();
        enemies[i] = -1;
    }

    while(cin >> op >> x >> y) {
        if (op == 1) {
            int rootX = uf.find_root(x);
            int rootY = uf.find_root(y);
            if (enemies[rootX] == rootY) {
                cout << "-1" << endl;
            } else {
                add_friends(&uf, enemies, x, y);
            }
        }
        else if (op == 2) {
            if (uf.connected(x, y)) {
                cout << "-1" << endl;
            } else {
                add_enemies(&uf, enemies, x, y);
            }
        }
        else if (op == 3) {
            cout << ((uf.connected(x, y)) ? "1" : "0") << endl;
        }
        else if (op == 4) {
            int rootX = uf.find_root(x);
            int rootY = uf.find_root(y);
            cout << ((enemies[rootX] == rootY) ? "1" : "0") << endl;
        }
    }

    return 0;
}