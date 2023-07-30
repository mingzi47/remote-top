include_directories(./include)

file(GLOB SRC_FILES ./*.cpp)

add_library(monitor STATIC ${SRC_FILES})

