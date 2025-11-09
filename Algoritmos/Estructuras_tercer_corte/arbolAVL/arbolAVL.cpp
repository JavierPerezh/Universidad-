#include "arbolAVL.h"

int main() 
{
    arbolAVL<int> arbolito;
    arbolito.insert(7);
    arbolito.insert(1);
    arbolito.insert(2);
    arbolito.insert(5);

    vector<int> inOrderArbolito = arbolito.inorder();
    vector<int> preOrderArbolito = arbolito.preorder();
    vector<int> postOrderArbolito = arbolito.postorder();
}