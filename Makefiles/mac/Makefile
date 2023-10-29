#=============================================================================
# Makefile for MacOS
#=============================================================================

CC=clang

INCLUDE_DIR=-I/Library/Frameworks/SDL2.framework/Headers -I$(CRUMBS_HOME)/include -I../..
LINK_DIR=-F/Library/Frameworks -L$(CRUMBS_HOME)/lib

CFLAGS=-Wall -Werror
LIBS=-lcrumbs -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer

# This is needed to get access to SDL's main function without having to
# include the SDL header file.
MAIN_DEF=-D_main=_SDL_main

D=../..
SRC=$(D)/*.c $(D)/common/*.c $(D)/entities/*.c $(D)/input/*.c $(D)/scenes/*.c $(D)/util/*.c

all:
	$(CC) $(MAIN_DEF) $(SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)

debug:
	$(CC) -g $(MAIN_DEF) $(SRC) -o tootnsploot.out $(INCLUDE_DIR) $(LINK_DIR) $(LIBS)
