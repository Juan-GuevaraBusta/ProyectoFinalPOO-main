#ifndef HORMIGAINFECTADA_H
#define HORMIGAINFECTADA_H
#include "Personaje.h"
#include <SFML/Graphics.hpp>
#include <vector>

class HormigaInfectada : public Personaje {
private:
    sf::Sprite sprite;
    std::vector<sf::Texture> texturasDerecha;
    std::vector<sf::Texture> texturasIzquierda;
    sf::Texture texturaMuerta;  // Nueva textura para cuando está muerta

    int frameActual = 0;
    bool moviendoDerecha = true;
    bool enMovimiento = true;
    bool viva = true;

    sf::Clock relojAnimacion;
    sf::Clock relojMovimiento;

    // Variables para movimiento errático
    float velocidad = 0.05f;
    int direccionActual = 1; // 1: derecha, -1: izquierda

public:
    HormigaInfectada();
    HormigaInfectada(std::string nombre, int vitalidad, std::vector<int> posicion);
    virtual ~HormigaInfectada();

    int liberarEsporas();
    void caminarErraticamente();

    // Métodos básicos necesarios
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);
    void recibirDaño(int daño);

    // Getters
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaViva() const { return viva; }

    // Setter para posición
    void setPosicion(float x, float y) { sprite.setPosition(x, y); }

private:
    void cargarTexturas();
};

#endif //HORMIGAINFECTADA_H