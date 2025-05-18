#ifndef ESCENARIO_H
#define ESCENARIO_H

#include <SFML/Graphics.hpp>
#include <vector>

// Enumeración para los tipos de colisión
enum class ColisionTipo {
    NINGUNA,
    ARRIBA,    // Colisión con la parte superior de la plataforma
    ABAJO,     // Colisión con la parte inferior de la plataforma
    IZQUIERDA, // Colisión con el lado izquierdo de la plataforma
    DERECHA    // Colisión con el lado derecho de la plataforma
};

// Estructura para información de colisión
struct ColisionInfo {
    bool colision;
    ColisionTipo tipo;
    sf::FloatRect plataformaBounds;
};

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

    // Nuevo método mejorado para verificar colisiones con plataformas
    ColisionInfo verificarColisionConPlataformas(const sf::FloatRect& objetoBounds, const sf::Vector2f& velocidad);

    // Método modificado para obtener la altura de la plataforma
    float getAlturaPlatformaEn(float posX, float posY, float alturaObjeto, bool& enPlataforma);

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