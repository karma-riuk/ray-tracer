CPPFLAGS := -Wall -O3
.PHONY: clean run

ALL_SRCS = $(wildcard */*.cc)
ALL_SRCS += $(wildcard *.cc)
ALL_HPPS = $(wildcard */*.hpp)
ALL_HPPS += $(wildcard *.hpp)
SRCS = $(filter-out main.cc,$(ALL_SRCS))
OBJS = $(SRCS:.cc=.o)

main: $(OBJS) $(ALL_HPPS) main.cc
	g++ $(CPPFLAGS) -o $@ $(OBJS) main.cc

%.o: %.cc %.hpp 
	g++ $(CPPFLAGS) -c -o $@ $<

clean:
	rm -f main
	rm -f $(OBJS)

run: main
	./main
	@if [[ "$(shell ps aux | grep "feh.* image.ppm" | grep -v "grep")" = "" ]]; then (feh image.ppm&); fi

