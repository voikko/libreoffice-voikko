/* Openoffice.org-voikko: Finnish linguistic extension for OpenOffice.org
 * Copyright (C) 2010 Harri Pitkänen <hatapitk@iki.fi>
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

#ifndef _VOIKKOHANDLEPOOL_HXX_
#define _VOIKKOHANDLEPOOL_HXX_

#include <libvoikko/voikko.h>
#include <com/sun/star/lang/Locale.hpp>

namespace voikko {

//using namespace ::rtl;
using namespace ::com::sun::star;

class VoikkoHandlePool {
	public:
		static VoikkoHandlePool * getInstance();
		
		void putHandle(VoikkoHandle * handle/*, const lang::Locale & locale TODO: temporary solution*/);
		
		VoikkoHandle * getHandle(const lang::Locale & locale);
		
		void closeAllHandles();
		
		void setGlobalBooleanOption(int option, bool value);
		
		void setGlobalIntegerOption(int option, int value);
	private:
		VoikkoHandle * handle;
		static VoikkoHandlePool * instance;
};

}

#endif
