
// Online C++ Compiler - Build, Compile and Run your C++ programs online in your favorite browser

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#endif // FUNCIONES_H

using namespace std;

#ifndef FUNCIONES_TO_STRING
#define FUNCIONES_TO_STRING

// Verificación si un tipo tiene un método to_string()
template <typename, typename = void>
struct has_to_string : std::false_type {};

template <typename T>
struct has_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>>
    : std::is_same<decltype(std::declval<T>().to_string()), std::string> {
};

#endif // FUNCIONES_H



template <typename T>
class Vector {
    T* v;
    int capacity;
    int size;


public:
    Vector() {
        capacity = 10;
        size = 0;
        v = new T[capacity];
    }

    Vector(int c) {
        capacity = c;
        size = 0;
        v = new T[capacity];
    }

    ~Vector() {
        //delete[] v;
    }

    int getSize() {
        return size;
    }

    int getCapacity() {
        return capacity;
    }

    T get(int i) {
        if (i >= 0 && i < size) {
            return v[i];
        }
        else {
            return -1;
        }
    }

    void corrimientoDer(int i) {
        //Corre todos los elementos desde la posición insert
        //una posición a la derecha
        if (size == capacity) {
            resize();
        }
        for (int j = size; j > i; j--) {
            v[j] = v[j - 1];
        }
        size++;
    }

    void insert(T d, int i) {
        //Inserta el elemento d en la posición i
        //luego de correr a la derecha el elemento 
        //en i y todos los elementos a la derecha
        //de i

        corrimientoDer(i);
        v[i] = d;

    }

    void corrimientoIzq(int i) {
        //Corre todos los elementos desd ela izquerda hacia la posición i
        for (int j = i; j < size - 1; j++) {
            v[j] = v[j + 1];
        }
        size--;
    }

    void removeI(int i) {
        corrimientoIzq(i);
    }

    int find(T d) {
        int i = 0;
        while (i < size && v[i] != d) {
            i++;
        }
        if (i < size) {
            return i;
        }
        return -1;
    }

    void resize() {
        capacity *= 2;
        T* nv = new T[capacity];
        for (int i = 0; i < size; i++) {
            nv[i] = v[i];
        }
        delete[] v;
        v = nv;
    }

    void add(T d) {
        if (size == capacity) {
            resize();
        }
        v[size] = d;
        size++;
    }

    void print() {
        for (int i = 0; i < size; i++) {
            cout << v[i] << "\t";
        }
        cout << endl;
    }


    T& operator[](size_t index) {
        return v[index];
    }

    const T& operator[](size_t index) const {
        return v[index];
    }


    // Método to_string() para objetos con método to_string()
    template <typename U = T>
    typename std::enable_if<has_to_string<U>::value, string>::type
        to_string() const {
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            oss << v[i].to_string();
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Método to_string() para punteros a objetos con to_string()
    template <typename U = T>
    typename std::enable_if<std::is_pointer<U>::value&& has_to_string<typename std::remove_pointer<U>::type>::value, string>::type
        to_string() const {
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            if (v[i]) {
                oss << v[i]->to_string();
            }
            else {
                oss << "null";
            }
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Método to_string() para tipos numéricos
    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, string>::type
        to_string() const {
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            oss << std::to_string(v[i]);
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Método to_string() por defecto
    template <typename U = T>
    typename std::enable_if<!has_to_string<U>::value && !std::is_arithmetic<U>::value, string>::type
        to_string() const {
        return "[No to_string() available]";
    }



    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& obj) {
        return os << obj.to_string();
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            v = other.v;
            size = other.size;
            capacity = other.capacity;
        }
        return *this;
    }

    void clear() {
        //delete[] v;
        size = 0;
        v = new T[capacity];
    }


    int der(int i) {
        return 2 * i + 1;
    }

    int izq(int i) {
        return 2 * (i + 1);
    }

    bool isHoja(int i, int n) {
        if (der(i) < n) {
            return false;
        }
        return true;
    }

    int padre(int i) {
        return i / 2 - 1;
    }


    void heapify(int i, int n) {
        /*
               i
             /   \
           /       \
         der(i)   izq(i)
        */
        T may = v[i];
        int mayind = i;

        if (der(i) < n) { //Si el nodo no es hoja
            if (v[der(i)] > may) {
                may = v[der(i)];
                mayind = der(i);
            }
            if (izq(i) < n && v[izq(i)] > may) {
                may = v[izq(i)];
                mayind = izq(i);
            }
            if (v[i] != may) {
                T temp = v[i];
                v[i] = may;
                v[mayind] = temp;
                heapify(mayind, n);
            }

        }

    }

    void sort() {
        //cout<<"El vector\n"<<endl;
        //cout<<"Size"<<size<<endl;
        //print();
        //cout<<"entre0"<<endl;

        for (int i = (size / 2) - 1; i >= 0; i--) {
            //cout<<"entre"<<i<<endl;
            heapify(i, size);
        }

        T temp;
        for (int i = size - 1; i >= 0; i--) {
            temp = v[i];
            v[i] = v[0];
            v[0] = temp;
            heapify(0, i);

            /*print();
            cout<<endl;
            */
        }

    }

    int search(T t) {
        return search(t, 0, size - 1);
    }

    int search_bin(T t, int ini, int fin) {
        int mid = (ini + fin) / 2;
        if (v[mid] == t) {
            return mid;
        }
        else {
            if (v[mid] < t) {
                return search_bin(t, mid + 1, fin);
            }
            else {
                return search_bin(t, ini, mid - 1);
            }
        }
    }

};
