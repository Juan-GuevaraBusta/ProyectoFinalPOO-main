#include "HormigaNormal.h"
#include <iostream>

HormigaNormal::HormigaNormal() {
}

HormigaNormal::HormigaNormal(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion) : Personaje(nombre, vitalidad, alimento, posicion) {
    // Reservar espacio para las texturas
    //texturasDerecha.resize(2);
    //texturasIzquierda.resize(2);

    // Cargar texturas
    cargarTexturas();

    // Utilizamos la textura derecha como inicial
    sprite.setTexture(texturasDerecha[0]);
    sprite.setPosition(posicion[0], posicion[1]);
    sprite.setScale(2.0f, 2.0f);
}

HormigaNormal::~HormigaNormal() {
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

    // Guardar la posición actual para restaurarla en caso de colisión
    sf::Vector2f posicionAnterior = sprite.getPosition();
    sf::Vector2f velocidadActual(0.0f, velocidadY);

    // Física del salto
    if (enAire) {
        // Aplicar gravedad
        velocidadY += gravedad;
        velocidadActual.y = velocidadY;

        // Mover verticalmente
        sprite.move(0, velocidadY);
    }

    // Verificar colisiones con plataformas
    if (escenario) {
        sf::FloatRect objetoBounds = sprite.getGlobalBounds();
        ColisionInfo info = escenario->verificarColisionConPlataformas(objetoBounds, velocidadActual);

        if (info.colision) {
            switch (info.tipo) {
                case ColisionTipo::ARRIBA:
                    // Colisión con la parte superior de una plataforma (personaje saltando)
                    sprite.setPosition(posicionAnterior.x, info.plataformaBounds.top + info.plataformaBounds.height + 1.0f);
                    velocidadY = 0.1f; // Rebote pequeño
                    break;

                case ColisionTipo::ABAJO:
                    // Colisión con la parte inferior de una plataforma (personaje cayendo)
                    sprite.setPosition(posicionAnterior.x, info.plataformaBounds.top - objetoBounds.height);
                    velocidadY = 0.0f;
                    enAire = false;
                    break;

                case ColisionTipo::IZQUIERDA:
                    // Colisión con el lado izquierdo de una plataforma
                    sprite.setPosition(info.plataformaBounds.left - objetoBounds.width, posicionAnterior.y);
                    break;

                case ColisionTipo::DERECHA:
                    // Colisión con el lado derecho de una plataforma
                    sprite.setPosition(info.plataformaBounds.left + info.plataformaBounds.width, posicionAnterior.y);
                    break;

                default:
                    break;
            }
        } else {
            // No hay colisión directa, verificar si debe caer
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
        }

        // Verificar límites del escenario
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

// Implementa el método para ajustar la hitbox
void HormigaNormal::ajustarHitbox(float offsetX, float offsetY, float width, float height) {
    hitboxOffsetX = offsetX;
    hitboxOffsetY = offsetY;
    hitboxWidth = width;
    hitboxHeight = height;
    usingCustomHitbox = true;
}

// Sobrescribe getBounds para usar los valores personalizados
sf::FloatRect HormigaNormal::getBounds() const {
    if (usingCustomHitbox) {
        // Retornar hitbox personalizada
        return sf::FloatRect(
            sprite.getPosition().x + hitboxOffsetX,
            sprite.getPosition().y + hitboxOffsetY,
            hitboxWidth,
            hitboxHeight
        );
    } else {
        // Retornar hitbox por defecto (la del sprite)
        return sprite.getGlobalBounds();
    }
}

void HormigaNormal::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite);
}