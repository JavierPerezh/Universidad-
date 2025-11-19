#include <iostream>
#include <functional>
#include "Vector.h"
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

template <typename T>
class Node
{
public:
    T data;
    Node <T> *left;
    Node <T> *right;

    Node(T data): data(data), left(nullptr), right(nullptr){}
};

template<typename T>
class BinaryTree {
private:
    Node<T>* root;
    template<typename U>
    friend BinaryTree<U>* espejo(BinaryTree<U>* tree);
    template<typename U>
    friend int** tree_to_matrix(BinaryTree<U>* tree, int& size_out);
    
    Node<T>* insertNode(Node<T>* pseudoRoot, T value) 
    {
        if (!pseudoRoot) return new Node<T>(value);
        
        if (value < pseudoRoot -> data) 
        {
            pseudoRoot -> left = insertNode(pseudoRoot -> left, value);
        } 
        else if (value > pseudoRoot -> data) 
        {
            pseudoRoot -> right = insertNode(pseudoRoot -> right, value);
        }
        
        return pseudoRoot;
    }

    Node<T>* searchNode(Node<T>* pseudoRoot, T value) 
    {
        if (!pseudoRoot || pseudoRoot -> data == value) 
            return pseudoRoot;

        if (value < pseudoRoot -> data) 
        {
            return searchNode(pseudoRoot -> left, value);
        } 
        else 
        {
            return searchNode(pseudoRoot -> right, value);
        }
    }

    int heightNode(Node<T>* pseudoRoot) 
    {
        if (!pseudoRoot) return -1;
        return 1 + max(heightNode(pseudoRoot -> left), heightNode(pseudoRoot -> right));
    }

    int countNodes(Node<T>* pseudoRoot) 
    {
        if (!pseudoRoot) return 0;
        return 1 + countNodes(pseudoRoot -> left) + countNodes(pseudoRoot -> right);
    }

    void inOrderNode(Node<T>* pseudoRoot, Vector<T>& arr) 
    {
        if (!pseudoRoot) return;
        inOrderNode(pseudoRoot -> left, arr);
        arr.add(pseudoRoot -> data);
        inOrderNode(pseudoRoot -> right, arr);
    }

    void preOrderNode(Node<T>* pseudoRoot, Vector<T>& arr) 
    {
        if (!pseudoRoot) return;
        arr.add(pseudoRoot -> data);
        preOrderNode(pseudoRoot -> left, arr);
        preOrderNode(pseudoRoot -> right, arr);
    }

    void postOrderNode(Node<T>* pseudoRoot, Vector<T>& arr) 
    {
        if (!pseudoRoot) return;
        postOrderNode(pseudoRoot -> left, arr);
        postOrderNode(pseudoRoot -> right, arr);
        arr.add(pseudoRoot -> data);
    }

    void displayNode(Node<T>* pseudoRoot, int level = 0) 
    {
        if (pseudoRoot) 
        {
            for (int i = 0; i < level; i++) 
            {
                cout << "-";
            }
            cout << pseudoRoot -> data << endl;
            displayNode(pseudoRoot -> left, level + 1);
            displayNode(pseudoRoot -> right, level + 1);
        }
    }

    Node<T>* arrInOrder(T* arr, int inicio, int fin) 
    {
        if (inicio > fin) return nullptr;
        int medio = (inicio + fin) / 2;
        Node<T>* root = new Node<T>(arr[medio]);
        root -> left = arrInOrder(arr, inicio, medio - 1);
        root -> right = arrInOrder(arr, medio + 1, fin);
        return root;
    }

    Node<T>* arrPreOrder(T* arr, int size) 
    {
        Node<T>* root = nullptr;
        for (int i = 0; i < size; i++) {
            root = insertNode(root, arr[i]);
        }
        return root;
    }

    Node<T>* arrPostOrder(T* arr, int size) 
    {
        Node<T>* root = nullptr;
        for (int i = size - 1; i >= 0; i--) {
            root = insertNode(root, arr[i]);
        }
        return root;
    }

    void destroyTree(Node<T>* nodo) 
    {
        if (nodo == nullptr) return;
        destroyTree(nodo -> left);
        destroyTree(nodo -> right);
        delete nodo;
    }

public:
    BinaryTree() : root(nullptr) {}
    
    BinaryTree(int order, T* arr, int size) 
    {
        if (order == 0) root = arrInOrder(arr, 0, size - 1);
        else if (order == 1) root = arrPreOrder(arr, size);
        else if (order == 2) root = arrPostOrder(arr, size);
        else cerr << "Order invalido" << endl;
    }

    ~BinaryTree() {
        destroyTree(root);
    }

    void insert(T value) {
        root = insertNode(root, value);
    }

    Node<T>* search(T value) {
        return searchNode(root, value);
    }

    int height() {
        return heightNode(root);
    }

    int count() {
        return countNodes(root);
    }

    T* inOrder(int& sizeOut) {
        Vector<T> arr;
        inOrderNode(root, arr);
        sizeOut = arr.getSize();
        T* arreglo = new T[sizeOut];
        for (int i = 0; i < sizeOut; i++) {
            arreglo[i] = arr[i];
        }
        return arreglo;
    }

    T* preOrder(int& sizeOut) {
        Vector<T> arr;
        preOrderNode(root, arr);
        sizeOut = arr.getSize();
        T* arreglo = new T[sizeOut];
        for (int i = 0; i < sizeOut; i++) {
            arreglo[i] = arr[i];
        }
        return arreglo;
    }

    T* postOrder(int& sizeOut) {
        Vector<T> arr;
        postOrderNode(root, arr);
        sizeOut = arr.getSize();
        T* arreglo = new T[sizeOut];
        for (int i = 0; i < sizeOut; i++) {
            arreglo[i] = arr[i];
        }
        return arreglo;
    }

    void display(int order) {
        int sizeArr;
        T* arr;
        string orders[] = {"inOrder", "preOrder", "postOrder"};
        
        if (order == 0) arr = inOrder(sizeArr);
        else if (order == 1) arr = preOrder(sizeArr);
        else if (order == 2) arr = postOrder(sizeArr);
        else {
            cerr << "Order invalido" << endl;
            return;
        }

        cout << "Arbol de forma " << orders[order] << ": ";
        for (int i = 0; i < sizeArr; i++) {
            cout << arr[i];
            if (i < sizeArr - 1) cout << ", ";
        }
        cout << endl;
        
        delete[] arr;
    }

    void displayTree() {
        cout << "Estructura del arbol:" << endl;
        displayNode(root);
    }
};

template <typename T>
BinaryTree<T>* espejo(BinaryTree<T>* tree) {
    function<Node<T>*(Node<T>*)> espejoNode = [&](Node<T>* nodo) -> Node<T>* {
        if (!nodo) return nullptr;
        Node<T>* nuevoNodo = new Node<T>(nodo->data);
        nuevoNodo->left = espejoNode(nodo->right);
        nuevoNodo->right = espejoNode(nodo->left);
        return nuevoNodo;
    };

    BinaryTree<T>* mirroredTree = new BinaryTree<T>();
    mirroredTree->root = espejoNode(tree->root);
    return mirroredTree;
} 

// Check a square adjacency matrix of size n: for every edge i->j there must be j->i
bool is_bidirectional(int** mat, int n) {
    if (!mat) return true;
    for (int i = 0; i < n; ++i) {
        if (!mat[i]) return false; // malformed
        for (int j = 0; j < n; ++j) {
            bool a = (mat[i][j] != 0);
            bool b = (mat[j][i] != 0);
            if (a != b) return false;
        }
    }
    return true;
}

bool is_bidirectional(int** mat) {
    if (!mat) return true;
    int n = 0;
    while (mat[n]) ++n; 
    return is_bidirectional(mat, n);
}

// Return true if there exists a path from node `a` to node `b` with length <= k
// Uses BFS to compute shortest path distances (unweighted graph). Time O(n^2) for adjacency matrix.
bool is_k_jumps(int **mat, int a, int b, int k, int size) {
    if (!mat || size <= 0 || a < 0 || b < 0 || a >= size || b >= size || k < 0) return false;
    if (a == b) return true; // zero jumps needed

    vector<int> dist(size, -1); // distance (number of edges) from a
    queue<int> q;
    dist[a] = 0;
    q.push(a);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        // If current distance already equals k, we cannot expand further from u
        if (dist[u] >= k) continue;

        for (int v = 0; v < size; ++v) {
            if (mat[u][v] != 0 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                if (v == b && dist[v] <= k) return true;
                q.push(v);
            }
        }
    }

    return false;
}

int main() {
    // Build a sample tree:
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    BinaryTree<int> tree;
    tree.insert(4);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(3);
    tree.insert(5);
    tree.insert(7);

    cout << "Original tree:" << endl;
    tree.displayTree();

    // Mirror the tree using the free function
    BinaryTree<int>* mirrored = espejo<int>(&tree);

    cout << "Mirrored tree:" << endl;
    mirrored->displayTree();

    // cleanup
    delete mirrored;

    // --- tree to matrix test ---
    int size_mat;
    int** tree_mat = tree_to_matrix<int>(&tree, size_mat);
    cout << "Tree as adjacency matrix (size=" << size_mat << "):\n";
    if (tree_mat) {
        for (int i = 0; i < size_mat; ++i) {
            for (int j = 0; j < size_mat; ++j) cout << tree_mat[i][j] << " ";
            cout << "\n";
        }
        // find node with max degree (undirected degree by default)
        int maxNode = -1;
        if (size_mat > 0) {
            // function will be defined below
            maxNode = node_with_max_degree(tree_mat, size_mat, false);
            cout << "Node with max degree: " << maxNode << "\n";
        }
        for (int i = 0; i < size_mat; ++i) delete[] tree_mat[i];
        delete[] tree_mat;
    }

    return 0;
}

// Convert a binary tree into a graph adjacency matrix using BFS.
// Nodes are indexed by the order they are discovered in BFS (starting at 0).
// Returns a newly allocated n x n matrix (as int**), and sets size_out to n.
// Edges are undirected between parent and child (both directions set to 1).
template<typename T>
int** tree_to_matrix(BinaryTree<T>* tree, int& size_out) {
    size_out = 0;
    if (!tree) return nullptr;

    // BFS over tree nodes to assign indices
    unordered_map<Node<T>*, int> index;
    queue<Node<T>*> q;
    if (tree->root) {
        q.push(tree->root);
        index[tree->root] = 0;
        size_out = 1;
    }

    while (!q.empty()) {
        Node<T>* u = q.front(); q.pop();
        int idx_u = index[u];
        if (u->left && index.find(u->left) == index.end()) {
            index[u->left] = size_out++;
            q.push(u->left);
        }
        if (u->right && index.find(u->right) == index.end()) {
            index[u->right] = size_out++;
            q.push(u->right);
        }
    }

    if (size_out == 0) return nullptr;

    // allocate matrix
    int** mat = new int*[size_out];
    for (int i = 0; i < size_out; ++i) {
        mat[i] = new int[size_out];
        for (int j = 0; j < size_out; ++j) mat[i][j] = 0;
    }

    // populate edges (undirected between parent and children)
    for (auto &entry : index) {
        Node<T>* node = entry.first;
        int u = entry.second;
        if (node->left) {
            int v = index[node->left];
            mat[u][v] = 1;
            mat[v][u] = 1;
        }
        if (node->right) {
            int v = index[node->right];
            mat[u][v] = 1;
            mat[v][u] = 1;
        }
    }

    return mat;
}

// Return index of node with maximum degree in adjacency matrix.
// If directed==false, degree = outdegree (row sum) (for undirected graphs row sum equals degree).
// If directed==true, degree = outdegree + indegree.
// Returns -1 on error.
int node_with_max_degree(int** mat, int n, bool directed = false) {
    if (!mat || n <= 0) return -1;
    int best = -1;
    int bestDeg = -1;
    for (int i = 0; i < n; ++i) {
        if (!mat[i]) return -1;
        int outdeg = 0;
        for (int j = 0; j < n; ++j) {
            if (mat[i][j] != 0) ++outdeg;
        }
        int deg = outdeg;
        if (directed) {
            int indeg = 0;
            for (int j = 0; j < n; ++j) if (mat[j][i] != 0) ++indeg;
            deg = outdeg + indeg;
        }
        if (deg > bestDeg) {
            bestDeg = deg;
            best = i;
        }
    }
    return best;
}