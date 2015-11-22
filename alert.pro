QT += widgets
HEADERS =   inc/controldialog.h \
            inc/livescreen.h
SOURCES =   src/main.cpp \
            src/controldialog.cpp \
            src/livescreen.cpp
DESTDIR = output
OBJECTS_DIR = tmp
MOC_DIR = tmp

RC_ICONS = yellow-bell-512.ico

TARGET = alert
