#!/bin/sh

set -e

sed=sed
which gsed >/dev/null 2>/dev/null && sed=gsed

PATCH_FILE=lua/makefile
PATCH_FILE_TARGET=tmp/lua/makefile

mkdir -p tmp/lua

if [ "x$1" = "xclean" ]; then
    rm -f "$PATCH_FILE_TARGET"
    exit 0
fi

$sed -r '
# Replace AR with ARR
s/\$\(AR\)/$(ARR)/g;

# Remove some variables
s/^(CC|RANLIB|RM)=/#\0/;

# Replace AR assignment
s/^AR= *ar/ARR=$(AR)/;

# Remove -lreadline from MYLIBS
s/^(MYLIBS=.*)-lreadline$/\1/;

# Remove -DLUA_USE_READLINE from MYCFLAGS, add -flto, add use_sdl=0
s~^(MYCFLAGS=.*)-DLUA_USE_LINUX -DLUA_USE_READLINE$~\1 -DLUA_USE_DLOPEN -flto -s USE_SDL=0 -s USE_SDL_TTF=0 -s USE_SDL_IMAGE=0 -s USE_SDL_NET=0 -s USE_SDL_MIXER=0 -DLUA_CPATH_DEFAULT="\\"/?.so;/lua/modules/?.so\\"" -DLUA_PATH_DEFAULT="\\"/?.lua;/?/init.lua;/lua/modules/?.lua;/lua/modules/?/init.lua\\""~;

# Remove -march=native
s~-march=native~~g;

# Remove CWARNGCC from CWARNS
s/^(CWARNS=.*)\$\(CWARNGCC\)$/\1/;
' "$PATCH_FILE" > "$PATCH_FILE_TARGET"
