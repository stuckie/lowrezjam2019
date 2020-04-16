LIBS = -lm
CFLAGS   = -O3 -Wall -Werror -Wpedantic -Wno-variadic-macros -Wcast-align -Wredundant-decls -Wextra -std=c89 -pedantic -pedantic-errors -mtune=native `sdl2-config --cflags` -Igae/glesgae
LDFLAGS  = -O3 `sdl2-config --libs`
#CFLAGS   = -g -Wall -Werror -Wpedantic -Wno-variadic-macros -Wcast-align -Wredundant-decls -Wextra -std=c89 -pedantic -pedantic-errors -mtune=native `sdl2-config --cflags` -Igae/glesgae
#LDFLAGS  = -g `sdl2-config --libs`

TARGETS	 = gae/glesgae.a fishy

.PHONY: all
all: $(TARGETS)

gae/glesgae.a:
	cd gae && make && cd ..

fishy: 	gae/glesgae.a src/*.c
	$(CC) $(CFLAGS) -o $@ $+ gae/glesgae.a $(LDFLAGS) ${LIBS}

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true
	@cd gae && make clean && cd ..
