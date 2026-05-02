cmake_minimum_required(VERSION 3.19)
project(TypeDough LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_CXX_STANDARD 20)

qt_standard_project_setup()

qt_add_executable(TypeDough
    WIN32 MACOSX_BUNDLE
    main.cpp
    Window/mainwindow.cpp
    Window/mainwindow.h
    Window/mainwindow.ui
    Core/Nodes/node.h Core/Nodes/node.cpp
    Core/Nodes/addnode.h Core/Nodes/addnode.cpp
    Core/Context/flowcontext.h Core/Context/flowcontext.cpp
    Core/graphmanager.h Core/graphmanager.cpp
    Core/Context/noderuntime.h Core/Context/noderuntime.cpp
    Core/Core.h
)

target_link_libraries(TypeDough
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS TypeDough
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TypeDough
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
