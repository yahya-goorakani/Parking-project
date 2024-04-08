



cmake_minimum_required(VERSION 3.10)
project(ParkingLotQt)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)



set(CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )





find_package(OpenCV  REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

# # Set the path to Qt6 installation directory
 set(Qt "/opt/homebrew/Cellar/qt/6.6.1/")

# Find Qt6
find_package(Qt6 COMPONENTS Widgets REQUIRED)
qt_standard_project_setup()



# find_package( PkgConfig REQUIRED)

# pkg_search_module( TESSERACT REQUIRED tesseract )

# pkg_search_module( LEPTONICA REQUIRED leptonica )

# include_directories( ${TESSERACT_INCLUDE_DIRS} )

# include_directories( ${LEPTONICA_INCLUDE_DIRS} )
# include_directories("/opt/homebrew/Cellar/leptonica/1.84.1/include")


# link_directories( ${TESSERACT_LIBRARY_DIRS} )

# link_directories( ${LEPTONICA_LIBRARY_DIRS} )




find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})


include_directories(${OpenCV_INCLUDE_DIRS})

# Find Qt5
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add the executable for parking.cpp
#  add_executable(MainExecutable src/parking.cpp)
#  target_link_libraries(MainExecutable PRIVATE Qt6::Widgets)

# Add the executable for qt.cpp
 add_executable(Main1Executable src/qt.cpp)
 target_link_libraries(Main1Executable PRIVATE Qt6::Widgets ${DEPENDENCIES} ${OpenCV_LIBRARIES} )

# Add the executable for car_detection.cpp
 add_executable(Main2Executable src/car_detection.cpp)
 target_link_libraries(Main2Executable ${OpenCV_LIBS})




# # Add your source files
# set(SOURCES
#     src/qt.cpp
#     src/car_detection.cpp
   
# )

# # Add the executable
# add_executable(Main2Executable ${SOURCES})

# # Link libraries
# target_link_libraries(Main2Executable PRIVATE Qt6::Widgets ${DEPENDENCIES} ${OpenCV_LIBRARIES} ${OpenCV_LIBS})



# # Add the executable for detect_numberPlate.cpp
#  add_executable(Main3Executable src/detect_numberPlate.cpp)
# target_link_libraries(Main3Executable ${LEPTONICA_LIBRARIES} ${OpenCV_LIBRARIES} ${TESSERACT_LIBRARIES})


# Add the directory containing moc files to the include path
# target_include_directories(Main1Executable PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_source_files_properties(qt.cpp PROPERTIES SKIP_AUTOMOC ON)












