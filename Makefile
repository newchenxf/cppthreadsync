# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

LINK = -lpthread

HEADER_PATH=src
IHEADER = -I$(HEADER_PATH)

# The build target 
TARGET = mockService

#源文件
DEPEND = src/*.cpp src/base/*.cpp
#DEPEND = src/main.cpp src/MainController.cpp src/base/ThreadTaskQueue.cpp src/base/WatchDog.cpp src/base/logutils.cpp

all: $(TARGET)

$(TARGET): $(DEPEND)
	$(CC) $(CFLAGS) $(IHEADER) -o $(TARGET) $(DEPEND) $(LINK)

clean:
			$(RM) $(TARGET)