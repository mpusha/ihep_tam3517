QT += core
# v1.0 13.04.2017 change write BT chan view/ Star from 1
TARGET = BtimerU70
TEMPLATE = app
CONFIG += exception
MOC_DIR = moc
OBJECTS_DIR = obj


SOURCES += main.cpp \
           bt.cpp serial.cpp \
           db.c gpio.c

HEADERS  += ccydalib/sh_ccydalib.h \
            bt.h serial.h \
            db.h gpio.h

OBJECTS += ccydalib/sh_ccydalib.o

LIBS += ccydalib/ccydalib.a
LIBS += -lrt \
        -lresolv
