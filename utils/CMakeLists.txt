add_library(utils INTERFACE)

target_sources(utils
    INTERFACE
        include/utils/random.hpp
)

target_include_directories(utils
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

