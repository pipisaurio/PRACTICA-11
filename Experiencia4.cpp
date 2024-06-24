#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <stdexcept>

using namespace std;

const int MAX_PRODUCTOS = 100;

class Producto {
private:
    int codigo;
    string nombre;
    double precio;
    int cantidad;

public:
    Producto() : codigo(0), precio(0), cantidad(0), nombre("") {}
    Producto(int c, const string& n, double p, int cant) : codigo(c), nombre(n), precio(p), cantidad(cant) {}

    int getCodigo() const { return codigo; }
    const string& getNombre() const { return nombre; }
    double getPrecio() const { return precio; }
    int getCantidad() const { return cantidad; }

    void setCodigo(int c) { codigo = c; }
    void setPrecio(double p) { precio = p; }
    void setCantidad(int cant) { cantidad = cant; }

    bool vender(int cantidadVendida) {
        if (cantidadVendida > cantidad) {
            throw runtime_error("Cantidad insuficiente para realizar la venta.");
        }
        cantidad -= cantidadVendida;
        return true;
    }

    void mostrarProducto() const {
        cout << "Código: " << codigo << ", Nombre: " << nombre << ", Precio: " << precio << ", Cantidad: " << cantidad << endl;
    }

    void guardarProducto(FILE* archivo) const {
        fprintf(archivo, "%d,%s,%.2f,%d\n", codigo, nombre.c_str(), precio, cantidad);
    }

    void cargarProducto(FILE* archivo) {
        char buffer[100];
        fscanf(archivo, "%d,%99[^,],%lf,%d\n", &codigo, buffer, &precio, &cantidad);
        nombre = buffer;
    }
};

struct IndiceProducto {
    string nombre;
    long posicion;
};

vector<IndiceProducto> indice;

void crearIndice(const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) return;

    char header[100];
    fgets(header, sizeof(header), archivo); // Leer la línea de encabezado

    indice.clear();
    char buffer[256];
    long pos = ftell(archivo);
    while (fgets(buffer, sizeof(buffer), archivo)) {
        char nombre[100];
        sscanf(buffer, "%*d,%99[^,]", nombre);
        indice.push_back({string(nombre), pos});
        pos = ftell(archivo);
    }

    fclose(archivo);
}

int cargarInventario(Producto inventario[], const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        cout << "El archivo no existe, creando uno nuevo." << endl;
        archivo = fopen(nombreArchivo, "w");
        if (archivo == NULL) {
            throw runtime_error("No se pudo crear el archivo de inventario.");
        }
        fprintf(archivo, "Código,Nombre,Precio,Cantidad\n");
        fclose(archivo);
        archivo = fopen(nombreArchivo, "r");
    }

    if (archivo == NULL) {
        throw runtime_error("No se pudo abrir el archivo de inventario.");
    }

    char header[100];
    fgets(header, sizeof(header), archivo);

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

void guardarInventario(const Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        throw runtime_error("No se pudo abrir el archivo de inventario para guardar.");
    }

    fprintf(archivo, "Código,Nombre,Precio,Cantidad\n");

    for (int i = 0; i < cantidadProductos; ++i) {
        inventario[i].guardarProducto(archivo);
    }

    fclose(archivo);
}

void agregarProducto(Producto inventario[], int &cantidadProductos, const char* nombreArchivo) {
    if (cantidadProductos >= MAX_PRODUCTOS) {
        cout << "No se puede agregar más productos. Inventario lleno." << endl;
        return;
    }

    string nombre;
    double precio;
    int cantidad;
    int codigo = cantidadProductos + 1;

    cout << "Ingrese el nombre del producto: ";
    cin.ignore();
    getline(cin, nombre);
    cout << "Ingrese el precio del producto: ";
    cin >> precio;
    cout << "Ingrese la cantidad disponible del producto: ";
    cin >> cantidad;

    inventario[cantidadProductos] = Producto(codigo, nombre, precio, cantidad);
    cantidadProductos++;

    try {
        guardarInventario(inventario, cantidadProductos, nombreArchivo);
        cout << "Producto agregado y guardado exitosamente." << endl;
        crearIndice(nombreArchivo);
    } catch (const runtime_error& e) {
        cerr << "Error al guardar el inventario: " << e.what() << endl;
    }
}

void listarProductos(const Producto inventario[], int cantidadProductos) {
    cout << "Productos disponibles:\n";
    for (int i = 0; i < cantidadProductos; ++i) {
        cout << i + 1 << ". ";
        inventario[i].mostrarProducto();
    }
}

void actualizarProducto(Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    listarProductos(inventario, cantidadProductos);

    int seleccion;
    cout << "Ingrese el número del producto a actualizar: ";
    cin >> seleccion;
    seleccion--;

    if (seleccion >= 0 && seleccion < cantidadProductos) {
        double nuevoPrecio;
        int nuevaCantidad;
        cout << "Ingrese el nuevo precio: ";
        cin >> nuevoPrecio;
        cout << "Ingrese la nueva cantidad: ";
        cin >> nuevaCantidad;
        inventario[seleccion].setPrecio(nuevoPrecio);
        inventario[seleccion].setCantidad(nuevaCantidad);

        try {
            guardarInventario(inventario, cantidadProductos, nombreArchivo);
            cout << "Producto actualizado y guardado exitosamente." << endl;
            crearIndice(nombreArchivo);
        } catch (const runtime_error& e) {
            cerr << "Error al guardar el inventario: " << e.what() << endl;
        }
    } else {
        cout << "Selección no válida." << endl;
    }
}

void venderProducto(Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    listarProductos(inventario, cantidadProductos);

    int seleccion;
    cout << "Ingrese el número del producto a vender: ";
    cin >> seleccion;
    seleccion--;

    if (seleccion >= 0 && seleccion < cantidadProductos) {
        int cantidadVendida;
        cout << "Ingrese la cantidad a vender: ";
        cin >> cantidadVendida;

        try {
            if (inventario[seleccion].vender(cantidadVendida)) {
                guardarInventario(inventario, cantidadProductos, nombreArchivo);
                cout << "Venta realizada y guardada exitosamente." << endl;
                crearIndice(nombreArchivo);
            }
        } catch (const runtime_error& e) {
            cerr << "Error al realizar la venta: " << e.what() << endl;
        }
    } else {
        cout << "Selección no válida." << endl;
    }
}

void generarInforme(const Producto inventario[], int cantidadProductos) {
    cout << "Informe de Inventario:\n";
    for (int i = 0; i < cantidadProductos; ++i) {
        inventario[i].mostrarProducto();
    }
}

Producto buscarProductoPorNombre(const string& nombre, const char* nombreArchivo) {
    auto it = find_if(indice.begin(), indice.end(),
                      [&nombre](const IndiceProducto& ip) { return ip.nombre == nombre; });

    if (it != indice.end()) {
        FILE* archivo = fopen(nombreArchivo, "r");
        if (archivo == NULL) throw runtime_error("No se pudo abrir el archivo para buscar el producto.");

        fseek(archivo, it->posicion, SEEK_SET);
        Producto producto;
        producto.cargarProducto(archivo);
        fclose(archivo);
        return producto;
    }

    return Producto();
}

void buscarProducto(const Producto inventario[], int cantidadProductos, const char* nombreArchivo) {
    string nombreBuscado;
    cout << "Ingrese el nombre del producto a buscar: ";
    cin.ignore();
    getline(cin, nombreBuscado);

    try {
        Producto productoEncontrado = buscarProductoPorNombre(nombreBuscado, nombreArchivo);

        if (productoEncontrado.getCodigo() != 0) {
            cout << "Producto encontrado:" << endl;
            productoEncontrado.mostrarProducto();
        } else {
            cout << "Producto no encontrado." << endl;
        }
    } catch (const runtime_error& e) {
        cerr << "Error al buscar el producto: " << e.what() << endl;
    }
}

int main() {
    Producto inventario[MAX_PRODUCTOS];
    int cantidadProductos;

    try {
        cantidadProductos = cargarInventario(inventario, "inventario.txt");
        crearIndice("inventario.txt");
    } catch (const runtime_error& e) {
        cerr << "Error al cargar el inventario: " << e.what() << endl;
        return 1;
    }

    int opcion;

    do {
        cout << "====== TIENDA UwU ======\n"
             << "1. Agregar nuevo producto\n"
             << "2. Actualizar producto\n"
             << "3. Vender producto\n"
             << "4. Generar informe de inventario\n"
             << "5. Buscar producto por nombre\n"
             << "6. Salir\n"
             << "Seleccione una opción: ";
        cin >> opcion;

        try {
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
                    buscarProducto(inventario, cantidadProductos, "inventario.txt");
                    break;
                case 6:
                    cout << "Saliendo del programa." << endl;
                    break;
                default:
                    cout << "Opción no válida. Intente nuevamente." << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    } while (opcion != 6);

    return 0;
}