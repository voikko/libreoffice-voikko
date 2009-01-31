/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2007 - 2008 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <cppuhelper/factory.hxx>
#include <cppuhelper/implementationentry.hxx>
#include <uno/lbnames.h>
#include <com/sun/star/uno/Reference.hxx>

#include "macros.hxx"
#include "spellchecker/SpellChecker.hxx"
#include "hyphenator/Hyphenator.hxx"
#include "grammar/GrammarChecker.hxx"
#include "settings/SettingsEventHandler.hxx"

using namespace ::com::sun::star::registry;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::lang;
using namespace ::rtl;

namespace voikko {

extern "C" SAL_DLLPUBLIC_EXPORT void SAL_CALL
	component_getImplementationEnvironment(sal_Char const ** ppEnvTypeName,
	                                       uno_Environment **) {
	VOIKKO_DEBUG("component_getImplementationEnvironment");
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME ":unsafe";
} 

Sequence<OUString> SAL_CALL regSettingsSupportedServiceNames() {
	Sequence<OUString> s(1);
	s[0] = OUString(RTL_CONSTASCII_USTRINGPARAM("org.puimula.ooovoikko.SettingsEventHandlerService"));
	return s;
}

Reference<XInterface> SAL_CALL regSpellCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * > (new SpellChecker(context));
}

Reference<XInterface> SAL_CALL regHyphenCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * >(new Hyphenator(context));
}

Reference<XInterface> SAL_CALL regGrammarCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * >(new GrammarChecker(context));
}

Reference<XInterface> SAL_CALL regSettingsCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * >(new SettingsEventHandler(context));
}

static ::cppu::ImplementationEntry const regEntries[] = {
	{ &regSpellCreate,
	  &SpellChecker::getImplementationName_static,
	  &SpellChecker::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
	{ &regHyphenCreate,
	  &Hyphenator::getImplementationName_static,
	  &Hyphenator::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
#ifndef DISABLE_GRAMMAR_CHECKER
	{ &regGrammarCreate,
	  &GrammarChecker::getImplementationName_static,
	  &GrammarChecker::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
#endif
	{ &regSettingsCreate,
	  &SettingsEventHandler::getImplementationName_static,
	  &regSettingsSupportedServiceNames,
	  &::cppu::createSingleComponentFactory, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

extern "C" SAL_DLLPUBLIC_EXPORT sal_Bool SAL_CALL component_writeInfo(void * serviceManager, void * registryKey) {
	VOIKKO_DEBUG("component_writeInfo");
	return ::cppu::component_writeInfoHelper(serviceManager, registryKey, regEntries);
}

extern "C" SAL_DLLPUBLIC_EXPORT void * SAL_CALL
	component_getFactory(const char * implName, void * serviceManager, void * registryKey) {
	VOIKKO_DEBUG("component_getFactory");
	return ::cppu::component_getFactoryHelper(implName, serviceManager, registryKey, regEntries);
}

}
