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


class TestingSystem
{
    private:
    int M = 10;
    public:
    Tree my_tree;
    //не понимаю, когда объявлять, когда опредять конструктор, надо ли туда что-то писать и что;
    TestingSystem(Tree my_tree)
    {
        this -> my_tree = my_tree;
    }
    ~TestingSystem()
    {

    }

    double get_time()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>
                       (std::chrono::steady_clock::now().time_since_epoch()).count()/1e6;
    }

using time_t = double;

    void checking_all(std::vector<int> & number_tests, std::string & final_file)
    {
        for(int k = 0; k < M; k++)
        {
            std::ofstream ofs_final(final_file); //без &?
            time_t start = get_time();
            for(int i = 0; i < number_tests.size(); i++)
            {
                my_tree -> add(number_tests[i]);
            }
            time_t end = get_time();
            std::cout << (end - start) / number_tests.size() << " "; //time for n = number_tests.size() elements
            start = get_time();
            for(int i = 0; i < number_tests.size(); i++)
            {
                my_tree -> check(number_tests[i]);
            }
            end = get_time();
            std::cout << (end - start) / number_tests.size() << " ";
            start = get_time();
            for (int i = 0; i < number_tests.size(); i++)
            {
                my_tree -> remove(number_tests[i]);
            }
            end = get_time();
            std::cout << (end - start) / number_tests.size() << " ";
            std::cout << "the " << k << "-th" << " running from" << M;

        }
    }

};


int main() {
   // Tree tree(new AVLTree<int>());
    std::shared_ptr<AbstractTree<T>> tree(new BlackRedTree<T>());
    std::string tree_file, number_file, final_file;
    std::cout << "Please, enter names of 3 files:1)for creation of the tree; 2)with numbers for tests; 3)for final answers";
    std::cin >> tree_file >> number_file >> final_file;

    int size_of_tree = 0;
    std::ifstream ifs_tree(tree_file);
    if (ifs_tree.is_open())
    {
        int a = 0;
        while(std::cin >> a)
        {
            size_of_tree += 1;
            tree -> add(a);
        }
    }
    ifs_tree.close();
    std::ofstream ofs_final(final_file);
    std::cout  << "size of tree = " << size_of_tree << std::endl;
    ofs_final.close();
    std::vector<int> numbers_for_test;
    std::ifstream ifs_test(number_file);
    if (ifs_test.is_open())
    {
        int a = 0;
        while(std::cin >> a)
        {
            numbers_for_test.push_back(a);
        }
    }
    ifs_test.close();


    TestingSystem(tree).checking_all(numbers_for_test, final_file);
}
