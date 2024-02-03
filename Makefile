CPPFLAGS := -Wall -O3
.PHONY: clean run

main: color.o objects/sphere.o

clean:
	rm -f main

run: main
	./main
	@if [[ "$(shell ps aux | grep "feh.* image.ppm" | grep -v "grep")" = "" ]]; then (feh image.ppm&); fi

