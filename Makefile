CC = gcc
JC = javac
CFLAGS = -Wall

SHELL_SRC = shell.c
SHELL_OUT = shell

JAVA_COMMANDS_SRC = JavaCommands.java
JAVA_COMMANDS_CLASS = JavaCommands.class

all: $(SHELL_OUT) $(JAVA_COMMANDS_CLASS)

$(SHELL_OUT): $(SHELL_SRC)
	$(CC) $(CFLAGS) -o $(SHELL_OUT) $(SHELL_SRC)

$(JAVA_COMMANDS_CLASS): $(JAVA_COMMANDS_SRC)
	$(JC) $(JAVA_COMMANDS_SRC)

clean:
	rm -f $(SHELL_OUT) $(JAVA_COMMANDS_CLASS)
