file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_library(voxel_engine STATIC ${SRC_FILES})

target_compile_options(voxel_engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wnull-dereference
        -Wimplicit-fallthrough
)

target_include_directories(voxel_engine
        PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${SDL2_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/extern/magnum/src
        ${PROJECT_SOURCE_DIR}/extern/corrade/src
        ${PROJECT_SOURCE_DIR}/extern/magnum-plugins/src
)

target_link_libraries(voxel_engine
        PUBLIC
        # Magnum
        Corrade::Main
        Magnum::Magnum
        Magnum::GL
        Magnum::Shaders
        Magnum::Sdl2Application
        Magnum::SceneGraph
        Magnum::MeshTools
        Magnum::Trade
        Magnum::AnyImageImporter
        MagnumExtras::Ui
        MagnumPlugins::StbTrueTypeFont
        MagnumPlugins::StbImageImporter
        # Rest of libs
        dbghelp
        fastnoise-lite::fastnoise-lite
        spdlog::spdlog
        concurrencpp::concurrencpp
        cpptrace::cpptrace
        tsl::hopscotch_map
)

target_compile_features(voxel_engine PRIVATE cxx_std_23)

add_executable(Radon main.cpp)
target_link_libraries(Radon PRIVATE voxel_engine)
