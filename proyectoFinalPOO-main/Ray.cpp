#include "Ray.h"
#include "HormigaInfectada.h"
#include <iostream>
#include <cmath>  // Para sqrt en cálculo de distancia

Ray::Ray(std::string nombreJugador) : Personaje("Ray", 3, 0, {0, 0}), Jugador(nombreJugador) {
    this->luz = 0;

    // Inicializar hitbox y collider
    hitbox.setSize(sf::Vector2f(45.0f, 45.0f));
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(1.0f);

    collider = new Collider(hitbox);

    // Cargar todas las texturas
    cargarTexturas();

    // Configurar el sprite inicialmente
    sprite.setTexture(texturasFrontal[0]);
    sprite.setTextureRect(sf::IntRect(3, 3, 26, 20));
    sprite.setPosition(100.f, alturaSuelo);
    sprite.setScale(2.0f, 2.0f);

    // Actualizar hitbox a la posición inicial
    actualizarHitbox();
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
    delete collider;
    std::cout << "Ray gone" << std::endl;
}

void Ray::setPosicion(float x, float y) {
    sprite.setPosition(x, y);
    actualizarHitbox();
}

void Ray::actualizarHitbox() {
    // Centrar la hitbox en el sprite
    sf::FloatRect bounds = sprite.getGlobalBounds();
    hitbox.setSize(sf::Vector2f(bounds.width * 0.8f, bounds.height * 0.8f));
    hitbox.setPosition(bounds.left + bounds.width * 0.1f,
                       bounds.top + bounds.height * 0.1f);
}

void Ray::caminarAdelante() {
    moviendoDerecha = true;
    moviendoIzquierda = false;
    mirandoDerecha = true;
    enMovimiento = true;
    sprite.move(1.2f, 0.f);
    actualizarHitbox();
}

void Ray::caminarAtras() {
    moviendoDerecha = false;
    moviendoIzquierda = true;
    mirandoDerecha = false;
    enMovimiento = true;
    sprite.move(-1.2f, 0.f);
    actualizarHitbox();
}

void Ray::detener() {
    moviendoDerecha = false;
    moviendoIzquierda = false;
    enMovimiento = false;
}

void Ray::saltar() {
    if (!enAire) {
        velocidadY = -10.0f;
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

    // PARTE 1: MOVIMIENTO VERTICAL (GRAVEDAD Y SALTO)
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;

        // Mover verticalmente
        sprite.move(0, velocidadY);
        actualizarHitbox();
    }

    // PARTE 2: MOVIMIENTO HORIZONTAL (IZQUIERDA/DERECHA)
    // Primero aplicamos el movimiento
    if (moviendoDerecha) {
        sprite.move(velocidadX, 0);
        actualizarHitbox();
    }
    else if (moviendoIzquierda) {
        sprite.move(-velocidadX, 0);
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
                // Colisión lateral - detener movimiento horizontal
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

        // PARTE 4: LIMITES DEL ESCENARIO
        sf::Vector2f pos = sprite.getPosition();

        // Límite izquierdo
        if (pos.x < escenario->getLimiteIzquierdo()) {
            sprite.setPosition(escenario->getLimiteIzquierdo(), pos.y);
            actualizarHitbox();
        }

        // Límite derecho (considerando el ancho del sprite)
        if (pos.x + sprite.getGlobalBounds().width > escenario->getLimiteDerecho()) {
            sprite.setPosition(escenario->getLimiteDerecho() - sprite.getGlobalBounds().width, pos.y);
            actualizarHitbox();
        }

        // Límite superior
        // Límite superior
        if (pos.y < escenario->getLimiteSuperior()) {
            sprite.setPosition(pos.x, escenario->getLimiteSuperior());
            velocidadY = 0.1f; // Para que comience a caer
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

void Ray::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    // Opcionalmente para debug, dibujar la hitbox
    //ventana.draw(hitbox);
}