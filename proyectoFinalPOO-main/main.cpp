#include <SFML/Graphics.hpp>
#include "Ray.h"
#include "HormigaNormal.h"
#include <iostream>

int main() {
    // Configuración de la ventana
    sf::RenderWindow ventana(sf::VideoMode(1366, 768), "MiJuego");
    ventana.setFramerateLimit(60); // Limitar a 60 FPS para un rendimiento constante

    // Variables para establecer los límites de la pantalla
    float limiteIzquierdo = -200.0f;
    float limiteDerecho = 1196.0f;

    // Crear una instancia de Ray
    Ray *ray = new Ray("Ray");

    // Crear una instancia de HormigaNormal que aparezca a la derecha de Ray
    HormigaNormal *hormigaNormal = new HormigaNormal("Hormiga", 2, 0, {0, 0});

    // Posicionar la hormiga normal a 300 píxeles a la derecha
    // Usamos la posición de Ray como referencia
    sf::Vector2f posRay = ray->getPosicion();
    hormigaNormal->setPosicion(posRay.x + 300.0f, posRay.y - 150);

    std::cout << "Ejecutando el juego. Controles:" << std::endl;
    std::cout << "- Flechas Izquierda/Derecha: Mover a Ray" << std::endl;
    std::cout << "- Flecha Arriba: Saltar" << std::endl;
    std::cout << "- A: Atacar" << std::endl;
    std::cout << "- Escape: Salir" << std::endl;

    // Bucle principal del juego
    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            // Salir con Escape
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
                ventana.close();
                std::cout << "Cerrando aplicación..." << std::endl;
            }

            // Detectar tecla de salto cuando se presiona
            if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Up) {
                ray->saltar();
            }
        }

        // Por defecto los personajes están quietos
        ray->detener();
        hormigaNormal->detener();

        // Obtener la posición actual de Ray
        sf::Vector2f posicion = ray->getPosicion();

        // Tamaño aproximado del sprite para los límites
        float anchoSprite = 64.0f;

        // Detectar tecla de ataque (A)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            int daño = ray->golpearConBaston();
        }

        // Movimiento de Ray con comprobación de límites
        if (!ray->estaAtacando()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                // Solo permitir movimiento si no toca el borde izquierdo
                if (posicion.x > limiteIzquierdo + 5.0f) {
                    ray->caminarAtras();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                // Solo permitir movimiento si no toca el borde derecho
                if (posicion.x < limiteDerecho - anchoSprite - 5.0f) {
                    ray->caminarAdelante();
                }
            }
        }

        // Actualizar estados de los personajes
        ray->actualizar();
        hormigaNormal->actualizar();

        // Renderizado
        ventana.clear(); // Fondo negro
        ray->dibujar(ventana);
        hormigaNormal->dibujar(ventana);
        ventana.display();
    }

    // Liberar memoria
    delete ray;
    delete hormigaNormal;

    return 0;
}