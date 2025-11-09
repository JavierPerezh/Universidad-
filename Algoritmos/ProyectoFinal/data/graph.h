#include <iostream> 
#include <vector>

using namespace std;

class User {
public:
    int id;
    string name;
    vector<int>* friends;
    
    User(int i, string na) : id(i), name(na){}
};

class socialNetwork {
public:
    
};