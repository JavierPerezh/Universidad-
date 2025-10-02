#ifndef LISTA_H
#define LISTA_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <chrono>

using namespace std;

template <typename T>
class Nodo {
public:

    T data;
    Nodo* next;
    
    Nodo(T value) : data(value), next(nullptr) {}
    
    T getData() const {
        return data;
    }
    
    void setData(T d) {
        data = d;
    }
    
    Nodo* getNext() const {
        return next;
    }
    
    void setNext(Nodo* n) {
        next = n;
    }
};

template <typename T>
class Lista {
private:
    Nodo<T>* head;
    int size;

public:
    Lista() : head(nullptr), size(0) {}
    
    ~Lista() {
        clear();
    }
    
    void clear() {
        while (head) {
            Nodo<T>* temp = head;
            head = head->getNext();
            delete temp;
        }
        size = 0;
        head = nullptr;
    }

    void insert(T value) {
        Nodo<T>* nuevoNodo = new Nodo<T>(value);
        if(head == nullptr) {
            head = nuevoNodo;
        } else {
            Nodo<T>* temp = head;
            while(temp->getNext() != nullptr) {
                temp = temp->getNext();
            }
            temp->setNext(nuevoNodo);
        }
        size++;
    }
    
    void insertFront(T value) {
        Nodo<T>* nuevoNodo = new Nodo<T>(value);
        if(head == nullptr) {
            head = nuevoNodo;
        } else {
            nuevoNodo -> next = head;
            head = nuevoNodo;
        }
        size++;
    }
    
    void remove(T value){
        Nodo<T> *temp = head;
        if(temp -> data == value)
        {
            head = temp -> next;
            delete temp;
            return;
        }
        
        while (temp -> next -> data != value)
        {
            temp = temp -> next;
        }
        Nodo<T> *nodoABorrar = temp -> next;
        temp -> next = nodoABorrar -> next;
        delete nodoABorrar;
    }
    
    int getSize() const {
        return size;
    }
    
    Nodo<T>* getAt(int index) const {
        if (index < 0 || index >= size) throw std::out_of_range("Index out of range");
        
        Nodo<T>* temp = head;
        for(int i = 0; i < index; i++) {
            temp = temp->getNext();
        }
        return temp;
    }

    T get(int index) const {
        return getAt(index)->getData();
    }

    void print() const {
        Nodo<T>* temp = head;
        while (temp) {
            std::cout << temp->getData() << " -> ";
            temp = temp->getNext();
        }
        cout << "nullptr\n";
    }

    void addRandomInt(int min, int max, int elements, int seed);
    void Swap(int index1, int index2);
    void selectSort();
    void insertionSort();
    void bubbleSort();
    void quickSort(int low, int high);
    void bucketSort();
    void mergeSort();
    void radixSort();
    void heapify(int n, int i);
    void heapSort(); 
};

#endif // LISTA_H