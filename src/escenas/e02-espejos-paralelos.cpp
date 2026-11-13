#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>
#include <checker.h>
#include <mirror.h>

using glm::vec3;
using std::make_shared;

void build_scene_02(Scene &scene, Camera &camera)
{
    camera.look_at(vec3(2.0f, 1.6f, 2.5f),   // ojo
                   vec3(-6.0f, 0.7f, -1.5f), // hacia donde mira, vendria a ser un punto, coordenadas exactas donde queremos centrar
                   vec3(0.0f, 1.0f, 0.0f), // arriba, esto si vendria a ser una felcha
                   55.0f);                 // fov vertical

    // definimos bg color
    vec3 background_color = vec3(0.02f, 0.02f, 0.03f);
    scene.set_background(background_color);

    // piso
    auto floor = make_shared<Checker>();
    
    vec3 posicion_piso = vec3(0.0f);           // posicion del piso
    vec3 normal_piso = vec3(0.0f, 1.0f, 0.0f); // normal del piso

    floor->size = 0.5f;
    floor->kd = vec3(0.85f, 0.85f, 0.85f);
    floor->ka = vec3(0.05f, 0.05f, 0.05f);
    floor->kd2 = vec3(0.1f, 0.1f, 0.1f); // amarillo
    floor->ka2 = vec3(0.006f, 0.006f, 0.006f);
    /*floor->ks   = vec3(0.0f);
    floor->shininess = 1.0f;*/

    scene.add_object(make_shared<Plane>(posicion_piso, normal_piso, floor));

    // espejos
    auto espejo = make_shared<Mirror>();
    vec3 posicion_espejo = vec3(-2.5f, 0.0f, 0.0f);
    vec3 normal_espejo = vec3(1.0f, 0.0f, 0.0f);

    vec3 ka_espejo = vec3(0.00f);
    vec3 kd_espejo = vec3(0.0f);
    vec3 ks_espejo = vec3(0.0f);

    espejo->ka = ka_espejo;
    espejo->kd = kd_espejo;
    espejo->ks = ks_espejo;
    espejo->shininess = 100.0f;
    espejo->kr = 0.85f;

    scene.add_object(make_shared<Plane>(posicion_espejo, normal_espejo, espejo));
    scene.add_object(make_shared<Plane>(posicion_espejo * -1.0f, normal_espejo * -1.0f, espejo));

    // --------------------------------

    // esfera
    auto white = make_shared<Diffuse>();
    vec3 ka_esfera = vec3(0.05f, 0.02f, 0.0f);
    vec3 kd_esfera = vec3(0.9f, 0.45f, 0.05f);
    vec3 ks_esfera = vec3(0.5f, 0.5f, 0.5f);
    float shininess_esfera = 50.0f;

    vec3 centro_esfera = vec3(0.0f, 0.7f, -1.0f);
    float radio_esfera = 0.7f;

    white->ka = ka_esfera;
    white->kd = kd_esfera;
    white->ks = ks_esfera;
    white->shininess = shininess_esfera;

    scene.add_object(make_shared<Sphere>(centro_esfera, radio_esfera, white));

    // luces
    const vec3 att(1.0f, 0.12f, 0.02f); // para fattr

    vec3 color_luz_1 = vec3(3.5f, 3.5f, 3.5f); // luz roja
    vec3 posicion_luz_1 = vec3(0.0f, 5.0f, 2.0f);
   
    scene.add_light(Light(posicion_luz_1, color_luz_1, att));
    scene.set_max_depth(8U); // para recursion
}