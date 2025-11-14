#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

class User
{
public:
    int id;
    string name;
    string city;
    string job;

    User(int id, const string &name, const string &city, const string &job)
        : id(id), name(name), city(city), job(job) {}
};

class SocialNetworkGraph
{
private:
    unordered_map<int, User *> users;
    unordered_map<int, vector<int>> adj;
    string gData = "graphData.txt", uData = "userData.csv"; //gData: conexiones entre nodos, uData: informacion de cada usuario (sin conexiones)

public:

    /*
    IMPORTANTE: El data set usado es de la base de datos SNAP de stanford https://snap.stanford.edu/data/egonets-Facebook.html, cuyo archivo facebook_combined.txt solo 
    contiene IDs y su conexion de la forma: 
                                                0 1
                                                0 2
                                                1 10
                                                ...
    para no dejar cada usuario con tan poca información, se uso un script de python que genera un archivo .csv con la informacion de nombre, ciudad y profesión para todos
    los nodos del grafo. 
    */

    void loadFromDataset()
    {
        ifstream graphData(gData);
        ifstream userData(uData);

        if (!graphData.is_open())
        {
            cerr << "No se pudo abrir el archivo - '" << gData << "'" << endl;
            return;
        }
        if (!userData.is_open())
        {
            cerr << "No se pudo abrir el archivo - '" << uData << "'" << endl;
            return;
        }

        string line;

        while (getline(userData, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);

            int id;
            string name, city, job;

            string idStr;
            getline(ss, idStr, ',');
            id = stoi(idStr);

            getline(ss, name, ',');
            getline(ss, city, ',');
            getline(ss, job, ',');

            User u(id, name, city, job);
            addUser(u);
        }

        int u, v;
        while (graphData >> u >> v)
        {

            if (!users.count(u) || !users.count(v))
            {
                cout << "Advertencia: conexion con usuario inexistente: " << u << " - " << v << endl;
                continue;
            }

            addEdge(u, v);
        }
    }

    void addUser(User &user)
    {
        if (users.count(user.id))
            return;
        users[user.id] = new User(user.id, user.name, user.city, user.job);
    }

    void addEdge(int u, int v)
    {
        if (u == v)
            return;
        if (!users.count(u) || !users.count(v))
            return;

        auto &Uf = adj[u];
        auto &Vf = adj[v];

        if (find(Uf.begin(), Uf.end(), v) != Uf.end())
            return;

        Uf.insert(lower_bound(Uf.begin(), Uf.end(), v), v);
        Vf.insert(lower_bound(Vf.begin(), Vf.end(), u), u);
    }
};