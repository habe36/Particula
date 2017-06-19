PROJECT (Particula)
# Well technicall you also need:
# http://cmake.org/Bug/bug.php?op=show&bugid=3582
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

IF(WIN32)
  SET(PATH_TO_RCFILE ../Particula.rc)
  SET(GUI_EXECUTABLE WIN32 ${PATH_TO_RCFILE})
ELSE(WIN32)
  IF(APPLE)
    SET(GUI_EXECUTABLE MACOSX_BUNDLE ../Particula.icns)
    SET(MACOSX_BUNDLE_ICON_FILE Particula.icns)
    SET_SOURCE_FILES_PROPERTIES(../Particula.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  ELSE(APPLE)
    # Ok X11 for sure, but just check:
    IF(NOT VTK_USE_X)
      MESSAGE(FATAL_ERROR "You need to have VTK_USE_X")
    ENDIF(NOT VTK_USE_X)
    # CMake 2.6:
    # technically those packages are not required since one can still
    #  use the Motif/X11 version and not the gtk one:
    FIND_PACKAGE(PkgConfig)
    pkg_check_modules (GTK2 gtk+-2.0)
    #MESSAGE("${GTK2_INCLUDE_DIRS}")
    INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
    LINK_LIBRARIES(${GTK2_LIBRARIES})
    # Can I require all my user to have the gl lib on linux,
    #  even if they do not really need it...
    SET(WXGLCANVASLIBS "gl")
  ENDIF(APPLE)
ENDIF(WIN32)

FIND_PACKAGE(loki)
#  INCLUDE({$LOKI_USE_FILE})

FIND_PACKAGE(GLUT)

FIND_PACKAGE(OpenGL)

FIND_PACKAGE(Eigen3)

FIND_PACKAGE(wxWidgets COMPONENTS base core aui adv xml gl ${WXGLCANVASLIBS})

IF(wxWidgets_FOUND)
  INCLUDE( ${wxWidgets_USE_FILE} )

#  SET( CMAKE_WX_CXX_FLAGS "`${CMAKE_WX_CONFIG} --cxxflags`" )
#  SET( WXWINDOWS_LIBRARIES "`${CMAKE_WX_CONFIG} --libs`" )

# for debug
MESSAGE("VTK:              ${VTK_FOUND}")
MESSAGE("VTK_INCLUDE_DIRS: ${VTK_INCLUDE_DIRS}")
MESSAGE("VTK_LIBRARY_DIRS: ${VTK_LIBRARY_DIRS}")
MESSAGE("VTK_KITS:         ${VTK_KITS}")
MESSAGE("VTK_DIR:          ${VTK_DIR}")
MESSAGE("VTK_LIBRARIES:    ${VTK_LIBRARIES}")

if(NOT VTK_LIBRARIES)
  SET(VTK_LIBRARIES vtkRendering vtkWidgets)
endif(VTK_LIBRARIES)

#-----------------------------------------------------------------------------
message("wxWidgets is latest ${wxWidgets_VERSION}")

SET(SRCS
../../../wxmathplot/mathplot.cpp
../../../wxmathplot/mathplot.h
../Model/adtree.cpp
../Model/barrier.h
../Model/diagnostics.h
../Model/force.cpp
../Model/octtree.h
../Model/world.cpp
../Model/adtree.h
../Model/datatypes.h
../Model/distributor.cpp
../Model/force.h
../Model/ppforce.cpp
../Model/world.h
../Model/barrier.c
../Model/diagnostics.cpp
../Model/distributor.h
../Model/octtree.cpp
../Model/ppforce.h
../View/DistributePanel.cpp
../View/DistributePanel.h
../View/View2D.cpp
../View/View2D.h
../View/View3D.cpp
../View/View3D.h
../Controller/ParticulaApp.cpp
../Controller/base_controller.cpp
../Controller/preferences.cpp
../Controller/world_controller.cpp
../Controller/ParticulaApp.h
../Controller/base_controller.h
../Controller/preferences.h
../Controller/world_controller.h
../../H/Hvec3D.h
)

LINK_LIBRARIES(
  ${wxWidgets_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARY}
)

#Add our own include path
INCLUDE_DIRECTORIES(
  ${PARTICULA_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ../../wxmathplot
  ../../
  ../
  /usr/local/include
)

SET(Particula_CXX_FLAGS "-D_H_ENABLE_EIGEN_" CACHE STRING "sss")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Particula_CXX_FLAGS}")

ADD_EXECUTABLE(Particula ${GUI_EXECUTABLE} ${SRCS})

ELSE(wxWidgets_FOUND)

  MESSAGE("Cannot find wxWidgets libraries and/or header files")

ENDIF(wxWidgets_FOUND)
