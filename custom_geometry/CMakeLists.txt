
set(EXE_PROJECT_NAME "custom_geometry")

#OSG Seting
find_package(OpenSceneGraph REQUIRED osgDB osgGA osgUtil osgText osgViewer osgWidget)


file(GLOB src_dir src/*.cpp)
source_group("Source Files" FILES ${src_dir})

file(GLOB include_dir include/*.h)
source_group("Include Files" FILES ${include_dir})

file(GLOB shader_dir shader/*.vert shader/*.frag)
source_group("Shader Files" FILES ${shader_dir})

include_directories("include")

add_executable(${EXE_PROJECT_NAME} ${src_dir} ${include_dir} ${shader_dir})
target_link_libraries(${EXE_PROJECT_NAME} ${OPENSCENEGRAPH_LIBRARIES} tool)



#install(TARGETS HelloWorld DESTINATION BIN)