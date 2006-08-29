/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@iki.fi), 2005
 *
 *  This file contains macros and definitions from lingucomponent-fi 0.1.2
 *  (files source/spell/sspellimp.cxx) and
 *  OOo_1.9.104 (file linguistic/source/misc.cxx).
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1, as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 *  MA  02110-1301, USA
 *
 ************************************************************************/

#include "lfmacros.hxx"

#include <cppuhelper/bootstrap.hxx>
#include <osl/file.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/beans/PropertyValue.hpp>
#include <com/sun/star/beans/PropertyState.hpp>
#include <com/sun/star/beans/XHierarchicalPropertySet.hpp>
#include <com/sun/star/util/XMacroExpander.hpp>

#ifdef WNT
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>
#endif //WNT

using namespace com::sun::star::uno;
using namespace com::sun::star::bridge;
using namespace com::sun::star::beans;
using namespace com::sun::star::util;
using namespace cppu;
using namespace osl;

#ifdef WNT
extern void LfLog(const sal_Char * format, ...)
{ 
    va_list args;
    char buffer[1024];
    int fh; 

    va_start(args, format);
    vsprintf(buffer, format, args);

    if ((fh = _open("c:\\voikko.log", _O_APPEND  | _O_CREAT | _O_WRONLY | _O_TEXT, _S_IREAD | _S_IWRITE )) != -1 ) {
        _write( fh, buffer, strlen(buffer)); 
        _close( fh ); 
    }
}
#endif //WNT

namespace LF_NAMESPACE {

OUString getInstallationPath() {
  try {
    Reference< XComponentContext > compContext = defaultBootstrap_InitialComponentContext();
    Reference< XMultiComponentFactory > servManager = compContext->getServiceManager();
    Reference< XInterface > iFace = servManager->createInstanceWithContext(
             A2OU("com.sun.star.configuration.ConfigurationProvider"), compContext);
    Reference< XMultiServiceFactory > provider(iFace, UNO_QUERY);
    PropertyValue pathArgument(A2OU("nodepath"), 0, (Any) A2OU("/").concat(A2OU(MACROTOSTRING(LF_NAMESPACE))).concat(A2OU(".Config/Yleiset")), PropertyState_DIRECT_VALUE);
    Sequence< Any > aArguments(1);
    aArguments.getArray()[0] = (Any) pathArgument;
    LF_LOG(("getInstallationPath() - 1\n"));
    Reference< XInterface > rootView = provider->createInstanceWithArguments(
             A2OU("com.sun.star.configuration.ConfigurationAccess"), aArguments);
    LF_LOG(("getInstallationPath() - 2\n"));
    Reference< XHierarchicalPropertySet > propSet(rootView, UNO_QUERY);
    Any locationProp = propSet->getHierarchicalPropertyValue(A2OU("location"));
    OUString locationVal;
    locationProp >>= locationVal;
    // Cut the "vnd.sun.star.expand:" part
    OUString locationFileURL = locationVal.replaceAt(0, 20, A2OU(""));
    Reference< XMacroExpander > expander(
                    compContext->getValueByName(
                    ::rtl::OUString::createFromAscii( "/singletons/com.sun.star.util.theMacroExpander" ) ),
                    UNO_QUERY );
    OUString expandedLocation = expander->expandMacros(locationFileURL);
    OUString locationSystemPath;
    FileBase::getSystemPathFromFileURL( expandedLocation, locationSystemPath );
    return locationSystemPath;
  }
  catch (Exception e) {
    // TODO: something more useful here
    LF_LOG(("getInstallationPath(): ERROR\n"));
    return A2OU("");
  }
}


/* These are from OOo_1.9.104 sources, file linguistic/source/misc.cxx */

osl::Mutex &    GetLinguMutex()
{
        static osl::Mutex       aMutex;
        return aMutex;
}

sal_Int32 GetPosInWordToCheck( const rtl::OUString &rTxt, sal_Int32 nPos )
{
        sal_Int32 nRes = -1;
        sal_Int32 nLen = rTxt.getLength();
        if (0 <= nPos  &&  nPos < nLen)
        {
        nRes = 0;
        for (sal_Int32 i = 0;  i < nPos;  ++i)
                {
            sal_Unicode cChar = rTxt[i];
            sal_Bool bSkip = IsHyphen( cChar ) || IsControlChar( cChar );
            if (!bSkip)
                ++nRes;
                }
        }
        return nRes;
}

/* Lingucomponent_fi 0.1.2 */
sal_Bool operator == ( const Locale &rL1, const Locale &rL2 )
{
  return rL1.Language ==  rL2.Language	&&
    rL1.Country  ==  rL2.Country	&&
    rL1.Variant  ==  rL2.Variant;
}

}
