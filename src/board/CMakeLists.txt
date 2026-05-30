add_library(board
    src/board.cpp
)

target_include_directories(board PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/utils/)
target_include_directories(board PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
target_link_libraries(board PUBLIC utils)

if(COMPILE_TESTS)
    add_subdirectory(test)
endif()
