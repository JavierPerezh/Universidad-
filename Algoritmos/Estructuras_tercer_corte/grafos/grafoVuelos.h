#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class Ciudad {
public:
    string nombre, codigo, aeropuerto;

    Ciudad(string n, string c, string a):nombre(n), codigo(c), aeropuerto(a){}

    bool operator==(const Ciudad& otro) const{
        if(codigo == otro.codigo) {
            return true;
        }
        return false;
    }
};

class Vuelo {
public:
    Ciudad* origen;
    Ciudad* destino;
    int tiempoVuelo;
    float costo;
    string aerolinea;
    int codigoVuelo;

    Vuelo(Ciudad& org, Ciudad& des, string aer, float co, int tiempo, int codVuelo): origen(&org), destino(&des), aerolinea(aer), costo(co), 
    tiempoVuelo(tiempo), codigoVuelo(codVuelo){}

    bool operator==(const Vuelo& otro) const {
        if(*origen == *otro.origen && *destino == *otro.destino && aerolinea == otro.aerolinea && codigoVuelo == otro.codigoVuelo) {
            return true;
        }
        return false;
    } 
};

class GrafoVuelos {
public:
    unordered_map<string, int> indicesCiudades; //Indices de la matriz por codigos de ciudades
    unordered_map<string, Ciudad*> ciudades; //Informacion de ciudades por su codigo
    unordered_map<string, vector<Vuelo*>> vuelosDesde; //Informacion de los vuelos guardados segun el codigo de su origen
    vector<vector<int>> matriz;

    int size() {
        return matriz.size();
    }

    void agregarCiudad(Ciudad& ciudad) {
        string codigo = ciudad.codigo;
        if (ciudades.count(codigo)) return;

        int n = size();
        indicesCiudades[codigo] = n;
        ciudades[codigo] = &ciudad;

        for (auto& fila : matriz) fila.push_back(-1);  
        matriz.push_back(vector<int>(n + 1, -1));      
    }

    void agregarVuelo(Vuelo& vuelo) {
        string codOrigen = vuelo.origen->codigo, codDestino = vuelo.destino->codigo;

        if (existeVuelo(vuelo)) return;

        vuelosDesde[codOrigen].push_back(&vuelo);

        matriz[indicesCiudades[codOrigen]][indicesCiudades[codDestino]] = vuelo.tiempoVuelo;
    }

    void eliminarVuelo(Vuelo& vuelo) {
        string codOrigen = vuelo.origen->codigo, codDestino = vuelo.destino->codigo;

        if(!existeVuelo(vuelo)) return;

        matriz[indicesCiudades[codOrigen]][indicesCiudades[codDestino]] = -1;

        for(int i = 0; i < vuelosDesde[codOrigen].size(); i++) {
            if(vuelo == *(vuelosDesde[codOrigen][i])) {
                vuelosDesde[codOrigen].erase(vuelosDesde[codOrigen].begin() + i);
                break;
            }
        }
    }

    void eliminarCiudad(Ciudad& ciudad) {
        string codigo = ciudad.codigo;

        if (indicesCiudades.count(codigo) == 0) return;

        matriz.erase(matriz.begin() + indicesCiudades[codigo]);
        for (int i = 0; i < size(); i++) {
            matriz[i].erase(matriz[i].begin() + indicesCiudades[codigo]);
        }

        int sizeV = vuelosDesde[codigo].size(); 
        for(int i = 0; i < sizeV; i++) {
            eliminarVuelo(*(vuelosDesde[codigo][i]));
        }
        vuelosDesde.erase(codigo);

        int idxEliminado = indicesCiudades[codigo];
        for (auto& [cod, idx] : indicesCiudades) {
            if (idx > idxEliminado) --idx;
        }
        indicesCiudades.erase(codigo);
        ciudades.erase(codigo);
    }

    bool existeVuelo(Vuelo& vuelo) {
        string codOrigen = vuelo.origen->codigo, codDestino = vuelo.destino->codigo;
        if (matriz[indicesCiudades[codOrigen]][indicesCiudades[codDestino]] != -1) return true;
        return false;
    }

    vector<string>* rutaMasCorta(Ciudad& origen, Ciudad& destino) {
        unordered_map<string, int> distancia;
        unordered_map<string, bool> visitado;
        unordered_map<string, string> anterior;

        distancia[origen.codigo] = 0;
        for (auto& [cod, ciudad] : ciudades) {
            if (cod != origen.codigo) distancia[cod] = INT_MAX;
            visitado[cod] = false;
            anterior[cod] = "";
        }

        for (int k = 0; k < size(); k++) {

            string ciudadActual = "";
            int menorDist = INT_MAX;

            for (auto& [cod, d] : distancia) {
                if (!visitado[cod] && d < menorDist) {
                    menorDist = d;
                    ciudadActual = cod;
                }
            }

            if (ciudadActual == "") break;

            visitado[ciudadActual] = true;

            int i = indicesCiudades[ciudadActual];

            for (auto& [codDestino, j] : indicesCiudades) {
                int peso = matriz[i][j];  
                if (peso != -1 && !visitado[codDestino]) {
                    int nuevaDist = distancia[ciudadActual] + peso;
                    if (nuevaDist < distancia[codDestino]) {
                        distancia[codDestino] = nuevaDist;
                        anterior[codDestino] = ciudadActual;
                    }
                }
            }
        }

        vector<string>* ruta = new vector<string>();

        if (distancia[destino.codigo] == INT_MAX) return ruta;

        string actual = destino.codigo;
        while (actual != "") {
            ruta->insert(ruta->begin(), actual);
            actual = anterior[actual];
        }

    return ruta;
    }

};