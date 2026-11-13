#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>
#include <dielectric.h>
#include <triangle.h>

using glm::vec3;
using std::make_shared;
using std::shared_ptr;
namespace
{
    // ========================
    // CREADORES DE PLANOS
    // ========================

    /*
    // Helper interno para no repetir los vértices en cada sobrecarga
    void agregar_triangulos_pared(Scene &scene, float x, const std::shared_ptr<Material> &mat)
    {
        const vec3 a(x, 0.0f,  14.0f);
        const vec3 b(x, 0.0f, -12.0f);
        const vec3 c(x, 5.0f, -12.0f);
        const vec3 d(x, 5.0f,  14.0f);

        scene.add_object(make_shared<Triangle>(a, b, c, mat));
        scene.add_object(make_shared<Triangle>(a, c, d, mat));
    }

    // Pared Diffuse
    void crear_pared(Scene &scene, float x,
                     const vec3 &ka, const vec3 &kd,
                     const vec3 &ks = vec3(0.0f), float shininess = 0.0f)
    {
        auto mat = make_shared<Diffuse>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        agregar_triangulos_pared(scene, x, mat);
    }

    // Pared Mirror (el compilador la elige automáticamente al pasar kr)
    void crear_pared(Scene &scene, float x,
                     const vec3 &ka, const vec3 &kd, const vec3 &ks,
                     float shininess, float kr)
    {
        auto mat = make_shared<Mirror>();
        mat->ka = ka;
        mat->kd = kd;
        mat->ks = ks;
        mat->shininess = shininess;
        mat->kr = kr;
        agregar_triangulos_pared(scene, x, mat);
    }
}
    */
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

    void crear_pared(Scene &scene, float x, const shared_ptr<Material> &material)
    {
        const vec3 a(x, 0.0f, 14.0f);
        const vec3 b(x, 0.0f, -12.0f);
        const vec3 c(x, 5.0f, -12.0f);
        const vec3 d(x, 5.0f, 14.0f);

        // Rectángulo dividido en dos triángulos: a-b-c y a-c-d
        scene.add_object(make_shared<Triangle>(a, b, c, material));
        scene.add_object(make_shared<Triangle>(a, c, d, material));
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

void build_scene_03(Scene &scene, Camera &camera)
{
    camera.look_at(vec3(-2.0f, 2.0f, 7.5f), // ojo
                   vec3(3.0f, 1.4f, 0.0f),  // hacia donde mira
                   vec3(0.0f, 1.0f, 0.0f),  // arriba
                   45.0f);                  // fov vertical

    scene.set_background(vec3(0.02f, 0.03f, 0.08f));

    // Piso Checker
    crear_plano(scene,
                vec3(0.0f), vec3(0.0f, 1.0f, 0.0f), // posicion y normal
                1.0f,                                 // size
                vec3(0.07f), vec3(0.7f),            // ka, kd
                vec3(0.1f), vec3(0.1f));            // ka2, kd2

    // Espejos paralelos
    // Paredes espejo (x = -3 y x = 3)
    auto pared_espejo = make_shared<Mirror>();
    pared_espejo->ka = vec3(0.0f);
    pared_espejo->kd = vec3(0.02f);
    pared_espejo->ks = vec3(0.0f);
    pared_espejo->kr = 0.9f;

    crear_pared(scene, -3.0f, pared_espejo);
    crear_pared(scene, 3.0f, pared_espejo);

    // Esfera roja (Diffuse)
    crear_esfera(scene,
                 vec3(0.2f, 1.0f, 0.0f), 1.0f, // centro y radio
                 vec3(0.08f, 0.01f, 0.01f),    // ka
                 vec3(0.75f, 0.08f, 0.06f),    // kd
                 vec3(0.5f), 60.0f);           // ks, shininess

    // Esfera verde (Diffuse)
    crear_esfera(scene,
                 vec3(1.2f, 0.6f, 2.5f), 0.6f, // centro y radio
                 vec3(0.01f, 0.06f, 0.02f),       // ka
                 vec3(0.1f, 0.6f, 0.2f),       // kd
                 vec3(0.4f), 40.0f);           // ks, shininess

    // Esfera espejo
    crear_esfera(scene,
                 vec3(-1.3f, 0.9f, -2.5f), 0.9f, // centro y radio
                 vec3(0.02f),  // ka
                 vec3(0.1f),          // kd
                 vec3(0.1f), 150.0f, // ks, shininess
                 0.85f);              // kr

    // Luces
    const vec3 att(1.0f, 0.12f, 0.02f);
    scene.add_light(Light(vec3(0.0f, 9.0f, 5.0f), vec3(4.0f), att));
    scene.set_max_depth(8U);
}