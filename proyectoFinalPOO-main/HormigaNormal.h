#ifndef HORMIGANORMAL_H
#define HORMIGANORMAL_H
#include "Personaje.h"
#include <SFML/Graphics.hpp>
#include <vector>

class HormigaNormal : public Personaje {
protected:
    sf::Sprite sprite;
    std::vector<sf::Texture> texturasDerecha;
    std::vector<sf::Texture> texturasIzquierda;
    int frameActual = 0;
    bool moviendoDerecha = true;
    bool enMovimiento = false;
    sf::Clock relojAnimacion;

    // Variables para el salto
    float velocidadY = 0.0f;
    float gravedad = 0.015f;
    bool enAire = false;
    float alturaSuelo = 150.0f; // Ajustar según tamaño de ventana

public:
    HormigaNormal();
    HormigaNormal(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion);
    virtual ~HormigaNormal();
    void caminarAdelante();
    void caminarAtras();
    void detener();
    void saltar();
    void recibirDañoEsporas(int esporas);
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
    sf::Vector2f getPosicion(){ return sprite.getPosition(); }
    bool estaEnAire() const { return enAire; }
};
#endif //HORMIGANORMAL_H