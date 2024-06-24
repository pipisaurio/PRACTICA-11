#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

// Función 
double divide(int numerator, int denominator) {
    if (denominator == 0) {
        throw runtime_error("No se puede dividir por cero");
    }
    return static_cast<double>(numerator) / denominator;
}

int main() {
    int num1, num2;
    double result;

    cout << "Ingrese el numerador: ";
    cin >> num1;
    cout << "Ingrese el denominador: ";
    cin >> num2;

    try {
        result = divide(num1, num2);
        cout << "Resultado de " << num1 << " / " << num2 << " = " << result << endl;
    } catch (const runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
