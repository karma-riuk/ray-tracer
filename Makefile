CPPFLAGS := -Wall -O3
.PHONY: clean run

ALL_SRCS = $(wildcard objects/*.cc)
ALL_SRCS += $(wildcard *.cc)
SRCS = $(filter-out main.cc,$(ALL_SRCS))
OBJS = $(SRCS:.cc=.o)

main: $(OBJS)

%.o: %.cc %.hpp 
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f main
	rm -f $(OBJS)

run: main
	./main
	@if [[ "$(shell ps aux | grep "feh.* image.ppm" | grep -v "grep")" = "" ]]; then (feh image.ppm&); fi

