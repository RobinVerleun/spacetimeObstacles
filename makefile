APPNAME="./src/main.c"
CC=gcc
CFLAGS=-g
CLINK=-L/usr/local/lib -L/user/X11R6/lib -lalleg -lpthread -lXxf86dga -lXxf86vm -ldl
EXEC= -o spacetimeObstacles
default:
	@$(CC) $(CFLAGS) $(APPNAME) $(EXEC) $(CLINK)

clean:
	rm spacetimeObstacles