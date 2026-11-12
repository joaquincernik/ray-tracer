#include <memory>
#include <escenas.h>
#include <plane.h>
#include <sphere.h>

using glm::vec3;
using std::make_shared;

void build_scene_01(Scene &scene, Camera &camera)
{
    camera.look_at(vec3(0.0f, 6.0f, 7.0f), // ojo
                   vec3(0.0f, 0.3f, 0.0f), // hacia donde mira, vendria a ser un punto, coordenadas exactas donde queremos centrar
                   vec3(0.0f, 1.0f, 0.0f), // arriba, esto si vendria a ser una felcha
                   45.0f);                      // fov vertical

    //definimos bg color
    vec3 background_color = vec3(0.02f);
    scene.set_background(background_color);

    //piso 
    auto floor = make_shared<Diffuse>();
    vec3 ka_piso = vec3(0.02f); // ka ambiental
    vec3 kd_piso = vec3(0.8f); // coeficiente difuso
    vec3 posicion_piso = vec3(0.0f); // posicion del piso
    vec3 normal_piso = vec3(0.0f, 1.0f, 0.0f); // normal del piso
    
    floor->ka = ka_piso; // ka ambiental
    floor->kd = kd_piso; // coeficiente difuso, esta en rgb, piso vendria a ser blanco que refleja el 80 por ciento de la luz que le llega
    scene.add_object(make_shared<Plane>(posicion_piso, normal_piso, floor));


    //esfera
    auto white = make_shared<Diffuse>();
    vec3 ka_esfera = vec3(0.02f);
    vec3 kd_esfera = vec3(0.8f);
    vec3 ks_esfera = vec3(0.3f);
    float shininess_esfera = 30.0f;
    
    vec3 centro_esfera = vec3(0.0f, 1.0f, 0.0f);
    float radio_esfera = 1.0f;
    
    white->ka = ka_esfera;
    white->kd = kd_esfera;
    white->ks = ks_esfera;
    white->shininess = shininess_esfera;
    
    scene.add_object(make_shared<Sphere>(centro_esfera, radio_esfera, white));
    
    //luces
    const vec3 att(1.0f, 0.12f, 0.02f); // para fattr

    vec3 color_luz_1 = vec3(2.4f, 0.0f, 0.0f); // luz roja
    vec3 color_luz_2 = vec3(0.0f, 2.4f, 0.0f); // luz verde
    vec3 color_luz_3 = vec3(0.0f, 0.0f, 2.4f); // luz azul

    vec3 posicion_luz_1 = vec3(0.0f, 5.0f, -4.0f);
    vec3 posicion_luz_2 = vec3(-3.464f, 5.0f, 2.0f);
    vec3 posicion_luz_3 = vec3(3.464f, 5.0f, 2.0f);

    scene.add_light(Light(posicion_luz_1, color_luz_1, att));
    scene.add_light(Light(posicion_luz_2, color_luz_2, att));
    scene.add_light(Light(posicion_luz_3, color_luz_3, att));
    
    scene.set_max_depth(8U); //para recursion
}