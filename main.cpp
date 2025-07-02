using namespace std;
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "boyer_moore.h"
#include "toString.h"

int main(int argc, char* argv[])
{
    //Por el funcionamiento de "argv", el primer elemento corresponde al nombre del programa, por lo tanto debemos fijarnos si argc es mayor a 2
    if (argc < 2) {
    cerr << "Uso: " << argv[0] << " <archivo1> [archivo2] ...\n";
    return 0;
    }

    //Con nuestro función toString tomamos todos los textos de los archivos y los concatenamos en un solo string, podemos escoger el separador que queramos
    string separador ="$";
    string textoDondeBuscar = toString(argc - 1, &argv[1], separador);
    string patron;

    return 0;


}
