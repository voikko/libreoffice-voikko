/*************************************************************************
 *
 *  Authors:
 *  Harri Pitkänen (hatapitk@iki.fi), 2006
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

 !insertmacro MUI_LANGUAGE "English"
 
;-------------------------------- 
; Installer

LicenseLangString MUILicense ${LANG_ENGLISH} "${LICENSE_FILE_EN}"

LangString InstTypeFull    ${LANG_ENGLISH} "Full"
LangString IntsTypeMinimal ${LANG_ENGLISH} "Minimal"

LangString SecCoreText   ${LANG_ENGLISH} "Oo2-soikko"
LangString SecCoreDesc   ${LANG_ENGLISH} "Required parts of the Oo2-soikko software."
LangString SecConfigText ${LANG_ENGLISH} "Configuring"
LangString SecConfigDesc ${LANG_ENGLISH} \
    "OpenOffice.org will be automatically set to use Oo2-soikko in spellchecking and \
    hyphenation for finnish language."

LangString InstLanguageDialogWindowTitle ${LANG_ENGLISH} "Installer Language"
LangString InstLanguageDialogInfoText ${LANG_ENGLISH} "Please select the language of the installer"
LangString InstWelcomepageText ${LANG_ENGLISH} \
    "This wizard will guide you through the installation of ${INSTALLER_NAME}.\r\n\r\n\
     Before starting the Setup, make sure that the OpenOffice.org starts normally and then \
     close all OpenOffice.org applications including the QuickStart. The Setup Wizard will start \
     and close the OpenOffice.org in the end of the installation.\r\n\r\n\
     During the installation ${LF_CONFIGURATOR} tries to connect to OpenOffice.org \
     at the local address 127.0.0.1 and this may cause firewall software to raise a warning. \
     Accept the connection if you want Setup to configure OpenOffice.org automatically.\r\n\r\n\
     Click Next to continue."
LangString InstLicensepageTextBottom ${LANG_ENGLISH} \
    "If you accept the terms of the agreement, click I Agree to continue. \
     You must accept the agreement to install ${INSTALLER_NAME}.$\n\
     License files can be found in the destination folder."
LangString InstPageHeaderText ${LANG_ENGLISH} "Choose OpenOffice.org 2.0 Installation"
LangString InstPageHeaderSubtext ${LANG_ENGLISH} "Choose the OpenOffice.org 2.0 folder in which to install ${INSTALLER_NAME}."
LangString InstDirectorypageTextTop ${LANG_ENGLISH} \
   "Setup will install ${INSTALLER_NAME} to the following OpenOffice.org 2.0 \
    installation. To install to a different installation, click \
    Browse and select another installation folder. Click Install to continue.$\n$\n\
    Note that the Install button will be activated only if the given installation folder contains \
    a valid OpenOffice.org 2.0 installation."
LangString InstDirectorypageTextDestination ${LANG_ENGLISH} "Folder of the OpenOffice.org 2.0 installation"
LangString InstOnInitInternalErrorText ${LANG_ENGLISH} \
   "Internal error. Default destination folder is not defined. Exiting."
LangString InstUninstallPreviousInstallationText ${LANG_ENGLISH} \
      "Uninstall $R0 from Add/remove programs and try again."
LangString OpenOfficeRunningText ${LANG_ENGLISH} "OpenOffice.org is running. Close it and try again."
LangString OpenOfficeQuickStartRunningText ${LANG_ENGLISH} "OpenOffice.org QuickStart is running. Close it and try again."
LangString InstSearchingExistingPackagesDetailText ${LANG_ENGLISH} "Searching for existing packages ..."
LangString InstSearchingExistingPackagesProgressBarText ${LANG_ENGLISH} "Searching for existing packages"
LangString InstRemovingPackageDetailText ${LANG_ENGLISH} "Removing $R0 from OpenOffice.org ..."
LangString InstRemovingPackageProgressBarText ${LANG_ENGLISH} "Removing $R0"
LangString InstCopyingFilesDetailText ${LANG_ENGLISH} "Copying files to $INSTDIR folder ..."
LangString InstAddingPackageDetailText ${LANG_ENGLISH} "Installing ${LF_PACKAGEFILE} to OpenOffice.org ..."
LangString InstAddingPackageProgressBarText ${LANG_ENGLISH} "Installing ${LF_PACKAGEFILE}"
LangString InstAddingPackageFailedDetailText ${LANG_ENGLISH} \
     "Failed to add ${LF_PACKAGEFILE} to the OpenOffice.org. Exit code = $0"
LangString InstAddingPackageFailedText ${LANG_ENGLISH} \
      "Failed to add ${LF_PACKAGEFILE} to the OpenOffice.org.$\n$\n\
      Close the installer, make sure that the OpenOffice.org starts normally$\n\
      and start the installer again.$\n$\n\
      The package can be added also manually from OpenOffice.org."
LangString InstStartingOpenOfficeDetailText ${LANG_ENGLISH} "Starting OpenOffice.org for setting configuration ..."
LangString InstStartingOpenOfficeFailedDetailText ${LANG_ENGLISH} "Failed to start $OO_PATH\program\soffice.exe."
LangString InstStartingOpenOfficeFailedText ${LANG_ENGLISH} \ 
     "Failed to start the OpenOffice.org.$\n$\n\
      Configure ${LF_PACKAGEFILE} manually after the installation."
LangString InstConfiguringOpenOfficeOrgProgressBarText ${LANG_ENGLISH} "Configuring OpenOffice.org"
LangString InstConfiguringOpenOfficeOrgFailedDetailText ${LANG_ENGLISH} \
      "Failed to configure ${LF_PACKAGEFILE} to the OpenOffice.org. Exit code = $0"
LangString InstConfiguringOpenOfficeFailedText ${LANG_ENGLISH} \
      "Failed to configure ${LF_PACKAGEFILE} to the OpenOffice.org.$\n$\n\
       Configure ${LF_PACKAGEFILE} manually after the installation."
LangString InstClosingOpenOfficeFailedDetailText ${LANG_ENGLISH} "Closing OpenOffice.org ..."
LangString InstAddingUninstallerToAddRemoveProgramsDetailText ${LANG_ENGLISH} \ 
      "Adding uninstaller to Add/remove Programs ..."
      
;-------------------------------- 
; Uninstaller

LangString UninstWelcomepageText ${LANG_ENGLISH} \
    "This wizard will guide you through the uninstallation of ${INSTALLER_NAME}.\r\n\r\n\
     Before starting the uninstallation, make sure that the OpenOffice.org starts normally and then \
     close all OpenOffice.org applications including the QuickStart.\r\n\r\n\
     Click Next to continue."
LangString UninstSearchingOpenOfficeInstallationFromRegistryDetailText ${LANG_ENGLISH} \
      "Searching for OpenOffice.org installation ..."
LangString UninstRemovingPackageDetailText ${LANG_ENGLISH} \
      "Removing ${LF_PACKAGEFILE} from OpenOffice.org  ..."
LangString UninstRemovingPackageProgressBarText ${LANG_ENGLISH} "Removing"
LangString UninstRemovingUninstallerDetailText ${LANG_ENGLISH} \
      "Removing uninstaller from Add/remove programs ..."
LangString UninstInstallationFolderNotDefinedDetailText ${LANG_ENGLISH} \
      "Installation folder was not defined. Folder is not deleted."
LangString UninstInstallationFolderNotDefinedText ${LANG_ENGLISH} \
      "Installation folder was not defined. Folder is not deleted."
LangString UninstDeletingInstallationFolderDetailText ${LANG_ENGLISH} \
      "Deleting installation folder $INSTDIR ..."
