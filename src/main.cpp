#include <iostream>
#include <sstream>
using namespace std;

#include "Procedure.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        return -1; // error
    }
    const char* inpname = argv[1];
    const char* outname = argv[2];
    bool tree_view = false;
    const char* flag = "-t";
    if (argc == 4 && argv[3] == flag) tree_view = true;
    stringstream buffer;
    Procedure P = Procedure();
    try {
        P.Buchi_Rabin(buffer, inpname, outname, tree_view);
    }
    catch (const std::exception&) {
        return -1;
    }
    std::cout << buffer.str();
    return 0;
}
