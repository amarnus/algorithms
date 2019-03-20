#include <iostream>
#include <vector>

using namespace std;

struct Node {
	int min;
	int max;
	int mid;
	Node *left_node;
	Node *right_node;
	int max_frequency;
	int rank;
	int height;
};

vector<Node*> get_leaves(vector<int> input, int input_size) {
	int i = 0;
	int prev = input[0];
	int frequency = 1;
	int start_pos = 0;
	vector<Node*> leaves;

	while(i < input_size) {
		if (i > 0) {
			if (input[i] != prev) {
				Node *node = new Node();
				node->min = start_pos;
				node->max = i - 1;
				node->mid = i - 1;
				node->left_node = NULL;
				node->right_node = NULL;
				node->max_frequency = frequency;
				node->rank = 1;
				node->height = 1;
				leaves.push_back(node);

				start_pos = i;
				frequency = 1;
			} else {
				frequency++;
			}
		}
		prev = input[i];
		i++;
	}

	Node *node = new Node();
	node->min = start_pos;
	node->max = input_size - 1;
	node->mid = input_size - 1;
	node->left_node = NULL;
	node->right_node = NULL;
	node->max_frequency = frequency;
	node->rank = 1;
	node->height = 1;
	leaves.push_back(node);

	return leaves;
}

Node* build_tree(vector<Node*> leaves) {
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
			parent_node->max_frequency = max(nodes[i-1]->max_frequency, nodes[i]->max_frequency);
			parent_node->rank = nodes[i]->rank + nodes[i-1]->rank + 1;
			parent_node->height = nodes[i]->height + 1;
			parent_nodes.push_back(parent_node);
			i += 2;
		}
		if (node_count < nodes.size()) {
			Node *last_node = nodes[nodes.size() - 1];
			last_node->height += 1;
			parent_nodes.push_back(last_node);
		}
		nodes = parent_nodes;
	}
	return nodes[0];
}

int query_tree(Node* node, int start, int end) {
	if (node->left_node == NULL && node->right_node == NULL) {
		if (start >= node->min && end <= node->max) {
			return end - start + 1;
		} else {
			return node->max_frequency;
		}
	} else if ((start == node->min) && (end == node->max)) {
		return node->max_frequency;
	} else {
		int mid = node->mid;
		if (start > mid) {
			return query_tree(node->right_node, start, end);
		} else if (end <= mid) {
			return query_tree(node->left_node, start, end);
		} else {
			return max(
				query_tree(node->left_node, start, mid),
				query_tree(node->right_node, mid + 1, end)
			);
		}
	}
}

int main() {
	int input_size;
	int nq;
	int qc;
	while(1) {
		vector<int> input;
		vector<Node*> leaves;
		cin >> input_size;
		if (input_size == 0) {
			break;
		}
		// cout << "Input size: " << input_size << endl;

		cin >> nq;
		qc = nq;
		// cout << "Number of queries: " << nq << endl;

		for (int i = 0; i < input_size; i++) {
			int x;
			cin >> x;
			input.push_back(x);
		}
		
		// cout << "Collecting leaves..." << endl;
		leaves = get_leaves(input, input_size);
		// cout << "Building tree... " << endl;
		Node* tree = build_tree(leaves);
		// cout << "Tree height: " << tree->height << endl;
		// cout << "Tree rank: " << tree->rank << endl;
		// cout << "Tree min: " << tree->min << endl;
		// cout << "Tree max: " << tree->max << endl;
		// cout << "Tree max frequency: " << tree->max_frequency << endl;
		// cout << "Running queries... " << endl;

		while(qc > 0) {
			int start, end;
			cin >> start >> end;
			cout << query_tree(tree, start - 1, end - 1) << endl;
			qc--;
		}
	}

	return 0;	
}
