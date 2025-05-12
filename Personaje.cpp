#include "Personaje.h"

Personaje::Personaje() {
};

Personaje::Personaje(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion) {
    this -> nombre = nombre;
    this -> vitalidad = vitalidad;
    this -> alimento = alimento;
    this -> posicion = posicion;
};

Personaje::~Personaje() {
};

std::string Personaje::getNombre() {
    return nombre;
};

int Personaje::getVitalidad() {
    return vitalidad;
};

int Personaje::getAlimento() {
    return alimento;
};

std::vector<int> Personaje::getPosicion() {
    return posicion;
};

void Personaje::setNombre(std::string nombre) {
    this -> nombre = nombre;
};

void Personaje::setVitalidad(int vitalidad) {
    this -> vitalidad = vitalidad;
};

void Personaje::setAlimento(int alimento) {
    this -> alimento = alimento;
};

void Personaje::setPosicion(std::vector<int> posicion) {
    this -> posicion = posicion;
};
