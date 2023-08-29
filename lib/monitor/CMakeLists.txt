file(GLOB SRC_FILES ./src/*.cpp)

add_library(monitor ${SRC_FILES})

target_include_directories(monitor PUBLIC include)
