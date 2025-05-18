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
    sprite.setTextureRect(sf::IntRect(3, 3, 26, 20));
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
    sprite.move(3.0f, 0.f);
}

void Ray::caminarAtras() {
    moviendoDerecha = false;
    moviendoIzquierda = true;
    mirandoDerecha = false;
    enMovimiento = true;
    sprite.move(-3.0f, 0.f);
}

void Ray::detener() {
    moviendoDerecha = false;
    moviendoIzquierda = false;
    enMovimiento = false;
}

void Ray::saltar() {
    if (!enAire) {
        velocidadY = -15.0f;
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

void Ray::actualizar(Escenario* escenario) {
    // Velocidad constante para movimiento horizontal
    float velocidadX = 3.0f;

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

    // Cambiar frame cada 0.15 segundos para animación
    if (relojAnimacion.getElapsedTime().asSeconds() >= 0.1f) {
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

    // Guardar la posición actual para restaurarla en caso de colisión
    sf::Vector2f posicionAnterior = sprite.getPosition();

    // PARTE 1: MOVIMIENTO VERTICAL (GRAVEDAD Y SALTO)
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Guardar posición antes del movimiento vertical
        float posYAnterior = sprite.getPosition().y;

        // Mover verticalmente
        sprite.move(0, velocidadY);

        // Verificar colisiones verticales si hay escenario
        if (escenario) {
            sf::FloatRect objetoBounds = sprite.getGlobalBounds();

            // Verificar colisión después del movimiento vertical
            ColisionInfo infoVertical = escenario->verificarColisionConPlataformas(
                objetoBounds,
                sf::Vector2f(0.0f, velocidadY)
            );

            if (infoVertical.colision) {
                if (infoVertical.tipo == ColisionTipo::ABAJO) {
                    // Cayendo y colisiona con plataforma - aterrizaje
                    sprite.setPosition(sprite.getPosition().x, infoVertical.plataformaBounds.top - objetoBounds.height);
                    velocidadY = 0.0f;
                    enAire = false;
                }
                else if (infoVertical.tipo == ColisionTipo::ARRIBA) {
                    // Saltando y golpea el techo
                    sprite.setPosition(sprite.getPosition().x, infoVertical.plataformaBounds.top + infoVertical.plataformaBounds.height);
                    velocidadY = 0.1f; // Pequeño rebote
                }
            }
        }
    }

    // PARTE 2: MOVIMIENTO HORIZONTAL (IZQUIERDA/DERECHA)
    // Primero aplicamos el movimiento
    if (moviendoDerecha) {
        sprite.move(velocidadX, 0);
    }
    else if (moviendoIzquierda) {
        sprite.move(-velocidadX, 0);
    }

    // Después verificamos colisiones horizontales
    if ((moviendoDerecha || moviendoIzquierda) && escenario) {
        sf::FloatRect objetoBounds = sprite.getGlobalBounds();
        sf::Vector2f velocidadActual(moviendoDerecha ? velocidadX : -velocidadX, 0);

        ColisionInfo infoHorizontal = escenario->verificarColisionConPlataformas(
            objetoBounds,
            velocidadActual
        );

        if (infoHorizontal.colision) {
            // Si hay colisión horizontal, simplemente restauramos la posición X anterior
            // No importa si es izquierda o derecha, solo no permitimos el movimiento
            sprite.setPosition(posicionAnterior.x, sprite.getPosition().y);


        }
    }

    // PARTE 3: VERIFICAR SI DEBE CAER
    if (escenario) {
        bool enPlataforma = false;
        float alturaPlatforma = escenario->getAlturaPlatformaEn(
            sprite.getPosition().x + sprite.getGlobalBounds().width / 2,
            sprite.getPosition().y,
            sprite.getGlobalBounds().height,
            enPlataforma
        );

        // Si no está en el aire ni en una plataforma, debe caer
        if (!enAire && !enPlataforma && sprite.getPosition().y + sprite.getGlobalBounds().height < alturaPlatforma) {
            enAire = true;
            velocidadY = 0.1f; // Velocidad inicial pequeña
        }

        // Si está en el aire y ha llegado al suelo o una plataforma
        if (enAire) {
            if (sprite.getPosition().y + sprite.getGlobalBounds().height >= alturaPlatforma) {
                // Aterrizó en una plataforma o en el suelo
                sprite.setPosition(sprite.getPosition().x, alturaPlatforma - sprite.getGlobalBounds().height);
                velocidadY = 0.0f;
                enAire = false;
            }
        }

        // PARTE 4: LIMITES DEL ESCENARIO
        sf::Vector2f pos = sprite.getPosition();

        // Límite izquierdo
        if (pos.x < escenario->getLimiteIzquierdo()) {
            sprite.setPosition(escenario->getLimiteIzquierdo(), pos.y);
        }

        // Límite derecho (considerando el ancho del sprite)
        if (pos.x + sprite.getGlobalBounds().width > escenario->getLimiteDerecho()) {
            sprite.setPosition(escenario->getLimiteDerecho() - sprite.getGlobalBounds().width, pos.y);
        }

        // Límite superior
        if (pos.y < escenario->getLimiteSuperior()) {
            sprite.setPosition(pos.x, escenario->getLimiteSuperior());
            velocidadY = 0.1f; // Para que comience a caer
        }

        // Límite inferior (suelo)
        if (pos.y + sprite.getGlobalBounds().height > escenario->getAlturaSuelo()) {
            sprite.setPosition(pos.x, escenario->getAlturaSuelo() - sprite.getGlobalBounds().height);
            velocidadY = 0.0f;
            enAire = false;
        }
    }
}

void Ray::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}

