#include "archivos/utilidades.h"

class User
{
public:
    int id;
    string name;
    string city;
    string job;

    User(int id, string name, string city, string job) : id(id), name(name), city(city), job(job) {}
};

class SocialNetworkGraph
{
private:
    unordered_map<int, User*> users;
    unordered_map<int, vector<int>> adj;
    string gData = "graphData.txt", uData = "userData.csv";

public:
    
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
        int id = user.id;
        if (users.count(id))
            return;
        users[id] = &user;
    }

    void removeUser(int user)
    {
        if (adj.count(user) == 0)
            return;
        vector<int> &userfriends = adj[user];
        for (int other : userfriends)
        {
            removeEdge(other, user);
        }
        adj.erase(user);
        users.erase(user);
    }

    void addEdge(int u, int v)
    {
        if (u == v) return;
        if (!users.count(u)) return;
        if(!users.count(v)) return;

        vector<int> &ufriends = adj[u];
        vector<int> &vfriends = adj[v];

        if (search(ufriends, v) != -1 || search(vfriends, u) != -1) return;

        int posU = insertPos(ufriends, v);
        int posV = insertPos(vfriends, u);

        ufriends.insert(ufriends.begin() + posU, v);
        vfriends.insert(vfriends.begin() + posV, u);
    }

    void removeEdge(int u, int v)
    {
        if (u == v)
            return;
        vector<int> &ufriends = adj[u];
        vector<int> &vfriends = adj[v];

        int vpos = search(ufriends, v), upos = search(vfriends, u);
        if (vpos == -1)
            return;

        ufriends.erase(ufriends.begin() + vpos);
        vfriends.erase(vfriends.begin() + upos);
    }

    bool areConnected(int u, int v) const
    {
        if (!adj.count(u)) return false;
        if (!adj.count(v)) return false;
        return search(adj.at(u), v) != -1;
    }
};
