# Practica

## Escena 1
![alt text](practica-escena-1.png)

Donde sólo se bloquea la luz roja, la sombra es cian (verde más azul); donde sólo se bloquea la verde,
magenta; y donde sólo se bloquea la azul, amarilla (recordar Unidad III - Teoría del color). Fuera de
las sombras el piso recibe las tres luces, pero cada una se atenúa con la distancia, por lo que su color
tiende al de la luz más cercana. Debajo de la esfera, donde se bloquean las tres, queda sólo el término
ambiental.
Si en el bucle de luces el punto en sombra termina el cálculo (break en lugar de continue), la sombra
de la primera luz de la lista sale negra, la de la segunda queda del color de la primera y la de la tercera
no cambia.

### Preguntas

1. ¿De qué color es la zona donde se bloquean a la vez la luz roja y la verde?

Bloquear significa que no llega ninguno de los colores, por lo tanto abajo a la derecha veo que el color es `azul`

### Break en lugar de continue
Veamos que pasa si hacemos eso de continue
![alt text](image.png)


La diferencia ocurre porque continue solo saltea la luz actual que está tapada y sigue revisando las demás, mientras que break corta todo el ciclo for por completo, ignorando las luces que quedan más adelante en la lista



