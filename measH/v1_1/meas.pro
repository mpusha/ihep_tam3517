QT += core
# v1.0 31.01.2018 Start version program of measuring MF
# v1.1 09.02.2018 Remove processind db in new thread
TARGET = measH
TEMPLATE = app
CONFIG += exception
MOC_DIR = moc
OBJECTS_DIR = obj


SOURCES +=  main.cpp db.c meBehav.cpp serial.cpp \
    gpio.c \
    dbproc.cpp
HEADERS  += db.h meBehav.h serial.h \
            ccydalib/sh_ccydalib.h \ 
    dbproc.h

OBJECTS += ccydalib/sh_ccydalib.o

LIBS += ccydalib/ccydalib.a
LIBS += -lrt \
        -lresolv
