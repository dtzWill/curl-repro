
CXXFLAGS := -O2 -std=c++14

CURLFLAGS := $(pkg-config --cflags --libs libcurl)

test: test.cpp
	$(CXX) $^ -o $@ $(CXXFLAGS) $(CURLFLAGS)

clean:
	-rm test
