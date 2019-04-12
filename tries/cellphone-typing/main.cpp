#include <iostream>
#include <vector>
#include <string.h>
#include <map>
#include <iomanip>

using namespace std;

struct Node {
    string word;
    vector<Node*> children;
    bool is_assigned;
};

void print_trie(Node* node) {
    if (node->children.size() == 0) {
        cout << node->alphabet << endl;
    }
    for (int i = 0; i < node->children.size(); i++) {
        cout << node->children[i]->alphabet << " ";
    }
    cout << endl;
    for (int i = 0; i < node->children.size(); i++) {
        print_trie(node->children[i]);
    }
    cout << endl;
}

// A decision node is node that is either an end node 
// or has more than one child.
int count_decision_nodes(Node* node, string word) {
    if (word.size() > 0) {
        string first = word.substr(0, 1);
        string rest = word.substr(1);
        int count = 0;
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->alphabet == first) {
                if (node->alphabet != "") {
                    count = (node->children.size() > 1) || node->is_end  ? 1 : 0;
                }
                return count + count_decision_nodes(node->children[i], rest);
            }
        }
        return 0;
    }
    return 1;
}

Node* init_trie_node(string alphabet) {
    Node* node = new Node();
    node->alphabet = alphabet;
    return node;
}

void insert_trie(Node* node, string word) {
    if (word.size() == 0) {
        node->is_end = true;
    }
    else {
        string first = word.substr(0, 1);
        string rest = word.substr(1);
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->alphabet == first) {
                return insert_trie(node->children[i], rest);
            }
        }
        Node* new_node = init_trie_node(first);
        node->children.push_back(new_node);
        insert_trie(new_node, rest);
    }
}

int main() {
    int n;
    float sum;
    string word;
    vector<string> words;
    Node* tree;
    cout.precision(2);
    while (cin >> n && !cin.fail()) {
        tree = init_trie_node("");
        words.clear();
        sum = 0;
        while (n--) {
            cin >> word;
            words.push_back(word);
            insert_trie(tree, word);   
        }
        for (int i = 0; i < words.size(); i++) {
            sum += count_decision_nodes(tree, words[i]);
        }
        cout << fixed << (sum / words.size()) << endl;
    }
    return 0;
}