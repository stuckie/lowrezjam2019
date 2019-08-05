rm fishy.bc
emmake make -f Makefile
mv fishy fishy.bc
emcc -O3 fishy.bc -o fishy.js -s USE_SDL=2 --embed-file data -s 'EXPORTED_FUNCTIONS=["_main", "_emscripten_fullscreen"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall"]'

