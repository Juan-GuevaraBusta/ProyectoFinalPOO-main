#include "HormigaNormal.h"
#include <iostream>

HormigaNormal::HormigaNormal() {
}

HormigaNormal::HormigaNormal(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion) : Personaje(nombre, vitalidad, alimento, posicion) {
    // Reservar espacio para las texturas
    texturasDerecha.resize(2);
    texturasIzquierda.resize(2);

    // Cargar texturas por defecto (para hormigas normales)
    cargarTexturasDerecha("ant_right_1.png", "ant_right_2.png");
    cargarTexturasIzquierda("ant_left_1.png", "ant_left_2.png");

    // Utilizamos la textura derecha como inicial
    sprite.setTexture(texturasDerecha[0]);
    sprite.setPosition(posicion[0], posicion[1]);
    sprite.setScale(2.0f, 2.0f);
}

HormigaNormal::~HormigaNormal() {
}

void HormigaNormal::cargarTexturasDerecha(const std::string& ruta1, const std::string& ruta2) {
    if (!texturasDerecha[0].loadFromFile(ruta1) || !texturasDerecha[1].loadFromFile(ruta2)) {
        std::cerr << "Error cargando imágenes derecha: " << ruta1 << ", " << ruta2 << std::endl;
    }
}

void HormigaNormal::cargarTexturasIzquierda(const std::string& ruta1, const std::string& ruta2) {
    if (!texturasIzquierda[0].loadFromFile(ruta1) || !texturasIzquierda[1].loadFromFile(ruta2)) {
        std::cerr << "Error cargando imágenes izquierda: " << ruta1 << ", " << ruta2 << std::endl;
    }
}

void HormigaNormal::caminarAdelante() {
    moviendoDerecha = true;
    moviendoIzquierda = false;
    mirandoDerecha = true;  // Recordar que está mirando a la derecha
    enMovimiento = true;
    sprite.move(0.1, 0.f);
}

void HormigaNormal::caminarAtras() {
    moviendoDerecha = false;
    moviendoIzquierda = true;
    mirandoDerecha = false;  // Recordar que está mirando a la izquierda
    enMovimiento = true;
    sprite.move(-0.1, 0.f);
}

void HormigaNormal::detener() {
    moviendoDerecha = false;
    moviendoIzquierda = false;
    enMovimiento = false;
    // No cambiamos la textura aquí, se hará en actualizar()
}

void HormigaNormal::saltar() {
    // Solo permitir saltar si está en el suelo
    if (!enAire) {
        velocidadY = -2.0f; // Velocidad inicial negativa (hacia arriba)
        enAire = true;
    }
}

void HormigaNormal::recibirDañoEsporas(int esporas) {
    this->vitalidad -= esporas;
}

void HormigaNormal::actualizar(Escenario* escenario) {
    // Cambiar frame cada 0.15 segundos (aprox. 6.7fps)
    if (relojAnimacion.getElapsedTime().asSeconds() >= 0.15f) {
        frameActual = (frameActual + 1) % 2;

        // Seleccionar la textura correcta basada en el estado
        if (moviendoDerecha || (!moviendoIzquierda && mirandoDerecha)) {
            // Si se mueve a la derecha o está quieto pero mirando a la derecha
            sprite.setTexture(texturasDerecha[frameActual]);
        } else {
            // Si se mueve a la izquierda o está quieto pero mirando a la izquierda
            sprite.setTexture(texturasIzquierda[frameActual]);
        }

        relojAnimacion.restart();
    }

    // Física del salto
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

void HormigaNormal::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}