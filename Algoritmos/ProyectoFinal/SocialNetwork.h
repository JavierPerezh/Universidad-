#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

// Definición de la clase User (Nodos del grafo) y SocialNetworkGraph (Grafo de la red social)
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
    unordered_map<int, User *> users;                       // Mapa de ID de usuario a objeto User
    unordered_map<int, vector<int>> adj;                    // Lista de adyacencia
    string gData = "data/graphData.txt", uData = "data/userData.csv"; // Dataset y userData

public:
    // Constructor y destructor
    SocialNetworkGraph()
    {
        loadFromDataset();
    }

    ~SocialNetworkGraph()
    {
        // Liberar memoria de todos los usuarios
        for (auto &p : users)
        {
            delete p.second;
        }
    }

    /*
    IMPORTANTE: El data set usado es de la base de datos SNAP de stanford https://snap.stanford.edu/data/egonets-Facebook.html, cuyo archivo facebook_combined.txt solo
    contiene IDs y sus conexiones de la forma:
                                                0 1
                                                0 2
                                                1 3
                                                ...
    para no dejar cada usuario con tan poca información, se uso un script de python que genera un archivo .csv con la informacion de id, nombre, ciudad y profesión para todos
    los nodos del grafo. En la entrega tambien se inclute el script usado
    */

    // Cargar datos desde dataset
    void loadFromDataset()
    {
        ifstream graphData(gData);
        ifstream userData(uData);

        // Verificar si los archivos se abrieron correctamente
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

        // Leer datos de usuarios desde el archivo CSV
        while (getline(userData, line))
        {
            // Saltar líneas vacías
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

        // Leer datos de conexiones desde el archivo de grafo
        int u, v;
        while (graphData >> u >> v)
        {
            // Verificar que ambos usuarios existan antes de agregar la conexión
            if (!users.count(u) || !users.count(v))
            {
                cout << "Advertencia: conexion con usuario inexistente: " << u << " - " << v << endl;
                continue;
            }

            addEdge(u, v);
        }
    }

    // FUNCIONES BÁSICAS DEL GRAFO

    // Agregar usuario (nodo)
    void addUser(User &user)
    {
        // Verificar si el usuario ya existe
        if (users.count(user.id))
            return;
        users[user.id] = new User(user.id, user.name, user.city, user.job);
    }

    // Eliminar usuario (nodo)
    void removeUser(int id)
    {
        // Verificar si el usuario existe
        if (!users.count(id))
            return;

        // Eliminar todas las conexiones del usuario
        for (int other : adj[id])
        {
            removeEdge(other, id);
        }

        // Eliminar el usuario de las estructuras de datos
        adj.erase(id);
        delete users[id];
        users.erase(id);
    }

    // Agregar conexion (arista)
    void addEdge(int u, int v)
    {
        // No permitir conexiones consigo mismo
        if (u == v)
            return;
        // Verificar que ambos usuarios existan
        if (!users.count(u) || !users.count(v))
            return;

        auto &Uf = adj[u];
        auto &Vf = adj[v];

        // Verificar si la conexión ya existe
        if (find(Uf.begin(), Uf.end(), v) != Uf.end())
            return;

        // Insertar manteniendo la lista ordenada (para intersección eficiente después)
        Uf.insert(lower_bound(Uf.begin(), Uf.end(), v), v);
        Vf.insert(lower_bound(Vf.begin(), Vf.end(), u), u);
    }

    // Eliminar conexion (arista)
    void removeEdge(int u, int v)
    {
        auto &Uf = adj[u];
        auto &Vf = adj[v];

        // Eliminar v de la lista de adyacencia de u
        Uf.erase(remove(Uf.begin(), Uf.end(), v), Uf.end());
        // Eliminar u de la lista de adyacencia de v
        Vf.erase(remove(Vf.begin(), Vf.end(), u), Vf.end());
    }

    // FUNCIONES DE BÚSQUEDA Y CONEXIONES

    // Verificar si dos usuarios estan conectados
    bool areConnected(int u, int v) const
    {
        // Verificar si u existe en el grafo
        if (!adj.count(u))
            return false;
        // Buscar v en la lista de adyacencia de u
        return find(adj.at(u).begin(), adj.at(u).end(), v) != adj.at(u).end();
    }

    // Recorrido BFS para encontrar todos los usuarios conectados a un usuario dado
    vector<User*> BFS(int start)
    {
        // Verificar si el usuario inicial existe
        if (!users.count(start))
            return {};

        unordered_map<int, bool> visited;
        vector<User*> res;
        queue<int> q;

        q.push(start);
        visited[start] = true;

        // Procesar todos los nodos en el orden FIFO de la cola
        while (!q.empty())
        {
            int curr = q.front();
            q.pop();
            res.push_back(users.at(curr));

            // Visitar todos los vecinos no visitados
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

    // Función auxiliar recursiva para DFS
    void DFSUtil(int node, unordered_map<int, bool> &visited, vector<User*> &res) const
    {
        visited[node] = true;
        res.push_back(users.at(node));

        // Verificar si el nodo tiene vecinos
        if (!adj.count(node))
            return;

        // Visitar recursivamente todos los vecinos no visitados
        for (int neighbor : adj.at(node))
        {
            if (!visited[neighbor])
                DFSUtil(neighbor, visited, res);
        }
    }

    // Interfaz DFS principal
    vector<User*> DFS(int start) const
    {
        // Verificar si el usuario inicial existe
        if (!users.count(start))
            return {};

        unordered_map<int, bool> visited;
        vector<User*> res;

        DFSUtil(start, visited, res);
        return res;
    }

    // FUNCIONES AVANZADAS CON OBJETOS USER COMPLETOS

    // Obtener usuario por ID
    User* getUser(int id) const {
        auto it = users.find(id);
        return (it != users.end()) ? it->second : nullptr;
    }

    // Encontrar el camino mas corto entre dos usuarios usando BFS
    vector<User*> shortestPath(int start, int target) const
    {
        // Verificar que ambos usuarios existan
        if (!users.count(start) || !users.count(target))
            return {};

        unordered_map<int, bool> visited;
        unordered_map<int, int> prev;
        queue<int> q;

        q.push(start);
        visited[start] = true;

        // BFS para encontrar el camino más corto
        while (!q.empty())
        {
            int curr = q.front();
            q.pop();

            // Si encontramos el objetivo, terminar la búsqueda
            if (curr == target)
                break;

            // Explorar todos los vecinos
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

        // Si no se encontró camino, retornar vacío
        if (!visited[target])
            return {};

        // Reconstruir el camino desde target hasta start
        vector<int> pathIds;
        for (int at = target; at != start; at = prev[at])
            pathIds.push_back(at);
        pathIds.push_back(start);

        reverse(pathIds.begin(), pathIds.end());
        
        // Convertir IDs a objetos User completos
        vector<User*> pathUsers;
        for (int id : pathIds) {
            pathUsers.push_back(users.at(id));
        }
        return pathUsers;
    }

    // Encontrar amigos mutuos entre dos usuarios
    vector<User*> mutualFriends(int a, int b) const
    {
        vector<User*> res;
        // Verificar que ambos usuarios existan y tengan conexiones
        if (!adj.count(a) || !adj.count(b))
            return res;

        const auto &A = adj.at(a);
        const auto &B = adj.at(b);

        // Usar intersección de conjuntos ordenados
        vector<int> mutualIds;
        set_intersection(
            A.begin(), A.end(),
            B.begin(), B.end(),
            back_inserter(mutualIds));

        // Convertir IDs a objetos User
        for (int id : mutualIds) {
            res.push_back(users.at(id));
        }
        return res;
    }

    // Filtrar usuarios por ciudad (devuelve Users completos)
    vector<User*> filterByCity(const string &city) const
    {
        vector<User*> result;
        // Iterar sobre todos los usuarios
        for (auto &p : users)
        {
            // Si la ciudad coincide, agregar a resultados
            if (p.second->city == city)
                result.push_back(p.second);
        }
        return result;
    }

    // Filtrar usuarios por trabajo
    vector<User*> filterByJob(const string &job) const
    {
        vector<User*> result;
        // Iterar sobre todos los usuarios
        for (auto &p : users)
        {
            // Si el trabajo coincide, agregar a resultados
            if (p.second->job == job)
                result.push_back(p.second);
        }
        return result;
    }

    // Ordenar usuarios por grado devuelve pares 
    vector<pair<User*, int>> sortByDegree(bool desc) const
    {
        vector<pair<int, int>> degrees;

        // Recolectar grados de todos los usuarios
        for (auto &p : adj)
            degrees.push_back({p.first, (int)p.second.size()});

        // Ordenar por grado (ascendente o descendente)
        sort(degrees.begin(), degrees.end(),
             [&](auto &a, auto &b)
             {
                 return desc ? a.second > b.second : a.second < b.second;
             });

        vector<pair<User*, int>> res;
        // Convertir a objetos User con sus grados
        for (auto &p : degrees)
            res.push_back({users.at(p.first), p.second});
        return res;
    }

    // Recomendar amigos basado en amigos en común
    vector<User*> recommendFriends(int userId) const
    {
        unordered_map<int, int> commonFriendsCount;

        if (!adj.count(userId))
            return {};

        // Para cada amigo de mis amigos
        for (int friendId : adj.at(userId))
        {
            for (int friendOfFriend : adj.at(friendId))
            {
                // Si no es yo mismo y no es ya mi amigo
                if (friendOfFriend != userId &&
                    !areConnected(userId, friendOfFriend))
                {
                    commonFriendsCount[friendOfFriend]++;
                }
            }
        }

        // Ordenar por número de amigos en común
        vector<pair<int, int>> recommendations(commonFriendsCount.begin(),
                                               commonFriendsCount.end());
        sort(recommendations.begin(), recommendations.end(),
             [](auto &a, auto &b)
             { return a.second > b.second; });

        vector<User*> result;
        // Convertir a objetos User
        for (auto &rec : recommendations)
        {
            result.push_back(users.at(rec.first));
        }

        return result;
    }

    // FUNCIONES DE UTILIDAD

    // Obtener todos los usuarios
    vector<User*> getAllUsers() const {
        vector<User*> allUsers;
        for (auto& pair : users) {
            allUsers.push_back(pair.second);
        }
        return allUsers;
    }

    // Obtener conexiones de un usuario
    vector<User*> getUserConnections(int userId) const {
        vector<User*> connections;
        if (!adj.count(userId)) return connections;
        
        for (int friendId : adj.at(userId)) {
            connections.push_back(users.at(friendId));
        }
        return connections;
    }
};