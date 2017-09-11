#include <fstream>
#include <queue>
#include <iostream>
#include <assert.h>
using namespace std;

// http://www.geeksforgeeks.org/greedy-algorithms-set-3-huffman-coding/

class node {
  public:
    friend bool operator<(const node &n1, const node &n2) {
        return n1.freq > n2.freq;  // for q.pop to return lowest frequency
    }
    char value;
    int freq;
    node* left;
    node* right;
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
node huffman_tree(priority_queue<node> q, int n) {
    assert(n);
    if (n == 1) {
        node n = q.top();
        q.pop();
        return n;
    }
    node n1 = q.top(); q.pop();
    node n2 = q.top(); q.pop();
    cout << n1.freq << " " << n2.freq << " " << n << endl;
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
    return huffman_tree(q, --n);
}

int main () {
    ifstream infile("freq.txt");
    char a; int b, num = 0;
    priority_queue<node> q;
    while (infile >> a >> b) {
        node n;
        n.value = a;
        n.freq = b;
        q.push(n);
        num++;
    }
    // print_queue(q);
    if (num <= 1) {
        return 0;
    }
    node root = huffman_tree(q, num);
    return 0;
}
