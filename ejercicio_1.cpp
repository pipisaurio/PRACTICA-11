#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
	
	ofstream outFile("archivo.txt");
	outFile << "Este es el contenido del archivo de texto creado por el programa." << endl;
	outFile.close();
	
	
	string nombreArchivo;
	cout << "Ingrese el nombre del archivo de texto: ";
	getline(cin, nombreArchivo);
	
	ifstream inFile(nombreArchivo);
	if (inFile.is_open()) {
		string linea;
		while (getline(inFile, linea)) {
			cout << linea << endl;
		}
		inFile.close();
	} else {
		cout << "Error: El archivo " << nombreArchivo << " no existe." << endl;
	}
	
	return 0;
}
