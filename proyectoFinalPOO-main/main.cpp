#include <SFML/Graphics.hpp>
#include "Ray.h"
#include "HormigaNormal.h"
#include "HormigaInfectada.h"
#include "Escenario.h"
#include <iostream>

int main() {
    // Configuración de la ventana
    sf::RenderWindow ventana(sf::VideoMode(1366, 768), "MiJuego");
    ventana.setFramerateLimit(60);

    // *** CAMBIO 6: Crear el escenario y configurarlo correctamente ***
    Escenario escenario;

    // Configurar límites del escenario
    escenario.configurarLimites(0.0f, 1366.0f, 0.0f, 768.0f);

    // La altura del suelo ya se definió en el constructor (650.0f)

    // *** CAMBIO 7: Crear el personaje Ray y posicionarlo correctamente en el suelo ***
    Ray *ray = new Ray("Ray");
    ray->setPosicion(100.0f, escenario.getAlturaSuelo() - ray->getBounds().height);

    // Crear el resto de las hormigas y posicionarlas en el suelo
    HormigaNormal *hormigaNormal = new HormigaNormal("Hormiga", 2, 0, {0, 0});
    hormigaNormal->setPosicion(400.0f, escenario.getAlturaSuelo() - hormigaNormal->getBounds().height);

    HormigaInfectada *hormigaInfectada = new HormigaInfectada("Infectada", 1, {0, 0});
    hormigaInfectada->setPosicion(700.0f, escenario.getAlturaSuelo() - hormigaInfectada->getBounds().height);

    std::cout << "Ejecutando el juego. Controles:" << std::endl;
    std::cout << "- Flechas Izquierda/Derecha: Mover a Ray" << std::endl;
    std::cout << "- Flecha Arriba: Saltar" << std::endl;
    std::cout << "- A: Atacar (puedes eliminar a la hormiga infectada)" << std::endl;
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

        // Obtener la posición actual de Ray
        sf::Vector2f posicion = ray->getPosicion();

        // Tamaño aproximado del sprite para los límites
        float anchoSprite = 64.0f;

        // Detectar tecla de ataque (A)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            int daño = ray->golpearConBaston();

            // Intentar atacar a la hormiga infectada si se inició un ataque
            if (daño > 0) {
                if (ray->atacarEnemigo(hormigaInfectada)) {
                    std::cout << "¡Has golpeado a la hormiga infectada!" << std::endl;
                    if (!hormigaInfectada->estaViva()) {
                        std::cout << "¡La hormiga infectada ha sido derrotada!" << std::endl;
                    }
                }
            }
        }

        // Movimiento con teclas y comprobación de límites
        if (!ray->estaAtacando()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (posicion.x > escenario.getLimiteIzquierdo() + 5.0f) {
                    ray->caminarAtras();
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (posicion.x < escenario.getLimiteDerecho() - anchoSprite - 5.0f) {
                    ray->caminarAdelante();
                }
            }
        }

        // Actualizar personajes pasando el escenario para física y colisiones
        ray->actualizar(&escenario);
        hormigaNormal->actualizar(&escenario);
        hormigaInfectada->actualizar(&escenario);

        // Renderizado
        ventana.clear(); // Fondo negro

        // Dibujar el escenario (fondo y plataformas)
        escenario.dibujar(ventana);

        // Luego dibujar los personajes
        ray->dibujar(ventana);
        hormigaNormal->dibujar(ventana);
        hormigaInfectada->dibujar(ventana);

        ventana.display();
    }

    // Liberar memoria
    delete ray;
    delete hormigaNormal;
    delete hormigaInfectada;

    return 0;
}