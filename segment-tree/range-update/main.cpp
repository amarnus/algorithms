#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int min;
    int max;
    Node *left_node;
    Node *right_node;
    int sum;
};

Node* build_tree(vector<int> input, int min, int max) {
    Node *node = new Node();
    node->min = min;
    node->max = max;
    if (min == max) {
        node->sum = input[min];
        node->left_node = NULL;
        node->right_node = NULL;
        return node;
    } else {
        int mid = (min + max) / 2;
        node->left_node = build_tree(input, min, mid);
        node->right_node = build_tree(input, mid + 1, max);
        node->sum = node->left_node->sum + node->right_node->sum;
        return node;
    }
}

void update_tree(Node* node, int start, int end, int update) {
    if (node->min == node->max) {
        node->sum += update; 
    }
    else {
        int mid = (node->min + node->max) / 2;
        if (start > mid) {
            update_tree(node->right_node, start, end, update);
            node->sum = node->left_node->sum + node->right_node->sum;
        } else if (end <= mid) {
            update_tree(node->left_node, start, end, update);
            node->sum = node->left_node->sum + node->right_node->sum;
        } else {
            update_tree(node->left_node, start, mid, update);
            update_tree(node->right_node, mid + 1, end, update);
            node->sum = node->left_node->sum + node->right_node->sum;
        }
    }
}

int query_tree(Node *node, int start, int end) {
    // either leaf
    if (node->min == node->max) {
        return node->sum;
    } // or the target node
      else if (node->min == start && node->max == end) {
        return node->sum;
    }
    else {
        int mid = (node->min + node->max) / 2;
        if (start > mid) {
            return query_tree(node->right_node, start, end);
        } else if (end <= mid) {
            return query_tree(node->left_node, start, end);
        } else {
            return query_tree(node->left_node, start, mid) + query_tree(node->right_node, mid + 1, end);
        }
    }
}

int main() {
    int t;
    int i = 0;
    cin >> t;
    while (i < t) {
        int n, c;
        int j = 0;
        vector<int> input;
        Node* tree;
        cin >> n;
        cin >> c;
        for (int k = 0; k < n; k++) {
            input.push_back(0);
        }
        tree = build_tree(input, 0, input.size() - 1);
        while (j < c) {
            int command, start, end, update;
            cin >> command;
            if (command == 0) {
                cin >> start >> end >> update;
                update_tree(tree, start - 1, end - 1, update);
            } else if (command == 1) {
                cin >> start >> end;
                cout << query_tree(tree, start - 1, end - 1) << endl;
            }
            j++;
        }
        i++;
    }
    return 0;
}