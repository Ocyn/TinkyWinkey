CC = cl
SRCS_DIR = srcs
FLAGS = /Wall /WX /wd4668 /wd5045

all: svc.exe #winkey.exe

svc.exe: $(SRCS_DIR)\test.c
	$(CC) $(FLAGS) /Fe$@ $** user32.lib psapi.lib

# winkey.exe: $(SRCDIR)\winkey.c
# 	$(CC) $(FLAGS) /Fe$@ $**

clean:
	del /Q *.obj 2>nul

fclean: clean
	del /Q *.exe 2>nul

re: fclean all
