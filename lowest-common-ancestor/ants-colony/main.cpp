#include <iostream>
#include <vector>

using namespace std;

// maximum constraint (10^5)
const int N = 100001;

int visited[N], E[N * 2], L[N * 2], H[N];
uint64_t W[N], S[N * 2];
vector<int> A[N];
int ID = 0;

struct Node {
    int min;
    int max;
    Node *left_node;
    Node *right_node;
    int minIndex;
};

void print_adjacency_list(int max) {
    for (int i = 0; i < max; i++) {
        cout << i << " = ";
        for (int j = 0; j < A[i].size(); j++) {
            cout << A[i][j] << " (" << W[A[i][j]] << ") ";
        }
        cout << endl;
    }
}

void euler_tour(int u, int parent=-1) {
    visited[u] = 1;
    H[u] = ID;
    E[ID] = u;
    if (parent == -1) {
        L[ID] = 1;
        S[u] = 0;
    } else {
        L[ID] = L[H[parent]] + 1;
        S[u] = S[parent] + W[u];
    }
    ID++;
    for (int i = 0; i < A[u].size(); i++) {
        if (!visited[A[u][i]]) {
            euler_tour(A[u][i], u);
            E[ID] = u;
            L[ID] = (parent == -1) ? 1 : L[H[parent]] + 1;
            ID++;
        }
    }
}

Node* build_segment_tree(int min, int max) {
    Node *node = new Node();
    node->min = min;
    node->max = max;
    if (min == max) {
        node->minIndex = min;
        node->left_node = NULL;
        node->right_node = NULL;
        return node;
    } else {
        int mid = (min + max) / 2;
        node->left_node = build_segment_tree(min, mid);
        node->right_node = build_segment_tree(mid + 1, max);
        if (L[node->left_node->minIndex] <= L[node->right_node->minIndex]) {
            node->minIndex = node->left_node->minIndex;
        } else {
            node->minIndex = node->right_node->minIndex;
        }
        return node;
    }
}

uint64_t query_segment_tree(Node *node, int start, int end) {
    if (node->min == node->max) {
        return node->minIndex;
    }
    else if (node->min == start && node->max == end) {
        return node->minIndex;
    }
    else {
        int mid = (node->min + node->max) / 2;
        if (start > mid) {
            return query_segment_tree(node->right_node, start, end);
        } else if (end <= mid) {
            return query_segment_tree(node->left_node, start, end);
        } else {
            int minIndexLeft = query_segment_tree(node->left_node, start, mid);
            int minIndexRight = query_segment_tree(node->right_node, mid + 1, end);
            return (L[minIndexLeft] <= L[minIndexRight]) ? minIndexLeft : minIndexRight;
        }
    }
}

int main() {

    while(true) {
        int N, Q;
        int u, v, i = 0, k = 1;
        uint64_t w;

        cin >> N;
        if (cin.fail() || (N == 0)) {
            break;
        }

        while (i < N) {
            A[i].clear();
            visited[i] = 0;
            i++;
        }
        i = 1;

        while (i < N) {
            u = i;
            cin >> v >> w;
            A[v].push_back(u);
            W[u] = w;
            i++;
        }

        // print_adjacency_list(N);
        ID = 0;
        euler_tour(0);
        Node* tree = build_segment_tree(0, ID);

        cin >> Q;
        while (k <= Q) {
            int start, end, minStart, minEnd;
            cin >> start >> end;
            minStart = min(H[start], H[end]);
            minEnd = max(H[start], H[end]);
            cout << S[start] + S[end] - 2 * S[E[query_segment_tree(tree, minStart, minEnd)]];
            k++;
            if (k <= Q) {
                cout << " ";
            }
        }
        cout << endl;
    }

    return 0;
}