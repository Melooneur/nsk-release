cmake_minimum_required(VERSION 3.18)	
project(NeoSkullKing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

# SFML path
set(SFML_DIR "C:/SFML-2.6/lib/cmake/SFML")
include_directories("C:/SFML-2.6/include")

# Ne pas définir SFML_STATIC_LIBRARIES
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Sources communs
set(COMMON_SRC
    cpp/deck.cpp
    cpp/joueur.cpp
    cpp/jeu.cpp
    cpp/partie.cpp
    cpp/jcpu.cpp
    cpp/partieSfml.cpp
    cpp/network.cpp
    cpp/SfmlServ.cpp
)

# === CLIENT ===
add_executable(${PROJECT_NAME} ${COMMON_SRC})

target_link_directories(${PROJECT_NAME} PRIVATE "C:/SFML-2.6/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    ws2_32
)

# ASIO
set(ASIO_PATH "C:/asio/include")
target_include_directories(${PROJECT_NAME} PRIVATE ${ASIO_PATH})
target_compile_definitions(${PROJECT_NAME} PRIVATE ASIO_STANDALONE _WIN32_WINNT=0x0601)

# === SERVEUR ===
add_executable(${PROJECT_NAME}-serveur
    cpp/server/server.cpp
    cpp/partie.cpp
    cpp/joueur.cpp
    cpp/deck.cpp
    cpp/jcpu.cpp
    cpp/partieSfml.cpp
    cpp/SfmlServ.cpp
)

message(STATUS "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

target_link_directories(${PROJECT_NAME}-serveur PRIVATE "C:/SFML-2.6/lib")
target_include_directories(${PROJECT_NAME}-serveur PRIVATE ${ASIO_PATH})
target_compile_definitions(${PROJECT_NAME}-serveur PRIVATE ASIO_STANDALONE _WIN32_WINNT=0x0601)

target_link_libraries(${PROJECT_NAME}-serveur PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    ws2_32
)