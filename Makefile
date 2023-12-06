CC := gcc
CFLAGS := -Wall -Wextra
SRCS := client.c
OBJS := $(SRCS:.c=.o)
TARGET = client 


.PHONY: build clean


build: $(TARGET)


run: build 
	./$(TARGET)


%.c: %.o
	$(CC) $(CFLAGS) -c $< $@


$(TARGET): $(OBJS)
	$(CC) $< -o $(TARGET) 


clean:
	rm -f $(OBJS) $(TARGET)
