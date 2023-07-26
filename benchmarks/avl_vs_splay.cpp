#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <iostream>

#include "splay_tree.h"
#include "avl_tree.h"


using std::vector;


int SIZE = 500000;

hwt::AVLTree avl_tree;
hwt::SplayTree splay_tree;
hwt::BinarySearchTree bs_tree;

vector<int> data;


void insert_banchmark() {
    auto t0 = clock();
    for (int i: data)
        avl_tree.insert(i);
    
    auto t1 = clock();
    for (int i: data)
        splay_tree.insert(i);

    auto t2 = clock();
    for (int i: data)
        bs_tree.insert(i);

    auto t3 = clock();
    std::cout << "AVLTree Insert: " << (float)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SplayTree Insert: " << (float)(t2 - t1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "BinarySearchTree Insert: " << (float)(t3 - t2) / CLOCKS_PER_SEC << std::endl;
    std::cout << std::endl;
}


void random_access_banchmark() {
    auto t0 = clock();
    for (int i: data)
        avl_tree.find(i);
    
    auto t1 = clock();
    for (int i: data)
        splay_tree.find(i);

    auto t2 = clock();
    for (int i: data)
        bs_tree.find(i);

    auto t3 = clock();
    std::cout << "AVLTree RandomAccess: " << (float)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SplayTree RandomAccess: " << (float)(t2 - t1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "BinarySearchTree RandomAccess: " << (float)(t3 - t2) / CLOCKS_PER_SEC << std::endl;
    std::cout << std::endl;
}


void sequential_access_banchmark() {
    auto t0 = clock();
    avl_tree.range_find(0, SIZE);
    
    auto t1 = clock();
    splay_tree.range_find(0, SIZE);

    auto t2 = clock();
    bs_tree.range_find(0, SIZE);

    auto t3 = clock();
    std::cout << "AVLTree SequentialAccess: " << (float)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SplayTree SequentialAccess: " << (float)(t2 - t1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "BinarySearchTree SequentialAccess: " << (float)(t3 - t2) / CLOCKS_PER_SEC << std::endl;
    std::cout << std::endl;
}


void one_element_access_banchmark() {
    auto t0 = clock();
    for (int i = 0; i < 1000000; i++)
        avl_tree.find(1);
    
    auto t1 = clock();
    for (int i = 0; i < 1000000; i++)
        splay_tree.find(1);

    auto t2 = clock();
    for (int i = 0; i < 1000000; i++)
        bs_tree.find(1);

    auto t3 = clock();
    std::cout << "AVLTree OneElementAccess: " << (float)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SplayTree OneElementAccess: " << (float)(t2 - t1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "BinarySearchTree OneElementAccess: " << (float)(t3 - t2) / CLOCKS_PER_SEC << std::endl;
    std::cout << std::endl;
}


void range_access_banchmark() {
    auto t0 = clock();
    for (int i = 0; i < data.size() / 10; i++)
        avl_tree.range_find(data[i], data[i] + 10);
    
    auto t1 = clock();
    for (int i = 0; i < data.size() / 10; i++)
        splay_tree.range_find(data[i], data[i] + 10);

    auto t2 = clock();
    for (int i = 0; i < data.size() / 10; i++)
        bs_tree.range_find(data[i], data[i] + 10);

    auto t3 = clock();
    std::cout << "AVLTree RangeAccess: " << (float)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
    std::cout << "SplayTree RangeAccess: " << (float)(t2 - t1) / CLOCKS_PER_SEC << std::endl;
    std::cout << "BinarySearchTree RangeAccess: " << (float)(t3 - t2) / CLOCKS_PER_SEC << std::endl;
    std::cout << std::endl;
}


int main(int argc, char** argv) {
    if (argc > 1)
        SIZE = atoi(argv[1]);

    data.resize(SIZE);
    for (int i = 0; i < SIZE; i++)
        data[i] = i;

    auto rng = std::default_random_engine {};

    std::shuffle(data.begin(), data.end(), rng);
    insert_banchmark();
    
    std::shuffle(data.begin(), data.end(), rng);
    random_access_banchmark();
 
    sequential_access_banchmark();
    one_element_access_banchmark();
    range_access_banchmark();


    return 0;
}