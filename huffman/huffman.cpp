#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

// http://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/

class node {
  public:
    int value;
    int freq;
    node* left = NULL;
    node* right = NULL;
};

struct node_f {
    bool operator()(const node* a, const node* b) const {
        return a->freq > b->freq;
    }
};

struct dictval {
    int int_value;
    int length;
};

void destroy_tree(node* n) {
    if (n != NULL) {
        destroy_tree(n->left);
        destroy_tree(n->right);
        delete n;
    }
}

// pop off two lowest frequencies and combine them into one internal node I
// I's left child is smaller of two, right child is larger
// continue until there is only one node, which is the root. return that
node* huffman_tree(vector<node*> q) {
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
    node *parent_n = new node;
    parent_n->freq = n1->freq + n2->freq;
    parent_n->value = 128;  // outside ASCII range
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
void flatten_tree(node* n, int val, int length, vector<dictval>& res) {    
    if (n->left == NULL && n->right == NULL) {
        cout << static_cast<char>(n->value) << " " << val << endl;
        res[n->value].int_value = val;
        res[n->value].length = length;
        return;
    }
    if (n->left != NULL) {
        flatten_tree(n->left, val << 1, length + 1, res);
    }
    if (n->right != NULL) {
        flatten_tree(n->right, (val << 1) + 1, length + 1, res);
    }
}


void encode (vector<dictval>& dict) {
    ifstream infile("orig.txt");
    FILE* outfile = fopen("compress.bin", "wb");
    uint64_t towrite = 0;
    int curr_bits = 0;
    char c;
    while (infile.get(c)) {
        dictval d = dict[static_cast<int>(c)];
        int val = d.int_value;
        int l = d.length;
        // write each bit of value to the uint64_t
        // increment the number of bits used, and
        // write to file if number of bits reaches 64
        while (val != 0) {
            int bit = val & 1;
            if (bit == 1) {
                towrite |= 1 << (64 - curr_bits);
            }
            val >>= 1;
            curr_bits++;
            l--;
            if (curr_bits == 64) {
                fwrite(&towrite, sizeof(uint64_t), 1, outfile);
                towrite = 0;
                curr_bits = 0;
            }
        }
        // leading 0s would get lost with just int representation
        // "write" the remaining zeros by incrementing curr_bits
        // and writing when exceeding 64 bits
        for (int i = 1; i <= l; i++) {
            curr_bits++;
            if (curr_bits == 64) {
                fwrite(&towrite, sizeof(uint64_t), 1, outfile);
                towrite = 0;
                curr_bits = 0;
            }
        }
        // cout << "what is towrite? " << towrite << endl;
    }
}

int main () {
    ifstream infile("orig.txt");
    char c;
    unordered_map<int, int> char_freq_map;
    while (infile.get(c)) {
        int key = static_cast<int>(c);
        char_freq_map[key]++;
    }
    vector<node*> node_heap;
    for (const auto cf : char_freq_map) {
        node *n = new node;
        n->value = cf.first;
        n->freq = cf.second;
        n->left = NULL;
        n->right = NULL;
        node_heap.push_back(n);
    }
    make_heap(node_heap.begin(), node_heap.end(), node_f());
    node *root = huffman_tree(node_heap);
   
    dictval base = {0, 0};
    vector<dictval> dict(128, base);
    flatten_tree(root, 0, 0, dict);
    encode (dict);
    // TODO: decode it back and check if contents are the same
    
    // clean up
    infile.close();
    destroy_tree(root);
    return 0;
}
