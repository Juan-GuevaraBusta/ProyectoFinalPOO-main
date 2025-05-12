#include "Ray.h"
#include "HormigaInfectada.h"
#include <iostream>
#include <cmath>  // Para sqrt en cálculo de distancia

Ray::Ray(std::string nombreJugador) : Personaje("Ray", 3, 0, {0, 0}), Jugador(nombreJugador) {
    this->luz = 0;

    // Cargar todas las texturas
    cargarTexturas();

    // Configurar el sprite inicialmente
    sprite.setTexture(texturasFrontal[0]);
    sprite.setPosition(100.f, alturaSuelo);
    sprite.setScale(2.0f, 2.0f);
}

void Ray::cargarTexturas() {
    // Reservar espacio en los vectores
    texturasDerecha.resize(2);
    texturasIzquierda.resize(2);
    texturasFrontal.resize(2);
    texturasAtaque.resize(2);

    // Cargar texturas de movimiento
    if (!texturasDerecha[0].loadFromFile("ray/raysano/RSright_1.png") ||
        !texturasDerecha[1].loadFromFile("ray/raysano/RSright_2.png")) {
        std::cerr << "Error cargando imágenes derecha de Ray" << std::endl;
    }

    if (!texturasIzquierda[0].loadFromFile("ray/raysano/RSleft_1.png") ||
        !texturasIzquierda[1].loadFromFile("ray/raysano/RSleft_2.png")) {
        std::cerr << "Error cargando imágenes izquierda de Ray" << std::endl;
    }

    // Cargar texturas frontales
    if (!texturasFrontal[0].loadFromFile("ray/raysano/RSfront_1.png") ||
        !texturasFrontal[1].loadFromFile("ray/raysano/RSfront_2.png")) {
        std::cerr << "Error cargando imágenes frontales de Ray" << std::endl;
    }

    // Cargar texturas de ataque
    if (!texturasAtaque[0].loadFromFile("ray/raysano/RSAleft.png") ||
        !texturasAtaque[1].loadFromFile("ray/raysano/RSAright.png")) {
        std::cerr << "Error cargando imágenes de ataque de Ray" << std::endl;
    }
}

Ray::~Ray() {
    std::cout << "Ray gone" << std::endl;
}

void Ray::caminarAdelante() {
    moviendoDerecha = true;
    moviendoIzquierda = false;
    mirandoDerecha = true;
    enMovimiento = true;
    sprite.move(1.0f, 0.f);
}

void Ray::caminarAtras() {
    moviendoDerecha = false;
    moviendoIzquierda = true;
    mirandoDerecha = false;
    enMovimiento = true;
    sprite.move(-1.0f, 0.f);
}

void Ray::detener() {
    moviendoDerecha = false;
    moviendoIzquierda = false;
    enMovimiento = false;
}

void Ray::saltar() {
    if (!enAire) {
        velocidadY = -2.0f;
        enAire = true;
    }
}

int Ray::golpearConBaston() {
    if (!atacando) {
        atacando = true;
        relojAtaque.restart();
        return 1; // Valor de daño básico
    }
    return 0; // No hace daño si ya estaba atacando
}

bool Ray::atacarEnemigo(HormigaInfectada* enemigo) {
    if (atacando && enemigo && enemigo->estaViva()) {
        // Calcular distancia entre Ray y el enemigo
        sf::Vector2f posRay = getPosicion();
        sf::Vector2f posEnemigo = enemigo->getPosicion();

        float dx = posEnemigo.x - posRay.x;
        float dy = posEnemigo.y - posRay.y;
        float distancia = std::sqrt(dx*dx + dy*dy);

        // Verificar si está dentro del rango y en la dirección correcta
        bool direccionCorrecta = (mirandoDerecha && dx > 0) || (!mirandoDerecha && dx < 0);

        if (distancia <= rangoAtaque && direccionCorrecta) {
            enemigo->recibirDaño(1);  // Aplicar daño al enemigo
            return true;  // Ataque exitoso
        }
    }
    return false;  // No se pudo atacar
}

int Ray::golpearConLuz() {
    return 0;
}

int Ray::superGolpeDeLuz() {
    if (luz >= 10) {
        return 10;
    }
    return 0;
}

void Ray::setLuz(int luz) {
    this->luz = luz;
}

int Ray::getLuz() {
    return luz;
}

void Ray::actualizar() {
    // Verificar si el ataque ha terminado
    if (atacando && relojAtaque.getElapsedTime().asSeconds() >= 0.5f) {
        atacando = false;  // Terminar el ataque después de 0.5 segundos
    }

    // Si está atacando, impedir el movimiento
    if (atacando) {
        moviendoDerecha = false;
        moviendoIzquierda = false;
        enMovimiento = false;
    }

    // Cambiar frame cada 0.15 segundos
    if (relojAnimacion.getElapsedTime().asSeconds() >= 0.15f) {
        frameActual = (frameActual + 1) % 2;

        // Seleccionar la textura correcta basada en el estado
        if (atacando) {
            if (mirandoDerecha) {
                sprite.setTexture(texturasAtaque[1]);  // Ataque a la derecha
            } else {
                sprite.setTexture(texturasAtaque[0]);  // Ataque a la izquierda
            }
        } else if (moviendoDerecha) {
            sprite.setTexture(texturasDerecha[frameActual]);
        } else if (moviendoIzquierda) {
            sprite.setTexture(texturasIzquierda[frameActual]);
        } else {
            sprite.setTexture(texturasFrontal[frameActual]);
        }

        relojAnimacion.restart();
    }

    // Física del salto
    if (enAire) {
        velocidadY += gravedad;
        sprite.move(0, velocidadY);

        if (sprite.getPosition().y >= alturaSuelo) {
            sprite.setPosition(sprite.getPosition().x, alturaSuelo);
            velocidadY = 0;
            enAire = false;
        }
    }
}

void Ray::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}