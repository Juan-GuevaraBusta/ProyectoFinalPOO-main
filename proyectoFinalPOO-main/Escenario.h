//
// Created by ADRIANA on 3/05/2025.
//

#ifndef ESCENARIO_H
#define ESCENARIO_H
#include <vector>

class Escenario {
public:
    std::vector<std::vector<int>> posiciones;
    std::vector<int> posicionFinalizar;
    std::vector<std::vector<int>> posicionPersonajes;

    Escenario();
    virtual ~Escenario();

    // Métodos específicos
    void iniciarEscenario();
    void finalizarEscenario();

    // Getters y setters
    std::vector<std::vector<int>> getPosiciones();
    std::vector<int> getPosicionFinalizar();
    std::vector<std::vector<int>> getPosicionPersonajes();

    void setPosiciones(std::vector<std::vector<int>>& posiciones);
    void setPosicionFinalizar(std::vector<int>& posicionFinalizar);
    void setPosicionPersonajes(std::vector<std::vector<int>>& posicionPersonajes);
};

#endif // ESCENARIO_H
