#include "Lista.h"
#include<chrono>

template<typename T>
void Lista<T>::addRandomInt(int min, int max, int elements, int seed){
    if(getSize() > 0){
        clear();
    }
    srand(seed);
    while(getSize() < elements){
        insert(rand() % (max - min + 1) + min);
    }
    
}

template<typename T>
void Lista<T>::Swap(int index1, int index2){
    int firstIndex = (index1 < index2) ? index1 : index2;
    int secondIndex = (index1 < index2) ? index2 : index1;

    Nodo<T>* firstNodo = getAt(firstIndex);
    Nodo<T>* secondNodo = getAt(secondIndex);
    T temp = firstNodo -> data;
    firstNodo -> data = secondNodo -> data;
    secondNodo -> data = temp;

}

template<typename T>
void Lista<T>::selectSort(){
    int size = getSize();
    for(int i = 0; i < size - 1; i++){
        int min_id = i;
        for(int j = i + 1; j < size ; j++){
            if(getAt(j) -> data < getAt(min_id) -> data){
                min_id = j;
            }
        }
        Swap(i, min_id);
    }
}

template<typename T>
void Lista<T>::insertionSort(){
    Nodo<T>* temp;
    Nodo<T>* currentCheck;
    Nodo<T>* nextCheck;
    int size = getSize();
    if(size <= 1){
        return;
    }
    for(int i = 1; i < size; i++){

        temp = getAt(i);
        nextCheck = temp -> next;

        int checkIndex = i - 1;

        while(checkIndex >= 0 && getAt(checkIndex) -> data > temp -> data){
            getAt(checkIndex) -> next = nextCheck;
            nextCheck = getAt(checkIndex);
            checkIndex -= 1;
        }
        Nodo<T>* checkNode = getAt(checkIndex);
        if(!checkNode){
            head = temp;
        }
        else{
            checkNode -> next = temp;
        }
        temp -> next = nextCheck;

    }
}

template<typename T>
void Lista<T>::bubbleSort(){
    int size = getSize();
    bool swapped;

    for(int max = 0; max < size - 1; max++){
        swapped = false;
        for(int check = 0; check < size - max - 1; check++){
            if(getAt(check) -> data > getAt(check + 1) -> data){
                Swap(check, check + 1);
                swapped = true;
            }
        }
        if(!swapped){
            break;
        }
    }
}

template<typename T>
void Lista<T>::quickSort(int low, int high){
    if(low < high){
        T pivotData = getAt(high) -> data;
        int menor = low -1;
        for(int j = low; j < high; j++){
            if (getAt(j)-> data < pivotData){
                menor += 1;
                Swap(menor, j);   
            }
        }
        Swap(high, menor +1);
        int pivotIndex = menor +1;
        quickSort(low, pivotIndex -1);
        quickSort(pivotIndex +1, high);
    }
}

template<typename T>
void Lista<T>::bucketSort(){
    int size = getSize();
    if (size == 0) return;
    T maxValue = getAt(0) -> data;
    for (int i = 0; i < size; i++){
        maxValue = (getAt(i) -> data > maxValue)? getAt(i) -> data : maxValue;
    }
    Lista <Lista<T>*> buckets;
    for (int i =0; i < size; i++){
        buckets.insert(new Lista <T>());
    }
    for (int i = 0; i<size; i++){
        T pebble = getAt(i) -> data;
        auto normalizedValue = pebble/ (maxValue + 1.0);
        int bucketIndex = normalizedValue *size;
        buckets.getAt(bucketIndex) -> data -> insert(pebble);
    }
    for (int i = 0; i<size; i++){
        buckets.getAt(i) -> data -> insertionSort();
    }
    clear();
    for (int i = 0; i<size; i++){
        Lista <T> *bucket = buckets.getAt(i) -> data;
        int bucketsize = bucket -> getSize();
        for (int j = 0; j<bucketsize; j++){
            insert(bucket -> getAt(j) -> data);
        }
        delete bucket;
    }
}


template<typename T>
void Lista<T>::mergeSort(){
    int size = getSize();
    if(size == 1) return;
    int mid = size / 2;
    Lista <T> leftList, rightList;
    
    for (int i = 0; i < mid; i++)
    {
        leftList.insert(getAt(i) -> data);
    }
    for(int i = mid; i < size; i++)
    {
        rightList.insert(getAt(i) -> data);
    }
    
    leftList.mergeSort();
    rightList.mergeSort();
    clear();
    
    int lIndex = 0, rIndex = 0;
    int lSize = leftList.getSize();
    int rSize = rightList.getSize();
    while(lIndex < lSize && rIndex < rSize)
    {
        if(leftList.getAt(lIndex) -> data <= rightList.getAt(rIndex) -> data)
        {
            insert(leftList.getAt(lIndex) -> data);
            lIndex++;
        }
        else
        {
            insert(rightList.getAt(rIndex) -> data);
            rIndex++;
        }
        while(lIndex < lSize)
        {
            insert(leftList.getAt(lIndex) -> data);
            lIndex++;
        }
        while(rIndex < rSize)
        {
            insert(rightList.getAt(rIndex) -> data);
            rIndex++;
        }
    }
}

template<typename T>
void Lista<T>::radixSort() {
    int size = getSize();
    if (size <= 1) return;

    int maxValue = getAt(0)->data;
    for (int i = 1; i < size; i++) {
        int v = getAt(i)->data;
        if (v > maxValue) {maxValue = v;}
    }

    int cont = 0;
    int m = maxValue;
    if (m == 0) cont = 1;
    else {
        while (m != 0) { m /= 10; cont++; }
    }

    Lista< Lista<T>* > buckets;
    for (int b = 0; b < 10; b++) {
        buckets.insert(new Lista<T>());
    }
    
    int f = 1;
    for (int pass = 0; pass < cont; pass++) {

        for (int i = 0; i < size; i++) {
            int val = getAt(i)->data;
            int digito = (val / f) % 10;
            buckets.getAt(digito)->data->insert(val); 
        }

        clear();                          
        for (int b = 0; b < 10; ++b) {
            Lista<T>* buck = buckets.getAt(b)->data;
            int bs = buck->getSize();
            for (int j = 0; j < bs; ++j) {
                insert(buck->getAt(j)->data);
            }
        buck->clear(); 
        }

        f *= 10;
    }

    buckets.clear();

}

template<typename T>
void Lista<T>::heapify(int n, int i){
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if(l < n && getAt(l) -> data > getAt(largest) -> data){
        largest = l;
    }
    if(r < n && getAt(r) -> data > getAt(largest) -> data){
        largest = r;
    }
    if(largest != i){
        Swap(i, largest);
        heapify(n, largest);
    }
}


template<typename T>
void Lista<T>::heapSort() {
    int n = getSize();
    for(int i = n/2 - 1; i>=0; i--){
        heapify(n, i);
    }
    for(int i = n-1; i > 0; i--){
        Swap(0,i);
        heapify(i, 0);
    }
}

int main(){
    Lista<int> lista1;

    int elementos = 4000; // No superar los 4000
    int semilla = 1234;

    auto start = chrono::high_resolution_clock::now();
    auto end = chrono::high_resolution_clock::now();

    /*lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    lista1.print();
    cout << "======== SELECTION SORT ========" << endl;
    auto start = chrono::high_resolution_clock::now();
    lista1.selectSort();
    auto end = chrono::high_resolution_clock::now();
    lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    cout << "======== INSERTION SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.insertionSort();
    end = chrono::high_resolution_clock::now();
    lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    cout << "======== BUBBLE SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.bubbleSort();
    end = chrono::high_resolution_clock::now();
    lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
    */
    cout << "======== QUICK SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.quickSort(0, lista1.getSize() - 1);
    end = chrono::high_resolution_clock::now();
    //lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    cout << "======== BUCKET SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.bucketSort();
    end = chrono::high_resolution_clock::now();
    //lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    cout << "======== MERGE SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.mergeSort();
    end = chrono::high_resolution_clock::now();
    //lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;

    cout << "======== RADIX SORT ========" << endl;
    lista1.addRandomInt(0,2 * elementos,elementos, semilla);
    start = chrono::high_resolution_clock::now();
    lista1.radixSort();
    end = chrono::high_resolution_clock::now();
    //lista1.print();
    cout << "Tiempo demorado: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << endl;
} 