#=============================================================================
# Makefile for MacOS
#=============================================================================

CC=clang

# This file expects SDL2 to be installed on the system.
# For example, SDL2 could be built from source and installed in
# /usr/local/lib with the actual headers in /usr/local/include,or it could be
# installed in /Library/Frameworks and the headers in /usr/local/include could
# be symbolic links to to headers in the frameworks.

INCLUDE_DIR=-I../..
LINK_DIR=-F/Library/Frameworks

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
