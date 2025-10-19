cmake_minimum_required(VERSION 3.16)

project(MatchQuest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(PROJECT_SOURCES
        source/base/main.cpp
        include/base/mainwindow.h source/base/mainwindow.cpp
        include/base/userinterface.h source/base/userinterface.cpp
        include/base/level.h source/base/level.cpp
        include/base/playercontroller.h source/base/playercontroller.cpp
        include/base/character.h source/base/character.cpp
        include/base/configuration.h source/base/configuration.cpp
        include/base/emptygraphicsitem.h source/base/emptygraphicsitem.cpp
        include/base/particle.h source/base/particle.cpp
        include/base/actor.h source/base/actor.cpp
        include/base/serialization.h source/base/serialization.cpp
        include/world/tilelevel.h source/world/tilelevel.cpp
        include/world/tile.h source/world/tile.cpp
        include/world/prop.h source/world/prop.cpp
        include/world/startmenulevel.h source/world/startmenulevel.cpp
        include/world/singleplayerlevel.h source/world/singleplayerlevel.cpp
        include/world/dualplayerlevel.h source/world/dualplayerlevel.cpp
        include/world/propgenerator.h source/world/propgenerator.cpp
        include/world/hinter.h source/world/hinter.cpp
        include/ui/startmenuui.h source/ui/startmenuui.cpp
        include/ui/singleplayeringameui.h source/ui/singleplayeringameui.cpp
        include/ui/dualplayeringameui.h source/ui/dualplayeringameui.cpp
        include/ui/singleplayergameendui.h source/ui/singleplayergameendui.cpp
        include/ui/dualplayergameendui.h source/ui/dualplayergameendui.cpp
        include/ui/singleplayerpausegameui.h source/ui/singleplayerpausegameui.cpp
        include/particle/tileElimination.h source/particle/tileElimination.cpp
        include/player/matcher.h source/player/matcher.cpp
        include/player/matchercontroller.h source/player/matchercontroller.cpp
        include/player/singlematchercontroller.h source/player/singlematchercontroller.cpp
        include/player/dualmatchercontroller.h source/player/dualmatchercontroller.cpp
        include/player/startmenuplayercontroller.h source/player/startmenuplayercontroller.cpp
        resource/resourcestree.qrc
        resource/ui/singleplayeringameui.ui
        resource/ui/dualplayeringameui.ui
        resource/ui/singleplayergameendui.ui
        resource/ui/dualplayergameendui.ui
        resource/ui/singleplayerpausegameui.ui
        resource/ui/startmenuui.ui
)

set(PROJECT_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/base
        ${CMAKE_CURRENT_SOURCE_DIR}/include/ui
        ${CMAKE_CURRENT_SOURCE_DIR}/include/player
        ${CMAKE_CURRENT_SOURCE_DIR}/include/world
        ${CMAKE_CURRENT_SOURCE_DIR}/include/particle
        ${CMAKE_CURRENT_SOURCE_DIR}/resource
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MatchQuest
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MatchQuest APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MatchQuest SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MatchQuest
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(MatchQuest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MatchQuest)
endif()
set_target_properties(MatchQuest PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MatchQuest
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MatchQuest)
endif()

target_link_libraries(MatchQuest  PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

target_include_directories(MatchQuest PRIVATE ${PROJECT_INCLUDE_DIR})

qt_add_resources(MatchQuest "app_images"
    PREFIX "/"
    FILES
        resource/image/Lamball.png
        resource/image/Cattiva.png
)

enable_testing()

add_executable(NoSolutionTest
    test/nosolutiontest.cpp
)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

target_link_libraries(NoSolutionTest PRIVATE
    Qt6::Test
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

target_include_directories(NoSolutionTest PRIVATE ${PROJECT_INCLUDE_DIR})

add_test(NAME NoSolutionTests COMMAND NoSolutionTest)
