file( GLOB_RECURSE engine_SOURCES *.cpp )
add_library( engine ${engine_SOURCES} )
target_include_directories(engine PUBLIC ${PROJECT_SOURCE_DIR}/include)
