CFLAGS   = -O3 -Wall -Werror -Wpedantic -Wno-variadic-macros -Wcast-align -Wredundant-decls -Wextra -std=c89 -pedantic -pedantic-errors -mtune=native `sdl2-config --cflags` -Iglesgae
LDFLAGS  = -O3 `sdl2-config --libs`

TARGETS	 = fishy

.PHONY: all
all: $(TARGETS)

fishy: 	glesgae/*.c src/*.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS)

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true
