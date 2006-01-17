/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@cc.jyu.fi), 2006
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


#include <lfmacros.hxx>
#include <lfimpl.hxx>
//#include <cstdio>
//#include <cstring>


//#include <rtl/tencinfo.h>
#include <rtl/ustrbuf.hxx>


namespace LF_NAMESPACE {

OString consonants = OU2ISO_1(A2OU("bcdfghjklmnpqrstvwxz"));
OString vowels = OU2ISO_1(A2OU("aeiouy\xe4\xf6"));



/* Begin function implementations */
void lfInitSpeller() {

}

void lfInitHyphenator() {

}

void lfDisposeSpeller() {

}

void lfDisposeHyphenator() {

}

sal_Bool SAL_CALL lfIsValid( const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase) 
		throw(::com::sun::star::lang::IllegalArgumentException,	::com::sun::star::uno::RuntimeException) {

	MutexGuard	aGuard( GetLinguMutex() );
	if (rWord.equalsAscii("hevonen")) return TRUE;
	else return FALSE;
}



Reference< XSpellAlternatives > SAL_CALL lfSpell(const OUString& rWord, sal_Bool isSpellWithDigits, sal_Bool isSpellUpperCase) 
		throw(::com::sun::star::uno::RuntimeException) {

	MutexGuard aGuard(GetLinguMutex());
	Sequence< OUString > suggSeq(1);
	OUString * suggStrings = suggSeq.getArray();
	suggStrings[0] = A2OU("hevonen");
	SpellAlternatives *alternatives = new SpellAlternatives;
	alternatives->word = rWord;
	alternatives->alternatives = suggSeq;
	return alternatives;
}

/* Sets the elements of hyphenationPoints to '-' to all positions where a hyphen could
   occur in word. All other elements are set to ' '. */
void hfSetHyphenationPoints(char * hyphenationPoints, OString word) {
	if (word.getLength() == 0) return;
	hyphenationPoints[0] = ' ';
	if (word.getLength() == 1) return;
	// change to lowercase letters
	word = word.toAsciiLowerCase();
	word = word.replace('\xc4', '\xe4'); // Ä -> ä
	word = word.replace('\xd6', '\xf6'); // Ö -> ö
	// find all hyphenation points
	const sal_Char * wchars = word.getStr();
	for (int i = 1; i <= word.getLength() - 2; i++) {
		if (consonants.indexOf(wchars[i]) != -1 && vowels.indexOf(wchars[i+1]) != -1)
			hyphenationPoints[i] = '-';
		else hyphenationPoints[i] = ' ';
	}
	hyphenationPoints[word.getLength() - 1] = ' ';
}


Reference< XHyphenatedWord > SAL_CALL lfHyphenate(const ::rtl::OUString& aWord, sal_Int16 nMaxLeading)
		throw(::com::sun::star::uno::RuntimeException) { 
	OString oWord(OU2ISO_1(aWord));
	char * hyphenationPoints = new char[oWord.getLength()+1];
	hyphenationPoints[oWord.getLength()] = 0;
	hfSetHyphenationPoints(hyphenationPoints, oWord);
	LF_LOG(("lfHyphenate: aWord='%s'\n", oWord.getStr()));
	LF_LOG(("lfHyphenate: hyphenationPoints='%s'\n", hyphenationPoints));

	// find the hyphenation point
	sal_Int16 hyphenPos = -1;
	for (int i = nMaxLeading; i>0 && hyphenPos == -1; i--)
		if (hyphenationPoints[i] == '-') hyphenPos = i;
	LF_LOG(("lfHyphenate: nMaxLeading=%d, hyphenPos=%d\n", nMaxLeading, hyphenPos));
	
	// return the result
	delete[] hyphenationPoints;
	if (hyphenPos != -1) return new HyphenatedWord(aWord, hyphenPos - 1);
	else return NULL;
}


Reference< XPossibleHyphens > SAL_CALL lfCreatePossibleHyphens(const ::rtl::OUString& aWord)
		throw(::com::sun::star::uno::RuntimeException) {
	OString oWord(OU2ISO_1(aWord));
	Reference< XPossibleHyphens > xRes;
	char * hyphenationPoints = new char[oWord.getLength()];
	hfSetHyphenationPoints(hyphenationPoints, oWord);
	
	/* Count the number of hyphenation points but remove the ones that correspond
	* to a real hyphen in the word. This is required to prevent adding extra
	* soft hyphen where it is not needed. */
	sal_Int16 hpcount = 0;
	for (int i = 0; i < oWord.getLength(); i++) {
		if (hyphenationPoints[i] == '-') {
			if (aWord[i - 1] == '-') hyphenationPoints[i] = ' ';
			else hpcount++;
		}
	}

	Sequence< sal_Int16 > hyphenSeq(hpcount);
	sal_Int16 *pPos = hyphenSeq.getArray();
	OUStringBuffer hyphenatedWordBuffer;
	OUString hyphenatedWord;
	sal_Int16 nHyphCount = 0;
	for (int i = 0; i < oWord.getLength(); i++) {
		hyphenatedWordBuffer.append(aWord[i]);
		if (hyphenationPoints[i + 1] == '-') {
			pPos[nHyphCount] = i;
			hyphenatedWordBuffer.append(sal_Unicode('='));
			nHyphCount++;
		}
	}

	hyphenatedWord = hyphenatedWordBuffer.makeStringAndClear();
	LF_LOG(("\n  hyphenatedWord: '%s'\n", OU2A(hyphenatedWord)));
	xRes = new PossibleHyphens(aWord, hyphenatedWord, hyphenSeq);

	delete[] hyphenationPoints;
	return xRes;
}

OUString lfHyphenatorDisplayName(const Locale& rLocale) {
	if (rLocale == Locale( A2OU("fi"), A2OU("FI"), OUString() ))
		return A2OU("Suomen kielen tavutus (Hunspell-fi)");
	else
		return A2OU("Finnish hyphenator (Hunspell-fi)");
}

OUString lfSpellerDisplayName(const Locale& rLocale) {
	if (rLocale == Locale( A2OU("fi"), A2OU("FI"), OUString() ))
		return A2OU( "Suomen kielen oikoluku (Hunspell-fi)" );
	else
		return A2OU( "Finnish spellchecker (Hunspell-fi)" );
}

}

