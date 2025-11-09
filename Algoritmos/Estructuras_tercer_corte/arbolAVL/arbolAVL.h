#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Node {
public:
    T data;
    Node* left;
    Node* right;
    int height;

    Node() : data(0), left(nullptr), right(nullptr), height(0){}
    Node(T v) : data(v), left(nullptr), right(nullptr), height(0){}
};

template <typename T>
class arbolAVL {
private:
    Node<T>* root;

    int height(Node<T>* r)
    {
        if(r) return r -> height;
        else return -1;
    }

    int balanceFactor(Node<T>* pseudoRoot) 
    {
        if(!pseudoRoot) return 0;
        return height(pseudoRoot -> left) - height(pseudoRoot -> right);
    }

    Node<T>* simpleLeftRotation(Node<T>* r) 
    {
        Node<T>* newRoot = r -> right;
        Node<T>* t2 = newRoot -> left;
        newRoot -> left = r;
        r -> right = t2;

        r -> height = 1 + max(height(r -> left), height(r -> right));
        newRoot -> height = 1 + max(height(newRoot -> left), height(newRoot -> right));
        
        return newRoot;
    }

    Node<T>* simpleRightRotation(Node<T>* r) 
    {
        Node<T>* newRoot = r -> left;
        Node<T>* t2 = newRoot -> right;
        newRoot -> right = r;
        r -> left = t2;

        r -> height = 1 + max(height(r -> left), height(r -> right));
        newRoot -> height = 1 + max(height(newRoot -> left), height(newRoot -> right));
        
        return newRoot;
    }

    Node<T>* doubleLeftRightRotation(Node<T>* r) 
    {
        r -> left = simpleLeftRotation(r -> left);
        return simpleRightRotation(r);
    }

    Node<T>* doubleRightLeftRotation(Node<T>* r) 
    {
        r -> right = simpleRightRotation(r -> right);
        return simpleLeftRotation(r);
    }

    Node<T>* rebalance(Node<T>* r)
    {
        int bf = balanceFactor(r), bf2;
        if(bf > 1) 
        {
            bf2 = balanceFactor(r -> left);
            if (bf2 >= 0)
            {
                return simpleRightRotation(r);
            }
            else
            {
                return doubleLeftRightRotation(r);
            }
        }
        else if (bf < -1)
        {
            bf2 = balanceFactor(r -> right);
            if(bf2 >= 0)
            {
                return doubleRightLeftRotation(r);
            }
            else
            {
                return simpleLeftRotation(r);
            }
        }
        return r;
    }

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

        pseudoRoot -> height = 1 + max(height(pseudoRoot -> left), height(pseudoRoot -> right));
        return rebalance(pseudoRoot);
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

    Node<T>* minNode(Node<T>* pseudoRoot) 
    {
        Node<T>* current = pseudoRoot;
        while(current -> left != nullptr)
        {
            current = current -> left;
        }
        return current;
    }

    Node<T>* maxNode(Node<T>* pseudoRoot)
    {
        Node<T>* current = pseudoRoot;
        while (current -> right)
        {
            current = current -> right;
        }
        return current;
    }

    Node<T>* removeNode(Node<T>* pseudoRoot, T value)
    {
        if (pseudoRoot == nullptr) return pseudoRoot;

        if (value < pseudoRoot -> data)
        {
            pseudoRoot -> left = removeNode(pseudoRoot -> left, value);
        }
        else if(value > pseudoRoot -> data)
        {
            pseudoRoot -> right = removeNode(pseudoRoot -> right, value);
        }
        else
        {
            if (!(pseudoRoot -> left) && !(pseudoRoot -> right)) 
            {
                delete pseudoRoot;
                return nullptr;
            }
            else if (pseudoRoot -> left && pseudoRoot -> right)
            {
                Node<T>* min = minNode(pseudoRoot -> right);
                pseudoRoot -> data = min -> data;
                pseudoRoot -> right = removeNode(pseudoRoot -> right, min -> data);
                return pseudoRoot;
            }
            else
            {
                Node<T>* child = (pseudoRoot -> left) ? pseudoRoot -> left : pseudoRoot -> right;
                delete pseudoRoot;
                return child;
            }
        }

        pseudoRoot -> height = 1 + max(height(pseudoRoot -> left), height(pseudoRoot -> right));
        pseudoRoot = rebalance(pseudoRoot);
        return pseudoRoot;
    }

    void inOrderNode(Node<T>* pseudoRoot, vector<T>& vec)
    {
        if(!pseudoRoot) return;
        inOrderNode(pseudoRoot -> left, vec);
        vec.push_back(pseudoRoot -> data);
        inOrderNode(pseudoRoot -> right, vec);
    }   

    void preOrderNode(Node<T>* pseudoRoot, vector<T>& vec)
    {
        if(!pseudoRoot) return;
        vec.push_back(pseudoRoot -> data);
        preOrderNode(pseudoRoot -> left, vec);
        preOrderNode(pseudoRoot -> right, vec);
    }  

    void postOrderNode(Node<T>* pseudoRoot, vector<T>& vec)
    {
        if(!pseudoRoot) return;
        postOrderNode(pseudoRoot -> left, vec);
        postOrderNode(pseudoRoot -> right, vec);
        vec.push_back(pseudoRoot -> data);
    }  

public:
    arbolAVL() : root(nullptr){}
    arbolAVL(Node<T>* r) : root(r){}

    void insert(T value) {
        root = insertNode(root, value);
    }

    void remove(T value) {
        root = removeNode(root, value);
    }

    Node<T>* find(T value) {
        return searchNode(root, value);
    }

    vector<T> inorder()
    {
        vector<T> v;
        inOrderNode(root, v);
        int size = v.size();
        for(int i = 0; i < size; i++)
        {
            if(i == size - 1)
            {
                std::cout << v[i] << "." << endl;
            }
            else
            {
                std::cout << v[i] << ",";
            }
        }
        return v;
    }

    vector<T> preorder()
    {
        vector<T> v;
        preOrderNode(root, v);
        int size = v.size();
        for(int i = 0; i < size; i++)
        {
            if(i == size - 1)
            {
                std::cout << v[i] << "." << endl;
            }
            else
            {
                std::cout << v[i] << ",";
            }
        }
        return v;
    }

    vector<T> postorder()
    {
        vector<T> v;
        postOrderNode(root, v);
        int size = v.size();
        for(int i = 0; i < size; i++)
        {
            if(i == size - 1)
            {
                std::cout << v[i] << "." << endl;
            }
            else
            {
                std::cout << v[i] << ",";
            }
        }
        return v;
    }

    T maxAVL()
    {
        Node<T>* maNode = maxNode(root);
        return maNode -> data;
    }

    T minAVL()
    {
        Node<T> miNode = minNode(root);
        return miNode -> data;
    }
};