#include <iostream>
#include "../Bin/Md5.hpp"

using Lib::Algs::Hash::Md5;
using std::cout;

int main(int argc, char** argv)
{
    if (argc != 2) {
        return 1;
    }

    cout << Md5::hash(argv[1]) << '\n';
}