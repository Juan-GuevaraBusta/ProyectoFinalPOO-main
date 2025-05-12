#include "Escenario.h"
#include <iostream>

Escenario::Escenario() : fondoCargado(false) {
    // Cargar la textura del fondo desde una ruta fija dentro de la clase
    if (!texturaFondo.loadFromFile("escenarios/escenario1.png")) {
        std::cerr << "Error cargando imagen de fondo: escenario/fondo.png" << std::endl;
        // Intentar con una ruta alternativa
        if (!texturaFondo.loadFromFile("escenarios/escenario1.png")) {
            std::cerr << "Error cargando imagen de fondo alternativa: fondo.png" << std::endl;
        } else {
            fondoCargado = true;
        }
    } else {
        fondoCargado = true;
    }

    if (fondoCargado) {
        spriteFondo.setTexture(texturaFondo);

        // Obtener tamaño de la ventana (asumimos 1366x768 según el main.cpp)
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

    // Inicializar límites por defecto
    limiteIzquierdo = 0.0f;
    limiteDerecho = 1366.0f;
    limiteSuperior = 0.0f;
    limiteInferior = 768.0f;

    // Aquí puedes agregar plataformas predefinidas para tu escenario
    // Por ejemplo:
    agregarPlataforma(0.0f, 592.0f, 95.0f, 35.0f);  // Plataforma 1
    //agregarPlataforma(600.0f, 400.0f, 200.0f, 20.0f);  // Plataforma 2
    //agregarPlataforma(900.0f, 300.0f, 250.0f, 20.0f);  // Plataforma 3
}

Escenario::~Escenario() {
    // Destructor vacío
}

void Escenario::agregarPlataforma(float x, float y, float ancho, float alto) {
    sf::RectangleShape plataforma(sf::Vector2f(ancho, alto));
    plataforma.setPosition(x, y);

    // ESTA ES LA LÍNEA QUE CONTROLA EL COLOR DE LAS PLATAFORMAS
    // Para plataformas visibles (durante pruebas):
    plataforma.setFillColor(sf::Color(139, 69, 19, 128));  // Marrón semi-transparente

    // Para plataformas invisibles (versión final):
    // plataforma.setFillColor(sf::Color::Transparent);

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