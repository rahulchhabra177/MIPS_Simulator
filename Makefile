OBJS = ./code/main.cpp ./code/core.cpp ./code/mrm.cpp 
CC = g++
OBJ_NAME = main
all : $(OBJS)
	$(CC) $(OBJS) -o $(OBJ_NAME)
	
clean: $(OBJ_NAME)
	rm $(OBJ_NAME)