#include "Escenario.h"
#include <iostream>
#include <limits>

Escenario::Escenario() : fondoCargado(false) {
    // Inicializar límites por defecto
    limiteIzquierdo = 0.0f;
    limiteDerecho = 1366.0f;
    limiteSuperior = 0.0f;
    limiteInferior = 768.0f;

    // Definir una altura de suelo adecuada
    alturaSuelo = 700.0f;

    // Cargar la imagen de fondo
    if (!texturaFondo.loadFromFile("escenarios/escenario1.png")) {
        std::cerr << "Error cargando imagen de fondo: escenarios/escenario1.png" << std::endl;
        // Intentar con una ruta alternativa
        if (!texturaFondo.loadFromFile("fondo.png")) {
            std::cerr << "Error cargando imagen de fondo alternativa: fondo.png" << std::endl;
        } else {
            fondoCargado = true;
        }
    } else {
        fondoCargado = true;
    }

    if (fondoCargado) {
        spriteFondo.setTexture(texturaFondo);

        // Obtener tamaño de la ventana
        float ventanaAncho = 1366.0f;
        float ventanaAlto = 768.0f;

        // Obtener tamaño de la textura
        sf::Vector2u tamañoTextura = texturaFondo.getSize();

        // Calcular factores de escala para cada dimensión
        float escalaX = ventanaAncho / tamañoTextura.x;
        float escalaY = ventanaAlto / tamañoTextura.y;

        // Aplicar escala al sprite para que cubra exactamente la ventana
        spriteFondo.setScale(escalaX, escalaY);

        // Posicionar en la esquina superior izquierda
        spriteFondo.setPosition(0, 0);
    }

    // Agregar plataformas
    //plataformas horizontales
    //columna 1
    agregarPlataforma(0.0f, 592.0f, 95.0f, 35.0f);
    agregarPlataforma(0.0f, 485.0f, 300.0f, 35.0f);
    agregarPlataforma(0.0f, 380.0f, 95.0f, 35.0f);
    agregarPlataforma(215.0f, 275.0f, 145.0f, 35.0f);
    agregarPlataforma(73.0f, 170.0f, 145.0f, 35.0f);

    //columna 2
    agregarPlataforma(325.0f, 592.0f, 190.0f, 35.0f);
    agregarPlataforma(515.0f, 485.0f, 190.0f, 35.0f);
    agregarPlataforma(365.0f, 380.0f, 190.0f, 35.0f);
    agregarPlataforma(560.0f, 275.0f, 45.0f, 35.0f);
    agregarPlataforma(360.0f, 170.0f, 145.0f, 35.0f);
    agregarPlataforma(505.0f, 140.0f, 145.0f, 35.0f);

    //columna 3
    agregarPlataforma(620.0f, 592.0f, 390.0f, 35.0f);
    agregarPlataforma(680.0f, 380.0f, 280.0f, 35.0f);
    agregarPlataforma(680.0f, 245.0f, 190.0f, 35.0f);
    agregarPlataforma(780.0f, 140.0f, 45.0f, 35.0f);

    //columna 4
    agregarPlataforma(1115.0f, 540.0f, 95.0f, 35.0f);
    agregarPlataforma(910.0f, 485.0f, 95.0f, 35.0f);
    agregarPlataforma(1115.0f, 425.0f, 95.0f, 35.0f);
    agregarPlataforma(960.0f, 345.0f, 95.0f, 35.0f);
    agregarPlataforma(1020.0f, 245.0f, 45.0f, 35.0f);
    agregarPlataforma(1165.0f, 245.0f, 45.0f, 35.0f);
    agregarPlataforma(1070.0f, 140.0f, 300.0f, 35.0f);
    agregarPlataforma(942.0f, 165.0f, 43.0f, 35.0f);

    //plataformas verticales
    //columna 2
    agregarPlataforma(472.2f, 520.0f, 40.0f, 75.0f);

    //columna 3
    agregarPlataforma(618.0f, 625.0f, 46.0f, 69.0f);
    agregarPlataforma(868.0f, 415.0f, 43.0f, 175.0f);
    agregarPlataforma(828.0f, 70.0f, 42.0f, 210.0f);

    //columna 4
    agregarPlataforma(1213.0f, 275.0f, 41.0f, 340.0f);
}

Escenario::~Escenario() {
    // Destructor vacío
}

void Escenario::agregarPlataforma(float x, float y, float ancho, float alto) {
    sf::RectangleShape plataforma(sf::Vector2f(ancho, alto));
    plataforma.setPosition(x, y);

    // Para plataformas visibles (durante pruebas):
    plataforma.setFillColor(sf::Color::Transparent);

    plataformas.push_back(plataforma);
}

bool Escenario::verificarColisionPlataforma(const sf::FloatRect& objetoBounds) {
    for (const auto& plataforma : plataformas) {
        if (plataforma.getGlobalBounds().intersects(objetoBounds)) {
            return true;
        }
    }
    return false;
}

// MÉTODO SIMPLIFICADO: detectar colisiones con plataformas de manera más sencilla
ColisionInfo Escenario::verificarColisionConPlataformas(const sf::FloatRect& objetoBounds, const sf::Vector2f& velocidad) {
    ColisionInfo info;
    info.colision = false;
    info.tipo = ColisionTipo::NINGUNA;

    for (const auto& plataforma : plataformas) {
        sf::FloatRect plataformaBounds = plataforma.getGlobalBounds();

        // Si hay intersección entre el objeto y la plataforma
        if (plataformaBounds.intersects(objetoBounds)) {
            info.colision = true;
            info.plataformaBounds = plataformaBounds;

            // Determinar el tipo de colisión de manera simplificada
            // basándonos en la dirección del movimiento

            // Si hay movimiento vertical
            if (std::abs(velocidad.y) > 0.01f) {
                if (velocidad.y > 0) {
                    // Cayendo, colisión con la parte inferior del objeto
                    info.tipo = ColisionTipo::ABAJO;
                    return info;
                }
                else if (velocidad.y < 0) {
                    // Saltando, colisión con la parte superior del objeto
                    info.tipo = ColisionTipo::ARRIBA;
                    return info;
                }
            }

            // Si hay movimiento horizontal
            if (std::abs(velocidad.x) > 0.01f) {
                if (velocidad.x > 0) {
                    // Moviéndose a la derecha
                    info.tipo = ColisionTipo::DERECHA;
                    return info;
                }
                else if (velocidad.x < 0) {
                    // Moviéndose a la izquierda
                    info.tipo = ColisionTipo::IZQUIERDA;
                    return info;
                }
            }

            // Si no hay movimiento pero hay colisión, determinar basado
            // en la posición relativa objeto-plataforma
            float centroObjX = objetoBounds.left + objetoBounds.width / 2.0f;
            float centroPlataformaX = plataformaBounds.left + plataformaBounds.width / 2.0f;

            if (centroObjX < centroPlataformaX) {
                info.tipo = ColisionTipo::DERECHA;
            } else {
                info.tipo = ColisionTipo::IZQUIERDA;
            }

            return info;
        }
    }

    return info;
}

float Escenario::getAlturaPlatformaEn(float posX, float posY, float alturaObjeto, bool& enPlataforma) {
    enPlataforma = false;
    float alturaMinima = alturaSuelo;

    for (const auto& plataforma : plataformas) {
        sf::FloatRect bounds = plataforma.getGlobalBounds();

        // Verificar si el punto está directamente sobre una plataforma (en el eje X)
        if (posX >= bounds.left && posX < bounds.left + bounds.width) {
            // Calcular distancia vertical entre el objeto y la plataforma
            float distanciaAlPiso = bounds.top - (posY + alturaObjeto);

            // Si el objeto está por encima de la plataforma o muy cerca (tolerancia de 5 píxeles)
            if (distanciaAlPiso > -5.0f && distanciaAlPiso <= 10.0f) {
                // La plataforma está justo debajo o muy cerca del objeto
                if (bounds.top < alturaMinima) {
                    alturaMinima = bounds.top;
                    enPlataforma = true;
                }
            }
        }
    }

    return alturaMinima;
}

void Escenario::configurarLimites(float izq, float der, float sup, float inf) {
    limiteIzquierdo = izq;
    limiteDerecho = der;
    limiteSuperior = sup;
    limiteInferior = inf;
}

void Escenario::dibujar(sf::RenderWindow& ventana) {
    // Dibujar el fondo solo si se cargó correctamente
    if (fondoCargado) {
        ventana.draw(spriteFondo);
    }

    // Dibujar todas las plataformas
    for (const auto& plataforma : plataformas) {
        ventana.draw(plataforma);
    }
}