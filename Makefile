SRCS = srcs/logs.c srcs/main.c
FLAGS = /Wall /WX
RC_FILE = res\version.rc
RES_FILE = res\version.res

all: $(RES_FILE) winkey.exe svc.exe

$(RES_FILE): $(RC_FILE)
	rc /fo $(RES_FILE) $(RC_FILE)

winkey.exe: $(SRCS)
	cl $(FLAGS) /Fe$@ $** $(RES_FILE) user32.lib psapi.lib

svc.exe: service\svc.cpp
	$(CC) $(FLAGS) /Fe$@ $**

clean:
	del /Q *.obj 2>nul

fclean: clean
	del /Q res\*.res 2>nul
	del /Q *.exe 2>nul
	del /Q logs.txt 2>nul

re: fclean all
