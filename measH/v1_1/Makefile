#############################################################################
# Makefile for building: measH
# Generated by qmake (2.01a) (Qt 4.8.1) on: Tue Apr 3 16:03:21 2018
# Project:  meas.pro
# Template: app
# Command: /home/serg/tam3517/qt4.8.1/usr/local/Qt481/bin/qmake2 -o Makefile meas.pro
#############################################################################

####### Compiler, tools and options

CC            = arm-none-linux-gnueabi-gcc
CXX           = arm-none-linux-gnueabi-g++
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O3 -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O3 -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/qws/linux-TIarmv7-g++ -I. -I../../../../tam3517/qt4.8.1/usr/local/Qt481/include/QtCore -I../../../../tam3517/qt4.8.1/usr/local/Qt481/include/QtNetwork -I../../../../tam3517/qt4.8.1/usr/local/Qt481/include/QtGui -I../../../../tam3517/qt4.8.1/usr/local/Qt481/include -Imoc -I../../../../tam3517/tslib/include -IC
LINK          = arm-none-linux-gnueabi-g++
LFLAGS        = -Wl,-O1 -Wl,-rpath,/usr/local/Qt481/lib
LIBS          = $(SUBLIBS)  -L/home/serg/tam3517/tslib/lib -L/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib ccydalib/ccydalib.a -lrt -lresolv -lQtGuiE -L/usr/local/Qt481/lib -L/home/serg/tam3517/tslib/lib -lQtNetworkE -lQtCoreE -lts -lpthread 
AR            = arm-none-linux-gnueabi-ar cqs
RANLIB        = 
QMAKE         = /home/serg/tam3517/qt4.8.1/usr/local/Qt481/bin/qmake2
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = arm-none-linux-gnueabi-strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = main.cpp \
		db.c \
		meBehav.cpp \
		serial.cpp \
		gpio.c \
		dbproc.cpp moc/moc_meBehav.cpp \
		moc/moc_dbproc.cpp
OBJECTS       = ccydalib/sh_ccydalib.o \
		obj/main.o \
		obj/db.o \
		obj/meBehav.o \
		obj/serial.o \
		obj/gpio.o \
		obj/dbproc.o \
		obj/moc_meBehav.o \
		obj/moc_dbproc.o
DIST          = ../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/linux.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base-unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-base.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/qws.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/qconfig.pri \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/modules/qt_webkit_version.pri \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_functions.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_config.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/exclusive_builds.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_pre.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/release.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_post.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/warn_on.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/unix/thread.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/moc.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/resources.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/uic.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/yacc.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/lex.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/include_source_dir.prf \
		meas.pro
QMAKE_TARGET  = measH
DESTDIR       = 
TARGET        = measH

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: meas.pro  ../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/qws/linux-TIarmv7-g++/qmake.conf ../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/linux.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base-unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-base.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-unix.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/qws.conf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/qconfig.pri \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/modules/qt_webkit_version.pri \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_functions.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_config.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/exclusive_builds.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_pre.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/release.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_post.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/warn_on.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/unix/thread.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/moc.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/resources.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/uic.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/yacc.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/lex.prf \
		../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/include_source_dir.prf \
		/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtGuiE.prl \
		/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtNetworkE.prl \
		/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtCoreE.prl
	$(QMAKE) -o Makefile meas.pro
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/unix.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/linux.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/gcc-base-unix.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-base.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/g++-unix.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/common/qws.conf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/qconfig.pri:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/modules/qt_webkit_version.pri:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_functions.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt_config.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/exclusive_builds.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_pre.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/release.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/default_post.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/warn_on.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/qt.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/unix/thread.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/moc.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/resources.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/uic.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/yacc.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/lex.prf:
../../../../tam3517/qt4.8.1/usr/local/Qt481/mkspecs/features/include_source_dir.prf:
/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtGuiE.prl:
/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtNetworkE.prl:
/home/serg/tam3517/qt4.8.1/usr/local/Qt481/lib/libQtCoreE.prl:
qmake:  FORCE
	@$(QMAKE) -o Makefile meas.pro

dist: 
	@$(CHK_DIR_EXISTS) obj/measH1.0.0 || $(MKDIR) obj/measH1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) obj/measH1.0.0/ && $(COPY_FILE) --parents db.h meBehav.h serial.h ccydalib/sh_ccydalib.h dbproc.h obj/measH1.0.0/ && $(COPY_FILE) --parents main.cpp db.c meBehav.cpp serial.cpp gpio.c dbproc.cpp obj/measH1.0.0/ && (cd `dirname obj/measH1.0.0` && $(TAR) measH1.0.0.tar measH1.0.0 && $(COMPRESS) measH1.0.0.tar) && $(MOVE) `dirname obj/measH1.0.0`/measH1.0.0.tar.gz . && $(DEL_FILE) -r obj/measH1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc/moc_meBehav.cpp moc/moc_dbproc.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc/moc_meBehav.cpp moc/moc_dbproc.cpp
moc/moc_meBehav.cpp: serial.h \
		meBehav.h
	/home/serg/tam3517/qt4.8.1/usr/local/Qt481/bin/moc $(DEFINES) $(INCPATH) meBehav.h -o moc/moc_meBehav.cpp

moc/moc_dbproc.cpp: meBehav.h \
		serial.h \
		dbproc.h
	/home/serg/tam3517/qt4.8.1/usr/local/Qt481/bin/moc $(DEFINES) $(INCPATH) dbproc.h -o moc/moc_dbproc.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

obj/main.o: main.cpp meBehav.h \
		serial.h \
		dbproc.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o main.cpp

obj/db.o: db.c db.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o obj/db.o db.c

obj/meBehav.o: meBehav.cpp db.h \
		meBehav.h \
		serial.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/meBehav.o meBehav.cpp

obj/serial.o: serial.cpp serial.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/serial.o serial.cpp

obj/gpio.o: gpio.c 
	$(CC) -c $(CFLAGS) $(INCPATH) -o obj/gpio.o gpio.c

obj/dbproc.o: dbproc.cpp dbproc.h \
		meBehav.h \
		serial.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/dbproc.o dbproc.cpp

obj/moc_meBehav.o: moc/moc_meBehav.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/moc_meBehav.o moc/moc_meBehav.cpp

obj/moc_dbproc.o: moc/moc_dbproc.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/moc_dbproc.o moc/moc_dbproc.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

