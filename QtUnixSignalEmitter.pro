
PRE_TARGETDEPS += default.pri

QT -= gui
CONFIG += console
CONFIG -= debug release debug_and_release

# debug / release
CONFIG += debug
#CONFIG += release

# warnings
QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra
QMAKE_CXXFLAGS_WARN_ON += -Werror    # treat warnings as errors

# misc
#QMAKE_CXXFLAGS += -std=c++0x

# tuning
QMAKE_LFLAGS += -rdynamic
debug {
    QMAKE_CXXFLAGS += -O0 -fno-inline
}
release {
    QMAKE_CXXFLAGS += -g
    QMAKE_CXXFLAGS += -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -Wl,-gc-sections
}

# GCC flags
#QMAKE_CXXFLAGS +=
#INCLUDEPATH +=
#LIBS += -L. -lDanadamLog

# Qt modules
#QT += gui
#QT += network
#QT += sql

include(default.pri)
