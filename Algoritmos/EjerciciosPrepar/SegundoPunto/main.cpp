// Online C++ Compiler - Build, Compile and Run your C++ programs online in your favorite browser

#include<iostream>
#include"Vector.h"

using namespace std;

class Estudiante {

    int id;
    int numNotas;
    Vector<float> notas; //Si lo desea, puede usar otra estructura lineal dinámica
    /*
    Cómo poder modelar la selección del atributo (id o nota) que debe ser usada para ordenar?
    Cómo definir el orden en que se va a ordenar el atributo ?
    Pista: puede usar nuevos atributos de la clase Estudiante para eso.
    */
    bool porId; //True: Ordenar por id, False: Ordenar por notas
    bool ascendiente; //True: Orden ascendiente, False: Orden descendiente
    


public:
    Estudiante() {
        id = -1;
        numNotas = 3;
    }

    Estudiante(int i, int nN) {
        id = i;
        numNotas = nN;
    }

    float getNota(int i) {
        return notas[i];
    }

    void addNota(float n) {
        notas.add(n);
    }

    int getID() {
        return id;
    }


    void setOrdOrderAsc() {
        //Define el orden como ascendente
        ascendiente = true;
    }

    void setOrdOrderDesc() {
        //Define el orden como decendiente
        ascendiente = false;
    }

    void setOrdAtrib(int i) {
        //Define el atributo i como el usado para el ordenamiento. Use -1 para ordenar por el id.
        if(i == -1) porId = true;
        else porId = false;
    }

    float promedio() {
        //Calcular el promedio de notas del estudiante
        float sum = 0;
        for(int i = 0; i < numNotas; i++)
        {
            sum += notas[i];
        }
        float prom = sum / (float)numNotas;
        return prom;
    }

    string to_string() {
        string s = std::to_string(id) + " - ";

        for (int i = 0; i < numNotas; i++) {
            s = s + std::to_string(notas[i]) + ", ";
        }
        s = s + "Promedio: " + std::to_string(promedio());
        return s;
    }

    int compareTo(Estudiante e) {
        /*Defina el algoritmo de comparación entre estudiantes, de tal manera que use
        el atributo y el orden deseados
        */

        if(porId)
        {
            if(id > e.getID())
            {
                return 1;
            }
            else if(id < e.getID())
            {
                return -1;
            }
        }
        else
        {
            if(promedio() > e.promedio())
            {
                return 1;
            }
            else if(promedio() < e.promedio())
            {
                return -1;
            }
        }
        return 0; //Retorna 0 si son iguales
    }


    bool operator>(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) == 1) {
                return true;
            }
        }
        return false;
    }

    bool operator>=(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) >= 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator<(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) == -1) {
                return true;
            }
        }
        return false;
    }

    bool operator<=(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) <= 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator==(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) == 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator!=(Estudiante& other) {
        if (this != &other) {
            if (compareTo(other) != 0) {
                return true;
            }
        }

        return false;
    }

    Estudiante& operator=(Estudiante &other) {
        //Implemente la asignación directa entre estudiantes. Siga el ejemplo de los programas hechos en clase.
        
        numNotas = other.numNotas;
        id = other.numNotas;

        //Mantenga esta parte
        this -> notas.clear();
        for (int i = 0; i < other.numNotas; i++) {
            this -> notas.add(other.notas[i]);
        }
        
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, Estudiante& b) {
        return os << b.to_string();
    }

};

class Curso {
    int id;
    Vector<Estudiante> estudiantes;
    int numNotas;
    int numEstudiantes;
    bool ascendiente;
    bool criterio; //True: CantEstudiantes, false: Promedio

public:

    Curso() {
        id = -1;
        numNotas = 0;
        numEstudiantes = 0;
    }

    Curso(int i, int nN, int nE) {
        id = i;
        numNotas = nN;
        numEstudiantes = nE;
    }

    void setOrdOrderAsc() {
        //Define el orden como ascendente en todos los estudiantes del Curso
        
        ascendiente = true;
    }

    void setOrdOrderDesc() {
        //Define el orden como ascendente en todos los estudiantes del Curso
        
        ascendiente = false;
    }

    void setOrdAtrib(int j) {
        
        if(j == -1) criterio = true;
        else criterio = false;
    }

    void addEstudiante(Estudiante& e) {
        estudiantes.add(e);
    }

    Estudiante& getEstudiante(int i) {
        return estudiantes[i];
    }

    float promedio() {
        //Calcular el promedio del promedio de los cursos
        float sum = 0;
        for(int i = 0; i < numEstudiantes; i++)
        {
            sum += getEstudiante(i).promedio();
        }

        float prom = sum / (float)numEstudiantes;

        return prom;
    }

    string to_string() {
        string s = "***** Curso " + std::to_string(id) + " *****\n";

        for (int i = 0; i < numEstudiantes; i++) {
            s = s + estudiantes[i].to_string() + "\n";
        }
        s = s + "Promedio: " + std::to_string(this->promedio()) + "\n";
        s = s + "NumEstudiantes: " + std::to_string(this->numEstudiantes) + "\n";
        return s;
    }

    int getNumEstudiantes() {
        return numEstudiantes;
    }

    int compareTo(Curso &e) {
        /*Defina el algoritmo de comparación entre cursos
        */
        
        if(criterio)
        {
            if(numEstudiantes > e.getNumEstudiantes()) return 1;
            else if(numEstudiantes < e.getNumEstudiantes()) return -1;
        }
        else
        {
            if(promedio() > e.promedio()) return 1;
            else if(promedio() < e.promedio()) return -1;
        }
        return 0; //Retorna 0 si son iguales
    }


    bool operator>(Curso& other) {
        if (this != &other) {
            if (compareTo(other) == 1) {
                return true;
            }
        }
        return false;
    }

    bool operator>=(Curso& other) {
        if (this != &other) {
            if (compareTo(other) >= 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator<(Curso& other) {
        if (this != &other) {
            if (compareTo(other) == -1) {
                return true;
            }
        }
        return false;
    }

    bool operator<=(Curso& other) {
        if (this != &other) {
            if (compareTo(other) <= 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator==(Curso& other) {
        if (this != &other) {
            if (compareTo(other) == 0) {
                return true;
            }
        }
        else {
            return true;
        }

        return false;
    }

    bool operator!=(Curso& other) {
        if (this != &other) {
            if (compareTo(other) != 0) {
                return true;
            }
        }

        return false;
    }

    Curso& operator=(Curso &other) {
        //Implemente la asignación directa entre cursos. Siga el ejemplo de los programas hechos en clase.
        
        id = other.id;
        numEstudiantes = other.getNumEstudiantes();
        numNotas = other.numNotas;

        this->estudiantes.clear();
        for (int i = 0; i < other.getNumEstudiantes(); i++) {
            this->estudiantes.add(other.estudiantes[i]);
        }

        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, Curso& b) {
        return os << b.to_string();
    }

    void sort() {
        estudiantes.sort();
    }

};

class RegiNotas {
    Vector<Curso> cursos;

public:

    RegiNotas() {}

    void addCurso(Curso& c) {
        cursos.add(c);
    }

    Curso& getCurso(int i) {
        return cursos[i];
    }

    void setOrdOrderAsc() {
        //Define el orden como ascendente en todos los estudiantes del Curso
        
        
    }

    void setOrdOrderDesc() {
        //Define el orden como ascendente en todos los estudiantes del Curso
        
    }

    void setOrdAtrib(int j) {
        //Define el atributo i como el usado para el ordenamiento en todos los estudiantes del Curso. Use -1 para ordenar por el id.
        
    }


    void sort() {
        //Debe organizar los cursos, luego el interior de cada curso
        
    }

    string to_string() {
        string s = "***** Lista de Cursos *****\n";

        for (int i = 0; i < cursos.getSize(); i++) {
            s = s + cursos[i].to_string() + "\n";
        }
        return s;

    }

    friend std::ostream& operator<<(std::ostream& os, RegiNotas& b) {
        return os << b.to_string();
    }

};


int main()
{
    srand(1234);
    int numEstudiantes = 10;
    int numNotas = 5;

    RegiNotas r;
    Curso* tc = nullptr;
    Estudiante* te = nullptr;
    
    //10 estudiantes, 6 notas
    tc = new Curso(0, numNotas+1, numEstudiantes);
    for (int j = 0; j < numEstudiantes; j++) {
        te = nullptr;
        te = new Estudiante(0 * (numEstudiantes) + j, numNotas+1);
        for (int k = 0; k < numNotas+1; k++) {
            te->addNota((float)(rand() % 51) / 10);
        }
        tc->addEstudiante(*te);
    }
    r.addCurso(*tc);
    
    //11 estudiantes
    tc = new Curso(1, numNotas, numEstudiantes+1);
    for (int j = 0; j < numEstudiantes; j++) {
        te = nullptr;
        te = new Estudiante(1 * (numEstudiantes+1) + j, numNotas);
        for (int k = 0; k < numNotas; k++) {
            te->addNota((float)(rand() % 51) / 10);
        }
        tc->addEstudiante(*te);
    }
    r.addCurso(*tc);

    //10 estudiantes
    tc = new Curso(2, numNotas, numEstudiantes);
    for (int j = 0; j < numEstudiantes; j++) {
        te = nullptr;
        te = new Estudiante(2 * (numEstudiantes) + j, numNotas);
        for (int k = 0; k < numNotas; k++) {
            te->addNota((float)(rand() % 51) / 10);
        }
        tc->addEstudiante(*te);
    }
    r.addCurso(*tc);
    
    //10 estudiantes
    tc = new Curso(3, numNotas, numEstudiantes);
    for (int j = 0; j < numEstudiantes; j++) {
        te = nullptr;
        te = new Estudiante(3 * (numEstudiantes) + j, numNotas);
        for (int k = 0; k < numNotas; k++) {
            te->addNota((float)(rand() % 51) / 10);
        }
        tc->addEstudiante(*te);
    }
    r.addCurso(*tc);
    
    //13 estudiantes
    tc = new Curso(4, numNotas, numEstudiantes+3);
    for (int j = 0; j < numEstudiantes+3; j++) {
        te = nullptr;
        te = new Estudiante(4 * (numEstudiantes+3) + j, numNotas);
        for (int k = 0; k < numNotas; k++) {
            te->addNota((float)(rand() % 51) / 10);
        }
        tc->addEstudiante(*te);
    }
    r.addCurso(*tc);

    cout << r;
    
    //Criterios de ordenamiento de los cursos
    r.setOrdOrderAsc();
    r.setOrdAtrib(-1);
    
    cout<<endl;
    cout<<"*********** Vector de cursos ordenados **************"<<endl;
    cout<<endl;
    
    r.sort();
    
    cout << r;



    return 0;
}
