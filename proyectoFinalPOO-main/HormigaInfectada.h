#ifndef HORMIGAINFECTADA_H
#define HORMIGAINFECTADA_H
#include "Personaje.h"
#include "Escenario.h"  // Añadir include para Escenario
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

    // Variables para física
    float velocidadY = 0.0f;
    float gravedad = 0.015f;
    bool enAire = false;
    float alturaSuelo = 150.0f;

public:
    HormigaInfectada();
    HormigaInfectada(std::string nombre, int vitalidad, std::vector<int> posicion);
    virtual ~HormigaInfectada();

    int liberarEsporas();
    void caminarErraticamente();

    // Métodos básicos necesarios
    void actualizar(Escenario* escenario = nullptr);  // Modificado para recibir escenario
    void dibujar(sf::RenderWindow& ventana);
    void recibirDaño(int daño);

    // Getters
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaViva() const { return viva; }
    bool estaEnAire() const { return enAire; }
    float getAlturaSuelo() const { return alturaSuelo; }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    // Setter para posición
    void setPosicion(float x, float y) { sprite.setPosition(x, y); }

private:
    void cargarTexturas();
};

#endif //HORMIGAINFECTADA_H