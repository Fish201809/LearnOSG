cmake_minimum_required(VERSION 2.8)


include_directories("common/include")

add_subdirectory("common")
add_subdirectory("window")
add_subdirectory("moveselect")
add_subdirectory("custom_geometry")
add_subdirectory("shader")
add_subdirectory("camerawindow")
add_subdirectory("geometry_shader")
add_subdirectory("framebuffer")