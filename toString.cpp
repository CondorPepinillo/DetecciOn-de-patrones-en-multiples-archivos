#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

string toString(int argc, const char* argv[], string separator) {
    string text;
    for (int i = 0; i < argc; i++) {
        ifstream file(argv[i]);
        if (!file.is_open()) {
            cerr << "No se pudo abrir el archivo: " << argv[i] << endl;
            return "Error al abrir el archivo";
        }

        stringstream buffer;
        buffer << file.rdbuf();
        text += buffer.str();

        // Agregar separador solo entre archivos, no después del último
        if (i < argc - 1) {
            text += separator;
        }
    }
    return text;
}