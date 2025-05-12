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
    virtual void actualizar();  // Virtual para que Ray pueda sobrescribirlo
    void dibujar(sf::RenderWindow& ventana);

    // Getters
    sf::Vector2f getPosicion() const { return sprite.getPosition(); }
    bool estaEnAire() const { return enAire; }
    bool estaMirandoDerecha() const { return mirandoDerecha; }
    float getAlturaSuelo() const { return alturaSuelo; }

    // Setter adicional para la posición
    void setPosicion(float x, float y) { sprite.setPosition(x, y); }

    // Métodos para cargar texturas (protegidos para que las subclases puedan usarlos)
protected:
    void cargarTexturasDerecha(const std::string& ruta1, const std::string& ruta2);
    void cargarTexturasIzquierda(const std::string& ruta1, const std::string& ruta2);
};
#endif //HORMIGANORMAL_H