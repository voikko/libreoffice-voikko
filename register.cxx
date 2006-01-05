/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@cc.jyu.fi), 2005
 *
 *  Based on example code from OOo1.9.104 SDK, file
 *  examples/DevelopersGuide/Database/DriverSkeleton/SServices.cxx.
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

#include <cppuhelper/factory.hxx>
#include <uno/lbnames.h>

#include "sspellimp.hxx"
#include "hyphenimp.hxx"
#include "lfmacros.hxx"


namespace LF_NAMESPACE {

void REGISTER_PROVIDER(
                const OUString& aServiceImplName,
                const Sequence< OUString>& Services,
                const Reference< ::com::sun::star::registry::XRegistryKey > & xKey)
{
        OUString aMainKeyName;
        aMainKeyName = OUString::createFromAscii("/");
        aMainKeyName += aServiceImplName;
        aMainKeyName += OUString::createFromAscii("/UNO/SERVICES");

        Reference< ::com::sun::star::registry::XRegistryKey >  xNewKey( xKey->createKey(aMainKeyName) );
        OSL_ENSURE(xNewKey.is(), "SKELETON::component_writeInfo : could not create a registry key !");

        for (sal_Int32 i=0; i<Services.getLength(); ++i)
                xNewKey->createKey(Services[i]);
}



extern "C" void SAL_CALL component_getImplementationEnvironment(
                                const sal_Char  **ppEnvTypeName,
                                uno_Environment **ppEnv)
{
   LF_LOG(("component_getImplementationEnvironment start\n"));
   *ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
   LF_LOG(("component_getImplementationEnvironment end\n"));
}

extern "C" sal_Bool SAL_CALL component_writeInfo(
                                void* pServiceManager,
                                void* pRegistryKey)
{
  LF_LOG(("component_writeInfo start\n"));
  if (pRegistryKey) {
    try {
      Reference< ::com::sun::star::registry::XRegistryKey >
          xKey(reinterpret_cast< ::com::sun::star::registry::XRegistryKey*>(pRegistryKey));
      REGISTER_PROVIDER(
        SpellChecker::getImplementationName_Static(),
        SpellChecker::getSupportedServiceNames_Static(), xKey);
      REGISTER_PROVIDER(
        Hyphenator::getImplementationName_Static(),
        Hyphenator::getSupportedServiceNames_Static(), xKey);
      return sal_True;
    }
    catch (::com::sun::star::registry::InvalidRegistryException& ) { }
  }
  LF_LOG(("component_writeInfo end\n"));
  return sal_False;
}

Reference< XInterface > SAL_CALL 
SpellChecker_CreateInstance( const Reference< XMultiServiceFactory > & rSMgr )
  throw(Exception)
{
  Reference< XInterface > xService = (cppu::OWeakObject*) new SpellChecker;
  return xService;
}

Reference< XInterface > SAL_CALL 
Hyphenator_CreateInstance( const Reference< XMultiServiceFactory > & rSMgr )
  throw(Exception)
{
  Reference< XInterface > xService = (cppu::OWeakObject*) new Hyphenator;
  return xService;
}

typedef Reference< XSingleServiceFactory > (SAL_CALL *createFactoryFunc)
                (
                        const Reference< XMultiServiceFactory > & rServiceManager,
                        const OUString & rComponentName,
                        ::cppu::ComponentInstantiation pCreateFunction,
                        const Sequence< OUString > & rServiceNames,
                        rtl_ModuleCount* _pTemp
                );

struct ProviderRequest
{
        Reference< XSingleServiceFactory > xRet;
        Reference< XMultiServiceFactory > const xServiceManager;
        OUString const sImplementationName;

        ProviderRequest(
                void* pServiceManager,
                sal_Char const* pImplementationName
        )
        : xServiceManager(reinterpret_cast<XMultiServiceFactory*>(pServiceManager))
        , sImplementationName(OUString::createFromAscii(pImplementationName))
        {
        }

        inline
        sal_Bool CREATE_PROVIDER(
                                const OUString& Implname,
                                const Sequence< OUString > & Services,
                                ::cppu::ComponentInstantiation Factory,
                                createFactoryFunc creator
                        )
        {
                if (!xRet.is() && (Implname == sImplementationName))
                try

                {

                        xRet = creator( xServiceManager, sImplementationName,Factory, Services,0);
                }
                catch(...)
                {
                }
                return xRet.is();
        }

        void* getProvider() const { return xRet.get(); }
};


extern "C" void* SAL_CALL component_getFactory( const sal_Char* pImplementationName,
                                    void* pServiceManager, void* pRegistryKey)
{
  void* pRet = 0;
  LF_LOG(("component_getFactory start\n"));
  if (pServiceManager)
  {
    ProviderRequest aReq(pServiceManager,pImplementationName);

    aReq.CREATE_PROVIDER(
                         SpellChecker::getImplementationName_Static(),
                         SpellChecker::getSupportedServiceNames_Static(),
                         SpellChecker_CreateInstance, ::cppu::createSingleFactory);
    aReq.CREATE_PROVIDER(
                         Hyphenator::getImplementationName_Static(),
                         Hyphenator::getSupportedServiceNames_Static(),
                         Hyphenator_CreateInstance, ::cppu::createSingleFactory);
    if(aReq.xRet.is())
       aReq.xRet->acquire();

    pRet = aReq.getProvider();
  }
  LF_LOG(("component_getFactory end\n"));
  return pRet;
}

}


