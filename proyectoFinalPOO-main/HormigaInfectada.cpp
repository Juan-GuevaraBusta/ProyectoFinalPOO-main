#include "HormigaInfectada.h"
#include <iostream>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()

HormigaInfectada::HormigaInfectada() {
    // Inicializar semilla para números aleatorios
    srand((unsigned int)time(nullptr));

    // Inicializar hitbox y collider
    hitbox.setSize(sf::Vector2f(40.0f, 40.0f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.0f);

    collider = new Collider(hitbox);
}

HormigaInfectada::HormigaInfectada(std::string nombre, int vitalidad, std::vector<int> posicion)
    : Personaje(nombre, vitalidad, 0, posicion) {

    // Inicializar generador de números aleatorios
    srand((unsigned int)time(nullptr));

    // Inicializar hitbox y collider
    hitbox.setSize(sf::Vector2f(40.0f, 40.0f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Red);
    hitbox.setOutlineThickness(1.0f);

    collider = new Collider(hitbox);

    // Cargar texturas
    cargarTexturas();

    // Configurar sprite
    sprite.setTexture(texturasDerecha[0]);
    sprite.setPosition(posicion[0], posicion[1]);
    sprite.setScale(2.0f, 2.0f);

    // Actualizar hitbox a la posición inicial
    actualizarHitbox();

    // Iniciar con dirección aleatoria
    direccionActual = (rand() % 2 == 0) ? 1 : -1;
}

HormigaInfectada::~HormigaInfectada() {
    delete collider;
}

void HormigaInfectada::cargarTexturas() {
    // Reservar espacio para texturas
    texturasDerecha.resize(2);
    texturasIzquierda.resize(2);

    // Cargar texturas para hormiga infectada
    if (!texturasDerecha[0].loadFromFile("hormiga_infectada/INFright_1.png") ||
        !texturasDerecha[1].loadFromFile("hormiga_infectada/INFright_2.png")) {
        std::cerr << "Error cargando imágenes derecha de hormiga infectada" << std::endl;
    }

    if (!texturasIzquierda[0].loadFromFile("hormiga_infectada/INFleft_1.png") ||
        !texturasIzquierda[1].loadFromFile("hormiga_infectada/INFleft_2.png")) {
        std::cerr << "Error cargando imágenes izquierda de hormiga infectada" << std::endl;
    }

    // Cargar textura para hormiga muerta
    if (!texturaMuerta.loadFromFile("hormiga_infectada/INFMleft.png")) {
        std::cerr << "Error cargando imagen de hormiga muerta" << std::endl;

        // Si no se encuentra la textura, usamos una de las existentes (como fallback)
        texturaMuerta = texturasDerecha[0];
    }
}

void HormigaInfectada::setPosicion(float x, float y) {
    sprite.setPosition(x, y);
    actualizarHitbox();
}

void HormigaInfectada::actualizarHitbox() {
    // Centrar la hitbox en el sprite
    sf::FloatRect bounds = sprite.getGlobalBounds();
    hitbox.setSize(sf::Vector2f(bounds.width * 0.8f, bounds.height * 0.8f));
    hitbox.setPosition(bounds.left + bounds.width * 0.1f,
                       bounds.top + bounds.height * 0.1f);
}

int HormigaInfectada::liberarEsporas() {
    // Número aleatorio de esporas entre 1 y 3
    return rand() % 3 + 1;
}

void HormigaInfectada::caminarErraticamente() {
    // Cambiar dirección cada cierto tiempo (2-5 segundos)
    if (relojMovimiento.getElapsedTime().asSeconds() >= (rand() % 3 + 2)) {
        direccionActual = -direccionActual;  // Invertir dirección
        relojMovimiento.restart();
    }

    // Mover en la dirección actual
    sprite.move(velocidad * direccionActual, 0.f);
    actualizarHitbox();

    // Actualizar estado según dirección
    moviendoDerecha = (direccionActual > 0);
}

void HormigaInfectada::recibirDaño(int daño) {
    this->vitalidad -= daño;
    if (this->vitalidad <= 0) {
        viva = false;
        // Cambiar a la textura de hormiga muerta
        sprite.setTexture(texturaMuerta);
    }
}

void HormigaInfectada::actualizar(Escenario* escenario) {
    // Si está muerta, no hacer nada más
    if (!viva) return;

    // Cambiar frame cada 0.15 segundos
    if (relojAnimacion.getElapsedTime().asSeconds() >= 0.15f) {
        frameActual = (frameActual + 1) % 2;

        if (moviendoDerecha) {
            sprite.setTexture(texturasDerecha[frameActual]);
        } else {
            sprite.setTexture(texturasIzquierda[frameActual]);
        }

        relojAnimacion.restart();
    }

    // Guardar la posición actual para restaurarla en caso de colisión
    sf::Vector2f posicionAnterior = sprite.getPosition();

    // Realizar movimiento errático
    caminarErraticamente();

    // Física para caer por gravedad
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Mover verticalmente
        sprite.move(0, velocidadY);
        actualizarHitbox();
    }

    // Verificar colisiones con el escenario
    if (escenario) {
        sf::Vector2f direction(0.0f, 0.0f);
        bool colision = escenario->checkCollision(*collider, direction, 1.0f);

        if (colision) {
            // Actualizar la posición del sprite basado en el movimiento del collider
            sf::Vector2f hitboxPos = hitbox.getPosition();
            sf::FloatRect bounds = sprite.getGlobalBounds();

            // Manejar distintos tipos de colisión
            if (direction.x != 0.0f) {
                // Colisión lateral - cambiar dirección
                direccionActual = -direccionActual;
                moviendoDerecha = (direccionActual > 0);

                // Ajustar posición
                sprite.setPosition(
                    hitboxPos.x - (bounds.width * 0.1f),
                    sprite.getPosition().y
                );
            }

            if (direction.y < 0.0f) {
                // Colisión desde abajo (personaje saltando)
                velocidadY = 0.1f; // Pequeño rebote
                sprite.setPosition(
                    sprite.getPosition().x,
                    hitboxPos.y - (bounds.height * 0.1f)
                );
            }
            else if (direction.y > 0.0f) {
                // Colisión desde arriba (personaje cayendo)
                velocidadY = 0.0f;
                enAire = false;
                sprite.setPosition(
                    sprite.getPosition().x,
                    hitboxPos.y - (bounds.height * 0.1f)
                );
            }

            actualizarHitbox();
        }
        else {
            // No hay colisión directa, verificar si debe caer
            float alturaPlataforma;
            sf::Vector2f pos = sprite.getPosition();
            sf::FloatRect bounds = sprite.getGlobalBounds();

            // Usar el método mejorado para detectar si está sobre una plataforma
            bool sobrePlataforma = escenario->estaSobrePlataforma(
                pos.x + bounds.width / 2,
                pos.y,
                bounds.height,
                alturaPlataforma
            );

            // Si no está en el aire y no está sobre una plataforma, debe caer
            if (!enAire && !sobrePlataforma) {
                enAire = true;
                velocidadY = 0.1f; // Velocidad inicial pequeña
            }

            // Si está en el aire y ha llegado a una plataforma
            if (enAire && sobrePlataforma && pos.y + bounds.height > alturaPlataforma - 5.0f) {
                // Aterrizó en una plataforma
                sprite.setPosition(pos.x, alturaPlataforma - bounds.height);
                velocidadY = 0.0f;
                enAire = false;
                actualizarHitbox();
            }
        }

        // Verificar límites del escenario
        sf::Vector2f pos = sprite.getPosition();

        // Límite izquierdo
        if (pos.x < escenario->getLimiteIzquierdo()) {
            sprite.setPosition(escenario->getLimiteIzquierdo(), pos.y);
            // Cambiar dirección
            direccionActual = 1;
            moviendoDerecha = true;
            actualizarHitbox();
        }

        // Límite derecho (considerando el ancho del sprite)
        if (pos.x + sprite.getGlobalBounds().width > escenario->getLimiteDerecho()) {
            sprite.setPosition(escenario->getLimiteDerecho() - sprite.getGlobalBounds().width, pos.y);
            // Cambiar dirección
            direccionActual = -1;
            moviendoDerecha = false;
            actualizarHitbox();
        }

        // Límite inferior (suelo)
        if (pos.y + sprite.getGlobalBounds().height > escenario->getAlturaSuelo()) {
            sprite.setPosition(pos.x, escenario->getAlturaSuelo() - sprite.getGlobalBounds().height);
            velocidadY = 0.0f;
            enAire = false;
            actualizarHitbox();
        }
    }
}

void HormigaInfectada::dibujar(sf::RenderWindow& ventana) {
    // Siempre dibujar el sprite, ya sea vivo o muerto
    ventana.draw(sprite);
    // Opcionalmente para debug, dibujar la hitbox
    //ventana.draw(hitbox);
}