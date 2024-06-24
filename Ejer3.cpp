#include <iostream>
#include <fstream>
#include <string>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>

class Persona {
private:
    std::string nombre;
    int edad;
    std::string direccion;

public:
    Persona() : nombre(""), edad(0), direccion("") {}

    Persona(const std::string& nombre, int edad, const std::string& direccion)
        : nombre(nombre), edad(edad), direccion(direccion) {}

    const std::string& getNombre() const {
        return nombre;
    }

    void setNombre(const std::string& nombre) {
        this->nombre = nombre;
    }

    int getEdad() const {
        return edad;
    }

    void setEdad(int edad) {
        this->edad = edad;
    }

    const std::string& getDireccion() const {
        return direccion;
    }

    void setDireccion(const std::string& direccion) {
        this->direccion = direccion;
    }

    template<class Archive>
    void serialize(Archive & archive) {
        archive(nombre, edad, direccion);
    }
};

// Función para serializar un objeto Persona en un archivo binario
void serializePersona(const Persona& persona, const std::string& filename) {
    std::ofstream os(filename, std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    archive(persona);
}

// Función para deserializar un objeto Persona desde un archivo binario
Persona deserializePersona(const std::string& filename) {
    Persona persona;
    std::ifstream is(filename, std::ios::binary);
    cereal::BinaryInputArchive archive(is);
    archive(persona);
    return persona;
}

int main() {
    // Creamos un objeto Persona
    Persona persona1("Juan Perez", 30, "Calle Principal 123");

    // Serializamos el objeto en un archivo binario
    serializePersona(persona1, "persona.bin");

    // Deserializamos el objeto desde el archivo binario
    Persona persona2 = deserializePersona("persona.bin");

    // Mostramos los atributos del objeto deserializado
    std::cout << "Nombre: " << persona2.getNombre() << std::endl;
    std::cout << "Edad: " << persona2.getEdad() << std::endl;
    std::cout << "Dirección: " << persona2.getDireccion() << std::endl;

    return 0;
}
