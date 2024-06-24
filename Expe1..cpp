#include <iostream>
#include <string>
#include <cstdio>   // Para manejar archivos usando FILE*
using namespace std;

const int MAX_PRODUCTOS = 100; // Máximo número de productos que podemos manejar

// Definición de la clase Producto
class Producto {
private:
    string nombre;
    double precio;
    int cantidad;

public:
    // Constructor por defecto
    Producto() : precio(0), cantidad(0) {
        nombre = "";
    }

    // Constructor parametrizado
    Producto(const string& n, double p, int c) : nombre(n), precio(p), cantidad(c) {}

    // Getters y setters
    const string& getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    int getCantidad() const { return cantidad; }

    void setPrecio(double p) { precio = p; }
    void setCantidad(int c) { cantidad = c; }

    // Método para reducir la cantidad (ventas)
    bool vender(int cantidadVendida) {
        if (cantidadVendida > cantidad) {
            return false; // No hay suficiente cantidad para vender
        }
        cantidad -= cantidadVendida;
        return true;
    }

    // Método para mostrar la información del producto
    void mostrarProducto() const {
        cout << "Nombre: " << nombre << ", Precio: " << precio << ", Cantidad: " << cantidad << endl;
    }

    // Método para guardar la información del producto en un archivo
    void guardarProducto(FILE* archivo) const {
        fprintf(archivo, "%s %.2f %d\n", nombre.c_str(), precio, cantidad);
    }

    // Método para cargar la información del producto desde un archivo
    void cargarProducto(FILE* archivo) {
        char buffer[100];
        fscanf(archivo, "%s %lf %d", buffer, &precio, &cantidad);
        nombre = buffer; // Convertir el buffer a string
    }
};

// Función para cargar el inventario desde el archivo
int cargarInventario(Producto inventario[], const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r"); 
    if (archivo == NULL) {
        cout << "No se pudo abrir el archivo de inventario." << endl;
        return 0;
    }

    int contador = 0;
    while (!feof(archivo) && contador < MAX_PRODUCTOS) {
        Producto producto;
        producto.cargarProducto(archivo);
        if (feof(archivo)) break; // Evitar agregar una entrada vacía al final
        inventario[contador] = producto;
        contador++;
    }

    fclose(archivo);
    return contador;
}

// Función para guardar el inventario en el archivo
void guardarInventario(const Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        cout << "No se pudo abrir el archivo de inventario para guardar." << endl;
        return;
    }

    for (int i = 0; i < cantidadProductos; ++i) {
        inventario[i].guardarProducto(archivo);
    }

    fclose(archivo);
}

// Función para agregar un nuevo producto al inventario
void agregarProducto(Producto inventario[], int &cantidadProductos) {
    if (cantidadProductos >= MAX_PRODUCTOS) {
        cout << "No se puede agregar más productos. Inventario lleno." << endl;
        return;
    }

    string nombre;
    double precio;
    int cantidad;

    cout << "Ingrese el nombre del producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese el precio del producto: ";
    cin >> precio;
    cout << "Ingrese la cantidad disponible del producto: ";
    cin >> cantidad;

    inventario[cantidadProductos] = Producto(nombre, precio, cantidad);
    cantidadProductos++;
    cout << "Producto agregado exitosamente." << endl;
}

// Función para actualizar un producto existente
void actualizarProducto(Producto inventario[], int cantidadProductos) {
    string nombre;
    cout << "Ingrese el nombre del producto a actualizar: ";
    cin.ignore();
    getline(cin, nombre);

    bool encontrado = false;
    for (int i = 0; i < cantidadProductos; ++i) {
        if (inventario[i].getNombre() == nombre) {
            double nuevoPrecio;
            int nuevaCantidad;
            cout << "Ingrese el nuevo precio: ";
            cin >> nuevoPrecio;
            cout << "Ingrese la nueva cantidad: ";
            cin >> nuevaCantidad;
            inventario[i].setPrecio(nuevoPrecio);
            inventario[i].setCantidad(nuevaCantidad);
            cout << "Producto actualizado exitosamente." << endl;
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Producto no encontrado." << endl;
    }
}

// Función para realizar una venta
void venderProducto(Producto inventario[], int cantidadProductos) {
    string nombre;
    int cantidadVendida;
    cout << "Ingrese el nombre del producto a vender: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese la cantidad a vender: ";
    cin >> cantidadVendida;

    bool encontrado = false;
    for (int i = 0; i < cantidadProductos; ++i) {
        if (inventario[i].getNombre() == nombre) {
            if (inventario[i].vender(cantidadVendida)) {
                cout << "Venta realizada exitosamente." << endl;
            } else {
                cout << "Cantidad insuficiente para realizar la venta." << endl;
            }
            encontrado = true;
            break;
        }
    }

    if (!encontrado) {
        cout << "Producto no encontrado." << endl;
    }
}

// Función para generar un informe de inventario
void generarInforme(const Producto inventario[], int cantidadProductos) {
    cout << "Informe de Inventario:\n";
    for (int i = 0; i < cantidadProductos; ++i) {
        inventario[i].mostrarProducto();
    }
}

int main() {
    Producto inventario[MAX_PRODUCTOS];
    int cantidadProductos = cargarInventario(inventario, "inventario.txt");
    int opcion;

    do {
        cout << "\n1. Agregar nuevo producto\n"
             << "2. Actualizar producto\n"
             << "3. Vender producto\n"
             << "4. Generar informe de inventario\n"
             << "5. Salir\n"
             << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarProducto(inventario, cantidadProductos);
                guardarInventario(inventario, cantidadProductos, "inventario.txt");
                break;
            case 2:
                actualizarProducto(inventario, cantidadProductos);
                guardarInventario(inventario, cantidadProductos, "inventario.txt");
                break;
            case 3:
                venderProducto(inventario, cantidadProductos);
                guardarInventario(inventario, cantidadProductos, "inventario.txt");
                break;
            case 4:
                generarInforme(inventario, cantidadProductos);
                break;
            case 5:
                cout << "Hasta Luego." << endl;
                break;
            default:
                cout << "Opción no válida. Intente nuevamente." << endl;
        }
    } while (opcion != 5);

    return 0;
}
