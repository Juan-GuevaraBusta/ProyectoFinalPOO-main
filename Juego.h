//
// Created by ADRIANA on 3/05/2025.
//

#ifndef JUEGO_H
#define JUEGO_H

#include "Personaje.h"
#include "Jugador.h"
#include "Escenario.h"
#include <vector>
#include <map>
#include <string>

class Juego {
private:
    std::vector<Personaje*> personajes;
    std::map<std::string, Jugador*> jugadores;
    std::vector<Escenario*> escenarios;
    std::map<int, std::string> top10;  // Mapa de puntaje a nombre

public:
    Juego();
    ~Juego();

    // Métodos específicos
    void mostrarTop10();
    void guardarJugador(Jugador* jugador);
    void mostrarJugadores();

    // Getters
    std::vector<Personaje*> getPersonajes();
    std::map<std::string, Jugador*> getJugadores();
    std::vector<Escenario*> getEscenarios();
    std::map<int, std::string> getTop10();

    // Setters
    void setPersonajes(std::vector<Personaje*>& personajes);
    void setJugadores(std::map<std::string, Jugador*>& jugadores);
    void setEscenarios(std::vector<Escenario*>& escenarios);
    void setTop10(std::map<int, std::string>& top10);

    // Métodos adicionales
    void agregarPersonaje(Personaje* personaje);
    void agregarJugador(std::string& nombre, Jugador* jugador);
    void agregarEscenario(Escenario* escenario);
};

#endif // JUEGO_H