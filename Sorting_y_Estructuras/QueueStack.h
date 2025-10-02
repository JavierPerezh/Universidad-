#include<iostream>
using namespace std;

template <typename T>
class Stack {
private:
    T* arr;
    bool dynamic;
    int top;
    int capacity;
    int INITIAL_SIZE = 10;
    
public:
    Stack(int size = 10, bool isDynamic = false) : dynamic(isDynamic) {
        capacity = size > 0 ? size : INITIAL_SIZE;
        arr = new T[capacity];
        top = -1;
    }
    
    // Constructor de copia
    Stack(const Stack<T>& other) : dynamic(other.dynamic), capacity(other.capacity), 
                                   top(other.top), INITIAL_SIZE(other.INITIAL_SIZE) {
        arr = new T[capacity];
        for(int i = 0; i <= top; i++) {
            arr[i] = other.arr[i];
        }
    }
    
    // Operador de asignación
    Stack<T>& operator=(const Stack<T>& other) {
        if(this == &other) return *this;
        
        delete[] arr;
        dynamic = other.dynamic;
        capacity = other.capacity;
        top = other.top;
        INITIAL_SIZE = other.INITIAL_SIZE;
        arr = new T[capacity];
        for(int i = 0; i <= top; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }
    
    ~Stack() {
        delete[] arr;
    }
    
    bool isEmpty() const {
        return top == -1;
    }
    
    bool isFull() const {
        return top == capacity - 1;
    }
    
    int getSize() const {
        return top + 1;
    }
    
    void push(T element) {
        if (isFull() && !dynamic) {
            cout << "Stack Overflow\n";
            return;
        }
        if (isFull() && dynamic) {
            increaseCapacity();
        }
        arr[++top] = element;
    }
    
    T pop() {
        if (isEmpty()) {
            cout << "The stack is empty.";
            return T();
        }
        T poppedElement = arr[top--];
        
        // Solo reducir capacidad si es dinámico y hay mucha capacidad sin usar
        if (dynamic && top + 1 < capacity / 4 && capacity > INITIAL_SIZE) {
            decreaseCapacity();
        }
        return poppedElement;
    }
    
    T peek() const {
        if (isEmpty()) {
            cout << "The stack is empty.";
            return T();
        }
        return arr[top];
    }

    void increaseCapacity() {
        int newCapacity = capacity * 2;  // Duplicar en lugar de +1 para mejor rendimiento
        T* newArr = new T[newCapacity];
        for (int i = 0; i <= top; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
        cout << "Stack capacity increased to: " << capacity << endl;
    }
    
    void decreaseCapacity() {
        if (capacity <= INITIAL_SIZE) return;
        int newCapacity = max(INITIAL_SIZE, capacity / 2); // Reducir a la mitad
        T* newArr = new T[newCapacity];
        for (int i = 0; i <= top && i < newCapacity; i++) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
        cout << "Stack capacity decreased to: " << capacity << endl;
    }
    
    void display() const {
        if(isEmpty()) {
            cout << "Stack is empty" << endl;
            return;
        }
        cout << "Stack (top to bottom): ";
        for(int i = top; i >= 0; i--) {
            cout << arr[i] << " ";
        }
        cout << endl;
    }
};

template <typename T>
class Queue {
private:
    T* arr;
    bool dynamic;
    int front, rear, size, capacity;
    int INITIAL_SIZE = 10;
    
public:
    Queue(int cap = 10, bool isDynamic = false) : 
        capacity(cap > 0 ? cap : INITIAL_SIZE), 
        dynamic(isDynamic), 
        front(0), 
        rear(-1), 
        size(0) {
        arr = new T[capacity];
    }
    
    // Constructor de copia
    Queue(const Queue<T>& other) : 
        dynamic(other.dynamic), 
        capacity(other.capacity), 
        front(other.front), 
        rear(other.rear), 
        size(other.size),
        INITIAL_SIZE(other.INITIAL_SIZE) {
        arr = new T[capacity];
        for(int i = 0; i < capacity; i++) {
            arr[i] = other.arr[i];
        }
    }
    
    // Operador de asignación
    Queue<T>& operator=(const Queue<T>& other) {
        if(this == &other) return *this;
        
        delete[] arr;
        dynamic = other.dynamic;
        capacity = other.capacity;
        front = other.front;
        rear = other.rear;
        size = other.size;
        INITIAL_SIZE = other.INITIAL_SIZE;
        arr = new T[capacity];
        for(int i = 0; i < capacity; i++) {
            arr[i] = other.arr[i];
        }
        return *this;
    }
    
    ~Queue() {
        delete[] arr;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    bool isFull() const {
        return size == capacity;
    }
    
    int getSize() const {
        return size;
    }
    
    void enqueue(T element) {
        if (isFull() && !dynamic) {
            cout << "Queue Overflow\n";
            return;
        }
        if (isFull() && dynamic) {
            increaseCapacity();
        }
        rear = (rear + 1) % capacity;
        arr[rear] = element;
        size++;
    }
    
    T dequeue() {
        if (isEmpty()) {
            cout << "The queue is empty.";
            return T();
        }
        T dequeuedElement = arr[front];
        front = (front + 1) % capacity;
        size--;
        
        if (dynamic && size < capacity / 4 && capacity > INITIAL_SIZE) {
            decreaseCapacity();
        }
        return dequeuedElement;
    }
    
    T getFront() const {
        if (isEmpty()) {
            cout << "The queue is empty.";
            return T();
        }
        return arr[front];
    }

    T getRear() const {
        if (isEmpty()) {
            cout << "The queue is empty.";
            return T();
        }
        return arr[rear];
    }

    void increaseCapacity() {
        int newCapacity = capacity * 2; // Duplicar capacidad
        T* newArr = new T[newCapacity];
        
        // Copiar elementos manteniendo el orden
        for (int i = 0; i < size; i++) {
            newArr[i] = arr[(front + i) % capacity];
        }
        
        delete[] arr;
        arr = newArr;
        front = 0;
        rear = size - 1;
        capacity = newCapacity;
        cout << "Queue capacity increased to: " << capacity << endl;
    }
    
    void decreaseCapacity() {
        if (capacity <= INITIAL_SIZE) return;
        int newCapacity = max(INITIAL_SIZE, capacity / 2);
        T* newArr = new T[newCapacity];
        
        // Copiar elementos manteniendo el orden
        for (int i = 0; i < size && i < newCapacity; i++) {
            newArr[i] = arr[(front + i) % capacity];
        }
        
        delete[] arr;
        arr = newArr;
        front = 0;
        rear = size - 1;
        capacity = newCapacity;
        cout << "Queue capacity decreased to: " << capacity << endl;
    }
    
    void display() const {
        if(isEmpty()) {
            cout << "Queue is empty" << endl;
            return;
        }
        cout << "Queue (front to rear): ";
        for(int i = 0; i < size; i++) {
            cout << arr[(front + i) % capacity] << " ";
        }
        cout << endl;
    }
};