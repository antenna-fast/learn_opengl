project(window)
cmake_minimum_required(VERSION 3.16)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})


add_executable(window create_window.cpp)
target_link_libraries(window ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLEW_LIBRARIES} 
glfw dl X11 pthread Xrandr Xi Xxf86vm Xinerama Xcursor)
