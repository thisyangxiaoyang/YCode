#include <iostream>
#include "DataStruct/SequentialBinaryTree.h"


int main() {
    std::cout << "test" << std::endl;
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

    return 0;
}
