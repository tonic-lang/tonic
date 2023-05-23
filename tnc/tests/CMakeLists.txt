set(TEST_SOURCES
        errors_tests.cpp
        frontend/lexer_tests.cpp
        )

add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests gtest gtest_main tnc)

target_include_directories(runTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)