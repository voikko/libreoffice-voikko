/*************************************************************************
 *
 *  Authors:
 *  Harri Pitk‰nen (hatapitk@cc.jyu.fi), 2006
 *  Kai Solehmainen (kai.solehmainen@oikeatoliot.fi), 2006
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

;--------------------------------
; UI strings and translations for Oo2-soikko installer

  !insertmacro MUI_LANGUAGE "Finnish"

;-------------------------------- 
; Installer

LicenseLangString MUILicense ${LANG_FINNISH} "${LICENSE_FILE_FI}"

LangString InstTypeFull    ${LANG_FINNISH} "T‰ysi"
LangString IntsTypeMinimal ${LANG_FINNISH} "Minimaallinen"

LangString SecCoreText   ${LANG_FINNISH} "Oo2-soikko"
LangString SecCoreDesc   ${LANG_FINNISH} "Oo2-soikko-ohjelmiston pakolliset osat."
LangString SecConfigText ${LANG_FINNISH} "Asetusmuutokset"
LangString SecConfigDesc ${LANG_FINNISH} \
    "OpenOffice.org asetetaan automaattisesti k‰ytt‰m‰‰n Oo2-soikko-ohjelmistoa \
     suomen kielen oikoluvussa ja tavutuksessa."

LangString InstLanguageDialogWindowTitle ${LANG_FINNISH} "Asennusohjelman kieli"
LangString InstLanguageDialogInfoText ${LANG_FINNISH} "Valitse asennusohjelman kieli"
LangString InstWelcomepageText ${LANG_FINNISH} \
    "T‰m‰ ohjattu toiminto auttaa sinua asentamaan ohjelman ${INSTALLER_NAME}.\r\n\r\n\
     Varmista ennen asennuksen aloittamista, ett‰ OpenOffice.org k‰ynnistyy normaalisti. \
     Sulje t‰m‰n j‰lkeen OpenOffice.org ja sen pikakak‰ynnistystoiminto. Asennusohjelma \
     k‰ynnist‰‰ ja sulkee OpenOffice.orgin asennuksen loppuvaiheessa.\r\n\r\n\
     Asennuksen aikana ${LF_CONFIGURATOR} yritt‰‰ ottaa yhteytt‰ OpenOffice.orgiin \
     paikallisen verkko-osoitteen 127.0.0.1 kautta. T‰m‰ voi aiheuttaa palomuuriohjelmiston \
     varoituksen. Jos haluat asennusohjelman tekev‰n tarvittavat asetusmuutokset \
     OpenOffice.orgiin automaattisesti, hyv‰ksy t‰m‰ yhteys."
LangString InstLicensepageTextBottom ${LANG_FINNISH} \
    "Jos hyv‰ksyt n‰m‰ ehdot, paina Hyv‰ksyn. \
     Sinun on hyv‰ksytt‰v‰ n‰m‰ ehdot jotta voit asentaa ohjelman ${INSTALLER_NAME}.$\n\
     Lisenssitiedostot lˆytyv‰t asennuksen kohdehakemistosta."
LangString InstPageHeaderText ${LANG_FINNISH} "Valitse OpenOffice.org 2.0:n asennushakemisto"
LangString InstPageHeaderSubtext ${LANG_FINNISH} "Valitse OpenOffice.org 2.0:n asennushakemisto johon ${INSTALLER_NAME} asennetaan."
LangString InstDirectorypageTextTop ${LANG_FINNISH} \
   "Asennusohjelman asentaa ohjelman ${INSTALLER_NAME} seuraavaan OpenOffice.org 2.0:n asennukseen. \
    Jos haluat asentaa jonnekin muualle, paina Selaa ja valitse toinen kansio. Paina Asenna jatkaaksesi.$\n$\n\
    Huomaa, ett‰ painike Asenna on aktiivinen vain, jos annettu asennushakemisto sis‰lt‰‰ kelvollisen \
    OpenOffice.org 2.0:n asennuksen."
LangString InstDirectorypageTextDestination ${LANG_FINNISH} "OpenOffice.org 2.0:n asennushakemisto"
LangString InstOnInitInternalErrorText ${LANG_FINNISH} \
   "Ohjelman sis‰inen virhe. Oletusasennushakemisto ei ole m‰‰ritelty. Keskeytet‰‰n."
LangString InstUninstallPreviousInstallationText ${LANG_FINNISH} \
      "Poista asennus $R0 'Lis‰‰ tai poista sovellus'-toiminnon avulla ja yrit‰ uudelleen."
LangString OpenOfficeRunningText ${LANG_FINNISH} "OpenOffice.org on k‰ynniss‰. Sulje ohjelma ja yrit‰ uudelleen."
LangString OpenOfficeQuickStartRunningText ${LANG_FINNISH} "OpenOffice.org -pikak‰ynnistys on k‰ynniss‰. Sulje ohjelma ja yrit‰ uudelleen."
LangString InstSearchingExistingPackagesDetailText ${LANG_FINNISH} "Etsit‰‰n olemassa olevia paketteja ..."
LangString InstSearchingExistingPackagesProgressBarText ${LANG_FINNISH} "Etsit‰‰n olemassa olevia paketteja"
LangString InstRemovingPackageDetailText ${LANG_FINNISH} "Poistetaan $R0 OpenOffice.org-ohjelmasta ..."
LangString InstRemovingPackageProgressBarText ${LANG_FINNISH} "Poistetaan $R0"
LangString InstCopyingFilesDetailText ${LANG_FINNISH} "Kopioidaan tiedostoja hakemistoon $INSTDIR ..."
LangString InstAddingPackageDetailText ${LANG_FINNISH} "Asennetaan ${LF_PACKAGEFILE} OpenOffice.orgiin ..."
LangString InstAddingPackageProgressBarText ${LANG_FINNISH} "Asennetaan ${LF_PACKAGEFILE}"
LangString InstAddingPackageFailedDetailText ${LANG_FINNISH} \
     "Paketin ${LF_PACKAGEFILE} asennus OpenOffice.orgiin ep‰onnistui. Paluukoodi = $0"
LangString InstAddingPackageFailedText ${LANG_FINNISH} \
      "Paketin ${LF_PACKAGEFILE} asennus OpenOffice.orgiin ep‰onnistui.$\n$\n\
      Sulje asennusohjelma, varmista ett‰ OpenOffice.org k‰ynnistyy normaalisti$\n\
      ja k‰ynnist‰ asennusohjelmas uudelleen.$\n$\n\
      Paketti voidaan asentaa myˆs manuaalisesti OpenOffice.orgissa."
LangString InstStartingOpenOfficeDetailText ${LANG_FINNISH} "K‰ynnistet‰‰n OpenOffice.org asetusmuutosten tekemiseksi ..."
LangString InstStartingOpenOfficeFailedDetailText ${LANG_FINNISH} "Ohjelman $OO_PATH\program\soffice.exe k‰ynnist‰minen ep‰onnistui."
LangString InstStartingOpenOfficeFailedText ${LANG_FINNISH} \ 
     "OpenOffice.orgin k‰ynnist‰minen ep‰onnistui.$\n$\n\
      Tee tarvittavat asetusmuutokset paketille ${LF_PACKAGEFILE} $\n$\n\
      OpenOffice.orgissa asennuksen j‰lkeen."
LangString InstConfiguringOpenOfficeOrgProgressBarText ${LANG_FINNISH} "Tehd‰‰n asetusmuutokset OpenOffice.orgiin"
LangString InstConfiguringOpenOfficeOrgFailedDetailText ${LANG_FINNISH} \ 
      "Asetusmuutosten tekeminen paketille ${LF_PACKAGEFILE} ep‰onnistui OpenOffice.orgissa. Paluukoodi = $0"
LangString InstConfiguringOpenOfficeFailedText ${LANG_FINNISH} \
      "Asetusmuutosten tekeminen paketille ${LF_PACKAGEFILE} ep‰onnistui OpenOffice.orgissa.$\n$\n\
       Tee tarvittavat asetusmuutokset paketille ${LF_PACKAGEFILE} OpenOffice.orgissa asennuksen j‰lkeen."
LangString InstClosingOpenOfficeFailedDetailText ${LANG_FINNISH} "Suljetaan OpenOffice.org ..."
LangString InstAddingUninstallerToAddRemoveProgramsDetailText ${LANG_FINNISH} \
      "Lis‰t‰‰n asennuksen poisto 'Lis‰‰ tai poista sovellus'-toimintoon ..."

; Uninstaller

LangString UninstWelcomepageText ${LANG_FINNISH} \
    "T‰m‰ ohjattu toiminto auttaa sinua poistamaan ohjelman ${INSTALLER_NAME} asennuksen.\r\n\r\n\
     Varmista ennen asennuksen poiston aloittamista, ett‰ OpenOffice.org k‰ynnistyy normaalisti. \
     Sulje t‰m‰n j‰lkeen OpenOffice.org ja sen pikakak‰ynnistystoiminto.\r\n\r\n\
     Paina Seuraava jatkaaksesi."
LangString UninstSearchingOpenOfficeInstallationFromRegistryDetailText ${LANG_FINNISH} \
      "Etsit‰‰n OpenOffice.org asennusta ..."
LangString UninstRemovingPackageDetailText ${LANG_FINNISH} \
      "Poistetaan ${LF_PACKAGEFILE} OpenOffice.orgista  ..."
LangString UninstRemovingPackageProgressBarText ${LANG_FINNISH} "Poistetaan"
LangString UninstRemovingUninstallerDetailText ${LANG_FINNISH} \
      "Poistetaan asennuksen poisto 'Lis‰‰ tai poista sovellus'-toiminnosta ..."
LangString UninstInstallationFolderNotDefinedDetailText ${LANG_FINNISH} \
      "Asennushakemistoa ei oltu m‰‰ritelty. Hakemistoa ei poistettu."
LangString UninstInstallationFolderNotDefinedText ${LANG_FINNISH} \
      "Asennushakemistoa ei oltu m‰‰ritelty. Hakemistoa ei poistettu."
LangString UninstDeletingInstallationFolderDetailText ${LANG_FINNISH} \
      "Poistetaan asennushakemisto $INSTDIR ..."
