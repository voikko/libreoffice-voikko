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

using namespace com::sun::star::uno;
using namespace com::sun::star::bridge;
using namespace com::sun::star::beans;
using namespace com::sun::star::util;
using namespace cppu;
using namespace osl;

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
    LF_LOG((OU2A(locationVal)));
    LF_LOG(("\n"));
    // Cut the "vnd.sun.star.expand:" part
    OUString locationFileURL = locationVal.replaceAt(0, 20, A2OU(""));
    Reference< XMacroExpander > expander(
                    compContext->getValueByName(
                    ::rtl::OUString::createFromAscii( "/singletons/com.sun.star.util.theMacroExpander" ) ),
                    UNO_QUERY );
    OUString expandedLocation = expander->expandMacros(locationFileURL);
    LF_LOG((OU2A(expandedLocation)));
    LF_LOG(("\n"));
    OUString locationSystemPath;
    FileBase::getSystemPathFromFileURL( expandedLocation, locationSystemPath );
    LF_LOG((OU2A(locationSystemPath)));
    LF_LOG(("\n"));
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
