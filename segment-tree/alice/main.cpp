#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int min;
    int max;
    int mid;
    Node *left_node;
    Node *right_node;
    int count_even;
    int count_odd;
};

Node* build_tree_from_leaves(vector<Node*> leaves) {
	vector<Node*> nodes = leaves;
	while(nodes.size() > 1) {
		vector<Node*> parent_nodes;
		int i = 1;
		int node_count = nodes.size();
		if (node_count % 2 != 0) {
			node_count -= 1;
		}
		while (i < node_count) {
			Node *parent_node = new Node();
			parent_node->min = nodes[i-1]->min;
			parent_node->max = nodes[i]->max;
            parent_node->mid = nodes[i-1]->max;
			parent_node->left_node = nodes[i-1];
			parent_node->right_node = nodes[i];
            parent_node->count_even = parent_node->left_node->count_even + parent_node->right_node->count_even;
            parent_node->count_odd = parent_node->left_node->count_odd + parent_node->right_node->count_odd;
			parent_nodes.push_back(parent_node);
			i += 2;
		}
		if (node_count < nodes.size()) {
			Node *last_node = nodes[nodes.size() - 1];
			parent_nodes.push_back(last_node);
		}
		nodes = parent_nodes;
	}
	return nodes[0];
}

Node* build_tree(vector<int> input, int min, int max) {
    Node *node = new Node();
    node->min = min;
    node->max = max;
    node->mid = (min + max) / 2;
    if (min == max) {
        if (input[min] % 2 == 0) {
            node->count_even = 1;
            node->count_odd = 0;
        } else {
            node->count_odd = 1;
            node->count_even = 0;
        }
        node->left_node = NULL;
        node->right_node = NULL;
        return node;
    } else {
        int mid = (min + max) / 2;
        node->left_node = build_tree(input, min, mid);
        node->right_node = build_tree(input, mid + 1, max);
        node->count_even = node->left_node->count_even + node->right_node->count_even;
        node->count_odd = node->left_node->count_odd + node->right_node->count_odd;
        return node;
    }
}

void update_tree(Node* node, int pos, int new_value) {
    if (node->min == node->max && node->min == pos) {
        if (new_value % 2 == 0) {
            node->count_even = 1;
            node->count_odd = 0;
        } else {
            node->count_odd = 1;
            node->count_even = 0;
        }     
    }
    else {
        if (pos > node->mid) {
            update_tree(node->right_node, pos, new_value);
            node->count_even = node->left_node->count_even + node->right_node->count_even;
            node->count_odd = node->left_node->count_odd + node->right_node->count_odd;
        } else {
            update_tree(node->left_node, pos, new_value);
            node->count_even = node->left_node->count_even + node->right_node->count_even;
            node->count_odd = node->left_node->count_odd + node->right_node->count_odd;
        }
    }
}

int query_tree(Node *node, int is_even, int start, int end) {
    // either leaf
    if (node->min == node->max) {
        return (is_even == 1) ? node->count_even : node->count_odd;
    } // or the target node
      else if (node->min == start && node->max == end) {
        return (is_even == 1) ? node->count_even : node->count_odd;
    }
    else {
        int mid = node->mid;
        if (start > mid) {
            return query_tree(node->right_node, is_even, start, end);
        } else if (end <= mid) {
            return query_tree(node->left_node, is_even, start, end);
        } else {
            return query_tree(node->left_node, is_even, start, mid) + query_tree(node->right_node, is_even, mid + 1, end);
        }
    }
}

int main() {
    int n, q, v;
    int j = 0;
    vector<int> input;
    vector<Node*> leaves;
    Node* tree;
    cin >> n;
    for (int k = 0; k < n; k++) {
        cin >> v;
        input.push_back(v);
        Node *node = new Node();
        node->min = k;
        node->max = k;
        node->mid = k;
        node->left_node = NULL;
        node->right_node = NULL;
        if (v % 2 == 0) {
            node->count_even = 1;
            node->count_odd = 0;
        } else {
            node->count_odd = 1;
            node->count_even = 0;
        }
        leaves.push_back(node);
    }
    cin >> q;
    // tree = build_tree(input, 0, input.size() - 1);
    tree = build_tree_from_leaves(leaves);
    while (j < q) {
        int command, pos, update, start, end;
        cin >> command;
        if (command == 0) {
            cin >> pos >> update;
            if ((input[pos - 1] % 2) != (update % 2)) {
                update_tree(tree, pos - 1, update);
            }
            input[pos - 1] = update;
        } else if (command == 1) {
            cin >> start >> end;
            cout << query_tree(tree, 1, start - 1, end - 1) << endl;
        } else if (command == 2) {
            cin >> start >> end;
            cout << query_tree(tree, 0, start - 1, end - 1) << endl;   
        }
        j++;
    }
    return 0;
}