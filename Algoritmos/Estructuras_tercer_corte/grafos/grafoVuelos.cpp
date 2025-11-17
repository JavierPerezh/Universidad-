#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <limits>
#include <algorithm>
using namespace std;

class CityNode;
class FlightArc
{
public:
    CityNode *fromCity;
    CityNode *toCity;
    float distance;
    FlightArc(CityNode *from, CityNode *to, float d) : fromCity(from), toCity(to), distance(d) {}
};

class CityNode
{
public:
    string cityName;
    vector<FlightArc *> flights;
    CityNode(string name) : cityName(name) {}
};

class AirlineGraph
{
public:
    vector<CityNode *> cities;
    vector<FlightArc *> flights;
    map<string, CityNode *> citiesMap;
    AirlineGraph() : citiesMap(), cities(), flights() {};

    void AddCity(const string &cityName)
    {
        if (citiesMap.find(cityName) == citiesMap.end())
        {
            CityNode *newCity = new CityNode(cityName);
            cities.push_back(newCity);
            citiesMap[cityName] = newCity;
        }
    }

    void AddFlight(const string &from, const string &to, float distance)
    {
        CityNode *fromCity = citiesMap[from];
        CityNode *toCity = citiesMap[to];
        if (fromCity && toCity)
        {
            FlightArc *newFlight = new FlightArc(fromCity, toCity, distance);
            flights.push_back(newFlight);
            fromCity->flights.push_back(newFlight);
        }
    }
    void Display()
    {
        for (const auto &city : cities)
        {
            cout << "City: " << city->cityName << endl;
            for (const auto &flight : city->flights)
            {
                cout << "->Flight to " << flight->toCity->cityName << ". Distance " << flight->distance << endl;
            }
        }
    }

    void AddGraphFromMatrix(const vector<vector<double>> &adjMatrix, const vector<string> &citiesNames)
    {
        for (const auto &city : citiesNames)
        {
            AddCity(city);
        }
        for (int i = 0; i < adjMatrix.size(); i++)
        {
            for (int j = 0; j < adjMatrix.size(); j++)
            {
                if (adjMatrix[i][j] > 0)
                {
                    AddFlight(cities[i]->cityName, cities[j]->cityName, adjMatrix[i][j]);
                }
            }
        }
    }

    void DepthFirstSearch(CityNode *cityRoot)
    {
        set<CityNode *> visited;
        RecursiveDFS(cityRoot, visited);
    }

    void RecursiveDFS(CityNode *cityRoot, set<CityNode *> &visited)
    {
        if (visited.find(cityRoot) != visited.end())
            return;
        cout << "Visited " << cityRoot->cityName << endl;
        visited.insert(cityRoot);
        for (const auto &flight : cityRoot->flights)
        {
            RecursiveDFS(flight->toCity, visited);
        }
    }

    void BreadthFirstSearch(CityNode *cityRoot)
    {
        set<CityNode *> visited;
        vector<CityNode *> queue;
        queue.push_back(cityRoot);
        visited.insert(cityRoot);
        while (!queue.empty())
        {
            CityNode *currentCity = queue.front();
            queue.erase(queue.begin());
            cout << "Visited " << currentCity->cityName << endl;
            for (const auto &flight : currentCity->flights)
            {
                CityNode *neighbor = flight->toCity;
                if (visited.find(neighbor) == visited.end())
                {
                    visited.insert(neighbor);
                    queue.push_back(neighbor);
                }
            }
        }
    }

    void PrimMST(CityNode *startingCity)
    {
        if (cities.empty())
        {
            cout << "No hay ciudades" << endl;
        }
        cout << "Ciudad de inicio: " << startingCity->cityName << endl;
        set<CityNode *> visited;
        vector<FlightArc *> mstEdges;
        visited.insert(startingCity);
        while (visited.size() != cities.size())
        {
            FlightArc *minArc = nullptr;
            double minDistance = 1000000;
            for (auto city : visited)
            {
                for (auto flight : city->flights)
                {
                    CityNode *neighbor = flight->toCity;
                    if (visited.find(neighbor) == visited.end() && flight->distance < minDistance)
                    {
                        minDistance = flight->distance;
                        minArc = flight;
                    }
                }
            }
            if (minArc == nullptr)
            {
                cout << "Grafo no conexo" << endl;
                break;
            }
            mstEdges.push_back(minArc);
            CityNode *newCity = minArc->toCity;
            visited.insert(newCity);

            cout << "Añadida " << newCity->cityName << endl;
        }
    }

    void Dijkstra(const string &startCity, const string &endCity)
    {
        if (citiesMap.find(startCity) == citiesMap.end() || citiesMap.find(endCity) == citiesMap.end())
        {
            cout << "No existe una ciudad" << endl;
        }
        map<CityNode *, float> distances;
        map<CityNode *, CityNode *> previous;
        set<CityNode *> visited;
        for (auto city : cities)
        {
            distances[city] = numeric_limits<float>::infinity();
            previous[city] = nullptr;
        }
        CityNode *start = citiesMap[startCity];
        CityNode *end = citiesMap[endCity];
        distances[start] = 0;
        while (visited.size() < cities.size())
        {
            CityNode *current = nullptr;
            float minDist = numeric_limits<float>::infinity();
            for (auto &pair : distances)
            {
                if (visited.find(pair.first) == visited.end() && pair.second < minDist)
                {
                    current = pair.first;
                    minDist = pair.second;
                }
            }
            if (current == nullptr || current == end)
                break;
            visited.insert(current);
            for (auto flight : current->flights)
            {
                CityNode *neighbor = flight->toCity;
                float newDist = distances[current] + flight->distance;
                if (newDist < distances[neighbor])
                {
                    distances[neighbor] = newDist;
                    previous[neighbor] = current;
                }
            }
        }

        if (distances[end] == numeric_limits<float>::infinity())
        {
            cout << "No existe camino" << endl;
            return;
        }
        vector<string> path;
        for (CityNode *at = end; at != nullptr; at = previous[at])
        {
            path.push_back(at->cityName);
        }
        reverse(path.begin(), path.end());
        cout << "Camino desde: " << startCity << " hasta " << endCity << endl;
        for (int i = 0; i < path.size(); i++)
        {
            cout << path[i] << "->";
        }
        cout << endl;
        cout << "Distancia total: " << distances[end] << endl;
    }

    float Heuristic(CityNode *current, CityNode *goal)
    {
        float minToGoal = numeric_limits<float>::infinity();

        for (auto city : cities)
        {
            for (auto flight : city->flights)
            {
                if (flight->toCity == goal)
                {
                    minToGoal = min(minToGoal, flight->distance);
                }
            }
        }

        if (minToGoal == numeric_limits<float>::infinity())
        {
            return 0.0f;
        }

        return minToGoal;
    }

    void AStar(const string &startCity, const string &endCity)
    {
        if (citiesMap.find(startCity) == citiesMap.end() ||
            citiesMap.find(endCity) == citiesMap.end())
        {
            cout << "Una de las ciudades no existe en el grafo." << endl;
            return;
        }

        CityNode *start = citiesMap[startCity];
        CityNode *goal = citiesMap[endCity];

        map<CityNode *, float> gScore;
        map<CityNode *, float> fScore;
        map<CityNode *, CityNode *> cameFrom;

        for (auto city : cities)
        {
            gScore[city] = numeric_limits<float>::infinity();
            fScore[city] = numeric_limits<float>::infinity();
            cameFrom[city] = nullptr;
        }

        gScore[start] = 0.0f;
        fScore[start] = Heuristic(start, goal);

        set<pair<float, CityNode *>> openSet;
        openSet.insert({fScore[start], start});

        while (!openSet.empty())
        {
            CityNode *current = openSet.begin()->second;
            openSet.erase(openSet.begin());

            if (current == goal)
                break;

            for (auto edge : current->flights)
            {
                CityNode *neighbor = edge->toCity;
                float tentative_g = gScore[current] + edge->distance;

                if (tentative_g < gScore[neighbor])
                {
                    cameFrom[neighbor] = current;
                    gScore[neighbor] = tentative_g;
                    fScore[neighbor] = tentative_g + Heuristic(neighbor, goal);

                    openSet.insert({fScore[neighbor], neighbor});
                }
            }
        }

        if (gScore[goal] == numeric_limits<float>::infinity())
        {
            cout << "No existe un camino entre "
                 << startCity << " y " << endCity << endl;
            return;
        }

        vector<string> path;
        for (CityNode *at = goal; at != nullptr; at = cameFrom[at])
        {
            path.push_back(at->cityName);
        }
        reverse(path.begin(), path.end());

        cout << "Camino (A*) desde " << startCity << " hasta " << endCity << ": ";
        for (auto &c : path)
        {
            cout << c;
            if (c != path.back())
                cout << " -> ";
        }
        cout << endl;

        cout << "Distancia total: " << gScore[goal] << endl;
    }
};

int main()
{
    AirlineGraph graph;
    vector<string> cities = {"Bogotá", "Manizales", "Cali", "Medellín", "Barranquilla", "Cartagena", "Pereira"};
    vector<vector<double>> adjMatrix = {{0, 200.5, 0, 0, 0, 0, 350.0}, {200.5, 0, 150.0, 0, 0, 0, 0}, {0, 150.0, 0, 300.0, 0, 0, 0}, {0, 0, 300.0, 0, 400.0, 0, 0}, {0, 0, 0, 400.0, 0, 100.0, 0}, {0, 0, 0, 0, 100.0, 0, 250.0}, {350.0, 0, 0, 0, 0, 250.0, 0}};
    graph.AddGraphFromMatrix(adjMatrix, cities);
    graph.DepthFirstSearch(graph.citiesMap["Bogotá"]);
    cout << endl;
    graph.DepthFirstSearch(graph.citiesMap["Bogotá"]);
    graph.BreadthFirstSearch(graph.citiesMap["Bogotá"]);
    cout << endl;
    graph.PrimMST(graph.citiesMap["Bogotá"]);
    /// graph.Display();
    cout << endl;
    graph.Dijkstra("Bogotá", "Cartagena");
    graph.AStar("Bogotá", "Cartagena");
}