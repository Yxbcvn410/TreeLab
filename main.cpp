#include <chrono>
#include "AbstractTree.cpp"
#include "AVLTree.cpp"
#include<fstream>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "BlackRedTree.cpp"


using T = int;
using Tree = std::shared_ptr<AbstractTree<T>>;

class TestingSystem {
public:
    Tree my_tree;

    explicit TestingSystem(Tree my_tree) {
        this->my_tree = my_tree;
    }

    ~TestingSystem()
    = default;

    static double get_time() {
        return std::chrono::duration_cast<std::chrono::microseconds>
                (std::chrono::steady_clock::now().time_since_epoch()).count(); // use nanoseconds
    }

    void check_all(std::vector<int> &number_tests, std::string &final_file, int M) {
        std::vector<double> add, check, remove;
        for (int k = 0; k < M; k++) {
            double start = get_time();
            for (int number_test : number_tests) {
                my_tree->add(number_test);
            }
            double end = get_time();
            add.push_back((end - start) / number_tests.size());

            start = get_time();
            for (int number_test : number_tests) {
                my_tree->check(number_test);
            }
            end = get_time();
            check.push_back((end - start) / number_tests.size());

            start = get_time();
            for (int number_test : number_tests) {
                my_tree->remove(number_test);
            }
            end = get_time();
            remove.push_back((end - start) / number_tests.size());
        }
        std::ofstream ofs_final(final_file); //без &? Да. без &.
        for (auto a : add)
            ofs_final << a << " ";
        ofs_final << std::endl;
        for (auto a : check)
            ofs_final << a << " ";
        ofs_final << std::endl;
        for (auto a : remove)
            ofs_final << a << " ";
        ofs_final << std::endl;
    }

};


int main() {
    Tree tree;
    std::string type_of_tree;
    std::cout << "What type of tree do you want to use? BlackRedTree or AVLTree?";
    std::cin >> type_of_tree;
    if (type_of_tree == "BlackRedTree") {
        tree = Tree(new BlackRedTree<T>());
    } else if (type_of_tree == "AVLTree") {
        tree = Tree(new AVLTree<T>());

    }
    std::string tree_file, number_file, final_file;
    std::cout
            << "tree? tests? answers?" << std::endl;
    std::cin >> tree_file >> number_file >> final_file;
    int m = 0;
    std::cout << " Please, enter number of repeats";
    std::cin >> m;


    int size_of_tree = 0;
    std::ifstream ifs_tree(tree_file);
    if (ifs_tree.is_open()) {
        int a = 0;
        while (ifs_tree >> a) {
            size_of_tree += 1;
            tree->add(a);
        }
    }
    ifs_tree.close();
    std::ofstream ofs_final(final_file);
    std::cout << "size of tree = " << size_of_tree << std::endl;
    ofs_final.close();
    std::vector<int> numbers_for_test;
    std::ifstream ifs_test(number_file);
    if (ifs_test.is_open()) {
        int a = 0;
        while (ifs_test >> a) {
            numbers_for_test.push_back(a);
        }
    }
    ifs_test.close();

    TestingSystem(tree).check_all(numbers_for_test, final_file, m);
}
