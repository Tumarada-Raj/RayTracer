cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
${MY_ADDITIONAL_SOURCES}
main/a_rcScene.cpp
)


file(COPY
    models/blinn1_baseColor.jpeg
    models/blinn1_metallicRoughness.png
    models/blinn1_normal.png

    models/Material.001_baseColor.jpeg
    models/Material.002_baseColor.jpeg
    models/Material.008_baseColor.png

    models/floor.jpg
    models/Walls.jpg

    models/Final_interior.obj
    models/Final_interior.mtl
    models/paint.jpg

    models/Final_stage.obj
    models/Final_stage.mtl

    models/Final_ceilMat.obj
    models/Final_ceilMat.mtl

    models/Final_people.obj
    models/Final_people.mtl
    models/02_person_baseColor.jpeg
    models/02_person_normal.jpeg
    models/03_person_baseColor.jpeg
    models/03_person_normal.jpeg

    models/Final_car.obj
    models/Final_car.mtl
    DESTINATION ${CMAKE_BINARY_DIR}/models
)