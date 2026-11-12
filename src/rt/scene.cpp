#include <scene.h>
using glm::dot;
using glm::length;
using glm::max;
using glm::normalize;
using glm::pow;
using glm::reflect;
using glm::vec3;

bool Scene::find_nearest(const Ray &r, Hit &closest) const
{
    bool any = false;
    Hit current;

    for (const auto &obj : objects_)
    {
        if (obj->intersect(r, current) && current.t < closest.t)
        {
            closest = current;
            any = true;
        }
    }
    return any;
}

glm::vec3 Scene::background(const Ray &r) const
{
    const float up = glm::max(glm::normalize(r.d()).y, 0.0f);
    return glm::mix(bg_horizon_, bg_zenith_, up);
}

glm::vec3 Scene::normal_color(const Ray &r) const
{
    Hit h; // instanciamos un rayo con t en el infinito
    if (!find_nearest(r, h))
    { // me devuelve el objeto mas cercano al rayo, si no hay intersección devuelve false
        return background(r);
    }
    // De [-1,1] a [0,1], componente a componente
    return 0.5f * (h.n + glm::vec3(1.0f));
}

LocalTerms Scene::local_terms(const Ray &r, const Hit &h) const
{
    LocalTerms terms;
    const Material &m = *h.mat;

    const glm::vec3 v = glm::normalize(-r.d()); // hacia el observador
    const glm::vec3 &n = h.n;                   // ya orientada hacia v

    const vec3 punto_interseccion = h.p; // punto de intersección

    terms.ambient = m.ka_at(punto_interseccion);

    for (const auto &L : lights_)
    {
        const vec3 punto_fuente = L.pos();          // posición de la fuente
        vec3 s = punto_fuente - punto_interseccion; // hacia la luz
        const float dist = length(s);
        s /= dist; // normalizzamos elegantemente el vector s para que tenga longitud 1

        // Sombra: ¿qué fracción de la luz llega hasta el punto? (paso 5)
        const float visibility = light_transmission(punto_interseccion, n, punto_fuente);
        if (visibility <= 0.0f)
        {
            continue;
        }

        // Difuso
        const float n_dot_s = max(dot(n, s), 0.0f); // al ser normales el producto punto equivale a hacer el coseno del ángulo entre ambos vectores, si es negativo significa que la luz está detrás de la superficie y no hay difuso
        // si se encuentra en el rango de 0 a 1, significa que la luz está delante de la superficie y hay difuso

        // Especular, Phong (sin brillo si la luz está detrás de la superficie)
        float k_spec = 0.0f;
        if (n_dot_s > 0.0f)
        {
            const vec3 refl = reflect(-s, n); // la lrey de reflexion esa que aparece un 2, direccion negativa de s ya que va hacia la luz, y n es la normal de la superficie
            const float ns = m.shininess;
            const float v_dot_r = max(dot(refl, v), 0.0f);
            k_spec = pow(v_dot_r, ns);
        }

        // Intensidad que llega del foco
        const vec3 Li = L.color() * (L.attenuation(dist) * visibility);

        terms.diffuse += m.kd_at(h.p) * n_dot_s * Li;
        terms.specular += m.ks * k_spec * Li;
    }

    return terms;
}

float Scene::light_transmission(const glm::vec3 &p, const glm::vec3 &n,
                                const glm::vec3 &light_pos) const
{
    constexpr int kMaxCrossings = 16;
    vec3 origin = p + n * EPSILON;
    float transmission = 1.0f;

    // El rayo de sombra avanza hacia la luz; cada superficie que cruza deja
    // pasar shadow_transmission() de la luz (0 si es opaca)
    for (int i = 0; i < kMaxCrossings; ++i)
    {
        const vec3 to_light = light_pos - origin;
        const float dist = length(to_light);

        const vec3 s_normalized = to_light / dist; // normalizamos el vector que va desde el punto de intersección hasta la luz
        // En sombra si algún objeto está entre el punto y la luz. Un objeto que
        // está más allá de la luz no cuenta
        Hit h;
        const bool found = find_nearest(Ray(origin, s_normalized), h);
        const bool objeto_detras_foco = h.t >= dist - EPSILON; // si el objeto está detrás de la luz, no cuenta
        if (!found || objeto_detras_foco)
        {
            return transmission;
        }
        transmission *= h.mat->shadow_transmission(); // si deja pasar algo de luz
        if (transmission <= 0.0f) // sigue hasta que choca con algo opaco
        {
            return 0.0f;
        }
        origin = h.p + s_normalized * EPSILON; // el rayo sale ahora de dentro de la esfera, es el rayo defractado
    }
    return transmission;
}

glm::vec3 Scene::trace(const Ray &r, unsigned int depth) const
{
     // Rama truncada: no aporta luz
    if( depth > max_depth_ ){
        return glm::vec3(0.0f);
    }

    Hit h;
    if( !find_nearest(r, h) ){
        return background(r);
    }
    const Material& m = *h.mat;

    // I = I_local + suma de (peso_i * I(rayo_i))
    //
    // La escena calcula los términos de Phong con sombras (local_terms) y el
    // material elige cuáles usa. La llamada es virtual: trace no necesita
    // saber qué material tiene el objeto
    glm::vec3 color = m.local(*this, r, h);

    Secondary sec[2];
    const int count = m.secondary(r, h, sec);
    for( int i = 0; i < count; ++i ){
        color += sec[i].weight * trace(sec[i].ray, depth + 1U);
    }

    // Sin recortar a [0,1]: el recorte va al final, en el píxel
    return color;
}
