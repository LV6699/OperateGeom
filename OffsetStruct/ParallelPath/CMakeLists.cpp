PROJECT(ParallelFinishing)
CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
AUX_SOURCE_DIRECTORY(./ SRC_DIR)
AUX_SOURCE_DIRECTORY(./LineCut3DAlg SRC_DIR)
AUX_SOURCE_DIRECTORY(./UnitStruct SRC_DIR)
add_library(ParallelFinishing ${SRC_DIR})
target_include_directories(ParallelFinishing PUBLIC $<BUILD_INTERFACE:$
                           {CMAKE_CURRENT_SOURCE_DIR}>)
target_include_directories(ParallelFinishing PUBLIC 
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(ParallelFinishing PRIVATE TBB::tbb)
install(DIRECTORY . DESTINATION include/ParallelFinishing FILES_MATCHING PATTERN
        "*.h" PATTERN "*.hpp" PATTERN "*.hxx" PATTERN "build" EXCLUDE)
install(TARGETS ParallelFinishing EXPORT ${PROJECT_NAME}Targets RUNTIME 
        DESTINATION lib LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(EXPORT CAMAlgorithmTargets FILE ${PROJECT_NAME}Targets.cmake DESTINATION
        lib/cmake/ParallelFinishing)
include(CMakePackageConfigHelpers)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CAMAlgorithmConfig.cmake DESTINATION 
        lib/cmake/ParallelFinishing)
if (MSVC)
add_compile_options(/MP)
add_compile_options(/wd4267)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4267")
target_compile_options(ParallelFinishing PUBLIC /utf-8)
else (UNIX)
target_compile_options(ParallelFinishing PUBLIC -fPIC)
endif()
