set(TARGET_NAME LedyClient)
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${TARGET_NAME}
    ${SRC}/finder.cpp
    ${SRC}/client.cpp
)
target_include_directories(${TARGET_NAME} PUBLIC ${SRC})

# ixwebsocket
find_package(ixwebsocket CONFIG REQUIRED)

# mdnsresponder

target_link_libraries(${TARGET_NAME} PUBLIC
    LedyPlatform
    crypt32
    ws2_32
    dnsapi
    ixwebsocket::ixwebsocket
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
