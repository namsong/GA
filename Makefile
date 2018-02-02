TARGET = ga
OBJS = main.o ga.o genome.o pop.o util.o
CFLAGS = -O2 -g -Wall -std=c99
LIBS = -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	gcc -o $@ $(OBJS) $(LIBS) 

%.o: %.c
	gcc -c -o $@ ${CFLAGS} $^

clean:
	rm -f $(OBJS) $(TARGET)

