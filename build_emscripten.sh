emmake make -f Makefile
mv fishy fishy.bc
emcc -O3 fishy.bc -o fishy.html -s USE_SDL=2 --embed-file data
