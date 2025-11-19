#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>

using namespace std;

class Cancion {
public:
    string nombre;
    int duracion; //Segundos
    vector<Cantante*> cantantes;

    Cancion(string n, int d): nombre(n), duracion(d){}
};

class Cantante {
public:
    string nombre;
    vector<Cancion*> canciones;

    Cantante(string n): nombre(n){}
};

class Emisora {
public:
    vector<Cancion*> canciones;
    vector<Cantante*> cantantes;
    unordered_map<string, Cantante*> cantantesMap;
    unordered_map<string, Cancion*> cancionesMap;
    
    
};