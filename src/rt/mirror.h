#ifndef MIRROR_H
#define MIRROR_H

#include <material.h>
using glm::normalize;
using glm::reflect;
using glm::vec3;
/// Espejo o metal opaco: Phong completo más un rayo reflejado de peso kr
/// constante.  I = I_local + kr * I(R)
/// El brillo de Phong sigue haciendo falta: una luz puntual no es geometría, el
/// rayo reflejado nunca la encuentra
class Mirror : public Material
{
public:
    float kr = 0.8f;

    int secondary(const Ray &in, const Hit &h, Secondary out[2]) const override
    {
        // El origen se corre un poco hacia afuera para no chocar con la propia
        // superficie por redondeo

        // arreglo puede devolver 1 o 2 rayos, pero en este caso solo devuelve el reflejado
        // por eso se lo mandamos en la posicion 0
        vec3 origen_rayo_reflectado = h.p + h.n * EPSILON; // esto es de sacarlo un toque pa fuera
        vec3 direccion_rayo_reflectado = reflect(normalize(in.d()), h.n);
        out[0].ray = Ray(origen_rayo_reflectado, direccion_rayo_reflectado);
        out[0].weight = kr;
        return 1;
    }
};

#endif // MIRROR_H
