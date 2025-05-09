#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <string>
#include <vector>



class Personaje {
protected:
    std::string nombre;
    int vitalidad;
    int alimento;
    std::vector<int> posicion;

public:
    Personaje();
    Personaje(std::string nombre, int vitalidad, int alimento, std::vector<int> posicion);
    virtual ~Personaje();
    std::string getNombre();
    int getVitalidad();
    int getAlimento();
    std::vector<int> getPosicion();
    void setNombre(std::string nombre);
    void setVitalidad(int vitalidad);
    void setAlimento(int alimento);
    void setPosicion(std::vector<int> posicion);
};

#endif //PERSONAJE_H
