
// Online C++ Compiler - Build, Compile and Run your C++ programs online in your favorite browser

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif // FUNCIONES_H

using namespace std;

#ifndef FUNCIONES_TO_STRING
#define FUNCIONES_TO_STRING

// Verificación si un tipo tiene un método to_string()
template <typename, typename = void>
struct has_to_string : std::false_type {};

template <typename T>
struct has_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>> 
    : std::is_same<decltype(std::declval<T>().to_string()), std::string> {};

#endif // FUNCIONES_H



template <typename T>
class Vector{
    T* v;
    int capacity;
    int size;
    
    
public:
    Vector(){
        capacity = 10;
        size = 0;
        v = new T[capacity];
    }
    
    Vector(int c){
        capacity = c;
        size = 0;
        v = new T[capacity];
    }
    
    ~Vector(){
        delete[] v;
    }
    
    int getSize(){
        return size;
    }
    
    int getCapacity(){
        return capacity;
    }
    
    T get(int i){
        if(i>=0 && i<size){
            return v[i];
        }else{
            throw std::out_of_range("Index out of range");
        }
    }
    
    void corrimientoDer(int i){
        for (int j = size; j > i; j--) {  // Empieza desde size, no size-1
            v[j] = v[j-1];
        }
    }
    
    
    void insert(int i, T d){
    if(size == capacity){
        resize();
    }
    corrimientoDer(i);  
    v[i] = d;
    size++;             

    }
    
    void corrimientoIzq(int i){
        //Corre todos los elementos desd ela izquerda hacia la posición i

	//IMPLEMENTE CON BASE EN SU CÓDIGO          
        for (int j = i; j < size; j++) {
            v[j] = v[j+1];
        }
    }
    
    void removeI(int i){
        corrimientoIzq(i);
	size--;
    }
    
    int find(T d){
	//Retorne la posición donde encontró el dato o -1 si no lo encontró
       //IMPLEMENTE CON BASE EN SU CÓDIGO
       int posicion = -1;
        for (int i = 0; i < size; i++) {
            if(v[i] == d){
                posicion = i;
                break;
            }
        }
        return posicion;
    }
    
    void resize(){
        capacity *= 2;
        T* nv = new T[capacity];
        for(int i=0; i<size; i++){
            nv[i] = v[i];
        }
        delete[] v;
        v = nv;
    }
    
    void add(T d){
        if(size == capacity){
            resize();
        }
        v[size++] = d;
    }
    
    void print(){
        for(int i = 0; i<size ;i++){
            cout<<v[i]<<"\t";
        }
        cout<<endl;
    }
    
    
    T& operator[](size_t index) {
        return v[index];
    }

    const T& operator[](size_t index) const {
        return v[index];
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

    
};
