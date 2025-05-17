# Compiler
CXX       := g++

# Homebrew prefixes
MLPACK    := /opt/homebrew/opt/mlpack
ARMADILLO := /opt/homebrew/opt/armadillo
BOOST     := /opt/homebrew/opt/boost
CEREAL    := /opt/homebrew/opt/cereal
ENSMALLEN := /opt/homebrew/opt/ensmallen

# Include directories
INCLUDES  := \
  -Iinclude \
  -I$(MLPACK)/include \
  -I$(ARMADILLO)/include \
  -I$(BOOST)/include \
  -I$(CEREAL)/include \
  -I$(ENSMALLEN)/include

# Libraries & linker flags
LDFLAGS   := \
  -L$(MLPACK)/lib \
  -L$(ARMADILLO)/lib \
  -L$(BOOST)/lib

LIBS      := \
  -lmlpack \
  -larmadillo \
  -lboost_serialization \
  -lboost_program_options

# Compiler flags
CXXFLAGS  := -std=gnu++17 -Wall -g $(INCLUDES) \
  -Wno-unused-function \
  -Wno-deprecated-declarations \
  -Wno-unneeded-internal-declaration

# Sources & target
SRC       := $(wildcard src/*.cpp)
TARGET    := StrokeRiskSystem

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET) src/*.o
