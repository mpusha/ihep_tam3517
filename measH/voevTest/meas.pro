QT += core
# v1.0 13.04.2017 change write BT chan view/ Star from 1
TARGET = sh_test
TEMPLATE = app
CONFIG += exception
MOC_DIR = moc
OBJECTS_DIR = obj


SOURCES += sh_test.c

HEADERS  += ccydalib/sh_ccydalib.h 

OBJECTS += ccydalib/sh_ccydalib.o

LIBS += ccydalib/ccydalib.a
LIBS += -lrt \
        -lresolv
