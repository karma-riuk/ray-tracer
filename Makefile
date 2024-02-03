CPPFLAGS := -Wall -O3
.PHONY: all clean run

all: main

clean:
	rm -f main

run: main
	./main
	@if [[ "$(shell ps aux | grep "feh.* image.ppm" | grep -v "grep")" = "" ]]; then (feh image.ppm&); fi

