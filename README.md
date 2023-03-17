# Seguidor de Linea (No propeller)

Sketch en PlatformIO destinado a firmware de placas custom de robot seguidor de línea.

Es útil para las siguientes placas.

- [Open Lamborghino](https://lamborghino.com/producto/pcb-de-open-lamborghino/)
- [Predator](https://a360.co/3JqkoGa)

## Programación de Firmware

1. Clonar el repositorio
2. Descargar extensión de [Platformio](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) en VSCode
3. Dentro del archivo [src/seguidor.cpp](./src/seguidor.cpp)

   - Define con que placa trabajarás
   - En caso de trabajar con Open Lamborghino revisar que las lineas 42 y 45 estén de la siguiente forma

     ```cpp
     #define Lamborghino
     //#define Predator
     ```

   - En caso de trabaja con Predator se debe tener lo siguiente

     ```cpp
     //#define Lamborghino
     #define Predator
     ```

4. Subir el firmware

## Uso del robot

### Lamborhino

1. Encender robot
2. Inicio de calibración
   - Colocar los sensores en linea oscura y pulsar el botón (Mantener en la posición hasta que suene el buzzer).
   - Colocar los sensores en linea clara y pulsar el botón (Mantener en la posición hasta que suene el buzzer).
   - Colocar el robot en posición de arranque.
3. Pulsar nuevamente el botón para iniciar su rutina.

### Predator

1. Encender robot
2. Inicio de calibración
   - Colocar los sensores en linea oscura y pulsar el botón izquierdo (Mantener en la posición hasta que suene el buzzer).
   - Colocar los sensores en linea clara y pulsar el botón derecho (Mantener en la posición hasta que suene el buzzer).
   - Colocar el robot en posición de arranque.
3. Pulsar los botones izquierdo y derecho para iniciar su rutina.
