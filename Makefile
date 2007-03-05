# Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
# Copyright (C) 2005-2007 Harri Pitkänen <hatapitk@iki.fi>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#################################################################################

# Check that build environment is properly set up
ifndef OO_SDK_HOME
$(error You must run setsdkenv before running make)
endif
PRJ=$(OO_SDK_HOME)
# Load SDK settings
include $(PRJ)/settings/settings.mk
include $(PRJ)/settings/std.mk

# ===== Build settings =====

# Version number of the openoffice.org-voikko extension
VOIKKO_VERSION=1.9

# VOIKKO_DEBUG controls the amount of debugging information in the resulting UNO
# package. Possible values are NO (creates an optimized build without any
# debugging information), LOG (creates an optimized build with runtime debug
# logging) and FULL (creates a build with full debugging symbols and logging).
# Default is NO.
VOIKKO_DEBUG=FULL

# === End build settings ===

# Fix for Linux/SPARC. Needed until OpenOffice.org issue 72679 is fixed
ifeq "$(PROCTYPE)" "sparc"
	UNOPKG_PLATFORM=Linux_SPARC
endif

# Set up some variables
UNOPKG_EXT=oxt
LINK_FLAGS=$(COMP_LINK_FLAGS) -Wl,--no-undefined -L"$(OFFICE_PROGRAM_PATH)" \
           $(SALLIB) $(CPPULIB) $(CPPUHELPERLIB) -lvoikko
VOIKKO_CC_FLAGS=-Wall -Wno-non-virtual-dtor -Werror -Ibuild/hpp -I$(PRJ)/include/stl -I$(PRJ)/include
VOIKKO_CC_DEFINES=
ifeq "$(VOIKKO_DEBUG)" "NO"
        VOIKKO_PACKAGENAME=voikko
else
        VOIKKO_PACKAGENAME=voikko-dbg
        VOIKKO_CC_DEFINES+= -DVOIKKO_DEBUG_OUTPUT
endif
VOIKKO_PACKAGE=build/$(VOIKKO_PACKAGENAME).$(UNOPKG_EXT)
VOIKKO_EXTENSION_SHAREDLIB=voikko.$(SHAREDLIB_EXT)
VOIKKO_OBJECTS=registry common PropertyManager spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens
VOIKKO_HEADERS=macros common PropertyManager spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens
SED=sed

# Targets
.PHONY: all
all: $(VOIKKO_PACKAGE)


# Create extension files
MANIFEST_SEDSCRIPT="s/VOIKKO_EXTENSION_SHAREDLIB/$(VOIKKO_EXTENSION_SHAREDLIB)/g; \
	s/UNOPKG_PLATFORM/$(UNOPKG_PLATFORM)/g"
DESCRIPTION_SEDSCRIPT="s/VOIKKO_VERSION/$(VOIKKO_VERSION)/g"
build/oxt/META-INF/manifest.xml: oxt/META-INF/manifest.xml.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(MANIFEST_SEDSCRIPT) < $^ > $@

build/oxt/description.xml: oxt/description.xml.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(DESCRIPTION_SEDSCRIPT) < $^ > $@


# Type library C++ headers
build/hpp.flag:
	-$(MKDIR) build/hpp
	$(CPPUMAKER) -Gc -BUCR -O./build/hpp $(OFFICE_TYPE_LIBRARY)
	echo flagged > $@


# Compile the C++ source files
build/src/%.$(OBJ_EXT): src/%.cxx build/hpp.flag $(patsubst %,src/%.hxx,$(VOIKKO_HEADERS))
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CC) $(CC_FLAGS) $(VOIKKO_CC_FLAGS) $(CC_DEFINES) $(VOIKKO_CC_DEFINES) $(CC_OUTPUT_SWITCH) $@ $<


# Link the shared library
build/oxt/$(VOIKKO_EXTENSION_SHAREDLIB): $(patsubst %,build/src/%.$(OBJ_EXT),$(VOIKKO_OBJECTS))
	$(LINK) $(LINK_FLAGS) -o $@ $^


# Assemble the oxt extension under build/oxt
$(VOIKKO_PACKAGE) : build/oxt/META-INF/manifest.xml build/oxt/description.xml \
	          build/oxt/$(VOIKKO_EXTENSION_SHAREDLIB)
	cd build/oxt && $(SDK_ZIP) ../$(VOIKKO_PACKAGENAME).$(UNOPKG_EXT) \
	                           $(patsubst build/oxt/%,%,$^)
