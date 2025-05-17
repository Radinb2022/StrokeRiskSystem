CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = StrokeRiskSystem
CXXFLAGS = -std=gnu++14 -Iinclude -g

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) src/*.o
