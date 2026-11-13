#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <dielectric.h>

using glm::vec3;
using std::make_shared;

namespace
{
    // ========================
    // CREADORES DE PLANOS
    // ========================

    // Plano Diffuse
    void crear_plano(Scene &scene, const vec3 &posicion, const vec3 &normal,
                     const vec3 &ka, const vec3 &kd,
                     const vec3 &ks = vec3(0.0f), float shininess = 0.0f)
    {
        auto mat = make_shared<Diffuse>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        scene.add_object(make_shared<Plane>(posicion, normal, mat));
    }

    // Plano Checker (tablero)
    void crear_plano(Scene &scene, const vec3 &posicion, const vec3 &normal,
                     float size, const vec3 &ka, const vec3 &kd,
                     const vec3 &ka2, const vec3 &kd2)
    {
        auto mat = make_shared<Checker>();
        mat->size = size;
        mat->ka = ka;
        mat->kd = kd;
        mat->ka2 = ka2;
        mat->kd2 = kd2;
        scene.add_object(make_shared<Plane>(posicion, normal, mat));
    }

    // Plano Mirror
    void crear_plano(Scene &scene, const vec3 &posicion, const vec3 &normal,
                     const vec3 &ka, const vec3 &kd, const vec3 &ks,
                     float shininess, float kr)
    {
        auto mat = make_shared<Mirror>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        mat->kr = kr;
        scene.add_object(make_shared<Plane>(posicion, normal, mat));
    }

    // Plano Dielectric (útil para el agua de la Escena 7)
    void crear_plano(Scene &scene, const vec3 &posicion, const vec3 &normal,
                     float ior, float transparency,
                     const vec3 &ks = vec3(0.0f), float shininess = 0.0f)
    {
        auto mat = make_shared<Dielectric>();
        mat->ior = ior;
        mat->transparency = transparency;
        mat->ks = ks;
        mat->shininess = shininess;
        scene.add_object(make_shared<Plane>(posicion, normal, mat));
    }

    // ========================
    // CREADORES DE ESFERAS
    // ========================

    // Esfera Diffuse
    void crear_esfera(Scene &scene, const vec3 &centro, float radio,
                      const vec3 &ka, const vec3 &kd,
                      const vec3 &ks = vec3(0.0f), float shininess = 0.0f)
    {
        auto mat = make_shared<Diffuse>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        scene.add_object(make_shared<Sphere>(centro, radio, mat));
    }

    // Esfera Mirror
    void crear_esfera(Scene &scene, const vec3 &centro, float radio,
                      const vec3 &ka, const vec3 &kd, const vec3 &ks,
                      float shininess, float kr)
    {
        auto mat = make_shared<Mirror>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        mat->kr = kr;
        scene.add_object(make_shared<Sphere>(centro, radio, mat));
    }

    // Esfera Dielectric (útil para las Escenas 4, 5, 6 y 8)
    void crear_esfera(Scene &scene, const vec3 &centro, float radio,
                      float ior, float transparency,
                      const vec3 &ks = vec3(0.0f), float shininess = 0.0f)
    {
        auto mat = make_shared<Dielectric>();
        mat->ior = ior;
        mat->transparency = transparency;
        mat->ks = ks;
        mat->shininess = shininess;
        scene.add_object(make_shared<Sphere>(centro, radio, mat));
    }
}

void build_scene_02(Scene &scene, Camera &camera)
{
    camera.look_at(vec3(2.0f, 1.6f, 2.5f),   // ojo
                   vec3(-6.0f, 0.7f, -1.5f), // hacia donde mira
                   vec3(0.0f, 1.0f, 0.0f),   // arriba
                   55.0f);                   // fov vertical

    scene.set_background(vec3(0.02f, 0.02f, 0.03f));

    // Piso Checker
    crear_plano(scene,
                vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), // posicion y normal
                0.5f,                               // size
                vec3(0.05f), vec3(0.85f),           // ka, kd
                vec3(0.006f), vec3(0.1f));          // ka2, kd2

    // Espejos paralelos
    vec3 posicion_espejo = vec3(-2.5f, 0.0f, 0.0f);
    vec3 normal_espejo = vec3(1.0f, 0.0f, 0.0f);

    crear_plano(scene, posicion_espejo, normal_espejo,
                vec3(0.0f), vec3(0.0f), vec3(0.0f), 100.0f, 0.85f);
    crear_plano(scene, posicion_espejo * -1.0f, normal_espejo * -1.0f,
                vec3(0.0f), vec3(0.0f), vec3(0.0f), 100.0f, 0.85f);

    // Esfera naranja (Diffuse)
    crear_esfera(scene,
                 vec3(0.0f, 0.7f, -1.0f), 0.7f, // centro y radio
                 vec3(0.05f, 0.02f, 0.0f),      // ka
                 vec3(0.9f, 0.45f, 0.05f),      // kd
                 vec3(0.5f), 50.0f);            // ks, shininess

    // Luces
    const vec3 att(1.0f, 0.12f, 0.02f);
    scene.add_light(Light(vec3(0.0f, 5.0f, 2.0f), vec3(3.5f, 3.5f, 3.5f), att));
    scene.set_max_depth(8U);
}