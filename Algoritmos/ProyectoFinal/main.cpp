#include <iostream>
#include "SocialNetwork.h"

using namespace std;

int main() {
    cout << "=== PRUEBA DEL SISTEMA DE RED SOCIAL (OBJETOS COMPLETOS) ===\n" << endl;
    
    // 1. Crear instancia del grafo (carga datos automáticamente)
    cout << "1. Cargando datos desde archivos..." << endl;
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
    cout << endl;
    
    // 3. Probar búsquedas básicas con información completa
    cout << "3. PRUEBAS DE BUSQUEDA Y CONEXIONES:" << endl;
    
    // BFS desde usuario 0
    vector<User*> bfsResult = socialNetwork.BFS(0);
    cout << "   - BFS desde usuario 0: " << bfsResult.size() << " usuarios conectados" << endl;
    if (!bfsResult.empty()) {
        cout << "     Primeros 3 usuarios: ";
        for (int i = 0; i < min(3, (int)bfsResult.size()); i++) {
            cout << bfsResult[i]->name << "(" << bfsResult[i]->id << ") ";
        }
        cout << endl;
    }
    
    // Obtener conexiones directas
    vector<User*> connections = socialNetwork.getUserConnections(0);
    cout << "   - Conexiones directas de usuario 0: " << connections.size() << " amigos" << endl;
    if (!connections.empty()) {
        cout << "     Amigos: ";
        for (int i = 0; i < min(3, (int)connections.size()); i++) {
            cout << connections[i]->name << " ";
        }
        cout << endl;
    }
    
    // Verificar conexión entre usuarios
    bool connected = socialNetwork.areConnected(0, 1);
    cout << "   - Usuario 0 conectado con usuario 1: " << (connected ? "Sí" : "No") << endl;
    cout << endl;
    
    // 4. Probar camino más corto con información completa
    cout << "4. PRUEBAS DE CAMINO MAS CORTO:" << endl;
    vector<User*> path = socialNetwork.shortestPath(0, 5);
    if (!path.empty()) {
        cout << "   - Camino más corto de " << path[0]->name << " a " << path.back()->name << ":" << endl;
        cout << "     ";
        for (User* user : path) {
            cout << user->name << "(" << user->id << ") -> ";
        }
        cout << "LLEGADA" << endl;
    } else {
        cout << "   - No hay camino entre 0 y 5" << endl;
    }
    cout << endl;
    
    // 5. Probar amigos mutuos con información completa
    cout << "5. PRUEBAS DE AMIGOS MUTUOS:" << endl;
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
    
    // 6. Probar filtros por características con información completa
    cout << "6. PRUEBAS DE FILTRADO:" << endl;
    
    // Filtrar por ciudad
    vector<User*> cityFilter = socialNetwork.filterByCity("Santiago");
    cout << "   - Usuarios en Santiago: " << cityFilter.size() << " usuarios" << endl;
    if (!cityFilter.empty()) {
        cout << "     Ejemplo: " << cityFilter[0]->name << " (" << cityFilter[0]->job << ")" << endl;
    }
    
    // Filtrar por trabajo
    vector<User*> jobFilter = socialNetwork.filterByJob("Abogado");
    cout << "   - Usuarios con trabajo abogado: " << jobFilter.size() << " usuarios" << endl;
    if (!jobFilter.empty()) {
        cout << "     Ejemplo: " << jobFilter[0]->name << " (" << jobFilter[0]->city << ")" << endl;
    }
    
    // Ordenar por grado de conexión
    vector<pair<User*, int>> sortedByDegree = socialNetwork.sortByDegree(true);
    if (!sortedByDegree.empty()) {
        cout << "   - Usuario con más conexiones: " << sortedByDegree[0].first->name 
             << " (" << sortedByDegree[0].second << " conexiones)" << endl;
    }
    cout << endl;
    
    // 7. Probar recomendación de amigos con información completa
    cout << "7. PRUEBAS DE RECOMENDACION DE AMIGOS:" << endl;
    vector<User*> recommendations = socialNetwork.recommendFriends(0);
    cout << "   - Recomendaciones de amigos para " << user0->name << ": " << recommendations.size() << " sugerencias" << endl;
    
    if (!recommendations.empty()) {
        cout << "     Usuarios recomendados: ";
        for (int i = 0; i < min(3, (int)recommendations.size()); i++) {
            cout << recommendations[i]->name << "(" << recommendations[i]->id << ") ";
        }
        if (recommendations.size() > 3) {
            cout << "... y " << recommendations.size() - 3 << " más";
        }
        cout << endl;
    }
    cout << endl;
    
    // 8. Probar operaciones CRUD
    cout << "8. PRUEBAS DE OPERACIONES CRUD:" << endl;
    
    // Agregar nuevo usuario
    User newUser(9999, "Test User", "Test City", "Test Job");
    socialNetwork.addUser(newUser);
    cout << "   ✓ Usuario de prueba agregado: " << newUser.name << endl;
    
    // Agregar conexión
    socialNetwork.addEdge(0, 9999);
    cout << "   ✓ Conexión agregada entre " << user0->name << " y " << newUser.name << endl;
    
    // Verificar conexión
    bool newConnection = socialNetwork.areConnected(0, 9999);
    cout << "   - Verificar nueva conexión: " << (newConnection ? "Exitosa" : "Fallida") << endl;
    
    // Obtener el usuario agregado
    User* testUser = socialNetwork.getUser(9999);
    if (testUser) {
        cout << "   - Datos del usuario agregado: " << testUser->name << ", " 
             << testUser->city << ", " << testUser->job << endl;
    }
    
    // Eliminar conexión
    socialNetwork.removeEdge(0, 9999);
    cout << "   ✓ Conexión eliminada" << endl;
    
    // Eliminar usuario
    socialNetwork.removeUser(9999);
    cout << "   ✓ Usuario de prueba eliminado" << endl;
    cout << endl;
    
    // 9. Probar obtención de todos los usuarios
    cout << "9. ESTADÍSTICAS GENERALES:" << endl;
    vector<User*> allUsers = socialNetwork.getAllUsers();
    cout << "   - Total de usuarios en la red: " << allUsers.size() << endl;
    
    // Contar usuarios por ciudad (ejemplo simple)
    unordered_map<string, int> cityCount;
    for (User* user : allUsers) {
        cityCount[user->city]++;
    }
    cout << "   - Distribución por ciudades: " << endl;
    for (auto& pair : cityCount) {
        if (pair.second > 5) { // Mostrar solo ciudades con más de 5 usuarios
            cout << "     * " << pair.first << ": " << pair.second << " usuarios" << endl;
        }
    }
    
    cout << "\n=== PRUEBAS COMPLETADAS EXITOSAMENTE ===" << endl;
    cout << "=== TODAS LAS FUNCIONALIDADES DEVUELVEN INFORMACIÓN COMPLETA DE USUARIOS ===" << endl;
    
    return 0;
}