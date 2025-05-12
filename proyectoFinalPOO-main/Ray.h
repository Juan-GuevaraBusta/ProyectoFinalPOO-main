#ifndef RAY_H
#define RAY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Jugador.h"
#include "HormigaNormal.h"

class Ray : public HormigaNormal, public Jugador {
private:
    int luz;
    std::vector<sf::Texture> texturasAtaque;
    std::vector<sf::Texture> texturasFrontal;  // Específica para Ray
    bool atacando = false;
    sf::Clock relojAtaque;

public:
    Ray(std::string nombreJugador);
    virtual ~Ray() override;

    // Métodos de ataque específicos de Ray
    int golpearConBaston();
    int golpearConLuz();
    int superGolpeDeLuz();

    // Getters y setters para luz
    void setLuz(int luz);
    int getLuz();

    // Sobrescribir el método actualizar para incluir las animaciones de ataque
    virtual void actualizar() override;

    // Método para verificar si está atacando
    bool estaAtacando() const { return atacando; }

protected:
    // Método para cargar texturas frontales (específico de Ray)
    void cargarTexturasFrontales(const std::string& ruta1, const std::string& ruta2);
};

#endif