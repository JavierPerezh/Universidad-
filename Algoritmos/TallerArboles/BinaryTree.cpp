#include <iostream>
#include "Vector.h"

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

    // Métodos básicos
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

int main() {
    // Prueba del árbol básico
    BinaryTree<int> arbol;
    
    // Insertar elementos
    arbol.insert(50);
    arbol.insert(30);
    arbol.insert(70);
    arbol.insert(20);
    arbol.insert(40);
    arbol.insert(60);
    arbol.insert(80);
    
    cout << "=== ARBOL ORIGINAL ===" << endl;
    arbol.displayTree();
    arbol.display(0); // inOrder
    arbol.display(1); // preOrder  
    arbol.display(2); // postOrder
    cout << "Altura: " << arbol.height() << endl;
    cout << "Total nodos: " << arbol.count() << endl;

    // Prueba de constructores desde arreglos
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    cout << "\n=== ARBOLES DESDE ARREGLOS ===" << endl;
    
    BinaryTree<int> arbol1(0, arr, 10); // inOrder
    cout << "Arbol desde inOrder:" << endl;
    arbol1.displayTree();
    arbol1.display(0);
    
    BinaryTree<int> arbol2(1, arr, 10); // preOrder
    cout << "\nArbol desde preOrder:" << endl;
    arbol2.displayTree();
    arbol2.display(1);
    
    BinaryTree<int> arbol3(2, arr, 10); // postOrder
    cout << "\nArbol desde postOrder:" << endl;
    arbol3.displayTree();
    arbol3.display(2);
    
    return 0;
}