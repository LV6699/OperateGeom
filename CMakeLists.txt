cmake_minimum_required(VERSION 3.12)
project(GeomOperate VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# 将所有源文件合并成一个编译单元
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 50)  # 每批50个文件

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH D:/WorkRJAZ/Qt/Qt5.12.12/5.12.12/msvc2017_64)
include_directories(D:/WorkRJAZ/Qt/Qt5.12.12/5.12.12/msvc2017_64/include)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(OpenCASCADE_DIR "./etc/OCCT/cmake")
find_package(OpenCASCADE CONFIG REQUIRED)
# # find_package(jsoncpp REQUIRED)
# find_package(TBB REQUIRED VERSION 2021.11.0)
include_directories(${OpenCASCADE_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#add_subdirectory(./OffsetStruct)
add_subdirectory(./etc/Eigen)
add_subdirectory(./offset2D)

aux_source_directory(./ SRC_DIR)
aux_source_directory(./oft/CommonFile SRC_DIR)
aux_source_directory(./ViewWindow SRC_DIR)
aux_source_directory(./OperateView SRC_DIR)
aux_source_directory(./DiscreteModel SRC_DIR)
aux_source_directory(./CutterLocation SRC_DIR)
aux_source_directory(./CutterLocation/Common SRC_DIR)
aux_source_directory(./CutterLocation/GridStruct SRC_DIR)
aux_source_directory(./CutterLocation/Solution SRC_DIR)
aux_source_directory(./CutterLocation/DebugTool SRC_DIR)
aux_source_directory(./OctreeNode SRC_DIR)
aux_source_directory(./OffsetData SRC_DIR)
aux_source_directory(./ParallelFinishing/LineCutPath SRC_DIR)
aux_source_directory(./ParallelFinishing/UnitStruct SRC_DIR)

set(forms ./ViewWindow/MainWindow.ui ./CutterLocation/DebugTool/WidgetTool.ui)
set(user_source ./ViewWindow/Image.qrc)
add_executable(GeomOperate ${SRC_DIR} ${forms} ${user_source})

target_link_libraries(GeomOperate PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Eigen3::Eigen CAMAlgorithm #jsoncpp TBB::tbb
    Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui ${OpenCASCADE_LIBRARIES}
    TKBin TKBinL TKBinTObj TKBinXCAF TKBO TKBool TKBRep TKCAF TKCDF TKDCAF TKDE TKDECascade
    TKDEGLTF TKDEIGES TKDEOBJ TKDEPLY TKDESTEP TKDESTL TKDEVRML TKDraw TKernel TKExpress
    TKFeat TKFillet TKG2d TKG3d TKGeomAlgo TKGeomBase TKHLR TKLCAF TKMath TKMesh TKMeshVS
    TKOffset TKOpenGl TKOpenGlTest TKPrim TKQADraw TKRWMesh TKService TKShHealing TKStd
    TKStdL TKTObj TKTObjDRAW TKTopAlgo TKTopTest TKV3d TKVCAF TKViewerTest TKXCAF TKXDEDRAW
    TKXMesh TKXml TKXmlL TKXmlTObj TKXmlXCAF TKXSBase TKXSDRAW TKXSDRAWDE TKXSDRAWGLTF
    TKXSDRAWIGES TKXSDRAWOBJ TKXSDRAWPLY TKXSDRAWSTEP TKXSDRAWSTL TKXSDRAWVRML)

set_target_properties(GeomOperate PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GeomOperate)
endif()






