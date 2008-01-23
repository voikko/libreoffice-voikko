/* Openoffice.org-hspell: Hebrew spellchecker for OpenOffice.org
 * Copyright (C) 2007 - 2008 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *********************************************************************************/

#include <cppuhelper/factory.hxx>
#include <cppuhelper/implementationentry.hxx>
#include <uno/lbnames.h>
#include <com/sun/star/uno/Reference.hxx>

#include "macros.hxx"
#include "spellchecker/SpellChecker.hxx"

using namespace ::com::sun::star::registry;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::rtl;

namespace hspell {

extern "C" void SAL_CALL
	component_getImplementationEnvironment(sal_Char const ** ppEnvTypeName,
	                                       uno_Environment **) {
	VOIKKO_DEBUG("component_getImplementationEnvironment");
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
} 


OUString SAL_CALL regGetSpellImplementationName() {
	return SpellChecker::getImplementationName_static();
}

Sequence<OUString> SAL_CALL regSpellSupportedServiceNames() {
	Sequence<OUString> s(1);
	s[0] = OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.linguistic2.SpellChecker"));
	return s;
}

Reference<XInterface> SAL_CALL regSpellCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * > (new SpellChecker(context));
}

static ::cppu::ImplementationEntry const regEntries[] = {
	{ &regSpellCreate,
	  &regGetSpellImplementationName,
	  &regSpellSupportedServiceNames,
	  &::cppu::createSingleComponentFactory, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

extern "C" sal_Bool SAL_CALL component_writeInfo(void * serviceManager, void * registryKey) {
	VOIKKO_DEBUG("component_writeInfo");
	return ::cppu::component_writeInfoHelper(serviceManager, registryKey, regEntries);
}

extern "C" void * SAL_CALL
	component_getFactory(const char * implName, void * serviceManager, void * registryKey) {
	VOIKKO_DEBUG("component_getFactory");
	return ::cppu::component_getFactoryHelper(implName, serviceManager, registryKey, regEntries);
}

}
