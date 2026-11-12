#ifndef SPHERE_H
#define SPHERE_H

#include <object.h>

using glm::dot;
using glm::normalize;
using glm::sqrt;
using glm::vec3;

class Sphere : public Object
{
public:
    Sphere(const glm::vec3 &center, float radius, std::shared_ptr<Material> m) : Object(std::move(m)), c_(center), r_(radius) {}

    // Función auxiliar para resolver la ecuación cuadrática (Báscara)
    inline bool resolver_cuadratica(float a, float b, float cc, float &t0, float &t1) const
    {
        const float disc = b * b - 4.0f * a * cc;
        if (disc < 0.0f)
            return false;

        const float sq = glm::sqrt(disc);
        t0 = (-b - sq) / (2.0f * a);
        t1 = (-b + sq) / (2.0f * a);
        return true;
    }

    bool intersect(const Ray &r, Hit &out) const override
    {
        // TODO (paso 2): intersección rayo-esfera.
        // Un punto P está en la esfera si |P - c|^2 = r^2. Al reemplazar P por
        // el rayo, P(t) = o + t d: queda una cuadrática a t^2 + b t + cc = 0
        // (cc y no c, para no confundir el término independiente con el centro).
        //   - Discriminante negativo: el rayo no toca la esfera.
        //   - De las dos raíces, quedarse con la más chica que sea mayor que
        //     EPSILON. Si el origen del rayo está adentro de la esfera, la
        //     primera raíz es negativa y la segunda es la intersección que se
        //     ve desde dentro de la esfera.
        //   - Si ninguna es mayor que EPSILON, la esfera está detrás.
        //   - Completar out como en Plane. La normal hacia afuera es
        //     (P - c) normalizada.

        // |o + t d - c|^2 = r^2   ->   a t^2 + b t + cc = 0   (c es el centro)
        const vec3 origen_rayo = r.o();
        const vec3 direccion_rayo = r.d();
        const vec3 oc = origen_rayo - c_; // rayo que va desde origen del rayo hasta el centro de la esfera

        const float a = dot(direccion_rayo, direccion_rayo);
        const float b = 2.0f * dot(oc, direccion_rayo);
        const float cc = dot(oc, oc) - r_ * r_;

        // Resolvemos las raíces t0 y t1
        float t0, t1;
        if (!resolver_cuadratica(a, b, cc, t0, t1))
        {
            return false; // Discriminante negativo: el rayo no toca la esfera
        }

        // La raíz más chica que esté delante del origen
        const float t = (t0 > EPSILON) ? t0 : ((t1 > EPSILON) ? t1 : INF);

        // t0 > EPSILON: la primera raíz es la intersección que se ve desde fuera de la esfera
        // t1 > EPSILON: ESTAMOS DENTRO DE LA ESFERA
        if (t == INF) // ninguna raíz es mayor que EPSILON, la esfera está detrás
            return false;

        out.t = t;
        out.p = r.at(t);
        const vec3 normal_esfera = normalize(out.p - c_); // rayo que va desde el centro de la esfera hasta el punto de intersección
        out.mat = mat_.get();
        out.set_face(r, normal_esfera);
        return true;
    }

private:
    glm::vec3 c_;
    float r_;
};

#endif // SPHERE_H
