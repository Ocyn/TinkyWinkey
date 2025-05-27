
CC = cl
SRCS_DIR = srcs
FLAGS =  /Wall /WX

all: svc.exe winkey.exe

svc.exe: $(SRCS_DIR)\svc.c
	$(CC) $(FLAGS) /Fe$@ $**

winkey.exe: $(SRCDIR)\winkey.c
	$(CC) $(FLAGS) /Fe$@ $**

clean:
	del /Q *.obj 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: clean all
