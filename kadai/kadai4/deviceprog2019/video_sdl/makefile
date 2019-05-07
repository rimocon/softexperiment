#
# Module:      makefile
# Author:      Akinori Tsuji
# Date:        2001/07/19(Thu) -  2010/02/21(Sun)
# Description: makefile for opencv
# 
# Created by Akinori TSUJI
#
.SUFFIXES:
.SUFFIXES: .s .c .cpp .h .o

TARGET = video_sdl

SRCS = $(TARGET).c ipl2surface.c # ソースファイルを追加

CC           = gcc
CPP          = g++
AS           = as
OBJCP        = objcopy
OBJDMP       = objdump
RM           = rm -f
LIBS         = -lm
DEBUG        = -g
#SDLFLAGS    = -I/usr/include/SDL
#OPENCVFLAGS = -I/usr/include/opencv
CFLAGS       = -c -Wall -I. -I./include $(DEBUG) $(SDLFLAGS) $(OPENCVFLAGS)
LINK_SDL     = -lSDL -lSDL_image -lSDL_gfx -lSDL_mixer -lSDL_net -lSDL_ttf
LINK_OPENCV  =  -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_imgproc

.s.o:
	$(AS) $< -o $@
.c.o:
	$(CC) $(CFLAGS) $< $(LINK_SDL) $(LINK_OPENCV)
.cpp.o:
	$(CPP) $(CFLAGS) $< $(LINK_SDL) $(LINK_OPENCV)

all: clean $(TARGET)

$(TARGET): $(SRCS:.c=.o)
	$(CC) -o $@ $(SRCS:.c=.o) $(LINK_OPENCV) $(LINK_SDL) $(LIBS)

code:
	gtags; htags;

clean:
	$(RM) $(SRCS:.c=.o) $(TARGET) *.bak
	$(RM) -rf HTML GTAGS GRTAGS GPATH GSYMS

