file(GLOB SOURCES "src/*.cpp")

add_library(compiler ${SOURCES})

add_subdirectory(tests)