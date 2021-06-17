OBJS = ./Refined/main.cpp ./Refined/core.cpp ./Refined/mrm.cpp 
CC = g++
OBJ_NAME = main
all : $(OBJS)
	$(CC) $(OBJS) -o $(OBJ_NAME)
	
clean: $(OBJ_NAME)
	rm $(OBJ_NAME)