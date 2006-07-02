# This file contains compilation-related setting that are common
# between different spellchecker and hyphenator implementations.

LF_CONFIGURATOR = oo2-lingconfig

# LF_DEBUG controls the amount of debugging information in the resulting UNO
# package. Possible values are NO (creates an optimized build without any
# debugging information), LOG (creates an optimized build with runtime debug
# logging) and FULL (creates a build with full debugging symbols and logging).
# Default is NO.
LF_DEBUG=NO

# Compiler settings for Linux. Leave these all commented unless you need to
# use a non-default compiler.

#ifeq "$(PLATFORM)" "linux"
	# C++ compiler command (default is to use SDK configuration)
#	LF_CC=gcc-3.4

	# Linker command (default is to use SDK configuration)
#	LF_LINK=g++-3.4

	# C++ include path (default is the one for C++ compiler in SDK configuration)
#	SDK_GXX_INCLUDE_PATH=/usr/include/c++/3.4
#endif

# Fix for Linux/x86_64. Needed until OpenOffice.org issue 66162 is fixed.
ifeq "$(PROCTYPE)" "x86_64"
	UNOPKG_PLATFORM=Linux_x86_64
endif

# Fix compilation when C++ compiler in SDK configuration is gcc 4.0
# Not needed when building with OpenOffice.org >= 2.0.1
#ifeq "$(PLATFORM)" "linux"
#	CPPUHELPERLIB=-luno_cppuhelpergcc3
#	CPPU_ENV=gcc3
#endif

ifeq "$(LF_DEBUG)" "NO"
	LF_PACKAGENAME=$(LF_SHORTNAME)-$(UNOPKG_PLATFORM)-$(LF_VERSION)
else
	LF_PACKAGENAME=$(LF_SHORTNAME)-$(UNOPKG_PLATFORM)-dbg-$(LF_VERSION)
endif
LF_PACKAGE=$(LF_OUTDIR)/$(LF_PACKAGENAME).$(UNOPKG_EXT)
