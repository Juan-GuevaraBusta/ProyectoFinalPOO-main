#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>


class Jugador {
protected:
    std::string nombreJugador;
    int puntaje;

public:
    Jugador();
    Jugador(std::string nombreJugador);
    virtual ~Jugador();
    std::string getNombreJugador();
    int getPuntaje();
    void setNombreJugador(std::string nombreJugador);
    void setPuntaje(int puntaje);
};

#endif //JUGADOR_H
