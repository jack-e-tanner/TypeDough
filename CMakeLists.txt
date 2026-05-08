cmake_minimum_required(VERSION 3.19)
project(TypeDough LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_CXX_STANDARD 20)

qt_standard_project_setup()

qt_add_executable(TypeDough
    WIN32 MACOSX_BUNDLE
    main.cpp
    UI/mainwindow.cpp
    UI/mainwindow.h
    UI/mainwindow.ui
    Core/Nodes/node.h Core/Nodes/node.cpp
    Core/Nodes/addnode.h Core/Nodes/addnode.cpp

    Core/graphmanager.h Core/graphmanager.cpp
    Core/noderuntime.h Core/noderuntime.cpp
    Core/core.h
    UI/nodeitem.h UI/nodeitem.cpp
    UI/wireitem.h UI/wireitem.cpp
    Core/Nodes/NodeTypes.h
    Core/Nodes/AllNodes.h
    UI/errorutility.h
    UI/portitem.h UI/portitem.cpp
    Core/grapheditor.h Core/grapheditor.cpp
)

target_link_libraries(TypeDough
    PRIVATE
        Qt::Core
        Qt::Widgets
)

target_include_directories(TypeDough
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
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
