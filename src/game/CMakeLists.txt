file( GLOB game_SOURCES 
    *.cpp
    chess/*.cpp
    menus/*.cpp
    textual/*.cpp
    visual/*.cpp
)
add_library( game ${game_SOURCES} )
target_include_directories(game PUBLIC ${PROJECT_SOURCE_DIR}/include)
