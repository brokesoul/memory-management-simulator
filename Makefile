CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC = src/main.cpp \
      src/allocator/Allocator.cpp \
      src/buddy/BuddyAllocator.cpp \
      src/cache/Cache.cpp \
      src/virtual_memory/VirtualMemory.cpp

OUT = memsim

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
