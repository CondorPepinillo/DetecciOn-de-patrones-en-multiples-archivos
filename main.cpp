using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "boyer_moore.h"
#include "toString.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
    return 0;
    }

    string resultado = toString(argc - 1, &argv[1]);

    cout << "Texto combinado de los archivos:\n" << resultado << endl;
    return 0;
}
