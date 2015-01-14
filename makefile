CC  = /usr/bin/gcc

CFLAGS = `pkg-config --cflags --libs gtk+-3.0` -Wall -O2 -s
LDFLAGS = `pkg-config --libs gtk+-3.0`

SOURCES= buttons.c dir_read.c helper.c keyfile.c main.c temp_dir.c treeviewer.c
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=odt2pdf-gtk


all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf *o
