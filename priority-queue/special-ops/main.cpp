#include <iostream>
#include <queue>

using namespace std;

struct BSTNode {
    int value;
    int count;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int _value) {
        value = _value;
        count = 1;
        left = NULL;
        right = NULL;
    }
};

BSTNode* insert_bst(BSTNode* node, int value) {
    if (node == NULL) {
        return new BSTNode(value);
    } else if (node->value > value) {
        node->left = insert_bst(node->left, value);
    } else if (node->value < value) {
        node->right = insert_bst(node->right, value);
    } else if (node->value == value) {
        node->count++;
    }
    return node;
}

int delete_min(BSTNode** root) {
    BSTNode* current = *root;
    BSTNode* prev = NULL;
    while (current->left != NULL) {
        prev = current;
        current = current->left;
    }
    if (current->count == 1) {
        if (prev != NULL) {
            prev->left = current->right;
        } else {
            *root = current->right;
        }
    } else {
        current->count--;
    }
    return current->value;
}

int delete_max(BSTNode** root) {
    BSTNode* current = *root;
    BSTNode* prev = NULL;
    while (current->right != NULL) {
        prev = current;
        current = current->right;
    }
    if (current->count == 1) {
        if (prev != NULL) {
            prev->right = current->left;
        } else {
            *root = current->left;
        }
    } else {
        current->count--;
    }
    return current->value;
}

int main() {
    int n, nq, q, v;
    int min, max;
    uint64_t answers[100000];
    uint64_t sum;
    cin >> n >> nq;
    BSTNode* bst;

    // Build the Binary Search Tree
    cin >> v;
    bst = new BSTNode(v);
    sum = v;
    for (int i = 1; i < n; i++) {
        cin >> v;
        bst = insert_bst(bst, v);
        sum += v;
    }

    // Pre-compute answers for ALL possible iterations
    answers[0] = sum;
    for (int i = 1; i < n; i++) {
        min = delete_min(&bst);
        max = delete_max(&bst);
        sum = sum - 2 * min;
        insert_bst(bst, max - min);
        answers[i] = sum;
    }
    
    // Respond to queries
    while (nq > 0) {
        cin >> q;
        cout << answers[q] << endl;
        nq--;
    }

    return 0;
}