set(SOURCES
        src/frontend/lexer.cpp
        src/frontend/parser.cpp
        src/traversal/walker.cpp
        )

add_library(tnc ${SOURCES})

add_subdirectory(tests)

target_include_directories(tnc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)