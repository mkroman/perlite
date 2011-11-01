CXX ?= g++
CXXFLAGS = -Wall -Werror -g3 -I/usr/local/include -I/usr/include
LDFLAGS = -L/usr/local/lib -L /usr/lib
SOURCES = src/Perlite.cpp src/Client.cpp src/Network.cpp src/Command.cpp src/Channel.cpp src/User.cpp src/ISupport.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = perlite

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@

clean:
	-rm -f $(OBJECTS) $(OBJECTS) $(EXECUTABLE)

.cpp.o:
	$(CXX) -std=c++98 -c $< $(CXXFLAGS) $(LDFLAGS) -o $@
