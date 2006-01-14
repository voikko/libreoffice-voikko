/*************************************************************************
 *
 *  Authors:
 *  Kai Solehmainen (kai.solehmainen@oikeatoliot.fi), 2005
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

#include <stdio.h>

#ifdef WNT
#include <Windows.h>
#else
#include <unistd.h>
#endif // WNT

#include <cppuhelper/bootstrap.hxx>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/resource/XLocale.hpp>
#include <com/sun/star/linguistic2/XLinguServiceManager.hpp>


using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::bridge;
using namespace com::sun::star::linguistic2;
using namespace com::sun::star::resource;
using namespace rtl;
using namespace cppu;

sal_Bool setLinguServiceImp(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp);
sal_Bool isServiceAvailable(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp);
sal_Bool isServiceConfigured(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp);
sal_Bool contains(const Sequence<OUString>& rSequence, const OUString& rString);
sal_Bool getLinguServiceManager(Reference<XLinguServiceManager>& aManager, 
    sal_Int32 aPort, sal_Int32 aCount, sal_Int32 aWait);

/**
* Sets the given hyphenator and spellchecker for the given locale.
*
* Usage: linguistic-configurator [port] [count] [wait] [hyphenator] [spellChecker] [language] [country]
* 
* Arguments are explained below.
*
* Before the program is started following command has to be executed:
* > soffice "-accept=socket,host=localhost,port=[port];urp;StartOffice.ServiceManager"
*/
int main(int argc, char *argv[])
{
    sal_Bool retVal = 1;

    if (argc != 8)
    {
        // Incorrect arguments so display the usage
        printf("Sets the given hyphenator and spellchecker for the given locale.\n");
        printf("\n");
        printf("usage: %s [port] [count] [wait] [hyphenator] [spellChecker] [language] [country]\n", argv[0]);
        printf("  [port]         Port number of the OpenOffice.org service manager\n");
        printf("  [count]        Stop trying to connect after count re-tries\n");
        printf("  [wait]         Wait wait seconds between re-tries\n");
        printf("  [hyphenator]   Name of the hyphenator service (X.Hyphenator)\n");
        printf("  [spellChecker] Name of the spellChecker service (X.SpellChecker)\n");
        printf("  [language]     Language for which the services are installed\n");
        printf("  [country]      Country for which the services are installed\n");
        printf("\n");
        printf("Before the program is started following command has to be executed:\n");
        printf("> soffice \"-accept=socket,host=localhost,port=[port];urp;StartOffice.ServiceManager\"\n");
    }
    else
    {
        // Parse arguments
        sal_Int32 port         = atoi(argv[1]);
        sal_Int32 count        = atoi(argv[2]);
        sal_Int32 wait         = atoi(argv[3]);
        char*     hyphenator   = argv[4];
        char*     spellChecker = argv[5];
        char*     language     = argv[6];
        char*     country      = argv[7];

        // Try to get the Lingu service manager
        Reference<XLinguServiceManager> rLinguServiceManager;

        if (getLinguServiceManager(rLinguServiceManager, port, count, wait))
        {
            retVal = 0;

            // Configure service for Finnish
            Locale locale(OUString::createFromAscii(language), // "fi"
                OUString::createFromAscii(country), // "FI" 
                OUString::createFromAscii(""));

            // Set the hyphenator
            OUString hyphenatorService = OUString::createFromAscii(
                "com.sun.star.linguistic2.Hyphenator");

            OUString hyphenatorServiceImp = OUString::createFromAscii(
                hyphenator); // "LF_NAMESPACE.Hyphenator"

            if (!setLinguServiceImp(rLinguServiceManager, hyphenatorService,
                locale, hyphenatorServiceImp))
            {
                retVal = 1;
            }

            // Set the spell checker
            OUString spellCheckerService = OUString::createFromAscii(
                "com.sun.star.linguistic2.SpellChecker");
            OUString spellCheckerServiceImp = OUString::createFromAscii(
                spellChecker); // "LF_NAMESPACE.SpellChecker"

            if (!setLinguServiceImp(rLinguServiceManager, spellCheckerService, 
                locale, spellCheckerServiceImp))
            {
                retVal = 1;
            }
        }
    }

	return retVal;
}

/**
 * Sets the linguistic service implementation for the locale and service
 */
sal_Bool setLinguServiceImp(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp)
{
	try
	{
        // Check that the service implementation is available
        if (isServiceAvailable(aManager, aService, aLocale, aServiceImp))
        {
            printf("Kytkeytyminen OpenOffice.orgiin onnistui...\n");

            // Configure the service implementation
            Sequence<OUString> serviceImps(1);
            serviceImps[0] = aServiceImp;

            aManager->setConfiguredServices(aService, aLocale, serviceImps);

            // Check if the service was successfully configured
            if (isServiceConfigured(aManager, aService, aLocale, aServiceImp))
            {
                printf("%s-palvelu otettiin käyttöön\n", OUStringToOString(
                    aServiceImp, RTL_TEXTENCODING_ASCII_US).getStr());
                return 1;
            }
        }
        else
        {
            printf("Virhe: %s-palvelu ei ole käytettävissä\n", 
                OUStringToOString(aServiceImp, 
                RTL_TEXTENCODING_ASCII_US).getStr());
        }
	}
	catch(Exception &e)
	{
		OString o = OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US);
		printf("Kytkeytyminen OpenOffice.orgiin epäonnistui...\n");
	}

    printf("Virhe: %s-palvelun käyttöönotto epäonnistui\n", 
        OUStringToOString(aServiceImp, RTL_TEXTENCODING_ASCII_US).getStr());

	return 0;
}

/**
 * Checks if the service implementation is available in the manager for the locale.
 */
sal_Bool isServiceAvailable(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp)
{
    sal_Bool retVal = sal_False;

    Sequence<OUString> services = 
        aManager->getAvailableServices(aService, aLocale);

    retVal = contains(services, aServiceImp);

    return retVal;
}

/**
 * Checks if the service implementation is configured in the manager for the locale.
 */
sal_Bool isServiceConfigured(Reference<XLinguServiceManager>& aManager,
    const OUString& aService, const Locale& aLocale, 
    const OUString& aServiceImp)
{
    sal_Bool retVal = sal_False;

    Sequence<OUString> services =
        aManager->getConfiguredServices(aService, aLocale);

    retVal = contains(services, aServiceImp);

    return retVal;
}

/**
 * Checks if the sequence contains the string
 */
sal_Bool contains(const Sequence<OUString>& rSequence, const OUString& rString)
{
    sal_Bool retVal = sal_False;

    const OUString* pArray = rSequence.getConstArray();

    for(sal_Int32 i = 0; i < rSequence.getLength(); i++)
    {
        if (pArray[0].equals(rString))
        {
            retVal = sal_True;
            break;
        }
    }

    return retVal;
}
   
/**
 * Returns linguistic service manager of the OpenOffice.org
 */
sal_Bool  getLinguServiceManager(Reference<XLinguServiceManager>& aManager, 
    sal_Int32 aPort, sal_Int32 aCount, sal_Int32 aWait)
{
	// create the initial component context
	Reference<XComponentContext> rComponentContext =
		defaultBootstrap_InitialComponentContext();

	// retrieve the servicemanager from the context
	Reference<XMultiComponentFactory> rServiceManager = 
		rComponentContext->getServiceManager();

	// instantiate a sample service with the servicemanager.
	Reference<XInterface> rInstance =
		rServiceManager->createInstanceWithContext( 
			OUString::createFromAscii("com.sun.star.bridge.UnoUrlResolver"),
			rComponentContext);
	
	// Query for the XUnoUrlResolver interface
	Reference<XUnoUrlResolver> rResolver(rInstance, UNO_QUERY);

	if(!rResolver.is())
	{
		printf("Error: Couldn't instantiate com.sun.star.bridge.UnoUrlResolver service\n");
		return 0;
	}

    sal_Int32 count = 0;

    while(count++ < aCount)
    {
	    try
	    {
		    // resolve the uno-url
            sal_Char tmp[256];
            sprintf(tmp, 
                "uno:socket,host=localhost,port=%u;urp;StarOffice.ServiceManager", 
                aPort);

		    rInstance = rResolver->resolve(OUString::createFromAscii(tmp));
    	
		    if(!rInstance.is())
		    {
			    printf("Error: StarOffice.ServiceManager is not exported from remote counterpart\n");
		    }
            else
            {
		        Reference<XMultiServiceFactory> rOfficeServiceManager(
                    rInstance, UNO_QUERY);

		        if(!rInstance.is())
                {
                    printf("Error: XMultiServiceFactory interface is not exported for StarOffice.ServiceManager\n");
                }
                else
                {
                    // query for linguistic service manager
                    rInstance = rOfficeServiceManager->createInstance(
                        OUString::createFromAscii(
                            "com.sun.star.linguistic2.LinguServiceManager"));
                    
                    if(!rInstance.is())
		            {
			            printf("Error: XLinguServiceManager interface is not exported from XMultiServiceFactory\n");
		            }
                    else
                    {
                        Reference<XLinguServiceManager> rLinguServiceManager(
                            rInstance, UNO_QUERY);

                        aManager = rLinguServiceManager;

                        return 1;
                    }
                }
            }
	    }
	    catch(Exception &e)
	    {
		    OString o = OUStringToOString(e.Message, RTL_TEXTENCODING_ASCII_US);
		    printf("Kytkeytyminen OpenOffice.orgiin epäonnistui...\n");
	    }

#ifdef WNT
        Sleep(aWait * 1000);
#else
        sleep(aWait);
#endif // WNT
    }

	return 0;
}
