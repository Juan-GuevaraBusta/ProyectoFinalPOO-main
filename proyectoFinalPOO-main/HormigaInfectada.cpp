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

    // Guardar la posición actual para restaurarla en caso de colisión
    sf::Vector2f posicionAnterior = sprite.getPosition();

    // Realizar movimiento errático
    caminarErraticamente();

    // Vector de velocidad actual (para colisiones)
    sf::Vector2f velocidadActual(velocidad * direccionActual, velocidadY);

    // Física para caer por gravedad
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
                    // Colisión con la parte superior de una plataforma (hormiga saltando)
                    sprite.setPosition(posicionAnterior.x, info.plataformaBounds.top + info.plataformaBounds.height + 1.0f);
                    velocidadY = 0.1f; // Rebote pequeño
                    break;

                case ColisionTipo::ABAJO:
                    // Colisión con la parte inferior de una plataforma (hormiga cayendo)
                    sprite.setPosition(posicionAnterior.x, info.plataformaBounds.top - objetoBounds.height);
                    velocidadY = 0.0f;
                    enAire = false;
                    break;

                case ColisionTipo::IZQUIERDA:
                    // Colisión con el lado izquierdo de una plataforma
                    sprite.setPosition(info.plataformaBounds.left - objetoBounds.width, posicionAnterior.y);
                    // Cambiar dirección al chocar
                    direccionActual = -1;
                    moviendoDerecha = false;
                    break;

                case ColisionTipo::DERECHA:
                    // Colisión con el lado derecho de una plataforma
                    sprite.setPosition(info.plataformaBounds.left + info.plataformaBounds.width, posicionAnterior.y);
                    // Cambiar dirección al chocar
                    direccionActual = 1;
                    moviendoDerecha = true;
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
            // Cambiar dirección
            direccionActual = 1;
            moviendoDerecha = true;
        }

        // Límite derecho (considerando el ancho del sprite)
        if (pos.x + sprite.getGlobalBounds().width > escenario->getLimiteDerecho()) {
            sprite.setPosition(escenario->getLimiteDerecho() - sprite.getGlobalBounds().width, pos.y);
            // Cambiar dirección
            direccionActual = -1;
            moviendoDerecha = false;
        }

        // Límite inferior (suelo)
        if (pos.y + sprite.getGlobalBounds().height > escenario->getAlturaSuelo()) {
            sprite.setPosition(pos.x, escenario->getAlturaSuelo() - sprite.getGlobalBounds().height);
            velocidadY = 0.0f;
            enAire = false;
        }
    }
}

void HormigaInfectada::dibujar(sf::RenderWindow& ventana) {
    // Siempre dibujar el sprite, ya sea vivo o muerto
    ventana.draw(sprite);
}