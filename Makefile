
CXXFLAGS := -O2 -std=c++14

CURLFLAGS := $(shell pkg-config --cflags --libs libcurl)

TARGETS := test multi-stress

all: $(TARGETS)

test: test.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CURLFLAGS)

multi-stress: multi-stress.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CURLFLAGS)

clean:
	-rm $(TARGETS)
