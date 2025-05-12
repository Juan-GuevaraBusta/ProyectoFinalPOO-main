cmake_minimum_required(VERSION 3.16)
project(practica_sfml)

set(CMAKE_CXX_STANDARD 20)

# Ruta local donde está SFMLConfig.cmake
set(SFML_DIR "C:/Users/ADRIANA/SFML-2.5.1/lib/cmake/SFML")
include_directories("C:/Users/ADRIANA/SFML-2.5.1/include")
link_directories("C:/Users/ADRIANA/SFML-2.5.1/lib")
#set(SFML_DIR "/usr/local/opt/sfml@2/lib/cmake/SFML")
#include_directories("/usr/local/opt/sfml@2/include")
#link_directories("/usr/local/opt/sfml@2/lib")

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(practica_sfml
        main.cpp
        Ray.cpp
        Ray.h
        Personaje.cpp
        Personaje.h
        ReyHongo.cpp
        ReyHongo.h
        HormigaNormal.cpp
        HormigaNormal.h
        HormigaNPC.cpp
        HormigaNPC.h
        HormigaInfectada.cpp
        HormigaInfectada.h
        Jugador.cpp
        Jugador.h
        Juego.cpp
        Juego.h
        Escenario.cpp
        Escenario.h
)

target_link_libraries(practica_sfml
        sfml-graphics
        sfml-window
        sfml-system
)