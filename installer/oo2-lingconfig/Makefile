#########################################################################
#
#  Authors:
#  Kai Solehmainen (kai.solehmainen@oikeatoliot.fi), 2005
#  Harri Pitkänen (hatapitk@iki.fi), 2005
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


# Builds the ProfUNO cpp binding example of the Developers Guide

ifndef OO_SDK_HOME
$(error You must run setsdkenv before running make)
endif
PRJ=$(OO_SDK_HOME)
SETTINGS=$(PRJ)/settings
LF_SOIKKODIR=../..

include $(PRJ)/settings/settings.mk
include $(PRJ)/settings/std.mk
include $(PRJ)/settings/dk.mk
include $(LF_SOIKKODIR)/impl/setup.mk
include $(LF_SOIKKODIR)/setup.mk

ifdef LF_CC
    CC=$(LF_CC)
endif

ifdef LF_LINK
    LINK=$(LF_LINK)
endif

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
endif
ifeq "$(PLATFORM)" "linux"
	CC_FLAGS=-c -O -fpic
	CC_INCLUDES=-I. -I"$(PRJ)/include"
	LINK_LIBS=-L$(PRJ)/$(PLATFORM)/lib -L"$(OFFICE_PROGRAM_PATH)"
endif

# Define non-platform/compiler specific settings

LF_SOIKKODIR=../..
LF_OUTDIR=$(LF_SOIKKODIR)/build
LF_TYPEFLAG=$(LF_OUTDIR)/types.flag
LF_HPPDIR=$(LF_OUTDIR)/hpp

LF_CXXFILES = linguistic-configurator.cxx
LF_SLOFILES = $(patsubst %.cxx,$(LF_OUTDIR)/%.$(OBJ_EXT),$(LF_CXXFILES))

# Targets
.PHONY: all
all : $(LF_OUTDIR)/$(LF_CONFIGURATOR)$(EXE_EXT)

include $(SETTINGS)/stdtarget.mk

$(LF_OUTDIR)/%.rdb :
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(REGMERGE) $@ / $(OFFICE_TYPE_LIBRARY)
	
$(LF_TYPEFLAG) :
	$(CPPUMAKER) -Gc -BUCR -O$(LF_HPPDIR) $(OFFICE_TYPE_LIBRARY)
	echo flagged > $@

$(LF_OUTDIR)/%.$(OBJ_EXT) : %.cxx $(LF_TYPEFLAG)
	-$(MKDIR) $(subst /,$(PS),$(@D))
	$(CC) $(CC_FLAGS) $(CC_INCLUDES) -I$(STL_INCLUDES) -I$(LF_HPPDIR) \
	  -I$(LF_SOIKKODIR) $(CC_DEFINES) $(CC_OUTPUT_SWITCH)$(subst /,$(PS),$@) $<

$(LF_OUTDIR)/$(LF_CONFIGURATOR)$(EXE_EXT) : $(LF_SLOFILES)
	-$(MKDIR) $(subst /,$(PS),$(@D))
ifeq "$(OS)" "WIN"
	$(LINK) $(EXE_LINK_FLAGS) /OUT:$@ \
	  $< $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(STLPORTLIB)
else
	$(LINK) $(EXE_LINK_FLAGS) $(LINK_LIBS) -o $@ $< \
	  $(CPPUHELPERLIB) $(CPPULIB) $(SALLIB) $(STLPORTLIB) $(STDC++LIB)
endif

%.run: $(LF_OUTDIR)/%$(EXE_EXT) 
	cd $(subst /,$(PS),$(LF_OUTDIR)) && $(basename $@)

.PHONY: clean
clean :
	-$(DEL) $(subst /,$(PS),$(LF_OUTDIR))$(PS)$(LF_CONFIGURATOR).*
