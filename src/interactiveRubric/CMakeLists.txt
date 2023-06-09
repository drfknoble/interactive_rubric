cmake_minimum_required(VERSION 3.5)

project(interactiveRubric LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(interactiveRubric SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(interactiveRubric WIN32
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    rubric.cpp
    rubric.h
    rubric.ui
    row.cpp
    row.h
    row.ui
    col.cpp
    col.h
    col.ui)
endif()

target_link_libraries(interactiveRubric PRIVATE Qt5::Widgets)

find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "C:/Qt/5.15.0/mingw81_64/bin/")

add_custom_command(TARGET interactiveRubric POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E
        env PATH="C:/Qt/Tools/mingw810_64/bin" "${WINDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE:interactiveRubric>" -dir "./bin/" -compiler-runtime
    COMMAND "${CMAKE_COMMAND}" -E
        copy "$<TARGET_FILE:interactiveRubric>" "./bin/$<TARGET_FILE_NAME:interactiveRubric>"
    COMMENT "Running windeployqt..."
)



