include_directories(./include)

file(GLOB SRC_FILES ./src/*.cpp)

add_library(monitor STATIC ${SRC_FILES})

