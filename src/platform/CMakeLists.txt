set(TARGET_NAME LedyPlatform)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

if (APPLE)

elseif (UNIX)

elseif (WIN32)
    list(APPEND SRCS
    ${SRC}/platform.cpp
    ${SRC}/platform_windows.cpp
    ${SRC}/audio_windows.cpp
    )
endif()

add_library(${TARGET_NAME}
${SRCS}
)
target_include_directories(${TARGET_NAME} PUBLIC ${SRC})
