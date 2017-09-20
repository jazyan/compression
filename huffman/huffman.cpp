#include <fstream>
#include <iostream>
#include <assert.h>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

// http://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/

class node {
  public:
    char value = ' ';
    int freq = 0;
    node* left = NULL;
    node* right = NULL;
};

struct node_f{
    bool operator()(const node* a, const node* b) const {
        return a->freq > b->freq;
    }
};

// TODO: need to delete tree (traverse tree and destroy)

// pop off two lowest frequencies and combine them into one internal node I
// I's left child is smaller of two, right child is larger
// continue until there is only one node, which is the root. return that
node* huffman_tree(vector<node*> q) {
    assert(q.size());  // TODO: asserts are only in debug mode?
    if (q.size() == 1) {
        pop_heap(q.begin(), q.end()); 
        node* n = q.back(); 
        q.pop_back();
        return n;
    }
    pop_heap(q.begin(), q.end(), node_f()); 
    node *n1 = q.back(); q.pop_back();
    pop_heap(q.begin(), q.end(), node_f());
    node *n2 = q.back(); q.pop_back();
    cout << n1->freq << " " << n2->freq << endl;
    node *parent_n = new node;
    parent_n->freq = n1->freq + n2->freq;
    parent_n->value = ' ';
    if (n1->freq < n2->freq) {
        parent_n->left = n1;
        parent_n->right = n2;
    } else {
        parent_n->left = n2;
        parent_n->right = n1;
    } // TODO: add case of value ties? 
    // but then have to define value of internal nodes
    q.push_back(parent_n); push_heap(q.begin(), q.end(), node_f());
    return huffman_tree(q);
}

// left node should be 0, right node should be 1
void flatten_tree(node* n, int val, vector<int>& res) {    
    if (n->left == NULL && n->right == NULL) {
        int ind = static_cast<int>(n->value);
        cout << "char to ind " << ind << " " << val << endl;
        assert(ind < 128);
        res[ind] = val; 
        return;
    }
    if (n->left != NULL) {
        flatten_tree(n->left, val << 1, res);
    }
    if (n->right != NULL) {
        flatten_tree(n->right, (val << 1) + 1, res);
    }
}

int main () {
    ifstream infile("freq.txt");
    char a; int b;
    vector<node*> node_heap;
    while (infile >> a >> b) {
        node *n = new node;
        n->value = a;
        n->freq = b;
        n->left = NULL;
        n->right = NULL;
        node_heap.push_back(n);
    }
    make_heap(node_heap.begin(), node_heap.end(), node_f());

    node *root = huffman_tree(node_heap);
    vector<int> dict(128, 0);
    flatten_tree(root, 0, dict);
    return 0;
}
