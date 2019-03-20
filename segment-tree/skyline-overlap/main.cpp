#include <iostream>
#include <vector>

using namespace std;

enum HeapType { MIN, MAX };

template<typename T> struct Heap {
    vector<T> queue;
    int s;
    HeapType type;
    less<T> L;
    greater<T> G;

    Heap(HeapType heapType) {
        s = 0;
        type = heapType;
        L = less<T>();
        G = greater<T>();
        queue.push_back(0);
    }

    void exch(int x, int y) {
        T t = queue[x];
        queue[x] = queue[y];
        queue[y] = t;
    }
    
    bool compare(int x, int y) {
        if (type == MAX) {
            return L(queue[x], queue[y]);
        } else {
            return G(queue[x], queue[y]);
        }
    }

    void swim(int k) {
        while(k > 1 && compare(k/2, k)) {
            exch(k, k/2);
            k = k/2;
        };
    }

    void sink(int k) {
        while(2 * k <= s) {
            int j = 2 * k;
            if (j < s && compare(j, j + 1)) j++;
            if (compare(j, k)) break;
            exch(k, j);
            k = j;
        };
    }

    Heap<T>* add(T x) {
        queue.push_back(x);
        s++;
        swim(s);
        return this;
    }

    T pop() {
        T max = queue[1];
        exch(1, s--);
        sink(1);
        queue.pop_back();
        return max;
    }

    T peek() {
        return queue[1];
    }

    int get_size() {
        return s;
    }

};

struct Node {
	int min;
	int max;
	int mid;
	Node *left_node;
	Node *right_node;
	int height;
};

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

int find_overlay_sum(Node* node, int start, int end, int height) {
    if (node->left_node == NULL && node->right_node == NULL) {
        node->height = max(node->height, height);
        return (node->height == height) ? (end - start) : 0;
    } else {
        if ((start == node->min) && (end == node->max)) {
            node->height = max(node->height, height);
        }
		int mid = node->mid;
		if (start >= mid) {
			return find_overlay_sum(node->right_node, start, end, height);
		} else if (end <= mid) {
			return find_overlay_sum(node->left_node, start, end, height);
		} else {
			return find_overlay_sum(node->left_node, start, mid, height) + 
                   find_overlay_sum(node->right_node, mid, end, height);
		}
	}
}

int main() {
    int D;
    cin >> D;
    int i = 0;

    while (i < D) {
        int n, l, r, h;
        cin >> n;
        Heap<int> *min_heap = new Heap<int>(MIN);
        vector<Node*> leaves;
        vector<int> L;
        vector<int> R;
        vector<int> H;

        // add all ranges to a min-heap
        for (int j = 0; j < n; j++) {
            cin >> l >> r >> h;
            min_heap->add(l);
            min_heap->add(r);
            L.push_back(l);
            R.push_back(r);
            H.push_back(h);
        }

        // pair sorted range boundaries and collect them as leaves
        int prev_r = -1;
        while (min_heap->get_size() > 0) {
            if (prev_r == -1) {
                l = min_heap->pop();
            } else {
                l = prev_r;
            }
            r = min_heap->pop();
            prev_r = r;
            Node* node = new Node();
            node->min = l;
            node->max = r;
            node->mid = r;
            node->left_node = NULL;
            node->right_node = NULL;
            node->height = -1;
            leaves.push_back(node);
        }

        // build a segment tree from the leaves
        Node* segment_tree = build_tree(leaves);

        int overlay_sum = 0;
        for (int k = 0; k < n; k++) {
            int current_overlay_sum = find_overlay_sum(segment_tree, L[k], R[k], H[k]);
            cout << current_overlay_sum << endl;
            overlay_sum += current_overlay_sum;
        }

        cout << overlay_sum << endl;

        i++;
    }
    return 0;
}