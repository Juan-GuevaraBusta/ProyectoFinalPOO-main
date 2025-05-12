#include "Jugador.h"

Jugador::Jugador(std::string nombreJugador) {
    this -> nombreJugador = nombreJugador;
    this -> puntaje = 0;
};

Jugador::Jugador() {
};

Jugador::~Jugador() {

};

std::string Jugador::getNombreJugador() {
    return nombreJugador;
};

int Jugador::getPuntaje() {
    return puntaje;
};

void Jugador::setNombreJugador(std::string nombreJugador) {
    this -> nombreJugador = nombreJugador;
};

void Jugador::setPuntaje(int puntaje) {
    this -> puntaje = puntaje;
};