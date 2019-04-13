#include <iostream>
#include <vector>
#include <string.h>
#include <map>
#include <iomanip>

using namespace std;

// https://gist.github.com/hrsvrdhn/1ae71c25ef1c620c022a544d52df8928

struct Node {
    map<char, Node*> children;
    int count;
    bool is_end;
};

Node* init_trie_node(string alphabet) {
    Node* node = new Node();
    node->count = 0;
    node->is_end = false;
    return node;
}

void insert_trie(Node* root, string word) {
	Node *current = root;
	for(int i = 0; i < word.size(); i++) {
		char ch = word[i];
		Node *node = current->children[ch];
		if (!node) {
			node = new Node();
			current->children[word[i]] = node;
		}
		current = node;
	}
	current->is_end=true;
}

Node* search_word(Node *root, string word) {
	Node *current = root;
	for(int i = 0; i < word.size(); i++) {
		char ch = word[i];
		Node *node = current->children[ch];
		if (!node) {
			return NULL;
        }
		current = node;
	}
    return current->is_end ? current : NULL;
}

string find_and_set(Node *root, string word) {
	Node* node = search_word(root, word);
    if (node == NULL) {
        return "";
    } else {
        string addendum;
        Node* node_addendum;
        do {
            addendum = to_string(node->count);
            node_addendum = search_word(node, addendum);
            if (node_addendum != NULL) {
                node->count++;
            }
        } while (node_addendum != NULL);
        insert_trie(node, addendum);
        return word + addendum;
    }
}

string assign_name(Node* trie, string word) {
    string result = find_and_set(trie, word);
    if (result == "") {
        insert_trie(trie, word);
        result = word;
    }
    return result;
}

int main() {
    int n;
    string login;
    cin >> n;
    Node* trie = init_trie_node("");
    while (n > 0) {
        cin >> login;
        cout << assign_name(trie, login) << endl;
        n--;
    }
    return 0;
}