cmake_minimum_required(VERSION 3.10)
project(AudioVisualisation)
set(CMAKE_C_STANDARD 99)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
add_executable(main src/gui/main.c src/gui/mainmenu.c src/gui/plot_page.c src/gui/gl_utils.c src/utils/utils.c src/core/fourier.c)

target_include_directories(main PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_libraries(main PRIVATE ${GTK4_LIBRARIES} -lGL glad m)
target_compile_options(main PRIVATE ${GTK4_CFLAGS_OTHER})
# target_compile_options(main PRIVATE ${GTK4_CFLAGS_OTHER} -Wall -Wextra -Wpedantic -Werror)

add_subdirectory(glad)