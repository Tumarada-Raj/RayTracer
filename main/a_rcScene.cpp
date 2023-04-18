#include <core/image.h>
#include <core/color.h>
#include <core/random.h>
#include <stdio.h>
#include <core/float4.h>

#include <rt/world.h>   
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/integrators/recraytrace.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/casting.h>

#include <rt/groups/simplegroup.h>
#include <rt/groups/bvh.h>

#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/solids/quad.h>
#include <rt/solids/sphere.h>
#include <rt/solids/triangle.h>
#include <rt/solids/disc.h>
#include <rt/solids/environmentSolid.h>

#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/textures/perlin.h>
#include <rt/coordmappers/environmentMapper.h>

#include <rt/materials/flatmaterial.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/fuzzymirror.h>
#include <rt/materials/glass.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>
#include <rt/materials/cookTorrance.h>
#include <rt/materials/mirror.h>
#include <rt/materials/dummy.h>

#include <rt/lights/arealight.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>
#include <rt/lights/directional.h>

#include <algorithm>
#include <chrono>

using namespace rt;

MatLib* getManMatlib() {
    /*MatLib* matlib = new MatLib;
    Texture* black = new ConstantTexture(RGBColor::rep(0.0f));

    ImageTexture* tyres = new ImageTexture("models/Material.008_baseColor.png");
    ImageTexture* body1 = new ImageTexture("models/Material.001_baseColor.jpeg");
    ImageTexture* interior = new ImageTexture("models/Material.002_baseColor.jpeg");

    matlib->insert(std::pair<std::string, Material*>("Material.003", new FlatMaterial(tyres)));
    matlib->insert(std::pair<std::string, Material*>("Material.008", new FlatMaterial(body1)));
    matlib->insert(std::pair<std::string, Material*>("Material.009", new FlatMaterial(interior)));

    return matlib;*/
    MatLib* matlib = new MatLib;
    ImageTexture* body = new ImageTexture("models/blinn1_baseColor.jpeg");
    matlib->insert(std::pair<std::string, Material*>("blinn1.001", new FlatMaterial(body)));
    return matlib;
}

MatLib* mirrorCeil_matlib() {
    MatLib* matlib = new MatLib;
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    Texture* greytex = new ConstantTexture(RGBColor(0.376f, 0.337f, 0.373f));
    Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));

    CombineMaterial* frame = new CombineMaterial();
    frame->add(new LambertianMaterial(greytex, blacktex), 0.27);
    frame->add(new MirrorMaterial(1.45f, 1.9f), 0.33);
    frame->add(new PhongMaterial(whitetex, 100.0), 0.4);

    //Material* frame = new LambertianMaterial(greytex, blacktex);

    matlib->insert(std::pair<std::string, Material*>("Material", frame));
    //matlib->insert(std::pair<std::string, Material*>("Material", new PhongMaterial(new ConstantTexture(RGBColor::rep(1.f)), 50.0f))); //play around
    matlib->insert(std::pair<std::string, Material*>("Material.009", new MirrorMaterial(1.5f, 1.9f)));

    return matlib;
}


void a_rcScene() {
    int scale = 4;
    //Image img(480, 320);
    //Image img(2160,1440);
    Image img(540* scale,360* scale);

    World world;
    BVH* scene = new BVH();
    //SimpleGroup* scene = new SimpleGroup();
    world.scene = scene;
    ImageTexture* bumptex = new ImageTexture("models/blinn1_normal.png", ImageTexture::REPEAT, ImageTexture::NEAREST);

    loadOBJ(scene, "models/", "Final_interior.obj", nullptr);
    loadOBJ(scene, "models/", "Final_stage.obj", bumptex);
    loadOBJ(scene, "models/", "Final_people.obj", nullptr);
    loadOBJ(scene, "models/", "Final_car.obj", nullptr);
    
    /* Had to remove this block for artistic merit*/
    //MatLib* matlib_mirrorCeil = mirrorCeil_matlib();  // Not looking good in the final scene
    //loadOBJ(scene, "models/", "Final_ceilMat.obj", nullptr, matlib_mirrorCeil);
    
    
    //PerspectiveCamera cam1(Point(2.444636, -0.970460, 10.103112)
    //  , Point(2.378689, -0.974486, 9.914341) - Point(2.444636, -0.970460, 10.103112)
    //   , Vector(-0.001332, 0.199959, -0.003799), pi / 6, pi / 4);

    //working camera
    //DOFPerspectiveCamera* dofcamt = new DOFPerspectiveCamera(Point(1.417532, -0.608686, 11.419414)
    //    , Point(1.382954, -0.621349, 11.222833) - Point(1.417532, -0.608686, 11.419414)
    //    , Vector(-0.002198, 0.199599, -0.012471), pi / 6, pi / 4, 7.912f, 0.08f);

    // Final Rendering Camera with DOF
    DOFPerspectiveCamera* dofcam = new DOFPerspectiveCamera(Point(1.527959, -0.655179, 11.316772)
        , Point(1.488954, -0.663659, 11.120796) - Point(1.527959, -0.655179, 11.316772)
        , Vector(-0.001659, 0.199820, -0.008316), pi / 6, pi / 4, 7.87f, 0.08f);


    //ImageTexture* envtex = new ImageTexture("models/SkyDome_baseColor.jpeg",ImageTexture::REPEAT,ImageTexture::NEAREST);
    //FlatMaterial env(envtex);
    //EnvironmentMapper* envMapper = new EnvironmentMapper();
    //scene2->add(new EnvironmentSolid(envMapper, &env));

    //Interior Point lights
    //lcorner - low intensity
    world.light.push_back(new PointLight(Point(-4.f, 1.2f, -1.14f), RGBColor::rep(7.f)));

    //center
    world.light.push_back(new PointLight(Point(0.f, 1.f, -1.1f), RGBColor::rep(6.f)));
    
    //rcorner
    world.light.push_back(new PointLight(Point(4.f, 1.f, -1.1f), RGBColor::rep(6.f)));

    //floor illumination
    world.light.push_back(new PointLight(Point(0.f, -2.7f, 3.4f), RGBColor::rep(400.f)));
    //Ceil Illumination
    world.light.push_back(new PointLight(Point(0.f, 4.1f, 2.f), RGBColor::rep(600.f)));

    //spotLight on stage
    float scale1 = 60;
    world.light.push_back(new SpotLight(Point(0.31f, 2.03f, 3.03f), Vector(0.0f, -1.0f, 0.0f), pi / 6, 1.0f, RGBColor(1.f * scale1, 0.058f * scale1, 0.379f * scale1)));
    //world.light.push_back(new SpotLight(Point(0.31f, 2.03f, 2.03f), Vector(0.0f, -1.0f, 0.0f), pi / 3, 1.0f, RGBColor(0.196f * scale1, 0.971f * scale1, 1.f * scale1)));
    
    
    //Area Light
    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    RGBColor lightColor = RGBColor(1.0,0.99,0.98);
    //RGBColor lightColor = RGBColor(255.0 / 255, 250.0 / 255, 233.0 / 255);
    Vector s1 = Vector(1, 0, 0);
    Vector s2 = Vector(0, 0, 1);
    ConstantTexture* lightsrctex = new ConstantTexture(lightColor * 1200.0);
    Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);
    Quad* light = new Quad(Point(1.27f, 3.64f, 13.74f), s1, s2, nullptr, lightsource);
    //Quad* light = new Quad(Point(-0.11f, -2.53f, 13.75f), Point(4.51f, -0.64f, 13.75f) - Point(-0.11f, -2.53f, 13.75f),
    //    Point(-2.00f, 2.08f, 13.75f) - Point(-0.11f, -2.53f, 13.75f), mapper, lightsource);
    AreaLight als(light);
    world.light.push_back(&als);
     
    //RayCastingIntegrator integrator(&world);
    //RayTracingIntegrator integrator(&world);
    RecursiveRayTracingIntegrator integrator(&world);
    
    auto start_build = std::chrono::high_resolution_clock::now();
    scene->rebuildIndex();
    auto stop_build = std::chrono::high_resolution_clock::now();

    Renderer engine1(dofcam, &integrator);
    //Renderer engine1(&cam1, &integrator);
    engine1.setSamples(200);

    auto start_cast = std::chrono::high_resolution_clock::now();
    engine1.render(img);
    auto stop_cast = std::chrono::high_resolution_clock::now();

    auto build_duration = std::chrono::duration_cast<std::chrono::minutes>(stop_build - start_build);
    auto cast_duration = std::chrono::duration_cast<std::chrono::minutes>(stop_cast - start_cast);

    std::cout << "Build duration(Minutes) " << build_duration.count() << std::endl;
    std::cout << "Cast duration(Minutes) " << cast_duration.count() << std::endl;

    //Build duration(Minutes) 0
    //Cast duration(Minutes) 127

    img.writePNG("Final.png");
}