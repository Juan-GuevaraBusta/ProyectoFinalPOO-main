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
    // Liberar memoria de los colisionadores
    for (auto& collider : colisionadores) {
        delete collider;
    }
}

void Escenario::agregarPlataforma(float x, float y, float ancho, float alto) {
    sf::RectangleShape plataforma(sf::Vector2f(ancho, alto));

    // Importante: Establecer el origen en el centro del bloque
    // Esta modificación es clave para que las colisiones funcionen como en el proyecto de referencia
    plataforma.setOrigin(ancho / 2.0f, alto / 2.0f);

    // La posición ahora debe ajustarse para tener en cuenta el origen centrado
    plataforma.setPosition(x + ancho / 2.0f, y + alto / 2.0f);

    // Color para las plataformas (puedes ajustar para que sean más o menos visibles)
    plataforma.setFillColor(sf::Color::Red);

    plataformas.push_back(plataforma);

    // Crear el colisionador para esta plataforma
    colisionadores.push_back(new Collider(plataformas.back()));
}

// Método que usa el sistema Collider para verificar colisiones
bool Escenario::checkCollision(Collider& personajeCollider, sf::Vector2f& direction, float pushForce) {
    bool colision = false;

    // Verificar colisión con cada plataforma
    for (auto& collider : colisionadores) {
        if (collider->checkCollision(personajeCollider, direction, pushForce)) {
            colision = true;
            // No salimos del bucle para verificar todas las posibles colisiones
        }
    }

    return colision;
}

// Método para verificar si un punto está sobre una plataforma
bool Escenario::estaSobrePlataforma(float posX, float posY, float altura, float& alturaPlataforma) {
    alturaPlataforma = alturaSuelo; // Por defecto, el suelo
    bool sobrePlataforma = false;

    for (const auto& plataforma : plataformas) {
        // Obtener el centro y tamaño de la plataforma considerando el origen centrado
        sf::Vector2f centro = plataforma.getPosition();
        sf::Vector2f tamaño = plataforma.getSize();

        // Calcular los límites reales de la plataforma
        float izquierda = centro.x - tamaño.x / 2.0f;
        float derecha = centro.x + tamaño.x / 2.0f;
        float arriba = centro.y - tamaño.y / 2.0f;
        float abajo = centro.y + tamaño.y / 2.0f;

        // Verificar si el punto (posX, posY + altura) está encima de esta plataforma
        if (posX >= izquierda && posX <= derecha) {
            // Calcular la distancia vertical entre el borde inferior del personaje y el borde superior de la plataforma
            float distanciaVertical = arriba - (posY + altura);

            // Si la distancia es pequeña y positiva (personaje justo encima de la plataforma)
            // o si la distancia es negativa pero muy pequeña (penetrando ligeramente la plataforma)
            if (distanciaVertical >= -5.0f && distanciaVertical <= 10.0f) {
                // Si esta plataforma está más arriba que la última encontrada
                if (arriba < alturaPlataforma) {
                    alturaPlataforma = arriba;
                    sobrePlataforma = true;
                }
            }
        }
    }

    return sobrePlataforma;
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