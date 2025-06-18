CC = cl
SRCS_DIR = srcs
FLAGS = /Wall  #/WX
SRCS = main.c \
	   logs.c

all: winkey.exe #svc.exe

winkey.exe: $(SRCS)
	cl $(FLAGS) /Fe$@ $** user32.lib psapi.lib

# winkey.exe: $(SRCDIR)\svc.c
# 	$(CC) $(FLAGS) /Fe$@ $**

clean:
	del /Q *.obj 2>nul

fclean: clean
	del /Q *.exe 2>nul
	del /Q logs.txt 2>nul

re: fclean all
