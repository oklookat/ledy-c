set(TARGET_NAME LedyDSP)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME}
${SRC}/dsp.cpp
)
target_include_directories(${TARGET_NAME} PUBLIC ${SRC})

# KISS FFT
find_package(kissfft CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PUBLIC
    kissfft::kissfft-float
)
