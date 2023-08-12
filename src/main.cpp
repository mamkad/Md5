#include "headers/md5.hpp"
using lib::algs::hash::md5;

#include <iostream>
using std::cout;

int main(int argc, char** argv)
{
    if (argc != 2) {
        return 1;
    }
    cout << md5::hash(argv[1]) << '\n';
    return 0;
}