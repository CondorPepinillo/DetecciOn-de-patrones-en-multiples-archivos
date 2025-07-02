#include <string>
#include <fstream>
#include <iostream>

#include <sstream>
using namespace std;

string toString(int argc, char* argv[], string separator) {
    string text;
    for(int i=0; i<argc; i++) {
        ifstream file(argv[i]);
        if (!file.is_open()) {
            cerr << "No se pudo abrir el archivo: " << argv[i] << endl;
            return "Error al abrir el archivo";
        }

        stringstream buffer;
        buffer << file.rdbuf();
        text = text + buffer.str() + separator;

    }
    return text;
}