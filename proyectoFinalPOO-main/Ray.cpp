#include "Ray.h"
#include <iostream>
#include <vector>

Ray::Ray(){
};

Ray::Ray(std::string nombreJugador) : HormigaNormal("Ray", 3, 0, {0, 0}), Jugador(nombreJugador){
    this -> luz = 0;
};

Ray::~Ray() {
    std::cout << "Ray gone" << std::endl;
};

int Ray::golpearConBaston() {
    return 0;
};

int Ray::golpearConLuz() {
return 0;
};

int Ray::superGolpeDeLuz() {
    if (luz >= 10) {
        return 10;
    }
    return 0;
};

void Ray::setLuz(int luz) {
    this -> luz = luz;
};

int Ray::getLuz() {
    return luz;
};