CC = cl
SRCS_DIR = srcs
FLAGS = /Wall  #/WX
SRCS = main.c \
	   logs.c

all: svc.exe #winkey.exe

svc.exe: $(SRCS_DIR)/main.c $(SRCS_DIR)/logs.c
	$(CC) $(FLAGS) /Fe$@ $** user32.lib psapi.lib

# winkey.exe: $(SRCDIR)\winkey.c
# 	$(CC) $(FLAGS) /Fe$@ $**

clean:
	del /Q *.obj 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: fclean all
