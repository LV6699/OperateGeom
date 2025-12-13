# Install script for directory: C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/GeomOperate")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/AdolcForward"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/AlignedVector3"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/ArpackSupport"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/AutoDiff"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/BVH"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/EulerAngles"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/FFT"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/IterativeSolvers"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/KroneckerProduct"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/LevenbergMarquardt"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/MatrixFunctions"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/MPRealSupport"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/NNLS"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/NonLinearOptimization"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/NumericalDiff"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/OpenGLSupport"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/Polynomials"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/SparseExtra"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/SpecialFunctions"
    "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "C:/Users/lvd/Desktop/GeomOperate/etc/Eigen/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/lvd/Desktop/GeomOperate/build/etc/Eigen/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

