# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.1.1\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Girift

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Girift\cmake-build-debug

# Utility rule file for UE4Editor.

# Include the progress variables for this target.
include CMakeFiles\UE4Editor.dir\progress.make

CMakeFiles\UE4Editor:
	call "C:/Program Files/Epic Games/UE_4.26/Engine/Build/BatchFiles/Build.bat" UE4Editor Win64 Development -project=C:/Girift/Girift.uproject -game -progress -buildscw

UE4Editor: CMakeFiles\UE4Editor
UE4Editor: CMakeFiles\UE4Editor.dir\build.make

.PHONY : UE4Editor

# Rule to build all files generated by this target.
CMakeFiles\UE4Editor.dir\build: UE4Editor

.PHONY : CMakeFiles\UE4Editor.dir\build

CMakeFiles\UE4Editor.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\UE4Editor.dir\cmake_clean.cmake
.PHONY : CMakeFiles\UE4Editor.dir\clean

CMakeFiles\UE4Editor.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\Girift C:\Girift C:\Girift\cmake-build-debug C:\Girift\cmake-build-debug C:\Girift\cmake-build-debug\CMakeFiles\UE4Editor.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\UE4Editor.dir\depend

