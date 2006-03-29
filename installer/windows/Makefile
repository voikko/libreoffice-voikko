#########################################################################
#
#  Authors:
#  Kai Solehmainen (kai.solehmainen@oikeatoliot.fi), 2005
#  Harri Pitkänen (hatapitk@cc.jyu.fi), 2005
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
LF_SOIKKODIR=../..

include $(PRJ)/settings/settings.mk
include $(PRJ)/settings/std.mk
include $(PRJ)/settings/dk.mk
include $(LF_SOIKKODIR)/impl/setup.mk
include $(LF_SOIKKODIR)/setup.mk

LF_OUTDIR=$(LF_SOIKKODIR)/build
LF_INSTALLSRC=install-oo2-soikko.nsi

# Targets
.PHONY: all
all : $(LF_OUTDIR)/$(LF_PACKAGENAME).exe

$(LF_OUTDIR)/$(LF_PACKAGENAME).exe : \
    $(LF_OUTDIR)/$(LF_PACKAGENAME)/COPYING \
    $(LF_OUTDIR)/$(LF_PACKAGENAME)/COPYING_FI \
    $(LF_OUTDIR)/$(LF_PACKAGENAME)/COPYING.soikko \
    $(LF_OUTDIR)/$(LF_PACKAGENAME)/COPYING.gpl \
    $(LF_OUTDIR)/$(LF_PACKAGENAME)/COPYING.lgpl \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/ASENNUS.txt \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_PACKAGE) \
	$(LF_OUTDIR)/$(LF_PACKAGENAME)/$(LF_CONFIGURATOR)$(EXE_EXT) \
	install-oo2-soikko.nsi \
	helper-functions.nsh \
	locale_english.nsh \
	locale_finnish.nsh
	$(ECHO) $(LF_PACKAGENAME)
	makensis.exe \
	    /DINSTALLER_NAME=$(LF_PACKAGENAME) \
        /DOUTDIR=$(LF_PACKAGENAME) \
		/DSRCDIR=$(subst /,$(PS),$(LF_OUTDIR))$(PS)$(LF_PACKAGENAME) \
		/DLICENSE_FILE_EN=$(subst /,$(PS),$(LF_OUTDIR)$(PS)$(LF_PACKAGENAME)$(PS)COPYING) \
		/DLICENSE_FILE_FI=$(subst /,$(PS),$(LF_OUTDIR)$(PS)$(LF_PACKAGENAME)$(PS)COPYING_FI) \
		/DINSTALLER_FILE=$(subst /,$(PS),$(LF_OUTDIR))$(PS)$(LF_PACKAGENAME)$(EXE_EXT) \
		/DUNINSTALLER_FILE=Uninstall-$(LF_PACKAGENAME)$(EXE_EXT) \
		/DLF_PORT=2083 \
		/DLF_CONFIGURATOR=$(LF_CONFIGURATOR)$(EXE_EXT) \
		/DLF_PACKAGEFILE=$(LF_PACKAGENAME).$(UNOPKG_EXT) \
		/DLF_NAMESPACE=$(LF_NAMESPACE) \
		$(LF_INSTALLSRC)

.PHONY: clean
clean :
	-$(DEL) $(LF_OUTDIR)/$(LF_PACKAGENAME).exe
