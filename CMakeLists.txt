project(fdf)

cmake_minimum_required(VERSION 3.13)

find_package(X11 REQUIRED)

set(C_STANDART 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -no-pie -lX11 -lXext")

add_executable(fdf main.c parse.c draw_line.c swaps.c)

target_link_libraries(fdf ${CMAKE_CURRENT_SOURCE_DIR}/libft/libft.a /usr/local/lib/libmlx.a ${X11_LIBRARIES}  m)