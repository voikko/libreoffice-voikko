/*************************************************************************
 *
 *  Authors:
 *  Sun Microsystems Inc., October, 2000
 *  Pasi Ryhänen, 2002/05/23
 *  Harri Pitkänen (hatapitk@iki.fi), 2005
 *
 *  The Contents of this file are made available subject to the terms of
 *  GNU Lesser General Public License Version 2.1
 *
 *  GNU Lesser General Public License Version 2.1
 *  =============================================
 *  Copyright 2000 by Sun Microsystems, Inc.
 *  901 San Antonio Road, Palo Alto, CA 94303, USA
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

#ifndef _HYPHENIMP_HXX_
#define _HYPHENIMP_HXX_


#include <cppuhelper/implbase5.hxx>
#include <cppuhelper/interfacecontainer.hxx>

#include <com/sun/star/lang/XComponent.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/XServiceDisplayName.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/beans/PropertyValues.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/linguistic2/XHyphenator.hpp>
#include <com/sun/star/linguistic2/XSearchableDictionaryList.hpp>
#include <com/sun/star/linguistic2/XLinguServiceEventBroadcaster.hpp>

#include "lfmacros.hxx"
#include "lfimpl.hxx"
#include "XHyphenatedWord_impl.hxx"
#include "XPossibleHyphens_impl.hxx"

using namespace ::rtl;
using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::linguistic2;



///////////////////////////////////////////////////////////////////////////

namespace LF_NAMESPACE {

class Hyphenator :
  public cppu::WeakImplHelper5
<
	XHyphenator,
	XInitialization,
	XComponent,
	XServiceInfo,
	XServiceDisplayName >
{

  Sequence< Locale > aSuppLocales;
	
  ::cppu::OInterfaceContainerHelper aEvtListeners;
  Reference< XPropertyChangeListener >	xPropHelper;
  Reference< XMultiServiceFactory > rSMgr;
  sal_Bool bDisposing;

  // disallow copy-constructor and assignment-operator for now
  Hyphenator(const Hyphenator &);
  Hyphenator & operator = (const Hyphenator &);


public:
  Hyphenator();
	
  virtual ~Hyphenator();

  // XSupportedLocales (for XHyphenator)
  virtual Sequence< Locale > SAL_CALL getLocales() 
    throw(RuntimeException);
  virtual sal_Bool SAL_CALL hasLocale( const Locale& rLocale ) 
    throw(RuntimeException);
	
  // XHyphenator
  virtual Reference< XHyphenatedWord > SAL_CALL
  hyphenate( const OUString& aWord,
	     const Locale& aLocale,
	     sal_Int16 nMaxLeading,                                                                       
	     const PropertyValues& aProperties )
    throw(IllegalArgumentException, RuntimeException);
			      
  virtual Reference< XHyphenatedWord > SAL_CALL
  queryAlternativeSpelling( const OUString& aWord,
			    const Locale& aLocale,
			    sal_Int16 nIndex,
			    const PropertyValues& aProperties )
    throw(IllegalArgumentException, RuntimeException);
					     
  virtual Reference< XPossibleHyphens > SAL_CALL
  createPossibleHyphens( const OUString& aWord,
			 const Locale& aLocale,
			 const PropertyValues& aProperties )
    throw(IllegalArgumentException, RuntimeException);

	
  // XServiceDisplayName
  virtual OUString SAL_CALL 
  getServiceDisplayName( const Locale& rLocale ) 
    throw(RuntimeException);

  // XInitialization
  virtual void SAL_CALL 
  initialize( const Sequence< Any >& rArguments ) 
    throw(Exception, RuntimeException);

  // XComponent
  virtual void SAL_CALL 
  dispose() 
    throw(RuntimeException);
  virtual void SAL_CALL 
  addEventListener( const Reference< XEventListener >& rxListener ) 
    throw(RuntimeException);
  virtual void SAL_CALL 
  removeEventListener( const Reference< XEventListener >& rxListener ) 
    throw(RuntimeException);

  ////////////////////////////////////////////////////////////
  // Service specific part
  //

  // XServiceInfo
  virtual OUString SAL_CALL 
  getImplementationName() 
    throw(RuntimeException);
  virtual sal_Bool SAL_CALL 
  supportsService( const OUString& rServiceName ) 
    throw(RuntimeException);
  virtual Sequence< OUString > SAL_CALL 
  getSupportedServiceNames() 
    throw(RuntimeException);


  static inline OUString	
  getImplementationName_Static() throw();
  static Sequence< OUString >	
  getSupportedServiceNames_Static() throw();
};

inline OUString Hyphenator::getImplementationName_Static() throw() {
	return A2OU(MACROTOSTRING(LF_NAMESPACE)).concat(A2OU(".Hyphenator"));
}

}

///////////////////////////////////////////////////////////////////////////

#endif

