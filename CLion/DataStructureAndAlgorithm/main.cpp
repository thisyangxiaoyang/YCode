#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <random>
#include "DataStruct/SequentialBinaryTree.h"
#include "DataStruct/AVLTree.h"

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
    random_device e;
    for (int i = 0; i < 10; ++i) {
        atr.insert((int)e() % 10);
    }

    std::cout << "size: " << atr.size() << std::endl;
}

int main() {
    std::cout << "test" << std::endl;

    AVLTreeTest();

    return 0;
}
