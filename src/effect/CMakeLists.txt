set(TARGET_NAME LedyEffect)

add_library(${TARGET_NAME}
${CMAKE_CURRENT_SOURCE_DIR}/effect.cpp
${CMAKE_CURRENT_SOURCE_DIR}/effect.h
)

target_link_libraries(${TARGET_NAME} PRIVATE
    LedyDSP
)
set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
