#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int   data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};

class BST {
public:
    Node* root;
    BST() : root(nullptr) {}
    ~BST() { destroyTree(root); }

    void destroyTree(Node* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    Node* insert(Node* node, int val) {
        if (!node) return new Node(val);
        if (val < node->data)      node->left  = insert(node->left,  val);
        else if (val > node->data) node->right = insert(node->right, val);
        return node;
    }

    bool search(Node* node, int val) {
        if (!node) return false;
        if (node->data == val) return true;
        return (val < node->data) ? search(node->left, val) : search(node->right, val);
    }

    int height(Node* node) {
        if (!node) return 0;
        return 1 + max(height(node->left), height(node->right));
    }

    int minVal(Node* node) {
        while (node->left) node = node->left;
        return node->data;
    }

    int maxVal(Node* node) {
        while (node->right) node = node->right;
        return node->data;
    }

    void inorder(Node* node, vector<int>& out) {
        if (!node) return;
        inorder(node->left, out);
        out.push_back(node->data);
        inorder(node->right, out);
    }

    void preorder(Node* node, vector<int>& out) {
        if (!node) return;
        out.push_back(node->data);
        preorder(node->left, out);
        preorder(node->right, out);
    }

    void postorder(Node* node, vector<int>& out) {
        if (!node) return;
        postorder(node->left, out);
        postorder(node->right, out);
        out.push_back(node->data);
    }

    Node* minNode(Node* node) {
        if (!node->left) return node;
        return minNode(node->left);
    }

    Node* deleteNode(Node* node, int val) {
        if (!node) return nullptr;
        if      (val < node->data) node->left  = deleteNode(node->left,  val);
        else if (val > node->data) node->right = deleteNode(node->right, val);
        else {
            if (!node->left)  { Node* tmp = node->right; delete node; return tmp; }
            if (!node->right) { Node* tmp = node->left;  delete node; return tmp; }
            Node* succ  = minNode(node->right);
            node->data  = succ->data;
            node->right = deleteNode(node->right, succ->data);
        }
        return node;
    }

    void inorderSilent(Node* node) {
        if (!node) return;
        inorderSilent(node->left);
        volatile int temp = node->data;
        inorderSilent(node->right);
    }

    bool isEmpty() { return root == nullptr; }
};

void printVec(const vector<int>& v) {
    for (int i = 0; i < (int)v.size(); i++)
        cout << v[i] << (i + 1 < (int)v.size() ? " " : "\n");
}

void separator(char c = '-', int w = 64) {
    cout << string(w, c) << "\n";
}

void section(const string& title) {
    cout << "\n";
    separator('=', 64);
    cout << "  " << title << "\n";
    separator('=', 64);
}

void runPerformanceTest() {
    const int SETS = 5;
    const int SIZE = 100;

    srand((unsigned)time(nullptr));

    double insertTimes[SETS];
    double traverseTimes[SETS];

    section("PERFORMANCE TEST  (5 sets x 100 random numbers)");

    cout << left
         << setw(8)  << "Set #"
         << setw(28) << "Time for Insertions (us)"
         << setw(28) << "Time for Inorder Trav. (us)"
         << "\n";
    separator();

    for (int s = 0; s < SETS; s++) {
        int arr[SIZE];
        for (int i = 0; i < SIZE; i++) arr[i] = rand() % 1000 + 1;

        BST bst;

        auto t1 = chrono::high_resolution_clock::now();
        for (int i = 0; i < SIZE; i++)
            bst.root = bst.insert(bst.root, arr[i]);
        auto t2 = chrono::high_resolution_clock::now();
        insertTimes[s] = chrono::duration<double, micro>(t2 - t1).count();

        auto t3 = chrono::high_resolution_clock::now();
        bst.inorderSilent(bst.root);
        auto t4 = chrono::high_resolution_clock::now();
        traverseTimes[s] = chrono::duration<double, micro>(t4 - t3).count();

        cout << left
             << setw(8)  << s + 1
             << setw(28) << fixed << setprecision(3) << insertTimes[s]
             << setw(28) << fixed << setprecision(3) << traverseTimes[s]
             << "\n";
    }

    double avgInsert = 0, avgTraverse = 0;
    for (int s = 0; s < SETS; s++) {
        avgInsert   += insertTimes[s];
        avgTraverse += traverseTimes[s];
    }
    avgInsert   /= SETS;
    avgTraverse /= SETS;

    separator();
    cout << left
         << setw(8)  << "Average"
         << setw(28) << fixed << setprecision(3) << avgInsert
         << setw(28) << fixed << setprecision(3) << avgTraverse
         << "\n";
    separator('=', 64);
}

void runDemo() {
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int n = sizeof(values) / sizeof(values[0]);

    BST bst;
    for (int i = 0; i < n; i++)
        bst.root = bst.insert(bst.root, values[i]);

    section("INSERT");
    cout << "  Inserted: ";
    for (int i = 0; i < n; i++) cout << values[i] << (i+1<n?" ":"\n");

    section("TREE INFO");
    cout << "  Height  : " << bst.height(bst.root) << "\n";
    cout << "  Min     : " << bst.minVal(bst.root)  << "\n";
    cout << "  Max     : " << bst.maxVal(bst.root)  << "\n";

    section("SEARCH");
    int searchTargets[] = {40, 99, 10, 55};
    for (int t : searchTargets)
        cout << "  Search(" << setw(3) << t << ") -> "
             << (bst.search(bst.root, t) ? "FOUND" : "NOT FOUND") << "\n";

    section("TRAVERSALS");
    vector<int> out;

    out.clear(); bst.inorder(bst.root, out);
    cout << "  Inorder   : "; printVec(out);

    out.clear(); bst.preorder(bst.root, out);
    cout << "  Preorder  : "; printVec(out);

    out.clear(); bst.postorder(bst.root, out);
    cout << "  Postorder : "; printVec(out);

    section("DELETE");
    int deleteTargets[] = {20, 50, 80};
    for (int d : deleteTargets) {
        cout << "  Deleting " << d << "...\n";
        bst.root = bst.deleteNode(bst.root, d);
        cout << "  Tree after: ";
        out.clear(); bst.inorder(bst.root, out);
        printVec(out);
    }
}

int main() {
    runPerformanceTest();
    runDemo();
    cout << "\n";
    return 0;
}