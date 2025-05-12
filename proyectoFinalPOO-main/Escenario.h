#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <SFML/Graphics.hpp>
#include <vector>

class Escenario {
private:
    // Textura y sprite para el fondo
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    bool fondoCargado;

    // Plataformas
    std::vector<sf::RectangleShape> plataformas;

    // Límites del escenario
    float limiteIzquierdo;
    float limiteDerecho;
    float limiteSuperior;
    float limiteInferior;

public:
    Escenario();
    ~Escenario();

    // Método para agregar una plataforma
    void agregarPlataforma(float x, float y, float ancho, float alto);

    // Método para verificar colisión con plataformas
    bool verificarColisionPlataforma(const sf::FloatRect& objetoBounds);

    // Método para dibujar el escenario (fondo y plataformas)
    void dibujar(sf::RenderWindow& ventana);

    // Getters para los límites
    float getLimiteIzquierdo() const { return limiteIzquierdo; }
    float getLimiteDerecho() const { return limiteDerecho; }
    float getLimiteSuperior() const { return limiteSuperior; }
    float getLimiteInferior() const { return limiteInferior; }

    // Setters para los límites
    void configurarLimites(float izq, float der, float sup, float inf);
};

#endif // ESCENARIO_H