# Trazador de rayos: solución del taller de iluminación global

Se incluyen los tres archivos con el código de los cuatro pasos resueltos. El resto
del código no cambia respecto del proyecto del taller de iluminación local con el
paquete de este taller ya aplicado.

| Archivo | Pasos |
|---|---|
| `src/rt/scene.cpp` | 7. Trazado recursivo, 10. Sombra a través del vidrio |
| `src/rt/mirror.h` | 8. Espejo |
| `src/rt/dielectric.cpp` | 9. Vidrio |

## Cómo usarla

Para completar lo que haya quedado sin terminar, alcanza con copiar el archivo
que corresponda encima del propio, en la misma carpeta. Conviene guardar antes
una copia del propio, para comparar las dos versiones.

`scene.cpp` contiene también los pasos 3, 4 y 5 del taller de iluminación local,
por lo que al copiarlo se reemplaza también esa parte. `mirror.h` y `dielectric.cpp`
se pueden copiar por separado, siempre que el paso 7 esté hecho: hasta ese momento
el proyecto no compila.

Para verificar, compilar con `make clean && make` y correr `./bin/rrt 11`: la imagen
`escena-11.png` tiene que mostrar la esfera de vidrio hueca con el tablero visto a
través de ella, la esfera espejo que refleja el piso y el cielo, y la sombra de la
esfera de vidrio más clara que la del espejo. El `make clean` hace falta porque un
archivo copiado puede conservar una fecha anterior a la de lo ya compilado, y en
ese caso `make` no lo recompila.

Por defecto la imagen sale a 800 x 600 y con un rayo por píxel. Con
`./bin/rrt 11 -r 1280 720 -ss 3` se obtiene una imagen de mayor calidad, a cambio de
un tiempo de cálculo unas diez veces mayor.
