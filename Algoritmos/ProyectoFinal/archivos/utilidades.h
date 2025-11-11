#include <iostream> 
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

int search(const vector<int>& arr, int val) {
    return search(arr, val, 0, arr.size() - 1);
}

int search(const vector<int>& arr, int val, int left, int right) {
    int mid = (left+right) / 2;
    if(arr[mid] == val) {
        return mid;
    }

    if(arr[mid] > val) {
        return search(arr, val, 0, mid - 1);
    }

    if(arr[mid] < val) {
        return search(arr, val, mid + 1, right);
    }

    return -1;
}

int insertPos(const vector<int>& arr, int val) {
    return insertPos(arr, val, 0, arr.size() - 1);
}

int insertPos(const vector<int>& arr, int val, int left, int right) {
    if (left >= right) {
        return left;
    }

    int mid = (left + right) / 2;

    if (arr[mid] >= val) {
        return insertPos(arr, val, left, mid - 1);
    }
    else {
        return insertPos(arr, val, mid + 1, right);
    }

    return -1;
}



