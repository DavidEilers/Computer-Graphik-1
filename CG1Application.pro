TARGET = CG1Application
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt
OBJECTS_DIR= obj
DESTDIR = $$PWD
TEMPLATE = app

SOURCES +=\
    source/CGContext/CGContext.cpp \
    source/CGContext/CGFrameBuffer.cpp \
    source/CGContext/CGMatrix.cpp \
    source/CGContext/CGPrimClipper.cpp \
    source/CGContext/CGTexture.cpp \
    source/CGContext/CGVertexOperations.cpp \
    source/CGContext/CGPrimitiveOperations.cpp \
    source/CGContext/CGFragmentOperations.cpp \
    source/CGContext/CGRasterizer_point.cpp \
    source/CGContext/CGRasterizer_line_stupid.cpp \
    source/CGContext/CGRasterizer_line_bresenham.cpp \
    source/CGContext/CGRasterizer_triangle.cpp \
    source/Utilities/CGImageFile.cpp \
    source/Utilities/CGQuadric.cpp \
    source/Utilities/Deer.cpp \
    source/Utilities/Snowflakes.cpp \
    source/Utilities/CG1Helper/CG1Helper.cpp \
    dependencies/glad/src/glad.c \
    source/Applications/CG1Main.cpp \
    source/Applications/Uebungen/U01_JumpingPoint.cpp \
    source/Applications/Uebungen/U07_U08_HappyHolidays.cpp \
    source/Applications/Uebungen/U09_Lighting.cpp \
    source/Applications/Uebungen/U10_Texturing.cpp \
    source/Applications/Uebungen/U02_LineBenchmark.cpp \
    source/Applications/Uebungen/U02_U03_TestBresenham.cpp \
    source/Applications/Uebungen/U03_U04_TestRotatingTriangle.cpp \
    source/Applications/Uebungen/U05_TestViewport.cpp \
    source/Applications/Uebungen/U05_U06_TestProjection.cpp \
    source/Applications/Uebungen/U08_AwesomeTriangle.cpp \
    source/Applications/Project/Project_Chess.cpp \
    source/Applications/Project/CGChess.cpp \
    source/Applications/Project/CGChessAI.cpp \
    source/Applications/Project/CGChessSet.cpp

HEADERS +=\
    source/CGContext/CG.h \
    source/CGContext/CGContext.h \
    source/CGContext/CGFrameBuffer.h \
    source/CGContext/CGMath.h \
    source/CGContext/CGMatrix.h \
    source/CGContext/CGPrimClipper.h \
    source/CGContext/CGProgramInterface.h \
    source/CGContext/CGTexture.h \
    source/CGContext/CGVector.h \
    source/CGContext/CGVertexOperations.h \
    source/CGContext/CGProgram.h \
    source/CGContext/CGPrimitiveOperations.h \
    source/CGContext/CGRasterizer.h \
    source/CGContext/CGFragmentOperations.h \
    source/CGContext/CGRectangle.h \
    source/Utilities/CGImageFile.h \
    source/Utilities/CGQuadric.h \
    source/Utilities/Deer.h \
    source/Utilities/DeerVertexData.h \
    source/Utilities/Snowflakes.h \
    source/Utilities/CG1Helper/CG1Helper.h \
    source/Utilities/CG1Helper/glutil.h \
    source/ShaderPrograms/Program_modelViewProjection.h \
    source/ShaderPrograms/Program_passThrough.h \
    source/ShaderPrograms/Program_perVertexLighting.h \
    source/ShaderPrograms/Program_phong.h \
    source/ShaderPrograms/Program_projectVertex.h \
    source/ShaderPrograms/Program_testVertex.h \
    source/Applications/Application.h \
    source/Applications/Uebungen/U08_AwesomeTriangle.h \
    source/Applications/Uebungen/U07_U08_HappyHolidays.h \
    source/Applications/Uebungen/U01_JumpingPoint.h \
    source/Applications/Uebungen/U09_Lighting.h \
    source/Applications/Uebungen/U02_LineBenchmark.h \
    source/Applications/Uebungen/U02_U03_TestBresenham.h \
    source/Applications/Uebungen/U05_U06_TestProjection.h \
    source/Applications/Uebungen/U03_U04_TestRotatingTriangle.h \
    source/Applications/Uebungen/U10_Texturing.h \
    source/Applications/Uebungen/U05_TestViewport.h \
    source/Applications/Project/Project_Chess.h \
    source/Applications/Project/CGChess.h \
    source/Applications/Project/CGChessAI.h \
    source/Applications/Project/CGChessdata.h \
    source/Applications/Project/CGChessSet.h

CONFIG += c++11


MAKEFILE = Makefile_qmake.$${TARGET}



CONFIG(release, debug|release): DEFINES += NDEBUG



unix{
    LIBS += -lglfw -lm -pthread -ldl -fopenmp
    INCLUDEPATH += dependencies/glad/include source
    QMAKE_CXXFLAGS += -fopenmp
}


win32{
    contains(QMAKE_COMPILER_DEFINES, __GNUC__) {
        message("experimental MinGW support");
        MINGW=1
    } else {
        MINGW=0
    }

    # select the architecture
    GLFW_LIB_DIR=$$PWD/dependencies/GLFW
    contains(QMAKE_TARGET.arch,x86_64){
        # 64 Bit
        if ($$MINGW) {
            GLFW_LIB_DIR=$$GLFW_LIB_DIR/lib-mingw-w64
        } else {
            GLFW_LIB_DIR=$$GLFW_LIB_DIR/lib-vc-x86_64
        }
    } else {
        # 32 Bit
        equals(MINGW,1) {
            GLFW_LIB_DIR=$$GLFW_LIB_DIR/lib-mingw
        } else {
            GLFW_LIB_DIR=$$GLFW_LIB_DIR/lib-vc
        }
    }

    # select the VS version
    contains(QMAKE_COMPILER_DEFINES, _MSC_VER=1600) {
        # MSVS2010
        message("Generating for MSVS 2010")
        GLFW_LIB_DIR=$$GLFW_LIB_DIR/10.0
    }
    contains(QMAKE_COMPILER_DEFINES, _MSC_VER=1700) {
        # MSVS2012
        message("Generating for MSVS 2012")
        GLFW_LIB_DIR=$$GLFW_LIB_DIR/11.0
    }
    contains(QMAKE_COMPILER_DEFINES, _MSC_VER=1800) {
        # MSVS2013
        message("Generating for MSVS 2013")
        GLFW_LIB_DIR=$$GLFW_LIB_DIR/12.0
    }
    contains(QMAKE_COMPILER_DEFINES, _MSC_VER=1900) {
        # MSVS2015
        message("Generating for MSVS 2015")
        GLFW_LIB_DIR=$$GLFW_LIB_DIR/14.0
    }
    contains(QMAKE_COMPILER_DEFINES, $$find(QMAKE_COMPILER_DEFINES,_MSC_VER=191)) {
        # MSVS2017
        message("Generating for MSVS 2017")
        GLFW_LIB_DIR=$$GLFW_LIB_DIR/15.0
    }

    LIBS += -L$$GLFW_LIB_DIR

    # select the build mode
    CONFIG(release, debug|release){
        # release
        equals(MINGW,1) {
            LIBS+= -lglfw3
        } else {
            LIBS+= -lglfw3
            QMAKE_CFLAGS_RELEASE -= -MD
            QMAKE_CXXFLAGS_RELEASE -= -MD
            QMAKE_CXXFLAGS_RELEASE += -MT
            QMAKE_CFLAGS_RELEASE += -MT
        }
    }else{
        # debug
        equals(MINGW,1) {
            LIBS+= -lglfw3
        } else {
            LIBS+= -lglfw3d
            QMAKE_CFLAGS_DEBUG -= -MDd
            QMAKE_CXXFLAGS_DEBUG -= -MDd
            QMAKE_CXXFLAGS_DEBUG += -MTd
            QMAKE_CFLAGS_DEBUG += -MTd
        }
    }
    # GLFW dependencies
    LIBS+= \
    -lkernel32 \
    -luser32 \
    -lgdi32 \
    -lshell32

    INCLUDEPATH += dependencies/glad/include dependencies/GLFW/include source
}
