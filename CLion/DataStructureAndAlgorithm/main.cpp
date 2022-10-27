#include <iostream>
#include "DataStruct/SequentialBinaryTree.h"
#include "DataStruct/AVLTree.h"
#include <set>

using namespace std;

void SequentialBinaryTreeTest() {
    yang::SequentialBinaryTree<int> tr;

    int arr[] = {6, 3, 2, 5, 0, 9, 4, 8, 7, 1};
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    for (auto v: arr) {
        tr.insert(v);
    }
    for (auto &v: arr2) {
        tr.erase(v);
    }
    for (auto &v: arr) {
        tr.insert(v);
    }

    std::cout << tr << std::endl;
}

void AVLTreeTest() {
    yang::AVLTree<int> atr;
    int arr[] = {3, 5, 1, 0, 9, 7, 6, 2, 8, 4};
    int del_arr[] = {4, 3, 2, 1, 0, 5, 6, 7, 8, 9};
    for (int & i : arr) {
        atr.insert(i);
    }
    for (int & i: del_arr) {
        atr.erase(i);
    }

    cout << atr << endl;
    std::cout << "size: " << atr.size() << std::endl;
}

int main() {
    std::cout << "test" << std::endl;

    AVLTreeTest();

    return 0;
}
