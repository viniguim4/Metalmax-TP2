CXX = g++
CXXFLAGS = -Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror 

SRCS = main.cpp network.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = tp2

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
