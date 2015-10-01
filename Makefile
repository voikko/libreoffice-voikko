# Libreoffice-voikko: Linguistic extension for LibreOffice
# Copyright (C) 2005 - 2015 Harri Pitkänen <hatapitk@iki.fi>
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
VOIKKO_VERSION=5.0

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

# Uncomment the following if you want to build a standalone extension.
# Files to be delivered with the extension must be placed under directory voikko/
# and libvoikko.py under oxt/pythonpath/
# STANDALONE_EXTENSION=1

# === End build settings ===

# Platform specific variables
ifdef SystemRoot # Windows
	COPYDIR=xcopy /E /I
	COPY=copy
	PS="\"
	MKDIR=mkdir
else
	COPYDIR=cp -r
	COPY=cp
	PS="/"
	MKDIR=mkdir -p
endif
ZIP=zip
SED=sed
FIND=find

# Build extension package name
ifdef SHOW_UGLY_WARNINGS
        VOIKKO_PACKAGENAME:=tekstintuho
else
        VOIKKO_PACKAGENAME:=voikko
endif

SRC_AND_DIST=config.xcu config.xcs icon.png SettingsDialog.xdl SettingsDialog_en_US.properties \
             SettingsDialog_fi_FI.properties SettingsDialog_en_US.default SettingsDialog.xcu Linguistic.xcu \
             voikko.components META-INF/manifest.xml lovoikko.py \
             pythonpath/SettingsEventHandler.py pythonpath/SpellChecker.py pythonpath/VoikkoHandlePool.py \
             pythonpath/SpellAlternatives.py pythonpath/PropertyManager.py pythonpath/Hyphenator.py \
             pythonpath/HyphenatedWord.py pythonpath/PossibleHyphens.py pythonpath/GrammarChecker.py
SRCDIST=COPYING Makefile README ChangeLog oxt/description.xml.template \
        $(patsubst %,oxt/%,$(SRC_AND_DIST)) \
        oxt/icon.svg oxt/license_fi.txt oxt/license_en-US.txt

COPY_TEMPLATES=$(SRC_AND_DIST)

ifdef SHOW_LICENSE
	COPY_TEMPLATES+=license_fi.txt license_en-US.txt
endif

ifdef STANDALONE_EXTENSION
	STANDALONE_EXTENSION_FILES=$(shell find voikko -type f '!' -name '.*' '!' -path 'voikko*/.*')
	COPY_TEMPLATES+=pythonpath/libvoikko.py
else
	STANDALONE_EXTENSION_FILES=
endif

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

$(patsubst %,build/oxt/%,$(STANDALONE_EXTENSION_FILES)): build/oxt/%: %
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
