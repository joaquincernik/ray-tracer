# Trazador de rayos: solución del taller de iluminación local

Se incluyen los cuatro archivos del esqueleto que tenían espacios para completar 
(`TODO (paso N)`) con el código de los seis pasos resueltos. El resto del código del
esqueleto no cambia.

| Archivo | Pasos |
|---|---|
| `src/rt/plane.h` | 1. Intersección rayo-plano |
| `src/rt/sphere.h` | 2. Intersección rayo-esfera |
| `src/rt/scene.cpp` | 3. Normales como color, 4. Modelo de Phong, 5. Sombras |
| `src/rt/checker.h` | 6. Tablero |

## Cómo usarla

Para completar lo que haya quedado sin terminar, alcanza con copiar el archivo
que corresponda encima del propio, en la misma carpeta. Conviene guardar antes
una copia del propio, para comparar las dos versiones.

Para verificar, compilar con `make clean && make` y correr `./bin/rrt`: la imagen
`escena-00.png` tiene que mostrar el tablero rojo y amarillo, las dos esferas
con su brillo y sus sombras sobre el piso. El `make clean` hace falta porque un
archivo copiado puede conservar una fecha anterior a la de lo ya compilado, y en
ese caso `make` no lo recompila.

Este es el punto de partida del taller de iluminación global, que agrega a
este proyecto la reflexión, la refracción y el trazado recursivo.
