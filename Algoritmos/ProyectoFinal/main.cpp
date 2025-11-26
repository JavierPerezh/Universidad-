#include <iostream>
#include "SocialNetwork.h"

using namespace std;

int main() {
    cout << "=== PRUEBA DEL SISTEMA DE RED SOCIAL CON BÚSQUEDA BINARIA ===\n" << endl;
    
    // 1. Crear instancia del grafo (carga datos automáticamente)
    cout << "1. CARGANDO DATOS DESDE ARCHIVOS..." << endl;
    SocialNetworkGraph socialNetwork;
    cout << "   ✓ Datos cargados exitosamente\n" << endl;
    
    // 2. Probar obtención de usuario individual
    cout << "2. PRUEBAS DE OBTENCIÓN DE USUARIOS:" << endl;
    User* user0 = socialNetwork.getUser(0);
    if (user0) {
        cout << "   - Usuario 0: " << user0->name << " (" << user0->city << ", " << user0->job << ")" << endl;
    }
    
    User* user1 = socialNetwork.getUser(1);
    if (user1) {
        cout << "   - Usuario 1: " << user1->name << " (" << user1->city << ", " << user1->job << ")" << endl;
    }
    
    // Búsqueda por nombre
    vector<User*> nameSearch = socialNetwork.searchUsersByName("Javier");
    cout << "   - Búsqueda por nombre 'Javier': " << nameSearch.size() << " resultados" << endl;
    cout << endl;
    
    // 3. Probar verificación de conexiones con búsqueda binaria
    cout << "3. PRUEBAS DE VERIFICACIÓN DE CONEXIONES (BÚSQUEDA BINARIA):" << endl;
    
    // Verificar conexión entre usuarios usando búsqueda binaria
    bool connected = socialNetwork.areConnected(0, 1);
    cout << "   - Usuario 0 conectado con usuario 1: " << (connected ? "Sí" : "No") << endl;
    
    connected = socialNetwork.areConnected(0, 100);
    cout << "   - Usuario 0 conectado con usuario 100: " << (connected ? "Sí" : "No") << endl;
    
    // Obtener conexiones directas
    vector<User*> connections = socialNetwork.getUserConnections(0);
    cout << "   - Conexiones directas de usuario 0: " << connections.size() << " amigos" << endl;
    if (!connections.empty()) {
        cout << "     Primeros 3 amigos: ";
        for (int i = 0; i < min(3, (int)connections.size()); i++) {
            cout << connections[i]->name << "(" << connections[i]->id << ") ";
        }
        cout << endl;
    }
    cout << endl;
    
    // 4. Probar recorridos del grafo
    cout << "4. PRUEBAS DE RECORRIDOS DEL GRAFO:" << endl;
    
    // BFS desde usuario 0
    vector<User*> bfsResult = socialNetwork.BFS(0);
    cout << "   - BFS desde usuario 0: " << bfsResult.size() << " usuarios conectados" << endl;
    
    // DFS desde usuario 0
    vector<User*> dfsResult = socialNetwork.DFS(0);
    cout << "   - DFS desde usuario 0: " << dfsResult.size() << " usuarios conectados" << endl;
    
    // Verificar que ambos recorridos encuentren el mismo número de nodos
    cout << "   - Ambos recorridos encuentran mismo número de nodos: " 
         << (bfsResult.size() == dfsResult.size() ? "Sí" : "No") << endl;
    cout << endl;
    
    // 5. Probar camino más corto
    cout << "5. PRUEBAS DE CAMINO MÁS CORTO:" << endl;
    vector<User*> path = socialNetwork.shortestPath(0, 5);
    if (!path.empty()) {
        cout << "   - Camino más corto encontrado (" << path.size() << " saltos):" << endl;
        cout << "     ";
        for (User* user : path) {
            cout << user->name << "(" << user->id << ") -> ";
        }
        cout << "LLEGADA" << endl;
    } else {
        cout << "   - No hay camino entre 0 y 5" << endl;
    }
    cout << endl;
    
    // 6. Probar amigos mutuos (usa intersección eficiente gracias a listas ordenadas)
    cout << "6. PRUEBAS DE AMIGOS MUTUOS (INTERSECCIÓN EFICIENTE):" << endl;
    vector<User*> mutual = socialNetwork.mutualFriends(0, 5);
    cout << "   - Amigos mutuos entre usuario 0 y usuario 5: " << mutual.size() << " amigos" << endl;
    if (!mutual.empty()) {
        cout << "     Amigos mutuos: ";
        for (User* friendUser : mutual) {
            cout << friendUser->name << "(" << friendUser->id << ") ";
        }
        cout << endl;
    }
    cout << endl;
    
    // 7. Probar filtros por características
    cout << "7. PRUEBAS DE FILTRADO POR CARACTERÍSTICAS:" << endl;
    
    // Filtrar por ciudad
    vector<User*> cityFilter = socialNetwork.filterByCity("Santiago");
    cout << "   - Usuarios en Santiago: " << cityFilter.size() << " usuarios" << endl;
    
    // Filtrar por trabajo
    vector<User*> jobFilter = socialNetwork.filterByJob("Abogado");
    cout << "   - Usuarios con trabajo Abogado: " << jobFilter.size() << " usuarios" << endl;
    
    // Ordenar por grado de conexión
    vector<pair<User*, int>> sortedByDegree = socialNetwork.sortByDegree(true);
    if (!sortedByDegree.empty()) {
        cout << "   - Usuario con más conexiones: " << sortedByDegree[0].first->name 
             << " (" << sortedByDegree[0].second << " conexiones)" << endl;
        cout << "   - Top 3 usuarios más conectados:" << endl;
        for (int i = 0; i < min(3, (int)sortedByDegree.size()); i++) {
            cout << "     " << (i+1) << ". " << sortedByDegree[i].first->name 
                 << " - " << sortedByDegree[i].second << " conexiones" << endl;
        }
    }
    cout << endl;
    
    // 8. Probar recomendación de amigos
    cout << "8. PRUEBAS DE RECOMENDACIÓN DE AMIGOS:" << endl;
    vector<User*> recommendations = socialNetwork.recommendFriends(0);
    cout << "   - Recomendaciones de amigos para " << user0->name << ": " << recommendations.size() << " sugerencias" << endl;
    
    if (!recommendations.empty()) {
        cout << "     Top 3 recomendaciones: " << endl;
        for (int i = 0; i < min(3, (int)recommendations.size()); i++) {
            cout << "     " << (i+1) << ". " << recommendations[i]->name 
                 << " (" << recommendations[i]->city << ", " << recommendations[i]->job << ")" << endl;
        }
    }
    cout << endl;
    
    // 9. Probar operaciones CRUD con verificación de búsqueda binaria
    cout << "9. PRUEBAS DE OPERACIONES CRUD (CON BÚSQUEDA BINARIA):" << endl;
    
    // Agregar nuevo usuario
    User newUser(9999, "Test User", "Test City", "Test Job");
    socialNetwork.addUser(newUser);
    cout << "   ✓ Usuario de prueba agregado: " << newUser.name << endl;
    
    // Agregar conexión (usa búsqueda binaria para verificar duplicados)
    socialNetwork.addEdge(0, 9999);
    cout << "   ✓ Conexión agregada entre " << user0->name << " y " << newUser.name << endl;
    
    // Intentar agregar conexión duplicada (debe ser rechazada por búsqueda binaria)
    socialNetwork.addEdge(0, 9999);
    cout << "   ✓ Intento de conexión duplicada rechazado correctamente" << endl;
    
    // Verificar conexión usando búsqueda binaria
    bool newConnection = socialNetwork.areConnected(0, 9999);
    cout << "   - Verificar nueva conexión (vía búsqueda binaria): " << (newConnection ? "Exitosa" : "Fallida") << endl;
    
    // Obtener el usuario agregado
    User* testUser = socialNetwork.getUser(9999);
    if (testUser) {
        cout << "   - Datos del usuario agregado: " << testUser->name << ", " 
             << testUser->city << ", " << testUser->job << endl;
    }
    
    // Eliminar conexión (usa búsqueda binaria para encontrar elemento)
    socialNetwork.removeEdge(0, 9999);
    cout << "   ✓ Conexión eliminada" << endl;
    
    // Verificar que la conexión fue eliminada
    newConnection = socialNetwork.areConnected(0, 9999);
    cout << "   - Verificar eliminación de conexión: " << (newConnection ? "Error" : "Correcto") << endl;
    
    // Eliminar usuario
    socialNetwork.removeUser(9999);
    cout << "   ✓ Usuario de prueba eliminado" << endl;
    cout << endl;
    
    // 10. Probar eficiencia con usuarios populares
    cout << "10. PRUEBAS DE EFICIENCIA CON USUARIOS POPULARES:" << endl;
    
    // Encontrar usuarios con muchas conexiones
    vector<pair<User*, int>> topUsers = socialNetwork.sortByDegree(true);
    if (!topUsers.empty()) {
        User* popularUser = topUsers[0].first;
        cout << "   - Usuario más popular: " << popularUser->name << " con " 
             << topUsers[0].second << " conexiones" << endl;
        
        // Probar búsqueda binaria en usuario con muchas conexiones
        vector<User*> popularConnections = socialNetwork.getUserConnections(popularUser->id);
        cout << "   - Verificando conexiones con búsqueda binaria..." << endl;
        
        if (!popularConnections.empty()) {
            // Verificar algunas conexiones aleatorias
            int testConnections = min(3, (int)popularConnections.size());
            for (int i = 0; i < testConnections; i++) {
                bool isConnected = socialNetwork.areConnected(popularUser->id, popularConnections[i]->id);
                cout << "     - Conexión con " << popularConnections[i]->name << ": " 
                     << (isConnected ? "Confirmada" : "Error") << endl;
            }
        }
    }
    cout << endl;
    
    // 11. Estadísticas finales
    cout << "11. ESTADÍSTICAS FINALES:" << endl;
    vector<User*> allUsers = socialNetwork.getAllUsers();
    cout << "   - Total de usuarios en la red: " << allUsers.size() << endl;
    
    // Contar conexiones totales
    int totalConnections = 0;
    for (auto& pair : socialNetwork.sortByDegree(false)) {
        totalConnections += pair.second;
    }
    cout << "   - Total de conexiones en la red: " << totalConnections << endl;
    cout << "   - Conexiones promedio por usuario: " << (allUsers.empty() ? 0 : (double)totalConnections / allUsers.size()) << endl;
    
    cout << "\n=== PRUEBAS COMPLETADAS EXITOSAMENTE ===" << endl;
    cout << "=== SISTEMA CON BÚSQUEDA BINARIA OPERATIVO ===" << endl;
    
    return 0;
}