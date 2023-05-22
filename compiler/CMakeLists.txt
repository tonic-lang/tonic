set(SOURCES
        src/frontend/lexer.cpp
        )

add_library(compiler ${SOURCES})

add_subdirectory(tests)

target_include_directories(compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)