cmake_minimum_required(VERSION 3.20)

project(Renegade)

file(GLOB SOURCES
    "src/*.c"
    "src/*.cpp"
)

add_executable(Renegade ${SOURCES})

target_compile_options(Renegade
    PRIVATE
        -Wall
        -Wextra
        -O3
        -g
)

target_link_libraries(Renegade
    PRIVATE
        SDL3
        SDL3_image
        SDL3_ttf
        SDL3_mixer
        cjson
)
