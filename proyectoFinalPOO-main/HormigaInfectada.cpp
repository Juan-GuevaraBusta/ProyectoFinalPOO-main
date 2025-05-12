#include "HormigaInfectada.h"
#include <iostream>
#include <cstdlib>  // Para rand()
#include <ctime>    // Para time()

HormigaInfectada::HormigaInfectada() {
    // Inicializar semilla para números aleatorios
    srand((unsigned int)time(nullptr));
}

HormigaInfectada::HormigaInfectada(std::string nombre, int vitalidad, std::vector<int> posicion)
    : Personaje(nombre, vitalidad, 0, posicion) {

    // Inicializar generador de números aleatorios
    srand((unsigned int)time(nullptr));

    // Cargar texturas
    cargarTexturas();

    // Configurar sprite
    sprite.setTexture(texturasDerecha[0]);
    sprite.setPosition(posicion[0], posicion[1]);
    sprite.setScale(2.0f, 2.0f);

    // Iniciar con dirección aleatoria
    direccionActual = (rand() % 2 == 0) ? 1 : -1;
}

HormigaInfectada::~HormigaInfectada() {
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

    // Realizar movimiento errático
    caminarErraticamente();

    // Física para caer por gravedad y colisiones con plataformas
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Mover verticalmente
        sprite.move(0, velocidadY);

        // Verificar si tocó el suelo o una plataforma
        if (escenario) {
            // Obtener posición actual
            sf::Vector2f posicion = sprite.getPosition();

            // Verificar colisión con plataformas
            float alturaPlatforma = escenario->getAlturaPlatformaEn(posicion.x);

            // Si está por debajo de alguna plataforma
            if (posicion.y >= alturaPlatforma - sprite.getGlobalBounds().height) {
                // Reposicionar sobre la plataforma
                sprite.setPosition(posicion.x, alturaPlatforma - sprite.getGlobalBounds().height);
                velocidadY = 0;
                enAire = false;
            }
        }

        // Verificar si tocó el suelo
        if (sprite.getPosition().y >= alturaSuelo) {
            // Reposicionar en el suelo exactamente
            sprite.setPosition(sprite.getPosition().x, alturaSuelo);
            velocidadY = 0;
            enAire = false;
        }
    } else if (escenario) {
        // Si no está en el aire, verificar si hay plataforma debajo
        sf::Vector2f posicion = sprite.getPosition();
        float alturaPlatforma = escenario->getAlturaPlatformaEn(posicion.x);

        // Si no hay plataforma debajo (está en el borde)
        if (posicion.y < alturaPlatforma - sprite.getGlobalBounds().height &&
            posicion.y < alturaSuelo - 1.0f) {
            enAire = true;  // Comenzar a caer
            velocidadY = 0.1f;  // Velocidad inicial pequeña
        }
    }
}

void HormigaInfectada::dibujar(sf::RenderWindow& ventana) {
    // Siempre dibujar el sprite, ya sea vivo o muerto
    ventana.draw(sprite);
}