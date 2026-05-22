CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = tarsau
OBJS = main.o arsivleme.o arsivdencikarma.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c tarsau.h
	$(CC) $(CFLAGS) -c main.c

arsivleme.o: arsivleme.c tarsau.h
	$(CC) $(CFLAGS) -c arsivleme.c

arsivdencikarma.o: arsivdencikarma.c tarsau.h
	$(CC) $(CFLAGS) -c arsivdencikarma.c

clean:
	rm -f $(TARGET) *.o *.sau