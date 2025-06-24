# TinkyWinkey

## Table of Contents
1. Introduction
2. Technologies Used
3. Service Tinky
4. Keylogger Winkey
5. How to Use
6. Project Structure

## Introduction

TinkyWinkey is a Windows-based project that demonstrates the implementation of a Windows service and keylogger system. The project consists of two main components:

- **Tinky Service**: A Windows service that manages and launches the keylogger
- **Winkey Keylogger**: A keylogger application that captures keyboard input and system information

This project explores Windows system programming concepts including service management, low-level keyboard hooks, process management, and system information gathering.

## Technologies Used

- **Language**: C/C++
- **Compiler**: Microsoft Visual C++ (CL)
- **Build System**: NMAKE with custom Makefile
- **Platform**: Windows (x86/x64)
- **APIs Used**:
  - Windows Service Control Manager (SCM) API
  - Windows Hooks API
  - Windows Terminal Services API
  - Windows Socket API (Winsock2)
  - Windows File API

## Service Tinky

The Tinky service (service/svc.cpp) is a Windows service that provides the following functionality:

### Features
- **Service Management**: Install, start, stop, and delete the service
- **Process Management**: Launches and manages the `winkey.exe` keylogger process
- **Session Management**: Uses Windows Terminal Services to run the keylogger in the active user session
- **Service Name**: "### Tinky ###" (configurable in service/svc.hpp)

### Key Components
- `InstallService()`: Creates and registers the service with the SCM
- `StartService()`: Starts the service
- `StopService()`: Stops the running service
- `DeleteService()`: Removes the service from the system
- `ServiceMain()`: Main service entry point
- `ServiceWorkerThread()`: Worker thread that launches `winkey.exe`

### Usage
```cmd
svc.exe install    # Install the service
svc.exe start      # Start the service
svc.exe stop       # Stop the service
svc.exe delete     # Delete the service
```

## Keylogger Winkey

The Winkey keylogger has two implementations:

### Standalone Executable (keylog/exemain.c)
- Runs as a standalone application
- Logs to `logs.txt` in the current directory

### DLL Version (keylog/dllmain.c)
- Can be injected into other processes using the injector
- Logs to `logs.txt` in the system temp directory

### Key Features

#### System Information Gathering
- **CPU Information**: Vendor, model, architecture, and processor count
- **Windows Version**: OS version and build number
- **Network Information**: Local IP address
- **Memory Information**: RAM size
- **Implementation**: `sys_getter.c`

#### Keylogging Capabilities
- **Low-level Keyboard Hook**: Captures all keyboard input system-wide
- **Special Key Detection**: Function keys, arrow keys, modifier keys, media keys
- **Unicode Support**: Properly handles international characters
- **Keyboard Layout Detection**: Monitors keyboard layout changes
- **Implementation**: `LowLevelKeyboardProc()` in `keylogger.c`

#### Window Tracking
- **Foreground Window Detection**: Tracks active window changes
- **Window Title Logging**: Records window titles with timestamps
- **Implementation**: `WinForeground()` in `init_logger.c`

#### Logging System
- **File-based Logging**: All data is written to log files
- **Timestamp Integration**: Each entry includes date/time information
- **UTF-8 Support**: Proper encoding for international text
- **Implementation**: `write_to_file()` in `keylogger.c`

### Supported Key Types
- Alphanumeric characters
- Function keys (F1-F24)
- Navigation keys (arrows, home, end, page up/down)
- Modifier keys (shift, ctrl, alt, windows key)
- Special keys (escape, enter, backspace, delete, insert)
- Media keys (volume, play/pause, next/previous track)
- System keys (print screen, pause, sleep)

## How to Use

### Building the Project

```cmd
# Build all components
nmake all

# Build only the keylogger DLL and injector (bonus)
nmake bonus

# Clean build artifacts
nmake clean

# Complete clean including executables
nmake fclean
```

### Running the System

1. **Install and start the service**:
   ```cmd
   svc.exe install
   svc.exe start
   ```

2. **Verify the service is running**:
   ```cmd
   sc query "### Tinky ###"
   ```

3. **Check if keylogger is active**:
   ```cmd
   tasklist | findstr winkey
   ```

4. **Stop and remove the service**:
   ```cmd
   svc.exe stop
   svc.exe delete
   ```

### Using the DLL Injector (Bonus)

```cmd
# Inject the keylogger DLL into a target process
injector.exe notepad.exe
```

## Project Structure

```
TinkyWinkey/
├── service/              # Service implementation
│   ├── svc.cpp          # Main service code
│   └── svc.hpp          # Service headers and definitions
├── keylog/              # Keylogger implementation
│   ├── keylogger.c      # Core keylogging functionality
│   ├── keylogger.h      # Keylogger headers
│   ├── logs.h           # Logging system headers
│   ├── exemain.c        # Standalone executable entry point
│   ├── dllmain.c        # DLL entry point
│   ├── init_logger.c    # Logger initialization
│   ├── sys_getter.c     # System information gathering
│   └── injector.c       # DLL injection utility
├── res/                 # Resources
│   ├── version.rc       # Version resource file
│   └── winkey.ico       # Application icon
├── Makefile            # Build configuration
└── README.md           # This file
```

### Key Files
- **`Makefile`**: Build configuration using NMAKE
- **`service/svc.hpp`**: Service configuration and global variables
- **`keylog/keylogger.h`**: Main keylogger headers and function declarations
- **`keylog/logs.h`**: Logging system definitions

The project demonstrates advanced Windows programming concepts including service development, system hooks, process management, and inter-process communication.