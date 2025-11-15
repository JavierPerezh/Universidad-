#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

//Definicion de la clase User (Nodos del grafo) y SocialNetworkGraph (Grafo de la red social)
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
    unordered_map<int, User *> users; // Mapa de ID de usuario a objeto User
    unordered_map<int, vector<int>> adj; // Lista de adyacencia
    string gData = "graphData.txt", uData = "userData.csv"; // Dataset y userData

public:

    // Constructor y destructor
    SocialNetworkGraph()
    {
        loadFromDataset();
    }

    ~SocialNetworkGraph()
    {
        for (auto &p : users)
            delete p.second;
    }
    
    // Cargar datos desde dataset
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

        //Leer datos de usuarios
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

        //Leer datos de conexiones
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

    //Metodos del grafo
    //Agregar usuario (nodo)
    void addUser(User &user)
    {
        if (users.count(user.id))
            return;
        users[user.id] = new User(user.id, user.name, user.city, user.job);
    }

    //Eliminar usuario (nodo)
    void removeUser(int id)
    {
        if (!users.count(id))
            return;

        for (int other : adj[id])
            removeEdge(other, id);

        adj.erase(id);
        delete users[id];
        users.erase(id);
    }

    //Agregar conexion (arista)
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

    //Eliminar conexion (arista)
    void removeEdge(int u, int v)
    {
        auto &Uf = adj[u];
        auto &Vf = adj[v];

        Uf.erase(remove(Uf.begin(), Uf.end(), v), Uf.end());
        Vf.erase(remove(Vf.begin(), Vf.end(), u), Vf.end());
    }

    //Verificar si dos usuarios estan conectados
    bool areConnected(int u, int v) const
    {
        if (!adj.count(u))
            return false;
        return find(adj.at(u).begin(), adj.at(u).end(), v) != adj.at(u).end();
    }

    //Recorrido BfS para encontrar todos los usuarios conectados a un usuario dado
    vector<int> BFS(int start)
    {
        if (!users.count(start))
            return {};

        unordered_map<int, bool> visited;
        vector<int> res;
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while (!q.empty())
        {
            int curr = q.front();
            q.pop();
            res.push_back(curr);

            for (int neighbor : adj.at(curr))
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }
        return res;
    }

    //Recorrido DFS para encontrar todos los usuarios conectados a un usuario dado
    void DFSUtil(int node, vector<bool> &visited, vector<int> &res) const
    {
        visited[node] = true;
        res.push_back(node);

        if (!adj.count(node))
            return;

        for (int neighbor : adj.at(node))
        {
            if (!visited[neighbor])
                DFSUtil(neighbor, visited, res);
        }
    }

    // Interfaz DFS
    vector<int> DFS(int start) const
    {
        if (!users.count(start))
            return {};

        vector<bool> visited(200000, false);
        vector<int> res;

        DFSUtil(start, visited, res);
        return res;
    }

    // Encontrar el camino mas corto entre dos usuarios usando BFS
    vector<int> shortestPath(int start, int target) const
    {
        if (!users.count(start) || !users.count(target))
            return {};

        unordered_map<int, bool> visited;
        unordered_map<int, int> prev;
        queue<int> q;

        q.push(start);
        visited[start] = true;

        while (!q.empty())
        {
            int curr = q.front();
            q.pop();

            if (curr == target)
                break;

            for (int next : adj.at(curr))
            {
                if (!visited[next])
                {
                    visited[next] = true;
                    prev[next] = curr;
                    q.push(next);
                }
            }
        }

        if (!visited[target])
            return {};

        vector<int> path;
        for (int at = target; at != start; at = prev[at])
            path.push_back(at);
        path.push_back(start);

        reverse(path.begin(), path.end());
        return path;
    }

    // Encontrar amigos mutuos entre dos usuarios
    vector<int> mutualFriends(int a, int b) const
    {
        vector<int> res;
        if (!adj.count(a) || !adj.count(b))
            return res;

        const auto &A = adj.at(a);
        const auto &B = adj.at(b);

        set_intersection(
            A.begin(), A.end(),
            B.begin(), B.end(),
            back_inserter(res));

        return res;
    }

    // Filtrar usuarios por ciudad
    vector<int> filterByCity(const string &city) const
    {
        vector<int> result;
        for (auto &p : users)
            if (p.second->city == city)
                result.push_back(p.first);
        return result;
    }

    // Filtrar usuarios por trabajo
    vector<int> filterByJob(const string &job) const
    {
        vector<int> result;
        for (auto &p : users)
            if (p.second->job == job)
                result.push_back(p.first);
        return result;
    }

    // Ordenar usuarios por grado (numero de conexiones)
    vector<int> sortByDegree(bool desc) const
    {
        vector<pair<int, int>> degrees;

        for (auto &p : adj)
            degrees.push_back({p.first, (int)p.second.size()});

        sort(degrees.begin(), degrees.end(),
             [&](auto &a, auto &b)
             {
                 return desc ? a.second > b.second : a.second < b.second;
             });

        vector<int> res;
        for (auto &p : degrees)
            res.push_back(p.first);
        return res;
    }
};
