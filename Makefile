#########################################################################
#
#  Authors:
#  Harri Pitkänen (hatapitk@iki.fi), 2005
#  Kai Solehmainen (kai.solehmainen@oikeatoliot.fi), 2005
#
#  The Contents of this file are made available subject to the terms of
#  GNU Lesser General Public License Version 2.1
#
#  GNU Lesser General Public License Version 2.1
#  =============================================
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License version 2.1, as published by the Free Software Foundation.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA  02110-1301, USA
#########################################################################

ifndef OO_SDK_HOME
$(error You must run setsdkenv before running make)
endif
PRJ=$(OO_SDK_HOME)
include $(PRJ)/settings/settings.mk
include $(PRJ)/settings/std.mk
include $(PRJ)/settings/dk.mk
include impl/setup.mk
include setup.mk

ifdef LF_CC
	CC=$(LF_CC)
endif

ifdef LF_LINK
	LINK=$(LF_LINK)
endif

CC_DEFINES+= -DLF_NAMESPACE=$(LF_NAMESPACE)
LF_REPLACE_NAMESPACE:=sed 's/LF_NAMESPACE/$(LF_NAMESPACE)/g'
LF_REPLACE_SHORTNAME:=sed 's/LF_SHORTNAME/$(LF_SHORTNAME)/g'
LF_REPLACE_FULLNAME:=sed 's/LF_FULLNAME/$(LF_FULLNAME)/g'

ifeq "$(PLATFORM)" "windows"

# Check if the cygwin is installed
    LF_CYGWIN_CHECK := $(shell cygcheck -V)
    LF_CYGWIN_CHECK := $(findstring Cygwin, $(LF_CYGWIN_CHECK))

    ifeq "$(LF_CYGWIN_CHECK)" ""
# Cygwin is not installed
    else
# Cygwin is installed so use Linux like defines
        PS=/
        ECHO=@echo
        MKDIR=mkdir -p
        CAT=cat
        OSEP=\<
        CSEP=\>
        QUOTE=$(subst S,\,S)
        QM=\"
        SQM='
        ECHOLINE=@echo
        P2BG=&
        DEL=rm -f
        DELRECURSIVE=rm -rf
        COPY=cp
        EMPTYSTRING=
        PATH_SEPARATOR=:
    endif
    
	CC_FLAGS=-c -GX -MT
	CC_INCLUDES=-I. -I"$(PRJ)/include"
	LF_INSTALLPKG_EXT=exe
	ifeq "$(LF_DEBUG)" "LOG"
		CC_DEFINES+= -DLF_DEBUG_OUTPUT
	endif
	ifeq "$(LF_DEBUG)" "FULL"
		CC_DEFINES+= -DLF_DEBUG_OUTPUT
		CC_FLAGS+= -Zi
	endif
endif
ifeq "$(PLATFORM)" "linux"
	CC_FLAGS=-c -O2 -fpic
	CC_INCLUDES=-I. -I$(PRJ)/include
	LINK_LIBS=-L$(PRJ)/$(PLATFORM)/lib -L"$(OFFICE_PROGRAM_PATH)" $(LF_LINK_EXTRA)
	LF_INSTALLPKG_EXT=tar.gz
	ifeq "$(LF_DEBUG)" "LOG"
		CC_DEFINES+= -DLF_DEBUG_OUTPUT
	endif
	ifeq "$(LF_DEBUG)" "FULL"
		CC_DEFINES+= -DLF_DEBUG_OUTPUT
		CC_FLAGS=-c -g -fpic -Wall
	endif
endif

LF_OUTDIR=./build
LF_TYPEFLAG=$(LF_OUTDIR)/types.flag
LF_HPPDIR=$(LF_OUTDIR)/hpp
LF_SLONAME=$(LF_SHORTNAME).$(SHAREDLIB_EXT)
LF_SLO=$(LF_OUTDIR)/pkg/$(UNOPKG_PLATFORM)/$(LF_SLONAME)

LF_CXXFILES := sprophelp.cxx sspellimp.cxx register.cxx \
	XSpellAlternatives_impl.cxx hyphenimp.cxx XHyphenatedWord_impl.cxx \
	XPossibleHyphens_impl.cxx lfmacros.cxx impl/$(LF_IMPL_CODE).cxx

LF_SLOFILES = $(patsubst %.cxx,$(LF_OUTDIR)/%.$(OBJ_EXT),$(LF_CXXFILES))
LF_DOCS := COPYING.gpl COPYING.lgpl

LF_IMPL_EXTRA_FILES := $(patsubst impl/%,%,$(wildcard impl/shared/*) $(wildcard impl/$(UNOPKG_PLATFORM)/*))
LF_IMPL_DOCS_SHARED := $(patsubst impl/doc/shared/%,%,$(wildcard impl/doc/shared/*))
LF_IMPL_DOCS_PLATF := $(patsubst impl/doc/$(UNOPKG_PLATFORM)/%,%,$(wildcard impl/doc/$(UNOPKG_PLATFORM)/*))

# Rules
.PHONY: all install_pkg

all : $(LF_PACKAGE)

$(LF_TYPEFLAG) :
	$(CPPUMAKER) -Gc -BUCR -O$(LF_HPPDIR) $(OFFICE_TYPE_LIBRARY)
	echo flagged > $@

$(LF_OUTDIR)/%.$(OBJ_EXT) : %.cxx $(LF_TYPEFLAG)
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) $(STL_INCLUDES) -I$(LF_HPPDIR) $(CC_DEFINES) $(CC_OUTPUT_SWITCH)$@ $<

ifeq "$(OS)" "WIN"
$(LF_SLO) : $(LF_SLOFILES)
	-$(ECHO) $(LF_CYGWIN_CHECK)
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(LINK) $(COMP_LINK_FLAGS) /OUT:$@ \
	/MAP:$(LF_OUTDIR)/$(LF_SHORTNAME).map $(LF_SLOFILES) \
	 $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(STLPORTLIB) msvcrt.lib kernel32.lib
else
$(LF_SLO) : $(LF_SLOFILES)
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(LINK) $(COMP_LINK_FLAGS) $(LINK_LIBS) -o $@ $^ \
	 $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(STLPORTLIB)
endif

$(LF_OUTDIR)/pkg/META-INF/manifest.xml :
	-$(MKDIR) $(subst /,$(PS),$(@D))
	@echo $(OSEP)?xml version="$(QM)1.0$(QM)" encoding="$(QM)UTF-8$(QM)"?$(CSEP) > $@
	@echo $(OSEP)!DOCTYPE manifest:manifest PUBLIC "$(QM)-//OpenOffice.org//DTD Manifest 1.0//EN$(QM)" "$(QM)Manifest.dtd$(QM)"$(CSEP) >> $@
	@echo $(OSEP)manifest:manifest xmlns:manifest="$(QM)http://openoffice.org/2001/manifest$(QM)"$(CSEP) >> $@
	@echo $(SQM)  $(SQM)$(OSEP)manifest:file-entry manifest:media-type="$(QM)application/vnd.sun.star.uno-component;type=native;platform=$(UNOPKG_PLATFORM)$(QM)" >> $@
	@echo $(SQM)                       $(SQM)manifest:full-path="$(QM)$(UNOPKG_PLATFORM)/$(LF_SLONAME)$(QM)"/$(CSEP) >> $@
	@echo $(SQM)  $(SQM)$(OSEP)manifest:file-entry manifest:media-type="$(QM)application/vnd.sun.star.configuration-data$(QM)" >> $@
	@echo $(SQM)                       $(SQM)manifest:full-path="$(QM)config.xcu$(QM)"/$(CSEP) >> $@
	@echo $(SQM)  $(SQM)$(OSEP)manifest:file-entry manifest:media-type="$(QM)application/vnd.sun.star.configuration-schema$(QM)" >> $@
	@echo $(SQM)                       $(SQM)manifest:full-path="$(QM)config.xcs$(QM)"/$(CSEP) >> $@
	@echo $(OSEP)/manifest:manifest$(CSEP) >> $@

$(LF_OUTDIR)/pkg/config.xcu : config.xcu
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CAT) $(subst /,$(PS),$<) | $(LF_REPLACE_NAMESPACE) > $(subst /,$(PS),$@)

$(LF_OUTDIR)/pkg/config.xcs : config.xcs
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CAT) $(subst /,$(PS),$<) | $(LF_REPLACE_NAMESPACE) > $(subst /,$(PS),$@)

$(LF_OUTDIR)/$(LF_PACKAGENAME)/install-$(LF_SHORTNAME) : linux-installer
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CAT) $(subst /,$(PS),$<) | $(LF_REPLACE_NAMESPACE) | $(LF_REPLACE_SHORTNAME) | $(LF_REPLACE_FULLNAME) > $(subst /,$(PS),$@)
	chmod a+x $(subst /,$(PS),$@)

$(patsubst %,$(LF_OUTDIR)/pkg/%,$(LF_IMPL_EXTRA_FILES)): $(LF_OUTDIR)/pkg/%: impl/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)

$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_SHARED)): \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/%: impl/doc/shared/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)

$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_PLATF)): \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/%: impl/doc/$(UNOPKG_PLATFORM)/%
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)

$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_DOCS)): $(LF_OUTDIR)/$(LF_PACKAGENAME)/%: %
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)

$(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_PACKAGENAME).$(UNOPKG_EXT) : $(LF_OUTDIR)/$(LF_PACKAGENAME).$(UNOPKG_EXT)
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)
	
$(LF_PACKAGE) : $(LF_SLO) $(LF_OUTDIR)/pkg/META-INF/manifest.xml \
	$(patsubst %,$(LF_OUTDIR)/pkg/%,$(LF_IMPL_EXTRA_FILES)) \
	$(LF_OUTDIR)/pkg/config.xcu \
	$(LF_OUTDIR)/pkg/config.xcs
	cd $(LF_OUTDIR)/pkg && $(SDK_ZIP) ../$(LF_PACKAGENAME).$(UNOPKG_EXT) META-INF/manifest.xml $(UNOPKG_PLATFORM)/$(LF_SLONAME) \
	$(LF_IMPL_EXTRA_FILES) config.xcu config.xcs

$(LF_OUTDIR)/$(LF_PACKAGENAME).zip : $(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_PACKAGENAME).$(UNOPKG_EXT) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_DOCS)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_SHARED)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_PLATF))
	cd $(LF_OUTDIR)/$(LF_PACKAGENAME) && $(SDK_ZIP) ../$(LF_PACKAGENAME).$(LF_INSTALLPKG_EXT) *

$(LF_OUTDIR)/$(LF_PACKAGENAME).tar.gz : $(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_PACKAGENAME).$(UNOPKG_EXT) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_DOCS)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_SHARED)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_PLATF)) \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/install-$(LF_SHORTNAME)
	tar czf $(LF_OUTDIR)/$(LF_PACKAGENAME).$(LF_INSTALLPKG_EXT) --owner 0 --group 0 \
		-C $(LF_OUTDIR) $(LF_PACKAGENAME)

$(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_CONFIGURATOR)$(EXE_EXT) : $(LF_OUTDIR)/$(LF_CONFIGURATOR)$(EXE_EXT) 
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(COPY) $(subst /,$(PS),$<) $(subst /,$(PS),$@)
		
$(LF_OUTDIR)/$(LF_CONFIGURATOR)$(EXE_EXT) :
	cd installer/$(LF_CONFIGURATOR) && $(MAKE)
	
$(LF_OUTDIR)/$(LF_PACKAGENAME).exe : $(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_PACKAGENAME).$(UNOPKG_EXT) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_DOCS)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_SHARED)) \
	$(patsubst %,$(LF_OUTDIR)/$(LF_PACKAGENAME)/%,$(LF_IMPL_DOCS_PLATF)) \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_CONFIGURATOR)$(EXE_EXT)
	cd installer/windows && $(MAKE)

install_pkg: $(LF_OUTDIR)/$(LF_PACKAGENAME).$(LF_INSTALLPKG_EXT)


