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

#include "VoikkoHandlePool.hxx"
#include "common.hxx"

namespace voikko {

using namespace ::rtl;
using namespace ::com::sun::star;

VoikkoHandlePool * VoikkoHandlePool::getInstance() {
	if (!instance) {
		instance = new VoikkoHandlePool();
	}
	return instance;
}

void VoikkoHandlePool::putHandle(VoikkoHandle * handle) {
	this->handle = handle;
}

VoikkoHandle * VoikkoHandlePool::getHandle(const lang::Locale & /*locale*/) {
	return handle;
}

void VoikkoHandlePool::closeAllHandles() {
	if (voikko_initialized) {
		voikkoTerminate(handle);
		handle = 0;
		voikko_initialized = sal_False;
	}
}

void VoikkoHandlePool::setGlobalBooleanOption(int option, bool value) {
	if (handle) {
		voikkoSetBooleanOption(handle, option, value ? 1 : 0);
	}
}

void VoikkoHandlePool::setGlobalIntegerOption(int option, int value) {
	if (handle) {
		voikkoSetIntegerOption(handle, option, value);
	}
}

VoikkoHandlePool * VoikkoHandlePool::instance = 0;

}
