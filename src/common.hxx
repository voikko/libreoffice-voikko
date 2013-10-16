/* Libreoffice-voikko: Linguistic extension for LibreOffice
 * Copyright (C) 2007 - 2008 Harri Pitkänen <hatapitk@iki.fi>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v. 2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at http://mozilla.org/MPL/2.0/.
 * 
 * Alternatively, the contents of this file may be used under the terms of
 * the GNU General Public License Version 3 or later (the "GPL"), in which
 * case the provisions of the GPL are applicable instead of those above.
 ******************************************************************************/

#ifndef _COMMON_HXX_
#define _COMMON_HXX_

#include <com/sun/star/uno/Reference.hxx>
#include <com/sun/star/uno/XComponentContext.hpp>
#include <com/sun/star/uno/XInterface.hpp>
#include <osl/mutex.hxx>

using namespace com::sun::star;

namespace voikko {

osl::Mutex & getVoikkoMutex();

/** Get property set for given group from registry */
uno::Reference<uno::XInterface> getRegistryProperties(const rtl::OUString & group,
	uno::Reference<uno::XComponentContext> compContext);

}

#endif
