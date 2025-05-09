#include <SFML/Graphics.hpp>
#include "Ray.h"
#include <iostream>

int main() {
    // Obtener la resolución del escritorio
    sf::RenderWindow ventana(sf::VideoMode(1366, 768), "MiJuego");

    // Variables para establecer los límites de la pantalla
    float limiteIzquierdo = -200.0f;
    float limiteDerecho = 1196.0f;

    // Crear una instancia de Ray
    Ray *ray = new Ray("Nombre");

    // Posicionar a Ray en una buena posición inicial para pantalla completa
    // (ajustar estos valores según sea necesario)
    ray->getPosicion(); // No necesitamos el valor, solo verificar que existe el método

    // Ajustar el punto de origen del sprite si es necesario
    // Esto es importante porque afecta cómo se aplican los límites

    std::cout << "Ejecutando en pantalla completa. Presiona Escape para salir." << std::endl;

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            // Salir de pantalla completa con Escape
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
                ventana.close();
                std::cout << "Cerrando aplicación..." << std::endl;
            }

            // Detectar tecla de salto cuando se presiona
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Up) {
                ray->saltar();
            }
        }

        // Por defecto está quieto, a menos que se mueva
        ray->detener();

        // Obtener la posición actual de Ray
        sf::Vector2f posicion = ray->getPosicion();

        // Obtener el tamaño del sprite para cálculos más precisos de límites
        // Como no tenemos acceso directo al sprite, usaremos un valor estimado
        // Idealmente, deberías añadir un método para obtener el tamaño del sprite
        float anchoSprite = 64.0f;  // Ajusta según el tamaño real (escala * ancho)

        // Movimiento con teclas y comprobación de límites más precisos
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // Solo permitir movimiento si no toca el borde izquierdo
            // Agregamos un pequeño margen (5 píxeles) para que no pegue exactamente al borde
            if (posicion.x > limiteIzquierdo + 5.0f) {
                ray->caminarAtras();
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            // Solo permitir movimiento si no toca el borde derecho
            // El límite considera el ancho del sprite y un pequeño margen
            if (posicion.x < limiteDerecho - anchoSprite - 5.0f) {
                ray->caminarAdelante();
            }
        }

        // Actualiza animación si está en movimiento
        ray->actualizar();

        ventana.clear();
        ray->dibujar(ventana);
        ventana.display();
    }

    delete ray; // Liberar memoria al finalizar
    return 0;
}