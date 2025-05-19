#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Collider.h"

class Escenario {
private:
    // Textura y sprite para el fondo
    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;
    bool fondoCargado;

    // Plataformas - usando clase Block en lugar de RectangleShape directamente
    std::vector<sf::RectangleShape> plataformas;

    // Colisionadores para las plataformas
    std::vector<Collider*> colisionadores;

    // Límites del escenario
    float limiteIzquierdo;
    float limiteDerecho;
    float limiteSuperior;
    float limiteInferior;

    // Altura del suelo general
    float alturaSuelo;

public:
    Escenario();
    ~Escenario();

    // Método para agregar una plataforma
    void agregarPlataforma(float x, float y, float ancho, float alto);

    // Método para colisiones usando el sistema Collider
    bool checkCollision(Collider& personajeCollider, sf::Vector2f& direction, float pushForce = 1.0f);

    // Método para verificar si un punto está sobre una plataforma
    bool estaSobrePlataforma(float posX, float posY, float altura, float& alturaPlataforma);

    // Método para dibujar el escenario
    void dibujar(sf::RenderWindow& ventana);

    // Getters
    float getLimiteIzquierdo() const { return limiteIzquierdo; }
    float getLimiteDerecho() const { return limiteDerecho; }
    float getLimiteSuperior() const { return limiteSuperior; }
    float getLimiteInferior() const { return limiteInferior; }
    float getAlturaSuelo() const { return alturaSuelo; }
    const std::vector<sf::RectangleShape>& getPlataformas() const { return plataformas; }

    // Setters
    void configurarLimites(float izq, float der, float sup, float inf);
    void setAlturaSuelo(float altura) { alturaSuelo = altura; }
};

#endif // ESCENARIO_H