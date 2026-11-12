# Trazador de rayos: paquete del taller de iluminación global

Archivos que hay que agregar al proyecto del taller de iluminación local para
ampliar el trazador con reflexión, refracción y trazado recursivo de rayos, que
permiten generar la imagen final *Spheres and Checkerboard* de Whitted (escena
11). Estos archivos no son un proyecto nuevo: hay que agregarlos / sobreescribir
sobre el proyecto del taller de iluminación local.

El punto de partida es el proyecto del taller 1 de iluminación local con los
seis pasos funcionando (`./bin/rrt` escribe el tablero con las dos esferas y
sus sombras). Si alguno está sin terminar se puede completar usando el archivo
de la solución que se publicó para ese taller.

## Cómo aplicar el paquete

Copiar la carpeta `src/` del paquete encima de la `src/` del proyecto. Se van a
presentar dos situaciones frente a los archivos que se están combinando:

**Archivos que se reemplazan.** En el taller de iluminación local no tenían
nada para completar, así que reemplazarlos no borra trabajo propio. Si alguno
se modificó, hay que copiar a mano sólo las partes nuevas.

| Archivo | Qué agrega |
|---|---|
| `src/rt/material.h` | El rayo secundario con su peso (`Secondary`) y dos métodos con su versión por defecto, la del material difuso y opaco: `secondary()` (no genera rayos) y `shadow_transmission()` (hace sombra plena) |
| `src/rt/scene.h` | La profundidad de recursión: el parámetro `depth` de `trace` y el tope `set_max_depth()` (8 por defecto) |
| `src/escenas/escenas.h`, `escenas.cpp` | La escena 11 en la tabla de escenas |

**Archivos nuevos.**

| Archivo | Contenido |
|---|---|
| `src/rt/triangle.h` | Intersección rayo-triángulo. Se necesita para el piso de la escena 11 que es un rectángulo finito armado con dos triángulos |
| `src/rt/mirror.h` | Material tipo especular con un espacio para completar (paso 8) |
| `src/rt/dielectric.h`, `dielectric.cpp` | Material dieléctrico (vidrio), con un espacio para completar (paso 9). Incluye la aproximación de Schlick, lista para usar |
| `src/escenas/e11-whitted.cpp` | El modelo completo de la escena 11 *Spheres and Checkerboard* de Whitted |

Después de copiar, `make clean && make` da este error:

```
error: no declaration matches 'glm::vec3 Scene::trace(const Ray&) const'
```

Es lo esperado que suceda porque ahora `scene.h` declara el método `trace` con 
la profundidad de recursión y la definición en `scene.cpp` no la tiene. Se 
resuelve en el paso 7.

## Actividades para el taller: cuatro pasos

La numeración continúa la del taller de iluminación local. Los pasos 8 y 9 están
marcados en el código con `TODO (paso N)`. Los pasos 7 y 10 no tienen marca porque
hay que escribirlos en la versión que ya existe de `scene.cpp` y por eso no
vienen en el paquete: más abajo se describen los cambios para estos pasos.

| Paso | Dónde | Qué se ve en la escena 11 al terminarlo |
|---|---|---|
| 7. Trazado recursivo | `Scene::trace` (`scene.cpp`) | Compila otra vez. La esfera de adelante sale negra con un brillo, la de atrás gris mate y las dos hacen sombra plena. La escena 0 no cambia |
| 8. Espejo | `Mirror::secondary` (`mirror.h`) | La esfera de atrás refleja el piso, el cielo y la esfera negra |
| 9. Vidrio | `Dielectric::secondary` (`dielectric.cpp`) | A través de la esfera de adelante se ve lo que está detrás, invertido y deformado, y en el borde se refleja el entorno |
| 10. Sombra a través del vidrio | `Scene::light_transmission` (`scene.cpp`) | La sombra de la esfera de vidrio se aclara |

### Paso 7: trazado recursivo

El modelo de Whitted suma a la iluminación local la luz que traen los rayos
secundarios, cada uno con su peso:

    I = I_local + suma de (peso_i * I(rayo_i))

y cada I(rayo_i) se calcula usando de nuevo `trace` y ahí es donde aparece la
recursión. En `scene.cpp` hay que:

1. Agregar el parámetro a la definición:
   `glm::vec3 Scene::trace(const Ray& r, unsigned int depth) const`
   (el valor por defecto, 0, va sólo en `scene.h`).
2. Al principio de `trace`, si el parámetro `depth` supera `max_depth_`, 
   devolver color negro: la rama del árbol de rayos termina y no aporta luz.
3. Después de la iluminación local, pedirle al material sus rayos
   secundarios (`h.mat->secondary(r, h, sec)`, con `Secondary sec[2]`, que
   devuelve cuántos escribió) y sumar al color, el color que trae cada uno
   multiplicado por su peso: `trace(sec[i].ray, depth + 1)`.

No hace falta recortar el color a [0,1] dentro de la recursión porque `main` lo
recorta al final, en cada muestra del píxel.

Con el material difuso no cambia nada, porque no genera rayos secundarios. La
recursión se ve recién con el espejo (paso 8).

### Paso 10: sombra a través del vidrio

Hasta acá `light_transmission` devuelve 0 cuando el rayo de sombra toca cualquier
objeto. Al agregar el material dieléctrico (vidrio) en la escena, la sombra 
plena que genera es incorrecta y se puede mejorar porque un dieléctrico puede 
dejar pasar parte de la luz.

La forma propuesta es que el rayo de sombra siga de largo. Cada vez que toca
una superficie, la fracción de luz que llega se multiplica por la que deja
pasar esa superficie (`h.mat->shadow_transmission()`: 0 para un material opaco,
el valor de `transparency` para los dieléctricos) y el rayo vuelve a salir un poco
más allá del punto de intersección (`h.p + s * EPSILON`, con `s` la dirección 
hacia la luz). El recorrido de este rayo termina al llegar a la luz o cuando la
fracción llega a 0. Conviene ponerle un tope de cruces (16, por ejemplo) por si
el rayo queda atrapado dentro de un objeto.

En una esfera de vidrio son dos cruces (entrada y salida) y en la esfera de la
escena de Whitted, como es hueca, son cuatro cruces.

## La esfera hueca de la escena 11

La esfera de vidrio de Whitted es hueca. En la escena se arma con dos esferas
concéntricas: la exterior con índice 1,5 (de aire a vidrio) y la interior,
de radio 0,96 veces el exterior, con índice 1/1,5, porque del lado de adentro
de esa superficie hay aire rodeado de vidrio. La superficie interior no tiene
brillo de Phong porque la luz le llega refractada por la cáscara y haría 
aparecer brillo en un lugar equivocado que no concuerda con la física.