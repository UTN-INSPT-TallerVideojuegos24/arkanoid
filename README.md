# Arkanoid - UTN-INSPT

En este ejemplo se muestra como hacer un uso básico de Sprites y efectos de sonido/música en un juego hecho de forma "rústica" con todo el código en el `main()`. No se sugiere este enfoque, debería usarse técnicas más escalables y flexibles como TDA en caso de no saber teoría de objetos.

## Cómo Usar

1. Si usas Linux, instala las dependencias de SFML usando el gestor de paquetes de tu sistema. En Ubuntu y otras distribuciones basadas en Debian, puedes usar los siguientes comandos:
    ```
    sudo apt update
    sudo apt install \
        libxrandr-dev \
        libxcursor-dev \
        libudev-dev \
        libfreetype-dev \
        libopenal-dev \
        libflac-dev \
        libvorbis-dev \
        libgl1-mesa-dev \
        libegl1-mesa-dev
    ```
2. Configura y construye tu proyecto. La mayoría de los IDE populares admiten proyectos de CMake con muy poco esfuerzo de tu parte.
    - [VS Code](https://code.visualstudio.com) a través de la [extensión CMake](https://code.visualstudio.com/docs/cpp/cmake-linux)
    - [Visual Studio](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)
    - [CLion](https://www.jetbrains.com/clion/features/cmake-support.html)
    - [Qt Creator](https://doc.qt.io/qtcreator/creator-project-cmake.html)

    Usar CMake desde la línea de comandos también es directo.

    Para un generador de configuración única (típicamente el caso en Linux y macOS):
    ```
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build
    ```

    Para un generador de configuración múltiple (típicamente el caso en Windows):
    ```
    cmake -S . -B build
    cmake --build build --config Release
    ```
3. ¡Disfruta!

## Configuraciones

En el archivo `include/conf.h` puede encontrar un set de constantes que flexibilizan algunos parámetros del juego, la idea es que pueda probar cambiar valores y ver qué ocurre.

## Licencia

El código fuente tiene licencia MIT.
