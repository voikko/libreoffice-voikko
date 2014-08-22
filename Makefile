# Libreoffice-voikko: Linguistic extension for LibreOffice
# Copyright (C) 2005 - 2014 Harri Pitkänen <hatapitk@iki.fi>
#
# This Source Code Form is subject to the terms of the Mozilla Public License,
# v. 2.0. If a copy of the MPL was not distributed with this file, You can
# obtain one at http://mozilla.org/MPL/2.0/.
# 
# Alternatively, the contents of this file may be used under the terms of
# the GNU General Public License Version 3 or later (the "GPL"), in which
# case the provisions of the GPL are applicable instead of those above.
###############################################################################


# Check that build environment is properly set up
ifndef OO_SDK_HOME
$(error You must run setsdkenv before running make)
endif
PRJ=$(OO_SDK_HOME)
# Load SDK settings
include $(PRJ)/settings/settings.mk
include $(PRJ)/settings/std.mk

# ===== Build settings =====

# Version number of the libreoffice-voikko extension
VOIKKO_VERSION=4.1

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

# Setting this option to 1 causes ugly warnings to be added to visible places
# in the extension without removing any functionality (codename "tekstintuho").
# Useful for binary builds that are released for public testing.
# SHOW_UGLY_WARNINGS=1

# Destination directory when installing unpacked extension with
# make install-unpacked
DESTDIR=/usr/lib/libreoffice-voikko

# === End build settings ===

# Platform specific variables
ifeq "$(PLATFORM)" "windows"
	WARNING_FLAGS=-Wall -WX -wd4061 -wd4127 -wd4265 -wd4365 -wd4514 -wd4619 -wd4625 -wd4626 -wd4668 -wd4710 -wd4711 -wd4820
	# The following warnings should be fixed in the future
	WARNING_FLAGS+= -wd4640
	COPYDIR=xcopy /E /I
	PS="\"
else
	WARNING_FLAGS=-Wall -Wno-non-virtual-dtor
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

# STANDALONE_EXTENSION_FILES must contain the libvoikko library (unless it will be
# linked statically) and versioned directories for dictionary data to be embedded.
ifdef STANDALONE_EXTENSION_PATH
	VOIKKO_CC_DEFINES= -DVOIKKO_STANDALONE_EXTENSION
	ifeq "$(PLATFORM)" "windows"
		STANDALONE_EXTENSION_FILES=libvoikko-1.dll 2 3
	else
		ifeq "$(PLATFORM)" "macosx"
			STANDALONE_EXTENSION_FILES=2 3
		else
			STANDALONE_EXTENSION_FILES=libvoikko.so.1 2 3
			LINK_LIBS += -lvoikko
		endif
	endif
else
	VOIKKO_CC_DEFINES=
	STANDALONE_EXTENSION_FILES=
	LINK_LIBS += -lvoikko
endif

# Build extension package name
ifdef SHOW_UGLY_WARNINGS
        VOIKKO_PACKAGENAME:=tekstintuho
        VOIKKO_CC_DEFINES += -DTEKSTINTUHO
else
        VOIKKO_PACKAGENAME:=voikko
endif
ifneq "$(VOIKKO_DEBUG)" "NO"
        VOIKKO_CC_DEFINES+= -DVOIKKO_DEBUG_OUTPUT
endif

ifdef LIBVOIKKO_PATH
	LINK_FLAGS+= -L$(LIBVOIKKO_PATH)/lib
	VOIKKO_CC_FLAGS+= -I$(LIBVOIKKO_PATH)/include
endif

VOIKKO_EXTENSION_SHAREDLIB=voikko.$(SHAREDLIB_EXT)
VOIKKO_OBJECTS=registry common PropertyManager VoikkoHandlePool \
               spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
VOIKKO_HEADERS=macros common PropertyManager VoikkoHandlePool \
               spellchecker/SpellAlternatives spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
COPY_TEMPLATES=config.xcu config.xcs icon.png SettingsDialog.xdl SettingsDialog_en_US.properties \
               SettingsDialog_fi_FI.properties SettingsDialog_en_US.default SettingsDialog.xcu Linguistic.xcu
ifdef SHOW_LICENSE
	COPY_TEMPLATES+=license_fi.txt license_en-US.txt
endif
SRCDIST=COPYING Makefile README ChangeLog $(patsubst %,src/%.hxx,$(VOIKKO_HEADERS)) \
        $(patsubst %,src/%.cxx,$(VOIKKO_OBJECTS)) oxt/description.xml.template \
        $(patsubst %,oxt/%,$(COPY_TEMPLATES)) \
        oxt/voikko.components.template oxt/META-INF/manifest.xml.template oxt/icon.svg
SED=sed

EXTENSION_FILES=build/oxt/META-INF/manifest.xml build/oxt/description.xml build/oxt/voikko.components \
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
	               build/oxt/description.xml build/oxt/voikko.components \
	               $(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)) \
	               $(patsubst %,build/oxt/%,$(COPY_TEMPLATES)) $(DESTDIR)

# Sed scripts for modifying templates
MANIFEST_SEDSCRIPT:=s/UNOPKG_PLATFORM/$(UNOPKG_PLATFORM)/g
COMPONENTS_SEDSCRIPT:=s/VOIKKO_EXTENSION_SHAREDLIB/$(VOIKKO_EXTENSION_SHAREDLIB)/g
DESCRIPTION_SEDSCRIPT:=s/VOIKKO_VERSION/$(VOIKKO_VERSION)/g
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

build/oxt/voikko.components: oxt/voikko.components.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(COMPONENTS_SEDSCRIPT) < $^ > $@

$(patsubst %,build/oxt/%,$(COPY_TEMPLATES)): build/oxt/%: oxt/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"

$(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)): build/oxt/%: $(STANDALONE_EXTENSION_PATH)/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPYDIR) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"

# Type library C++ headers
build/hpp.flag:
	-$(MKDIR) build$(PS)hpp
	$(CPPUMAKER) -Gc -O./build/hpp $(URE_TYPES) $(OFFICE_TYPES)
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
	 $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) msvcrt.lib kernel32.lib $(LIBVOIKKO_PATH)\lib\libvoikko-1.lib
	mt -manifest build/oxt/voikko.dll.manifest -outputresource:build/oxt/voikko.dll;2
else
ifeq "$(PLATFORM)" "macosx"
#		cat $(PRJ)/settings/component.uno.map > build/voikko.map
		$(LINK) $(COMP_LINK_FLAGS) $(LINK_LIBS) -o $@ $^ \
		$(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(CPPUHELPERDYLIB) $(CPPUDYLIB) $(SALDYLIB) \
		-lvoikko
		$(INSTALL_NAME_URELIBS)  $@
else
		$(LINK) $(LINK_FLAGS) $^ -o $@ $(LINK_LIBS)
endif
endif



# Rules for creating the source distribution
dist-gzip: libreoffice-voikko-$(VOIKKO_VERSION).tar.gz

libreoffice-voikko-$(VOIKKO_VERSION).tar.gz: $(patsubst %,libreoffice-voikko-$(VOIKKO_VERSION)/%, \
	                                      $(sort $(SRCDIST)))
	tar c --group 0 --owner 0 libreoffice-voikko-$(VOIKKO_VERSION) | gzip -9 > $@

$(patsubst %,libreoffice-voikko-$(VOIKKO_VERSION)/%, $(sort $(SRCDIST))): \
	libreoffice-voikko-$(VOIKKO_VERSION)/%: %
	install --mode=644 -D $^ $@


# The clean target
clean:
	rm -rf build libreoffice-voikko-$(VOIKKO_VERSION)
	rm -f libreoffice-voikko-$(VOIKKO_VERSION).tar.gz
