#include "Ray.h"
#include <iostream>
#include <vector>

Ray::Ray(std::string nombreJugador) : HormigaNormal("Ray", 3, 0, {0, 0}), Jugador(nombreJugador) {
    this->luz = 0;

    // Inicializar vectores de textura
    texturasAtaque.resize(2);
    texturasFrontal.resize(2);

    // Cargar texturas básicas heredadas
    cargarTexturasDerecha("ray/raysano/RSright_1.png", "ray/raysano/RSright_2.png");
    cargarTexturasIzquierda("ray/raysano/RSleft_1.png", "ray/raysano/RSleft_2.png");

    // Cargar texturas frontales específicas de Ray
    cargarTexturasFrontales("ray/raysano/RSfront_1.png", "ray/raysano/RSfront_2.png");

    // Cargar texturas de ataque
    if (!texturasAtaque[0].loadFromFile("ray/raysano/RSAleft.png") ||
        !texturasAtaque[1].loadFromFile("ray/raysano/RSAright.png")) {
        std::cerr << "Error cargando imágenes de ataque" << std::endl;
    }

    // Iniciar con textura frontal (específico de Ray)
    sprite.setTexture(texturasFrontal[0]);
}

void Ray::cargarTexturasFrontales(const std::string& ruta1, const std::string& ruta2) {
    if (!texturasFrontal[0].loadFromFile(ruta1) || !texturasFrontal[1].loadFromFile(ruta2)) {
        std::cerr << "Error cargando imágenes frontales" << std::endl;
    }
}

Ray::~Ray() {
    std::cout << "Ray gone" << std::endl;
}

int Ray::golpearConBaston() {
    // Este método inicia el ataque y devuelve el daño
    if (!atacando) {
        atacando = true;
        relojAtaque.restart();
        return 1; // Valor de daño básico al iniciar el ataque
    }
    return 0; // No hace daño si ya estaba atacando
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
        // Detener movimiento durante el ataque
        moviendoDerecha = false;
        moviendoIzquierda = false;
        enMovimiento = false;
    }

    // Cambiar frame cada 0.15 segundos (aprox. 6.7fps)
    if (relojAnimacion.getElapsedTime().asSeconds() >= 0.15f) {
        frameActual = (frameActual + 1) % 2;

        // Seleccionar la textura correcta basada en el estado
        if (atacando) {
            // Mostrar textura de ataque según la dirección que esté mirando
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
            // Si no se está moviendo, usar textura frontal (específico de Ray)
            sprite.setTexture(texturasFrontal[frameActual]);
        }

        relojAnimacion.restart();
    }

    // Física del salto (reutilizamos el código de la clase base)
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Mover verticalmente
        sprite.move(0, velocidadY);

        // Verificar si tocó el suelo
        if (sprite.getPosition().y >= alturaSuelo) {
            // Reposicionar en el suelo exactamente
            sprite.setPosition(sprite.getPosition().x, alturaSuelo);
            velocidadY = 0;
            enAire = false;
        }
    }
}