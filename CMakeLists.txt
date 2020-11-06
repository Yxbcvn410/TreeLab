cmake_minimum_required(VERSION 3.15)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(TreeLab AbstractTree.cpp main.cpp BlackRedTree.cpp AVLTree.cpp)