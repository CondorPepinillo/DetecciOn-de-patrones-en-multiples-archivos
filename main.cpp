using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "boyer_moore.h"



int main()
{
    ifstream file("datasets/English/english_00");
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo." << endl;
        return 1;
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string txt = buffer.str();

    // Patrón a buscar
    string pat = "This"; // Cambia esto por el patrón que desees

    int count = 0;

    while(getline(file, txt))
    {
        search(txt, pat, &count);
    }

    cout << "El patrón '" << pat << "' se encontró " << count << " veces en el archivo." << endl;

    file.close();
    return 0;
}
