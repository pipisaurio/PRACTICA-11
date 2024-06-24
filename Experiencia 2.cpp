#include <iostream>
#include <string>
#include <cstdio>   // Para manejar archivos usando FILE*
using namespace std;

const int MAX_PRODUCTOS = 100; // Máximo número de productos que podemos manejar

class Producto {
private:
    int codigo;
    string nombre;
    double precio;
    int cantidad;

public:
    // Constructor por defecto
    Producto() : codigo(0), precio(0), cantidad(0), nombre("") {}

    // Constructor parametrizado
    Producto(int c, const string& n, double p, int cant) : codigo(c), nombre(n), precio(p), cantidad(cant) {}

    // Getters y setters
    int getCodigo() const { return codigo; }
    const string& getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    int getCantidad() const { return cantidad; }

    void setCodigo(int c) { codigo = c; }
    void setPrecio(double p) { precio = p; }
    void setCantidad(int cant) { cantidad = cant; }

    // Método para reducir la cantidad (ventas)
    bool vender(int cantidadVendida) {
        if (cantidadVendida > cantidad) {
            return false;
        }
        cantidad -= cantidadVendida;
        return true;
    }

    // Método para mostrar la información del producto
    void mostrarProducto() const {
        cout << "Código: " << codigo << ", Nombre: " << nombre << ", Precio: " << precio << ", Cantidad: " << cantidad << endl;
    }

    // Método para guardar la información del producto en un archivo
    void guardarProducto(FILE* archivo) const {
        fprintf(archivo, "%d %s %.2f %d\n", codigo, nombre.c_str(), precio, cantidad);
    }

    // Método para cargar la información del producto desde un archivo
    void cargarProducto(FILE* archivo) {
        char buffer[100];
        fscanf(archivo, "%d %s %lf %d", &codigo, buffer, &precio, &cantidad);
        nombre = buffer;
    }
};

// Función para cargar el inventario desde el archivo
int cargarInventario(Producto inventario[], const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        cout << "El archivo no existe, creando uno nuevo." << endl;
        archivo = fopen(nombreArchivo, "w");
        if (archivo == NULL) {
            cout << "No se pudo crear el archivo de inventario." << endl;
            return 0;
        }
        fprintf(archivo, "Código Nombre Precio Cantidad\n"); // Agregar títulos
        fclose(archivo);
        archivo = fopen(nombreArchivo, "r");
    }

    if (archivo == NULL) {
        cout << "No se pudo abrir el archivo de inventario." << endl;
        return 0;
    }

    char header[100];
    fgets(header, sizeof(header), archivo); // Leer la primera línea de títulos

    int contador = 0;
    while (!feof(archivo) && contador < MAX_PRODUCTOS) {
        Producto producto;
        producto.cargarProducto(archivo);
        if (feof(archivo)) break;
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

    fprintf(archivo, "Código Nombre Precio Cantidad\n"); // Escribir títulos

    for (int i = 0; i < cantidadProductos; ++i) {
        inventario[i].guardarProducto(archivo);
    }

    fclose(archivo);
}

// Función para agregar un nuevo producto al inventario y guardarlo en el archivo
void agregarProducto(Producto inventario[], int &cantidadProductos, const char* nombreArchivo) {
    if (cantidadProductos >= MAX_PRODUCTOS) {
        cout << "No se puede agregar más productos. Inventario lleno." << endl;
        return;
    }

    string nombre;
    double precio;
    int cantidad;
    int codigo = cantidadProductos + 1; // Asignar un código único basado en el número de productos

    cout << "Ingrese el nombre del producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese el precio del producto: ";
    cin >> precio;
    cout << "Ingrese la cantidad disponible del producto: ";
    cin >> cantidad;

    inventario[cantidadProductos] = Producto(codigo, nombre, precio, cantidad);
    cantidadProductos++;

    FILE* archivo = fopen(nombreArchivo, "a");
    if (archivo != NULL) {
        inventario[cantidadProductos - 1].guardarProducto(archivo);
        fclose(archivo);
        cout << "Producto agregado y guardado exitosamente." << endl;
    } else {
        cout << "No se pudo abrir el archivo de inventario para agregar." << endl;
    }
}

// Función para mostrar una lista numerada de productos
void listarProductos(const Producto inventario[], int cantidadProductos) {
    cout << "Productos disponibles:\n";
    for (int i = 0; i < cantidadProductos; ++i) {
        cout << i + 1 << ". ";
        inventario[i].mostrarProducto();
    }
}

// Función para actualizar un producto existente y guardarlo en el archivo
void actualizarProducto(Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    listarProductos(inventario, cantidadProductos);

    int seleccion;
    cout << "Ingrese el número del producto a actualizar: ";
    cin >> seleccion;
    seleccion--; // Ajustar para índice de array

    if (seleccion >= 0 && seleccion < cantidadProductos) {
        double nuevoPrecio;
        int nuevaCantidad;
        cout << "Ingrese el nuevo precio: ";
        cin >> nuevoPrecio;
        cout << "Ingrese la nueva cantidad: ";
        cin >> nuevaCantidad;
        inventario[seleccion].setPrecio(nuevoPrecio);
        inventario[seleccion].setCantidad(nuevaCantidad);

        guardarInventario(inventario, cantidadProductos, nombreArchivo);
        cout << "Producto actualizado y guardado exitosamente." << endl;
    } else {
        cout << "Selección no válida." << endl;
    }
}

// Función para realizar una venta y actualizar el archivo
void venderProducto(Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    listarProductos(inventario, cantidadProductos);

    int seleccion;
    cout << "Ingrese el número del producto a vender: ";
    cin >> seleccion;
    seleccion--; // Ajustar para índice de array

    if (seleccion >= 0 && seleccion < cantidadProductos) {
        int cantidadVendida;
        cout << "Ingrese la cantidad a vender: ";
        cin >> cantidadVendida;

        if (inventario[seleccion].vender(cantidadVendida)) {
            guardarInventario(inventario, cantidadProductos, nombreArchivo);
            cout << "Venta realizada y guardada exitosamente." << endl;
        } else {
            cout << "Cantidad insuficiente para realizar la venta." << endl;
        }
    } else {
        cout << "Selección no válida." << endl;
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
        cout << "====== TIENDA UwU ======"
             <<"\n1. Agregar nuevo producto\n"
             << "2. Actualizar producto\n"
             << "3. Vender producto\n"
             << "4. Generar informe de inventario\n"
             << "5. Salir\n"
             << "Seleccione una opción: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                agregarProducto(inventario, cantidadProductos, "inventario.txt");
                break;
            case 2:
                actualizarProducto(inventario, cantidadProductos, "inventario.txt");
                break;
            case 3:
                venderProducto(inventario, cantidadProductos, "inventario.txt");
                break;
            case 4:
                generarInforme(inventario, cantidadProductos);
                break;
            case 5:
                cout << "Saliendo del programa." << endl;
                break;
            default:
                cout << "Opción no válida. Intente nuevamente." << endl;
        }
    } while (opcion != 5);

    return 0;
}