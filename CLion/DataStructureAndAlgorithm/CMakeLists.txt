cmake_minimum_required(VERSION 3.21)
project(DataStructureAndAlgorithm)

set(CMAKE_CXX_STANDARD 14)

add_executable(DataStructureAndAlgorithm main.cpp DataStruct/MyVector.h DataStruct/SequentialBinaryTree.h DataStruct/AVLTree.h DataStruct/_23Tree.h)
