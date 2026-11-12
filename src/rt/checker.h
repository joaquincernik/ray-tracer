#ifndef CHECKER_H
#define CHECKER_H

#include <cmath>

#include <material.h>
using std::floor;

/// Difuso con tablero de ajedrez en el plano xz: las casillas "impares" cambian
/// los coeficientes ambiental y difuso para poder alternar los colores del
/// tablero. Casillas impares -> ka2 y kd2 en lugar de ka y kd (heredados de la
/// clase base)
class Checker : public Diffuse {
public:
    glm::vec3 ka2 = glm::vec3(0.02f);
    glm::vec3 kd2 = glm::vec3(0.05f);
    float size = 1.0f;                  ///< lado de cada casilla

    glm::vec3 ka_at(const glm::vec3& p) const override { return odd(p) ? ka2 : ka; }
    glm::vec3 kd_at(const glm::vec3& p) const override { return odd(p) ? kd2 : kd; }

private:
    /// True si p cae en una casilla impar
    bool odd(const glm::vec3& p) const {

        // floor y no un cast a int, que trunca hacia cero. Sólo x y z: en el
        // plano y = 0, floor(y) salta entre -1 y 0 por el redondeo
        float scaled_x = p.x / size;
        float scaled_z = p.z / size;

        const int ix = int(floor(scaled_x));
        const int iz = int(floor(scaled_z));

        bool impar =  ((ix + iz) & 1) != 0;
        return impar; // lo mismo que (ix + iz) % 2 != 0, pero más rápido
    }
};

#endif // CHECKER_H
