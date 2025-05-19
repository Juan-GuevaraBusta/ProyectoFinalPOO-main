#include "HormigaNormal.h"
#include <iostream>

HormigaNormal::HormigaNormal() {
    hitbox.setSize(sf::Vector2f(40.0f, 40.0f));  // Tamaño predeterminado
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(1.0f);

    collider = new Collider(hitbox);
}

HormigaNormal::HormigaNormal(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion)
    : Personaje(nombre, vitalidad, alimento, posicion) {

    // Inicializar la hitbox para colisiones
    hitbox.setSize(sf::Vector2f(40.0f, 40.0f));  // Tamaño predeterminado
    hitbox.setFillColor(sf::Color::Transparent);
    hitbox.setOutlineColor(sf::Color::Green);
    hitbox.setOutlineThickness(1.0f);

    collider = new Collider(hitbox);

    // Cargar texturas
    cargarTexturas();

    // Utilizamos la textura derecha como inicial
    sprite.setTexture(texturasDerecha[0]);
    sprite.setPosition(posicion[0], posicion[1]);
    sprite.setScale(2.0f, 2.0f);

    // Actualizar hitbox a la posición inicial
    actualizarHitbox();
}

HormigaNormal::~HormigaNormal() {
    delete collider;
}

void HormigaNormal::cargarTexturas() {
    texturasDerecha.resize(2);
    texturasIzquierda.resize(2);
    // Cargar texturas para hormiga normal
    if (!texturasDerecha[0].loadFromFile("hormiga_normal/antright_1.png") ||
        !texturasDerecha[1].loadFromFile("hormiga_normal/antright_2.png")) {
        std::cerr << "Error cargando imágenes derecha de hormiga normal" << std::endl;
    }

    if (!texturasIzquierda[0].loadFromFile("hormiga_normal/antleft_1.png") ||
        !texturasIzquierda[1].loadFromFile("hormiga_normal/antleft_2.png")) {
        std::cerr << "Error cargando imágenes izquierda de hormiga normal" << std::endl;
    }
}

void HormigaNormal::setPosicion(float x, float y) {
    sprite.setPosition(x, y);
    actualizarHitbox();
}

void HormigaNormal::actualizarHitbox() {
    if (usingCustomHitbox) {
        hitbox.setPosition(sprite.getPosition().x + hitboxOffsetX,
                          sprite.getPosition().y + hitboxOffsetY);
    } else {
        // Centrar la hitbox en el sprite
        sf::FloatRect bounds = sprite.getGlobalBounds();
        hitbox.setSize(sf::Vector2f(bounds.width * 0.8f, bounds.height * 0.8f));
        hitbox.setPosition(bounds.left + bounds.width * 0.1f,
                          bounds.top + bounds.height * 0.1f);
    }
}

void HormigaNormal::caminarAdelante() {
    moviendoDerecha = true;
    moviendoIzquierda = false;
    mirandoDerecha = true;  // Recordar que está mirando a la derecha
    enMovimiento = true;
    sprite.move(0.1, 0.f);
    actualizarHitbox();
}

void HormigaNormal::caminarAtras() {
    moviendoDerecha = false;
    moviendoIzquierda = true;
    mirandoDerecha = false;  // Recordar que está mirando a la izquierda
    enMovimiento = true;
    sprite.move(-0.1, 0.f);
    actualizarHitbox();
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

    // Guardar posición anterior para restaurarla en caso de colisión
    sf::Vector2f posicionAnterior = sprite.getPosition();

    // Física del salto
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

            // Manejar distintos tipos de colisión
            if (direction.x != 0.0f) {
                // Colisión lateral - detener movimiento horizontal
                sprite.setPosition(usingCustomHitbox ?
                                  hitboxPos.x - hitboxOffsetX :
                                  hitboxPos.x - (sprite.getGlobalBounds().width * 0.1f),
                                  sprite.getPosition().y);
            }

            if (direction.y < 0.0f) {
                // Colisión desde abajo (personaje saltando)
                velocidadY = 0.1f; // Pequeño rebote
                sprite.setPosition(sprite.getPosition().x,
                                  usingCustomHitbox ?
                                  hitboxPos.y - hitboxOffsetY :
                                  hitboxPos.y - (sprite.getGlobalBounds().height * 0.1f));
            }
            else if (direction.y > 0.0f) {
                // Colisión desde arriba (personaje cayendo)
                velocidadY = 0.0f;
                enAire = false;
                sprite.setPosition(sprite.getPosition().x,
                                  usingCustomHitbox ?
                                  hitboxPos.y - hitboxOffsetY :
                                  hitboxPos.y - (sprite.getGlobalBounds().height * 0.1f));
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
            actualizarHitbox();
        }

        // Límite derecho (considerando el ancho del sprite)
        if (pos.x + sprite.getGlobalBounds().width > escenario->getLimiteDerecho()) {
            sprite.setPosition(escenario->getLimiteDerecho() - sprite.getGlobalBounds().width, pos.y);
            actualizarHitbox();
        }

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

// Implementa el método para ajustar la hitbox
void HormigaNormal::ajustarHitbox(float offsetX, float offsetY, float width, float height) {
    hitboxOffsetX = offsetX;
    hitboxOffsetY = offsetY;
    hitboxWidth = width;
    hitboxHeight = height;
    usingCustomHitbox = true;

    hitbox.setSize(sf::Vector2f(hitboxWidth, hitboxHeight));
    actualizarHitbox();
}

// Sobrescribe getBounds para usar los valores personalizados
sf::FloatRect HormigaNormal::getBounds() const {
    if (usingCustomHitbox) {
        // Retornar hitbox personalizada
        return hitbox.getGlobalBounds();
    } else {
        // Retornar hitbox por defecto (la del sprite)
        return sprite.getGlobalBounds();
    }
}

void HormigaNormal::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
    // Opcionalmente para debug, dibujar la hitbox
    //ventana.draw(hitbox);
}