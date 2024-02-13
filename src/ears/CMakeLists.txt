set(TARGET_NAME LedyEars)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME}
${SRC}/ears.cpp
${SRC}/ears.h
)
set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)

target_link_libraries(${TARGET_NAME} PUBLIC
    LedyDSP
)
