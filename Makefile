CC = g++

SRC_DIR=./src

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
INC_DIRS=./include
INC_FLAGS=$(addprefix -I,$(INC_DIRS))
CPPFLAGS=-g -Wall -std=c++11 -std=gnu++11 -lstdc++ -Wfatal-errors
TARGET=safra

default: $(TARGET)

$(TARGET): $(SRCS)
	g++ -o $(TARGET) $(CPPFLAGS) $(INC_FLAGS) $(SRCS)
