#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Jugador.h"
#include "HormigaNormal.h"


class Ray : public HormigaNormal, public Jugador {
private:
    int luz;

public:
    Ray();
    Ray(std::string nombreJugador);
    ~Ray();
    int golpearConBaston();
    int golpearConLuz();
    int superGolpeDeLuz();
    void setLuz(int luz);
    int getLuz();
};

#endif