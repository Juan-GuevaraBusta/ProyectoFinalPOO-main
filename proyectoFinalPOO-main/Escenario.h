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

    // Altura del suelo general
    float alturaSuelo;

public:
    Escenario();
    ~Escenario();

    // Método para agregar una plataforma
    void agregarPlataforma(float x, float y, float ancho, float alto);

    // Métodos para colisiones
    bool verificarColisionPlataforma(const sf::FloatRect& objetoBounds);
    float getAlturaPlatformaEn(float posX);

    // Método para dibujar el escenario
    void dibujar(sf::RenderWindow& ventana);

    // Getters
    float getLimiteIzquierdo() const { return limiteIzquierdo; }
    float getLimiteDerecho() const { return limiteDerecho; }
    float getLimiteSuperior() const { return limiteSuperior; }
    float getLimiteInferior() const { return limiteInferior; }
    float getAlturaSuelo() const { return alturaSuelo; }

    // Setters
    void configurarLimites(float izq, float der, float sup, float inf);
    void setAlturaSuelo(float altura) { alturaSuelo = altura; }
};

#endif // ESCENARIO_H