QT += core
# v1.0 31.01.2018 Start version program of measuring MF
TARGET = measH
TEMPLATE = app
CONFIG += exception
MOC_DIR = moc
OBJECTS_DIR = obj


SOURCES +=  main.cpp db.c meBehav.cpp serial.cpp \
    gpio.c
HEADERS  += db.h meBehav.h serial.h \
            ccydalib/sh_ccydalib.h 

OBJECTS += ccydalib/sh_ccydalib.o

LIBS += ccydalib/ccydalib.a
LIBS += -lrt \
        -lresolv
