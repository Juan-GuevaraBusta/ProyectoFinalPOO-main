#ifndef HORMIGANORMAL_H
#define HORMIGANORMAL_H
#include "Personaje.h"
#include "Escenario.h"  // Añadir include para Escenario
#include <SFML/Graphics.hpp>
#include <vector>

class HormigaNormal : public Personaje {
protected:
    sf::Sprite sprite;
    std::vector<sf::Texture> texturasDerecha;
    std::vector<sf::Texture> texturasIzquierda;

    int frameActual = 0;
    bool moviendoDerecha = false;
    bool moviendoIzquierda = false;
    bool enMovimiento = false;
    bool mirandoDerecha = true;  // Para recordar la última dirección

    sf::Clock relojAnimacion;

    // Variables para el salto
    float velocidadY = 0.0f;
    float gravedad = 0.015f;
    bool enAire = false;
    float alturaSuelo = 150.0f; // Ajustar según tamaño de ventana

    // Ajustes para la hitbox de colisión
    float hitboxOffsetX = 0.0f;
    float hitboxOffsetY = 0.0f;
    float hitboxWidth = 0.0f;
    float hitboxHeight = 0.0f;
    bool usingCustomHitbox = false;

public:
    HormigaNormal();
    HormigaNormal(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion);
    virtual ~HormigaNormal();

    // Métodos de movimiento básico para todas las hormigas
    void caminarAdelante();
    void caminarAtras();
    void detener();
    void saltar();
    void recibirDañoEsporas(int esporas);

    // Métodos para actualizar y dibujar
    virtual void actualizar(Escenario* escenario = nullptr);
    void dibujar(sf::RenderWindow& ventana);

    // Getters
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaEnAire() const { return enAire; }
    bool estaMirandoDerecha() const { return mirandoDerecha; }
    float getAlturaSuelo() const { return alturaSuelo; }

    // Sobrescribir getBounds para usar hitbox personalizada
    sf::FloatRect getBounds() const;

    // Setter adicional para la posición
    void setPosicion(float x, float y) { sprite.setPosition(x, y); }

    // Método para ajustar la hitbox de colisión
    void ajustarHitbox(float offsetX, float offsetY, float width, float height);

protected:
    // Método unificado para cargar texturas (como en HormigaInfectada)
    virtual void cargarTexturas();
};
#endif //HORMIGANORMAL_H