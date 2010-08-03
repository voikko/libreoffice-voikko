# Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
# Copyright (C) 2005-2009 Harri Pitkänen <hatapitk@iki.fi>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
##########################################################################


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
VOIKKO_VERSION=3.1.2

# VOIKKO_DEBUG controls the amount of debugging information in the resulting UNO
# package. Possible values are NO (creates an optimized build without any
# debugging information), LOG (creates an optimized build with runtime debug
# logging) and FULL (creates a build with full debugging symbols and logging).
VOIKKO_DEBUG=NO

# If you have installed libvoikko to some non-standard location, uncomment the
# following and adjust the path accordingly. For OS X this must be set if
# a standalone extension is to be built.
# LIBVOIKKO_PATH=/usr/local/voikko
# LIBVOIKKO_PATH=c:/voikko
# LIBVOIKKO_PATH=/Users/paakayttaja/voikko

# If you want to have all of the library and dictionary files included within
# the extension package, uncomment the following and adjust the path to point
# to a directory containing the required files.
# STANDALONE_EXTENSION_PATH=extras
# STANDALONE_EXTENSION_PATH=/Users/paakayttaja/voikko/lib/voikko

# If you want to have a license text to be displayed upon the installation
# of this extension, uncomment the following.
# SHOW_LICENSE=1

# If you want to enable the grammar checker, set this option to 1
ENABLE_GRAMMAR_CHECKER=1

# Setting this option to 1 causes ugly warnings to be added to visible places
# in the extension without removing any functionality (codename "tekstintuho").
# Useful for binary builds that are released for public testing.
# SHOW_UGLY_WARNINGS=1

# Destination directory when installing unpacked extension with
# make install-unpacked
DESTDIR=/usr/lib/openoffice.org-voikko

# === End build settings ===

# Fix for Intel OS X, see issues 69944 and 86121.
ifeq "$(PLATFORM)" "macosx"
	PROCTYPE=$(shell $(PRJ)/config.guess | cut -d "-" -f1)
	ifeq "$(PROCTYPE)" "i686"
		UNOPKG_PLATFORM=MacOSX_x86
	endif
endif

# Platform specific variables
ifeq "$(PLATFORM)" "windows"
	WARNING_FLAGS=-Wall -WX -wd4061 -wd4365 -wd4514 -wd4625 -wd4626 -wd4668 -wd4711 -wd4820
	# The following warnings should be fixed in the future
	WARNING_FLAGS+= -wd4640
	COPYDIR=xcopy /E /I
	PS="\"
else
	WARNING_FLAGS=-Wall -Wno-non-virtual-dtor -Werror
	COPYDIR=cp -r
endif
ifeq "$(PLATFORM)" "linux"
	LINKER_FLAGS=-Wl,--no-undefined
endif


# General variables
ifeq "$(VOIKKO_DEBUG)" "FULL"
	ifeq "$(PLATFORM)" "windows"
		OPT_FLAGS=-Od -Z7
	else
		OPT_FLAGS=-O0 -g
	endif
else
	OPT_FLAGS=-O2
	ifneq "$(PLATFORM)" "windows"
		WARNING_FLAGS+= -fno-strict-aliasing
	endif
endif
# separated generic link flags and linked libs are needed to build with -Wl,--as-needed
# this flag has been enabled by default on openSUSE-11.2
LINK_FLAGS=$(COMP_LINK_FLAGS) $(OPT_FLAGS) $(LINKER_FLAGS)
ifneq "$(PLATFORM)" "macosx"
	LINK_LIBS+=$(SALLIB) $(CPPULIB) $(CPPUHELPERLIB)
endif
VOIKKO_CC_FLAGS=$(OPT_FLAGS) $(WARNING_FLAGS) -Ibuild/hpp -I$(PRJ)/include/stl -I$(PRJ)/include

ifdef STANDALONE_EXTENSION_PATH
	VOIKKO_CC_DEFINES= -DVOIKKO_STANDALONE_EXTENSION
	ifeq "$(PLATFORM)" "windows"
		STANDALONE_EXTENSION_FILES=libvoikko-1.dll 2
	else
		ifeq "$(PLATFORM)" "macosx"
			STANDALONE_EXTENSION_FILES=2
		else
			STANDALONE_EXTENSION_FILES=libvoikko.so.1 2
			LINK_LIBS += -lvoikko
		endif
	endif
else
	VOIKKO_CC_DEFINES=
	STANDALONE_EXTENSION_FILES=
	LINK_LIBS += -lvoikko
endif

ifndef ENABLE_GRAMMAR_CHECKER
	VOIKKO_CC_DEFINES += -DDISABLE_GRAMMAR_CHECKER
endif

# Build extension package name
ifdef SHOW_UGLY_WARNINGS
        VOIKKO_PACKAGENAME:=tekstintuho
        VOIKKO_CC_DEFINES += -DTEKSTINTUHO
else
        VOIKKO_PACKAGENAME:=voikko
endif
ifneq "$(VOIKKO_DEBUG)" "NO"
        VOIKKO_PACKAGENAME:=$(VOIKKO_PACKAGENAME)-dbg
        VOIKKO_CC_DEFINES+= -DVOIKKO_DEBUG_OUTPUT
endif

ifdef LIBVOIKKO_PATH
	LINK_FLAGS+= -L$(LIBVOIKKO_PATH)/lib
	VOIKKO_CC_FLAGS+= -I$(LIBVOIKKO_PATH)/include
endif

VOIKKO_EXTENSION_SHAREDLIB=voikko.$(SHAREDLIB_EXT)
VOIKKO_OBJECTS=registry common PropertyManager spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
VOIKKO_HEADERS=macros common PropertyManager spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
COPY_TEMPLATES=config.xcu config.xcs icon.png SettingsDialog.xdl SettingsDialog_en_US.properties \
               SettingsDialog_fi_FI.properties SettingsDialog.xcu Linguistic.xcu
ifdef SHOW_LICENSE
	COPY_TEMPLATES+=license_fi.txt license_en-US.txt
endif
SRCDIST=COPYING Makefile README ChangeLog $(patsubst %,src/%.hxx,$(VOIKKO_HEADERS)) \
        $(patsubst %,src/%.cxx,$(VOIKKO_OBJECTS)) oxt/description.xml.template \
        $(patsubst %,oxt/%,$(COPY_TEMPLATES)) \
        oxt/META-INF/manifest.xml.template oxt/icon.svg
SED=sed

EXTENSION_FILES=build/oxt/META-INF/manifest.xml build/oxt/description.xml \
	      build/oxt/$(VOIKKO_EXTENSION_SHAREDLIB) \
	      $(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)) \
	      $(patsubst %,build/oxt/%,$(COPY_TEMPLATES))

# Targets
.PHONY: extension-files oxt install-unpacked all clean dist-gzip

extension-files : $(EXTENSION_FILES)

oxt: $(EXTENSION_FILES)
	cd build/oxt && $(SDK_ZIP) -r -9 ../$(VOIKKO_PACKAGENAME).oxt \
	   $(patsubst build/oxt/%,%,$^)

all: oxt

install-unpacked: extension-files
	install -m 755 -d "$(DESTDIR)" "$(DESTDIR)/META-INF"
	install -m 644 build/oxt/META-INF/manifest.xml "$(DESTDIR)/META-INF"
	install -m 644 build/oxt/$(VOIKKO_EXTENSION_SHAREDLIB) \
	               build/oxt/description.xml \
	               $(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)) \
	               $(patsubst %,build/oxt/%,$(COPY_TEMPLATES)) $(DESTDIR)

# Sed scripts for modifying templates
MANIFEST_SEDSCRIPT:=s/VOIKKO_EXTENSION_SHAREDLIB/$(VOIKKO_EXTENSION_SHAREDLIB)/g;s/UNOPKG_PLATFORM/$(UNOPKG_PLATFORM)/g
DESCRIPTION_SEDSCRIPT:=s/VOIKKO_VERSION/$(VOIKKO_VERSION)/g
ifdef ENABLE_GRAMMAR_CHECKER
	MANIFEST_SEDSCRIPT:=$(MANIFEST_SEDSCRIPT);/GRAMMAR_CHECKER_DISABLED/d
endif
ifdef SHOW_LICENSE
	DESCRIPTION_SEDSCRIPT:=$(DESCRIPTION_SEDSCRIPT);/SHOW_LICENSE/d
endif
ifdef SHOW_UGLY_WARNINGS
	DESCRIPTION_SEDSCRIPT:=$(DESCRIPTION_SEDSCRIPT);s/Voikko/TEKSTINTUHO/g
endif
MANIFEST_SEDSCRIPT:="$(MANIFEST_SEDSCRIPT)"
DESCRIPTION_SEDSCRIPT:="$(DESCRIPTION_SEDSCRIPT)"

# Create extension files
build/oxt/META-INF/manifest.xml: oxt/META-INF/manifest.xml.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(MANIFEST_SEDSCRIPT) < $^ > $@

build/oxt/description.xml: oxt/description.xml.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(DESCRIPTION_SEDSCRIPT) < $^ > $@

$(patsubst %,build/oxt/%,$(COPY_TEMPLATES)): build/oxt/%: oxt/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"

$(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)): build/oxt/%: $(STANDALONE_EXTENSION_PATH)/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPYDIR) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"

# Type library C++ headers
build/hpp.flag:
	-$(MKDIR) build$(PS)hpp
	$(CPPUMAKER) -Gc -BUCR -O./build/hpp $(URE_TYPES) $(OFFICE_TYPES)
	echo flagged > $@


# Compile the C++ source files
build/src/%.$(OBJ_EXT): src/%.cxx build/hpp.flag $(patsubst %,src/%.hxx,$(VOIKKO_HEADERS))
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CC) $(CC_FLAGS) $(VOIKKO_CC_FLAGS) $(CC_DEFINES) $(VOIKKO_CC_DEFINES) $(CC_OUTPUT_SWITCH)$@ $<


# Link the shared library
build/oxt/$(VOIKKO_EXTENSION_SHAREDLIB): $(patsubst %,build/src/%.$(OBJ_EXT),$(VOIKKO_OBJECTS))
ifeq "$(PLATFORM)" "windows"
	$(LINK) $(COMP_LINK_FLAGS) /OUT:$@ \
	/MAP:build/voikko.map $^ \
	 $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(STLPORTLIB) msvcrt.lib kernel32.lib $(LIBVOIKKO_PATH)\lib\libvoikko-1.lib
	mt -manifest build/oxt/voikko.dll.manifest -outputresource:build/oxt/voikko.dll;2
else
ifeq "$(PLATFORM)" "macosx"
		cat $(PRJ)/settings/component.uno.map > build/voikko.map
		nm -gx $^ | $(ADDSYMBOLS) >> build/voikko.map
		$(LINK) $(COMP_LINK_FLAGS) build/voikko.map $(LINK_LIBS) -o $@ $^ \
		$(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(CPPUHELPERDYLIB) $(CPPUDYLIB) $(SALDYLIB) \
		$(LIBVOIKKO_PATH)/lib/libvoikko.a \
		$(LIBVOIKKO_PATH)/lib/libiconv.a \
		$(LIBVOIKKO_PATH)/lib/libintl.a -framework CoreFoundation -framework Carbon
		$(INSTALL_NAME_URELIBS)  $@
else
		$(LINK) $(LINK_FLAGS) $^ -o $@ $(LINK_LIBS)
endif
endif



# Rules for creating the source distribution
dist-gzip: openoffice.org-voikko-$(VOIKKO_VERSION).tar.gz

openoffice.org-voikko-$(VOIKKO_VERSION).tar.gz: $(patsubst %,openoffice.org-voikko-$(VOIKKO_VERSION)/%, \
	                                      $(sort $(SRCDIST)))
	tar c --group 0 --owner 0 openoffice.org-voikko-$(VOIKKO_VERSION) | gzip -9 > $@

$(patsubst %,openoffice.org-voikko-$(VOIKKO_VERSION)/%, $(sort $(SRCDIST))): \
	openoffice.org-voikko-$(VOIKKO_VERSION)/%: %
	install --mode=644 -D $^ $@


# The clean target
clean:
	rm -rf build openoffice.org-voikko-$(VOIKKO_VERSION)
	rm -f openoffice.org-voikko-$(VOIKKO_VERSION).tar.gz
