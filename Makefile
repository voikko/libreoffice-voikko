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

# ===== Build settings =====

# Version number of the libreoffice-voikko extension
VOIKKO_VERSION=4.1

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
ifdef SystemRoot # Windows
	COPYDIR=xcopy /E /I
	COPY=copy
	PS="\"
else
	COPYDIR=cp -r
	COPY=cp
	PS="/"
endif
ZIP=zip
SED=sed
MKDIR=mkdir

# STANDALONE_EXTENSION_FILES must contain the libvoikko library (unless it will be
# linked statically) and versioned directories for dictionary data to be embedded.
ifdef STANDALONE_EXTENSION_PATH
	ifeq "$(PLATFORM)" "windows"
		STANDALONE_EXTENSION_FILES=libvoikko-1.dll 2 3
	else
		ifeq "$(PLATFORM)" "macosx"
			STANDALONE_EXTENSION_FILES=2 3
		else
			STANDALONE_EXTENSION_FILES=libvoikko.so.1 2 3
		endif
	endif
else
	STANDALONE_EXTENSION_FILES=
endif

# Build extension package name
ifdef SHOW_UGLY_WARNINGS
        VOIKKO_PACKAGENAME:=tekstintuho
else
        VOIKKO_PACKAGENAME:=voikko
endif

VOIKKO_OBJECTS=registry common PropertyManager VoikkoHandlePool \
               spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
VOIKKO_HEADERS=macros common PropertyManager VoikkoHandlePool \
               spellchecker/SpellChecker \
               hyphenator/Hyphenator hyphenator/HyphenatedWord hyphenator/PossibleHyphens \
               grammar/GrammarChecker settings/SettingsEventHandler
COPY_TEMPLATES=config.xcu config.xcs icon.png SettingsDialog.xdl SettingsDialog_en_US.properties \
               SettingsDialog_fi_FI.properties SettingsDialog_en_US.default SettingsDialog.xcu Linguistic.xcu \
               voikko.components META-INF/manifest.xml lovoikko.py \
               pythonpath/SettingsEventHandler.py pythonpath/SpellChecker.py pythonpath/VoikkoHandlePool.py \
               pythonpath/SpellAlternatives.py pythonpath/PropertyManager.py
ifdef SHOW_LICENSE
	COPY_TEMPLATES+=license_fi.txt license_en-US.txt
endif
SRCDIST=COPYING Makefile README ChangeLog $(patsubst %,src/%.hxx,$(VOIKKO_HEADERS)) \
        $(patsubst %,src/%.cxx,$(VOIKKO_OBJECTS)) oxt/description.xml.template \
        $(patsubst %,oxt/%,$(COPY_TEMPLATES)) \
        oxt/icon.svg

EXTENSION_FILES=build/oxt/description.xml \
	      $(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)) \
	      $(patsubst %,build/oxt/%,$(COPY_TEMPLATES))

# Targets
.PHONY: extension-files oxt install-unpacked all clean dist-gzip

extension-files : $(EXTENSION_FILES)

oxt: $(EXTENSION_FILES)
	cd build/oxt && $(ZIP) -r -9 ../$(VOIKKO_PACKAGENAME).oxt \
	   $(patsubst build/oxt/%,%,$^)

all: oxt

install-unpacked: extension-files
	install -m 755 -d "$(DESTDIR)" "$(DESTDIR)/META-INF"
	install -m 644 build/oxt/META-INF/manifest.xml "$(DESTDIR)/META-INF"
	install -m 644 build/oxt/description.xml \
	               $(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)) \
	               $(patsubst %,build/oxt/%,$(COPY_TEMPLATES)) $(DESTDIR)

# Sed scripts for modifying templates
DESCRIPTION_SEDSCRIPT:=s/VOIKKO_VERSION/$(VOIKKO_VERSION)/g
ifdef SHOW_LICENSE
	DESCRIPTION_SEDSCRIPT:=$(DESCRIPTION_SEDSCRIPT);/SHOW_LICENSE/d
endif
ifdef SHOW_UGLY_WARNINGS
	DESCRIPTION_SEDSCRIPT:=$(DESCRIPTION_SEDSCRIPT);s/Voikko/TEKSTINTUHO/g
endif
DESCRIPTION_SEDSCRIPT:="$(DESCRIPTION_SEDSCRIPT)"

# Create extension files
build/oxt/description.xml: oxt/description.xml.template
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(SED) -e $(DESCRIPTION_SEDSCRIPT) < $^ > $@

$(patsubst %,build/oxt/%,$(COPY_TEMPLATES)): build/oxt/%: oxt/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"

$(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)): build/oxt/%: $(STANDALONE_EXTENSION_PATH)/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPYDIR) "$(subst /,$(PS),$^)" "$(subst /,$(PS),$@)"


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
