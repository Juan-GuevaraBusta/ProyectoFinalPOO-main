#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Jugador.h"
#include "Personaje.h"

// Importante: Ya NO heredamos de HormigaNormal, sólo de Personaje
class Ray : public Personaje, public Jugador {
private:
    // Ahora definimos todos estos miembros propios
    sf::Sprite sprite;
    std::vector<sf::Texture> texturasDerecha;
    std::vector<sf::Texture> texturasIzquierda;
    std::vector<sf::Texture> texturasFrontal;
    std::vector<sf::Texture> texturasAtaque;

    int frameActual = 0;
    bool moviendoDerecha = false;
    bool moviendoIzquierda = false;
    bool enMovimiento = false;
    bool mirandoDerecha = true;
    bool atacando = false;

    sf::Clock relojAnimacion;
    sf::Clock relojAtaque;

    // Variables para el salto
    float velocidadY = 0.0f;
    float gravedad = 0.015f;
    bool enAire = false;
    float alturaSuelo = 150.0f;

    int luz;

public:
    Ray(std::string nombreJugador);
    virtual ~Ray();

    // Métodos de movimiento (ahora propios)
    void caminarAdelante();
    void caminarAtras();
    void detener();
    void saltar();

    // Métodos de ataque específicos de Ray
    int golpearConBaston();
    int golpearConLuz();
    int superGolpeDeLuz();

    // Getters y setters
    void setLuz(int luz);
    int getLuz();
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaEnAire() const { return enAire; }
    bool estaMirandoDerecha() const { return mirandoDerecha; }
    bool estaAtacando() const { return atacando; }
    float getAlturaSuelo() const { return alturaSuelo; }
    void setPosicion(float x, float y) { sprite.setPosition(x, y); }

    // Métodos para actualizar y dibujar
    void actualizar();
    void dibujar(sf::RenderWindow& ventana);

private:
    // Métodos auxiliares privados para cargar texturas
    void cargarTexturas();
};

#endif