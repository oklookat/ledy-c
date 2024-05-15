set(TARGET_NAME LedyEffect)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME}
${SRC}/effect.cpp
${SRC}/debug.cpp
)

target_include_directories(${TARGET_NAME} PUBLIC ${SRC})

target_link_libraries(${TARGET_NAME} PRIVATE
    LedyDSP
)
set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
