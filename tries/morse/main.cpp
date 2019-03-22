#include <iostream>
#include <vector>
#include <string.h>
#include <map>

#define MAX_INPUT_SIZE 10000
#define DOT "."
#define DASH "-"

using namespace std;

struct Node {
    string code;
    Node* left_child;
    Node* right_child;
    int count;
};

int phrase_counts[MAX_INPUT_SIZE];

string convert_word_to_morse(string word) {
    string code = "";
    for (int i = 0; i < word.length(); i++) {
        switch (word[i]) {
            case 'A': code += ".-"; break;
            case 'B': code += "-..."; break;
            case 'C': code += "-.-."; break;
            case 'D': code += "-.."; break;
            case 'E': code += "."; break;
            case 'F': code += "..-."; break;
            case 'G': code += "--."; break;
            case 'H': code += "...."; break;
            case 'I': code += ".."; break;
            case 'J': code += ".---"; break;
            case 'K': code += "-.-"; break;
            case 'L': code += ".-.."; break;
            case 'M': code += "--"; break;
            case 'N': code += "-."; break;
            case 'O': code += "---"; break;
            case 'P': code += ".--."; break;
            case 'Q': code += "--.-"; break;
            case 'R': code += ".-."; break;
            case 'S': code += "..."; break;
            case 'T': code += "-"; break;
            case 'U': code += "..-"; break;
            case 'V': code += "...-"; break;
            case 'W': code += ".--"; break;
            case 'X': code += "-..-"; break;
            case 'Y': code += "-.--"; break;
            case 'Z': code += "--.."; break;
        }
    }
    return code;
}

void print_trie(Node* node, string prefix_code = "") {
    if (node->count) {
        cout << prefix_code + node->code << endl;
    }
    if (node->left_child != NULL) {
        print_trie(node->left_child, prefix_code + node->code);
    }
    if (node->right_child != NULL) {
        print_trie(node->right_child, prefix_code + node->code);
    }
}

Node* find_word(Node* node, string word) {
    if (node->count && word.size() == 0) {
        return node;
    } else {
        string first = word.substr(0, 1);
        if (first == DASH && node->left_child != NULL) {
            return find_word(node->left_child, word.substr(1));
        } else if (first == DOT && node->right_child != NULL) {
            return find_word(node->right_child, word.substr(1));
        }
        return NULL;
    }
}

Node* init_trie_node(string code) {
    Node* node = new Node();
    node->count = 0;
    node->code = code;
    node->left_child = NULL;
    node->right_child = NULL;
    return node;
}

// void insert_trie_iterative(Node* node, string word) {
//     for (int i = 0; i < word.size(); i++) {
//         string first = word.substr(i, 1);
//         if (first == "-") {
//             if (node->left_child == NULL) {
//                 node->left_child = init_trie_node(first);
//             }
//             node = node->left_child;
//         } else {
//             if (node->right_child == NULL) {
//                 node->right_child = init_trie_node(first);
//             }
//             node = node->right_child;
//         }
//     }
//     node->count++;
//     node->is_end = true;
// }

void insert_trie(Node* node, string word) {
    if (word.size() == 0) {
        node->count++;
    }
    else {
        string first = word.substr(0, 1);
        string rest = word.substr(1);
        if (first == DASH) {
            if (node->left_child == NULL) {
                node->left_child = init_trie_node(first);
            }
            insert_trie(node->left_child, rest);
        } else {
            if (node->right_child == NULL) {
                node->right_child = init_trie_node(first);
            }
            insert_trie(node->right_child, rest);
        }
    }
}

// map<string, int> cache;
// map<string, int>::iterator itr;
// cache.clear();
// int count_phrases_slow(Node* tree, string input) {
//     if (input == "") {
//         return 0;
//     }
//     itr = cache.find(input);
//     if (itr != cache.end()) {
//         return cache.at(input);
//     } else {
//         Node* input_node = find_word(tree, input);
//         int sum = (input_node != NULL) ? input_node->count : 0;
//         for (int i = 0; i < input.length(); i++) {
//             Node* next_node = find_word(tree, input.substr(0, i + 1));
//             if (next_node != NULL) {
//                 sum += next_node->count * count_phrases(tree, input.substr(i + 1));
//             }
//         }
//         cache.insert(pair<string, int>(input, sum));
//         return sum;
//     }
// }

int count_phrases(Node* tree, string input) {
    phrase_counts[input.size()] = 1;
    for (int i = input.size() - 1; i >= 0; i--) {
        Node* node = tree;
        for (int j = i; j < input.size() && node != NULL; j++) {
            string character = input.substr(j, 1);
            if (character == DASH) {
                node = node->left_child;
            } else if (character == DOT) {
                node = node->right_child;
            }
            if (node != NULL && node->count) {
                phrase_counts[i] += node->count * phrase_counts[j + 1];
            }
        }
    }
    return phrase_counts[0];
}

int main() {
    int T, N;
    string input, word;
    cin >> T;
    while (T--) {
        cin >> input;
        cin >> N;
        Node* tree = init_trie_node("");
        while (N--) {
            cin >> word;
            string code = convert_word_to_morse(word);
            insert_trie(tree, code);
        }
        for (int i = 0; i < MAX_INPUT_SIZE; i++) {
            phrase_counts[i] = 0;
        }
        // print_trie(tree);
        cout << count_phrases(tree, input) << endl;
    }

    return 0;
}