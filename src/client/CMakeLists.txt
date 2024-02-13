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
find_path(MDNSRESPONDER_INCLUDE_DIR dns_sd.h)
find_library(MDNSRESPONDER_LIBRARY dnssd REQUIRED)
target_include_directories(${TARGET_NAME} PUBLIC ${MDNSRESPONDER_INCLUDE_DIR})

target_link_libraries(${TARGET_NAME} PUBLIC
    LedyPlatform
    ${MDNSRESPONDER_LIBRARY}
    crypt32
    ws2_32
    ixwebsocket::ixwebsocket
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 23)
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
