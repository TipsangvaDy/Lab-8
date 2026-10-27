// C S 2414 F26 Lab 8
// AI Policy: Any AI tools should not generate code automatically.
// Read Lab8.pdf. Write printBalance yourself. Do not use Copilot,
// ChatGPT, or any other tool to fill in the method.

#include <iostream>

using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node* left;
    Node* right;
    int height;
    Node(T val) : data(val), left(nullptr), right(nullptr), height(0) {}
};

template <typename T>
class AVLTree {
    Node<T>* root;

    void destroy(Node<T>* node);
    void inorder(Node<T>* node) const;

    int height(Node<T>* node) const;
    void updateHeight(Node<T>* node);
    int balance(Node<T>* node) const;

    Node<T>* rotateRight(Node<T>* z);
    Node<T>* rotateLeft(Node<T>* z);

    Node<T>* insert(Node<T>* node, T key);
    Node<T>* remove(Node<T>* node, T key);
    Node<T>* removeNoBalance(Node<T>* node, T key);
    Node<T>* minNode(Node<T>* node) const;

    // Write this helper. Print each node and its balance factor.
    void printBalance(Node<T>* node) const;

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    void insert(T key);
    void remove(T key);
    void removeNoBalance(T key);
    void printInorder() const;
    void printBalance() const;
    int treeHeight() const { return height(root); }
};

// ---------- definitions outside the class ----------

template <typename T>
void AVLTree<T>::destroy(Node<T>* node) {
    if (node == nullptr) return;
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <typename T>
int AVLTree<T>::height(Node<T>* node) const {
    if (node == nullptr) return -1;
    return node->height;
}

template <typename T>
void AVLTree<T>::updateHeight(Node<T>* node) {
    if (node == nullptr) return;
    int hl = height(node->left);
    int hr = height(node->right);
    node->height = 1 + (hl > hr ? hl : hr);
}

template <typename T>
int AVLTree<T>::balance(Node<T>* node) const {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

template <typename T>
Node<T>* AVLTree<T>::rotateRight(Node<T>* z) {
    Node<T>* y = z->left;
    Node<T>* t = y->right;

    y->right = z;
    z->left  = t;

    updateHeight(z);
    updateHeight(y);
    return y;
}

template <typename T>
Node<T>* AVLTree<T>::rotateLeft(Node<T>* z) {
    Node<T>* y = z->right;
    Node<T>* t = y->left;

    y->left  = z;
    z->right = t;

    updateHeight(z);
    updateHeight(y);
    return y;
}

template <typename T>
Node<T>* AVLTree<T>::insert(Node<T>* node, T key) {
    if (node == nullptr) return new Node<T>(key);

    if (key < node->data)
        node->left = insert(node->left, key);
    else if (key > node->data)
        node->right = insert(node->right, key);
    else
        return node;

    updateHeight(node);
    int bf = balance(node);

    if (bf > 1 && key < node->left->data)
        return rotateRight(node);
    if (bf < -1 && key > node->right->data)
        return rotateLeft(node);
    if (bf > 1 && key > node->left->data) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && key < node->right->data) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

template <typename T>
void AVLTree<T>::insert(T key) {
    root = insert(root, key);
}

template <typename T>
Node<T>* AVLTree<T>::remove(Node<T>* node, T key) {
    if (node == nullptr) return nullptr;

    if (key < node->data)
        node->left = remove(node->left, key);
    else if (key > node->data)
        node->right = remove(node->right, key);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            Node<T>* child = (node->left != nullptr) ? node->left : node->right;
            delete node;
            node = child;
        } else {
            Node<T>* succ = minNode(node->right);
            node->data = succ->data;
            node->right = remove(node->right, succ->data);
        }
    }

    if (node == nullptr) return nullptr;

    updateHeight(node);
    int bf = balance(node);

    if (bf > 1 && balance(node->left) >= 0)
        return rotateRight(node);
    if (bf > 1 && balance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && balance(node->right) <= 0)
        return rotateLeft(node);
    if (bf < -1 && balance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

template <typename T>
void AVLTree<T>::remove(T key) {
    root = remove(root, key);
}

// Same removal as remove, but does not rotate.
// Used only so main can print the tree after the node is gone.
template <typename T>
Node<T>* AVLTree<T>::removeNoBalance(Node<T>* node, T key) {
    if (node == nullptr) return nullptr;

    if (key < node->data)
        node->left = removeNoBalance(node->left, key);
    else if (key > node->data)
        node->right = removeNoBalance(node->right, key);
    else {
        if (node->left == nullptr || node->right == nullptr) {
            Node<T>* child = (node->left != nullptr) ? node->left : node->right;
            delete node;
            node = child;
        } else {
            Node<T>* succ = minNode(node->right);
            node->data = succ->data;
            node->right = removeNoBalance(node->right, succ->data);
        }
    }

    if (node == nullptr) return nullptr;
    updateHeight(node);
    return node;
}

template <typename T>
void AVLTree<T>::removeNoBalance(T key) {
    root = removeNoBalance(root, key);
}

template <typename T>
Node<T>* AVLTree<T>::minNode(Node<T>* node) const {
    while (node->left != nullptr) node = node->left;
    return node;
}

template <typename T>
void AVLTree<T>::inorder(Node<T>* node) const {
    if (node == nullptr) return;
    inorder(node->left);
    cout << node->data << " ";
    inorder(node->right);
}

template <typename T>
void AVLTree<T>::printInorder() const {
    inorder(root);
    cout << "\n";
}

// ---------- write these two ----------
// Write this yourself. Do not use an AI tool to fill it in.
// See Lab8.pdf for the required format.

template <typename T>
void AVLTree<T>::printBalance(Node<T>* node) const {
    // inorder: left, print data and balance factor, right
    // format: 10(0) 20(0) 25(0) 30(1) 40(0)
    // use a space after each node, including the last
}

template <typename T>
void AVLTree<T>::printBalance() const {
    printBalance(root);
    cout << "\n";
}

// ---------- demonstration ----------
// Read four trees from standard input.
// For each tree: first line is the number of keys, second line is the
// keys in insert order, third line is the key to remove.
// Do not change main.

int main() {
    for (int t = 1; t <= 4; t++) {
        int n;
        cin >> n;
        AVLTree<int> tree;
        AVLTree<int> deleted;
        for (int i = 0; i < n; i++) {
            int key;
            cin >> key;
            tree.insert(key);
            deleted.insert(key);
        }
        int removeKey;
        cin >> removeKey;

        cout << "Tree " << t << " after insertion: ";
        tree.printBalance();

        deleted.removeNoBalance(removeKey);
        cout << "Tree " << t << " after deletion: ";
        deleted.printBalance();

        tree.remove(removeKey);
        cout << "Tree " << t << " after rotation: ";
        tree.printBalance();
    }
    return 0;
}
