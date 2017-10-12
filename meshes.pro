CONFIG += c++11

TARGET = meshes
CONFIG += console
CONFIG -= app_bundle

HEADERS       = types.h \
                mesh.h \
                halfedge.h \
                shape.h \
                debugging.h
SOURCES       = main_meshes.cpp \
                halfedge.cpp \
                mesh.cpp \
                shape.cpp
