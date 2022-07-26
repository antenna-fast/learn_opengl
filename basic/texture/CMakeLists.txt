project(opengl_test)
cmake_minimum_required(VERSION 3.16)

#
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} 
                    ${OPENGL_INCLUDE_DIRS} 
                    ${GLUT_INCLUDE_DIRS} 
                    ${GLFW_INCLUDE_DIRS} 
                    ${GLEW_INCLUDE_DIRS} 
                    ${EIGEN_INCLUDE_DIR}
                    glm)

add_executable(opengl_test
triangle_texture.cpp
)

target_link_libraries(opengl_test
                      ${OPENGL_LIBRARIES}
                      ${GLUT_LIBRARY}
                      ${GLEW_LIBRARIES}
                      ${YAMLCPP_LIBRARIES}
                      glfw dl X11 pthread Xrandr Xi Xxf86vm Xinerama Xcursor glut) 
