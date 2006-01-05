# This file contains compilation-related setting that are common
# between different spellchecker and hyphenator implementations.

# LF_DEBUG controls the amount of debugging information in the resulting UNO
# package. Possible values are NO (creates an optimized build without any
# debugging information), LOG (creates an optimized build with runtime debug
# logging) and FULL (creates a build with full debugging symbols and logging).
# Default is NO.
LF_DEBUG=NO

# Compiler settings. Leave these all commented unless you need to use non-default
# compiler

# C++ compiler command (default is to use SDK configuration)
#ifeq "$(PLATFORM)" "linux"
#	LF_CC=gcc-3.3
#endif

# Linker command (default is to use SDK configuration)
#ifeq "$(PLATFORM)" "linux"
#	LF_LINK=g++-3.3
#endif

# C++ include path (default is the one for C++ compiler in SDK configuration)
#ifeq "$(PLATFORM)" "linux"
#	SDK_GXX_INCLUDE_PATH=/usr/include/c++/3.3
#endif


# Fix compilation when C++ compiler in SDK configuration is gcc 4.0
# Should not be needed anymore
#ifeq "$(PLATFORM)" "linux"
#	CPPUHELPERLIB=-luno_cppuhelpergcc3
#	CPPU_ENV=gcc3
#endif
