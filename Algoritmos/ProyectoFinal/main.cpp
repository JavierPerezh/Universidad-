#include "SocialNetwork.h"
#include <iostream>
using namespace std;

void printVector(const vector<int> &v) {
    for (int x : v) cout << x << " ";
    cout << "\n";
}

int main() {
    SocialNetworkGraph g;

    cout << "=========================================\n";
    cout << "    PROBANDO PROYECTO RED SOCIAL (GRAFO)\n";
    cout << "=========================================\n\n";

    cout << "[1] Cargando dataset...\n";
    g.loadFromDataset();
    cout << "Carga completada.\n\n";

    cout << "Cantidad de usuarios cargados: " << g.filterByCity("").size() << "\n";
    cout << "(solo para ver si el programa no se cae)\n\n";

    cout << "=========================================\n";
    cout << "    PRUEBA: BFS desde el usuario 0\n";
    cout << "=========================================\n";

    if (!g.BFS(0).empty()) {
        cout << "BFS(0): ";
        printVector(g.BFS(0));
    } else {
        cout << "Usuario 0 no existe en el dataset.\n";
    }

    cout << "\n=========================================\n";
    cout << "    PRUEBA: DFS desde el usuario 0\n";
    cout << "=========================================\n";

    auto dfsRes = g.DFS(0);
    if (!dfsRes.empty()) {
        cout << "DFS(0): ";
        printVector(dfsRes);
    }

    cout << "\n=========================================\n";
    cout << "    PRUEBA: Camino más corto 0 -> 10\n";
    cout << "=========================================\n";

    auto path = g.shortestPath(0, 10);
    if (!path.empty()) {
        cout << "Shortest path 0 to 10: ";
        printVector(path);
    } else {
        cout << "No existe camino entre 0 y 10.\n";
    }

    cout << "\n=========================================\n";
    cout << "    PRUEBA: Amigos en común (0, 1)\n";
    cout << "=========================================\n";

    auto mutual = g.mutualFriends(0, 1);
    cout << "Mutual friends entre 0 y 1: ";
    printVector(mutual);

    cout << "\n=========================================\n";
    cout << "    FILTROS (ciudad y trabajo)\n";
    cout << "=========================================\n";

    auto bogota = g.filterByCity("Bogota");
    cout << "Usuarios en Bogotá: ";
    printVector(bogota);

    auto engineers = g.filterByJob("Ingeniero");
    cout << "Usuarios Ingenieros: ";
    printVector(engineers);

    cout << "\n=========================================\n";
    cout << "    ORDENAR POR GRADO (popularidad)\n";
    cout << "=========================================\n";

    auto top = g.sortByDegree(true);
    cout << "Top usuarios con más amigos: ";
    printVector(top);

    cout << "\n=========================================\n";
    cout << "    PRUEBA: areConnected(u, v)\n";
    cout << "=========================================\n";

    cout << "¿0 y 1 están conectados? ";
    cout << (g.areConnected(0, 1) ? "Sí" : "No") << "\n";

    cout << "\n=========================================\n";
    cout << "    PRUEBA: agregar usuario y conexión manualmente\n";
    cout << "=========================================\n";

    User nuevo(9999, "Juan Pérez", "Bogota", "Ingeniero");
    g.addUser(nuevo);
    g.addEdge(9999, 0);

    cout << "Conexiones de 9999 después de añadir conexión con 0: ";
    printVector(g.BFS(9999));

    cout << "\n=========================================\n";
    cout << "    PRUEBA: remover arista\n";
    cout << "=========================================\n";

    g.removeEdge(9999, 0);
    cout << "¿9999 y 0 conectados después de eliminar arista? ";
    cout << (g.areConnected(9999, 0) ? "Sí" : "No") << "\n";

    cout << "\n=========================================\n";
    cout << "    PRUEBA: remover usuario\n";
    cout << "=========================================\n";

    g.removeUser(9999);
    cout << "¿Usuario 9999 existe? ";
    cout << (g.areConnected(9999, 0) ? "Sí" : "No") << " (debería ser No)\n";

    cout << "\n=========================================\n";
    cout << "          TODAS LAS PRUEBAS COMPLETADAS\n";
    cout << "=========================================\n";

    return 0;
}
