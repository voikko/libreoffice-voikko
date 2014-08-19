/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2007 - 2009 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

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

Reference<XInterface> SAL_CALL regSettingsCreate(const Reference<XComponentContext> & context)
	SAL_THROW((Exception)) {
	return static_cast< ::cppu::OWeakObject * >(new SettingsEventHandler(context));
}

static ::cppu::ImplementationEntry const regEntries[] = {
	{ &SpellChecker::get,
	  &SpellChecker::getImplementationName_static,
	  &SpellChecker::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
	{ &Hyphenator::get,
	  &Hyphenator::getImplementationName_static,
	  &Hyphenator::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
#ifndef DISABLE_GRAMMAR_CHECKER
	{ &GrammarChecker::get,
	  &GrammarChecker::getImplementationName_static,
	  &GrammarChecker::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
#endif
	{ &regSettingsCreate,
	  &SettingsEventHandler::getImplementationName_static,
	  &SettingsEventHandler::getSupportedServiceNames_static,
	  &::cppu::createSingleComponentFactory, 0, 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

extern "C" SAL_DLLPUBLIC_EXPORT void * SAL_CALL
	component_getFactory(const char * implName, void * serviceManager, void * registryKey) {
	VOIKKO_DEBUG("component_getFactory");
	return ::cppu::component_getFactoryHelper(implName, serviceManager, registryKey, regEntries);
}

}
