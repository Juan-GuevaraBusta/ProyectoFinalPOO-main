#include <SFML/Graphics.hpp>
#include "Ray.h"
#include "HormigaNormal.h"
#include "HormigaInfectada.h"
#include "Escenario.h"
#include <iostream>

int main() {
    // Configuración de la ventana
    sf::RenderWindow ventana(sf::VideoMode(1366, 768), "MiJuego");
    ventana.setFramerateLimit(60); // Limitar a 60 FPS para un rendimiento constante

    // Crear el escenario (la imagen y plataformas se cargan internamente)
    Escenario escenario;

    // Configurar límites del escenario
    escenario.configurarLimites(0.0f, 1366.0f, 0.0f, 768.0f);

    // Crear una instancia de Ray
    Ray *ray = new Ray("Ray");

    // Crear una instancia de HormigaNormal
    HormigaNormal *hormigaNormal = new HormigaNormal("Hormiga", 2, 0, {0, 0});

    // Crear una instancia de HormigaInfectada con solo 1 punto de vida
    HormigaInfectada *hormigaInfectada = new HormigaInfectada("Infectada", 1, {0, 0});

    // Posicionar las hormigas
    sf::Vector2f posRay = ray->getPosicion();
    hormigaNormal->setPosicion(posRay.x + 350.0f, posRay.y - 250.0f);
    hormigaInfectada->setPosicion(posRay.x + 450.0f, posRay.y + 10.0f);

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

        // Comprobar colisiones con plataformas (ejemplo básico)
        if (ray->estaEnAire()) {
            sf::FloatRect rayBounds = sf::FloatRect(
                ray->getPosicion().x, ray->getPosicion().y,
                64.0f, 64.0f  // Tamaño aproximado del sprite de Ray
            );

            if (escenario.verificarColisionPlataforma(rayBounds)) {
                // Si Ray colisiona con una plataforma mientras está en el aire
                // aquí podrías hacer que se detenga sobre la plataforma
                // Por simplicidad, no implementamos esto completamente
                std::cout << "¡Ray colisionó con una plataforma!" << std::endl;
            }
        }

        // Actualizar estados de los personajes
        ray->actualizar();
        hormigaNormal->actualizar();
        hormigaInfectada->actualizar();

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