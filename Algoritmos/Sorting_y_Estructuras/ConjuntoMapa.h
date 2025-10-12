#include<iostream>
using namespace std;

template <typename T>
class Node{
public:
    T data;
    Node* next;
    Node(T val) : data(val), next(nullptr) {}
};

template <typename T>
class Conjunto{
private:
    Node<T>* head;
    int count;
public:
    Conjunto() : head(nullptr), count(0) {}
    
    // Constructor de copia
    Conjunto(const Conjunto<T>& other) : head(nullptr), count(0) {
        Node<T>* current = other.head;
        while(current){
            add(current->data);
            current = current->next;
        }
    }
    
    ~Conjunto(){
        Node<T>* current = head;
        while(current){
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
    
    bool contains(T element){
        Node<T>* current = head;
        while(current){
            if(current->data == element){
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    void add(T element){
        // No permitir elementos duplicados
        if(contains(element)){
            return;
        }
        
        Node<T>* newNode = new Node<T>(element);
        if(!head){
            head = newNode;
        } else {
            Node<T>* current = head;
            while(current->next){
                current = current->next;
            }
            current->next = newNode;
        }
        count++;
    }
    
    void remove(T element){
        if(!head) return;
        
        // Caso especial: eliminar la cabeza
        if(head->data == element){
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            count--;
            return;
        }
        
        // Buscar el elemento a eliminar
        Node<T>* current = head;
        while(current->next && current->next->data != element){
            current = current->next;
        }
        
        // Si encontramos el elemento
        if(current->next && current->next->data == element){
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count--;
        }
    }
    
    int size() const {
        return count;
    }

    void print() const {
        Node<T>* current = head;
        cout << "{ ";
        while(current){
            cout << current->data << " ";
            current = current->next;
        }
        cout << "}" << endl;
    }

    Conjunto<T> Union(const Conjunto<T>& other) const {
        Conjunto<T> result(*this); // Copia de este conjunto
        
        Node<T>* current = other.head;
        while(current){
            // add() automáticamente evita duplicados
            result.add(current->data);
            current = current->next;
        }
        return result;
    }
    
    Conjunto<T> Intersection(const Conjunto<T>& other) const {
        Conjunto<T> result;
        
        Node<T>* current = head;
        while(current){
            if(other.contains(current->data)){
                result.add(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    Conjunto<T> operator-(const Conjunto<T>& other) const {
        Conjunto<T> result;
        
        Node<T>* current = head;
        while(current){
            if(!other.contains(current->data)){
                result.add(current->data);
            }
            current = current->next;
        }
        return result;
    }
    
    bool operator==(const Conjunto<T>& other) const {
        if(count != other.count){
            return false;
        }
        
        Node<T>* current = head;
        while(current){
            if(!other.contains(current->data)){
                return false;
            }
            current = current->next;
        }
        return true;
    }
    
    bool operator!=(const Conjunto<T>& other) const {
        return !(*this == other);
    }
    
    Conjunto<T>& operator=(const Conjunto<T>& other) {
        if(this == &other){
            return *this; // Evitar auto-asignación
        }
        
        // Limpiar el conjunto actual
        Node<T>* current = head;
        while(current){
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        count = 0;
        
        // Copiar elementos del otro conjunto
        current = other.head;
        while(current){
            add(current->data);
            current = current->next;
        }
        
        return *this;
    }
    
    // Método adicional útil: ver si el conjunto está vacío
    bool empty() const {
        return count == 0;
    }
};

template <typename T>
class Pareja{
public:
    string key;
    T value;
    Pareja() : key(""), value(T()) {}
    Pareja(string k, T v) : key(k), value(v) {}
};

template <typename T>
class Mapa{
private:
    Pareja<T>* array;
    int count;
    int capacity;
    int INITIAL_CAPACITY;
    
public:
    Mapa(int cap = 10) : INITIAL_CAPACITY(cap), capacity(cap), count(0) {
        array = new Pareja<T>[cap];
    }
    
    // Constructor de copia
    Mapa(const Mapa<T>& other) : INITIAL_CAPACITY(other.INITIAL_CAPACITY), 
                                 capacity(other.capacity), count(other.count) {
        array = new Pareja<T>[capacity];
        for(int i = 0; i < count; i++){
            array[i] = other.array[i];
        }
    }
    
    ~Mapa(){
        delete[] array;
    }
    
    // Operador de asignación
    Mapa<T>& operator=(const Mapa<T>& other){
        if(this == &other){
            return *this;
        }
        
        delete[] array;
        INITIAL_CAPACITY = other.INITIAL_CAPACITY;
        capacity = other.capacity;
        count = other.count;
        array = new Pareja<T>[capacity];
        for(int i = 0; i < count; i++){
            array[i] = other.array[i];
        }
        return *this;
    }
    
    int contains(string key){
        for(int i = 0; i < count; i++){
            if(array[i].key == key) return i;
        }
        return -1;
    }
    
    void put(string key, T value){
        int index = contains(key);
        if(index == -1){
            if(capacity == count){
                increaseCapacity();
            }
            array[count] = Pareja<T>(key, value);
            count++;
        } else {
            array[index].value = value;
        }
    }
    
    T get(string key){
        int index = contains(key);
        if(index == -1) return T();
        return array[index].value;
    }
    
    T operator[](string key){
        return get(key);
    }
    
    void remove(string key){
        int index = contains(key);
        if(index == -1) return;
        
        for(int i = index; i < count - 1; i++){
            array[i] = array[i + 1];
        }
        count--;
        
        if(count <= capacity / 4 && capacity > INITIAL_CAPACITY){
            decreaseCapacity();
        }
    }
    
    void increaseCapacity(){
        capacity *= 2;
        Pareja<T>* newArray = new Pareja<T>[capacity];
        for(int i = 0; i < count; i++){
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }
    
    void decreaseCapacity(){
        capacity /= 2;
        Pareja<T>* newArray = new Pareja<T>[capacity];
        for(int i = 0; i < count; i++){
            newArray[i] = array[i];
        }
        delete[] array;
        array = newArray;
    }
    
    void display() const {
        cout << "Mapa:" << endl;
        for(int i = 0; i < count; i++){
            cout << "  " << array[i].key << ": " << array[i].value << endl;
        }
    }
    
    // Métodos adicionales útiles
    int size() const {
        return count;
    }
    
    bool empty() const {
        return count == 0;
    }
    
    // Obtener todas las claves
    Conjunto<string> keys() const {
        Conjunto<string> result;
        for(int i = 0; i < count; i++){
            result.add(array[i].key);
        }
        return result;
    }
};

// Función main para probar las implementaciones
int main(){
    cout << "=== PRUEBA DE CONJUNTO ===" << endl;
    Conjunto<int> conj1;
    conj1.add(1);
    conj1.add(2);
    conj1.add(3);
    conj1.add(2); // Duplicado, no debe agregarse
    
    cout << "Conjunto 1: ";
    conj1.print();
    
    Conjunto<int> conj2;
    conj2.add(2);
    conj2.add(3);
    conj2.add(4);
    
    cout << "Conjunto 2: ";
    conj2.print();
    
    Conjunto<int> unionConj = conj1.Union(conj2);
    cout << "Unión: ";
    unionConj.print();
    
    Conjunto<int> interConj = conj1.Intersection(conj2);
    cout << "Intersección: ";
    interConj.print();
    
    Conjunto<int> diffConj = conj1 - conj2;
    cout << "Diferencia (conj1 - conj2): ";
    diffConj.print();
    
    cout << "¿Son iguales conj1 y conj2? " << (conj1 == conj2 ? "Sí" : "No") << endl;
    
    cout << "\n=== PRUEBA DE MAPA ===" << endl;
    Mapa<int> mapa;
    mapa.put("uno", 1);
    mapa.put("dos", 2);
    mapa.put("tres", 3);
    mapa.put("dos", 20); // Actualizar valor
    
    mapa.display();
    
    cout << "Valor de 'dos': " << mapa.get("dos") << endl;
    cout << "Valor de 'cuatro': " << mapa["cuatro"] << endl;
    
    mapa.remove("tres");
    cout << "\nDespués de eliminar 'tres':" << endl;
    mapa.display();
    
    return 0;
}