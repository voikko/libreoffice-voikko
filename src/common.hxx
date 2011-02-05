/* Libreoffice-voikko: Finnish linguistic extension for LibreOffice
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
