cmake_minimum_required(VERSION 3.16)
project(SimulateurLivraison CXX)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Automatically find all .cpp files inside the src/ folder
file(GLOB SOURCES "src/*.cpp")

# Create the executable using main.cpp and all the discovered source files
add_executable(simulateur
    main.cpp
    ${SOURCES}
)

# Tell the compiler where to look for the .h files
target_include_directories(simulateur PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Optional Qt GUI target
option(BUILD_QT_GUI "Build the Qt GUI application" ON)

if (BUILD_QT_GUI)
    find_package(Qt6 COMPONENTS Widgets QUIET)
    if (Qt6_FOUND)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)

        add_executable(simulateur_gui
            gui/main_gui.cpp
            gui/MainWindow.cpp
            gui/MainWindow.h
            gui/resources.qrc
            ${SOURCES}
        )

        target_include_directories(simulateur_gui PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

        target_link_libraries(simulateur_gui PRIVATE Qt6::Widgets)
    else()
        message(STATUS "Qt6 Widgets not found - GUI target disabled.")
    endif()
endif()
