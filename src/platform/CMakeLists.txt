set(TARGET_NAME LedyPlatform)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME}
${SRC}/platform.cpp
)
target_include_directories(${TARGET_NAME} PUBLIC ${SRC})
