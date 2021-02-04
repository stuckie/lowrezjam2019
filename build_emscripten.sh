#!/bin/bash
embuilder build sdl2 sdl2-mixer-ogg sdl2-image-png
emmake make -f Makefile.emscripten
