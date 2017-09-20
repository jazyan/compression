#include <fstream>
#include <queue>
#include <iostream>
#include <assert.h>
#include <string>
#include <vector>
using namespace std;

// http://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/

class node {
  public:
    bool operator<(const node& other) const {
        return freq > other.freq;  // for q.pop to return lowest frequency
    }
    char value = ' ';
    int freq = 0;
    node* left = NULL;
    node* right = NULL;
};

void print_queue(priority_queue<node> q) {
    while (!q.empty()) {
        node n = q.top();
        cout << n.freq<< " " << n.value << endl;
        q.pop();
    }
}

// pop off two lowest frequencies and combine them into one internal node I
// I's left child is smaller of two, right child is larger
// continue until there is only one node, which is the root. return that
node huffman_tree(priority_queue<node>& q) {
    assert(q.size());  // TODO: asserts are only in debug mode?
    if (q.size() == 1) {
        node n = q.top();
        q.pop();
        return n;
    }
    node n1 = q.top(); q.pop();
    node n2 = q.top(); q.pop();
    cout << n1.freq << " " << n2.freq << endl;
    node parent_n;
    parent_n.freq = n1.freq + n2.freq;
    parent_n.value = ' ';
    if (n1.freq < n2.freq) {
        parent_n.left = &n1;
        parent_n.right = &n2;
    } else {
        parent_n.left = &n2;
        parent_n.right = &n1;
    } // TODO: add case of value ties? 
    // but then have to define value of internal nodes
    q.push(parent_n);
    return huffman_tree(q);
}

// left node should be 0, right node should be 1 
void flatten_tree(node* n, int val, vector<int>& res) {    
    if (n->left == NULL && n->right == NULL) {
        int ind = static_cast<int>(n->value);
        cout << "char to ind" << ind << endl;
        assert(ind < 128);
        res[ind] = val; 
        cout << "success?" << endl; 
        return;
    }
    if (n->left != NULL) {
        assert(n->value != 'H');
        cout << "fff" << endl;
        flatten_tree(n->left, val << 1, res);
    }
    if (n->right != NULL) {
        assert(n->value == ' ' && n->value && "right");
        cout << "rrr" << endl;
        flatten_tree(n->right, (val << 1) + 1, res);
    }
}

int main () {
    ifstream infile("freq.txt");
    char a; int b;
    priority_queue<node> q;
    while (infile >> a >> b) {
        node n;
        n.value = a;
        n.freq = b;
        n.left = NULL;
        n.right = NULL;
        q.push(n);
    }
    node root = huffman_tree(q);
    vector<int> dict(128, 0);
    flatten_tree(&root, 0, dict);

    //cout << dict['a'] << endl; 
    return 0;
}
