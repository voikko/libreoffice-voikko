/*************************************************************************
 *
 *  Authors:
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

; These settings are given by the makefile in the command line
  !ifndef INSTALLER_NAME
  !define INSTALLER_NAME "oo2-soikko-Windows-1.1"
  !endif

  !ifndef OUTDIR
  !define OUTDIR "oo2-soikko-Windows-1.1"
  !endif

  !ifndef SRCDIR
  !define SRCDIR "..\..\build\oo2-soikko-Windows-1.1"
  !endif

  !ifndef LICENSE_FILE
  !define LICENSE_FILE "..\..\build\oo2-soikko-Windows-1.1\COPYING"
  !endif

  !ifndef INSTALLER_FILE
  !define INSTALLER_FILE "..\..\build\oo2-soikko-Windows-1.1.exe"
  !endif

  !ifndef UNINSTALLER_FILE
  !define UNINSTALLER_FILE "uninstall-oo2-soikko-Windows-1.1.exe"
  !endif

  !ifndef LF_PORT
  !define LF_PORT 2083
  !endif

  !ifndef LF_CONFIGURATOR
  !define LF_CONFIGURATOR "oo2-lingconfig.exe"
  !endif

  !ifndef LF_PACKAGEFILE
  !define LF_PACKAGEFILE "oo2-soikko-Windows-1.1.uno.pkg"
  !endif
  
  !ifndef LF_NAMESPACE 
  !define LF_NAMESPACE "soikko"
  !endif 

; Window class of the OpenOffice.org application
  !define OO_WINDOW_CLASS_APPLICATION "SALFRAME"
  !define OO_WINDOW_CLASS_QUICKSTART  "SALCOMWND"
  
; Default installation path of the OpenOffice.org
  !define OO_PATH_DEFAULT "$PROGRAMFILES\OpenOffice.org 2.0"
  
; Registry Key where the uninstallers are defined
  !define WINDOWS_UNINSTALLERS "Software\Microsoft\Windows\CurrentVersion\Uninstall" 

; Path of the OpenOffice.org installation
  var /GLOBAL OO_PATH

; The name of the installer
  Name ${INSTALLER_NAME}
  OutFile ${INSTALLER_FILE}

;--------------------------------
; UI strings and translations

LangString InstWelcomepageText ${LANG_ENGLISH} \
    "This wizard will guide you through the installation of ${INSTALLER_NAME}.\r\n\r\n\
     Before starting the Setup, make sure that the OpenOffice.org starts normally and then \
     close all OpenOffice.org applications including the QuickStart. The Setup Wizard will start \
     and close the OpenOffice.org in the end of the installation.\r\n\r\n\
     During the installation ${LF_CONFIGURATOR} tries to connect to OpenOffice.org \
     at the local address 127.0.0.1 and this may cause firewall software to raise a warning. \
     Accept the connection if you want Setup to configure OpenOffice.org automatically.\r\n\r\n\
     Click Next to continue."
LangString InstWelcomepageText ${LANG_FINNISH} \
    "T‰m‰ ohjattu toiminto auttaa sinua asentamaan ohjelman ${INSTALLER_NAME}.\r\n\r\n\
     Varmista ennen asennuksen aloittamista, ett‰ OpenOffice.org k‰ynnistyy normaalisti. \
     Sulje sen j‰lkeen OpenOffice.org ja sen pikakak‰ynnistystoiminto. Asennusohjelma \
     k‰ynnist‰‰ ja sulkee OpenOffice.orgin asennuksen loppuvaiheessa.\r\n\r\n\
     Asennuksen aikana ${LF_CONFIGURATOR} yritt‰‰ ottaa yhteytt‰ OpenOffice.orgiin \
     paikallisen verkko-osoitteen 127.0.0.1 kautta. T‰m‰ voi aiheuttaa palomuurohjelmiston \
     varoituksen. Jos haluat asennusohjelman tekev‰n tarvittavat asetusmuutokset \
     OpenOffice.orgiin automaattisesti, hyv‰ksy t‰m‰ yhteys.\r\n\r\n\
     Paina Seuraava jatkaaksesi."
LangString InstLicensepageTextBottom ${LANG_ENGLISH} \
    "If you accept the terms of the agreement, click I Agree to continue. \
     You must accept the agreement to install ${INSTALLER_NAME}.$\n\
     License files can be found in the destination folder."
LangString InstLicensepageTextBottom ${LANG_FINNISH} \
    "Jos hyv‰ksyt n‰m‰ ehdot, paina Hyv‰ksyn. \
     Sinun on hyv‰ksytt‰v‰ n‰m‰ ehdot jotta voit asentaa ohjelman ${INSTALLER_NAME}.$\n\
     Lisenssitiedostot lˆytyv‰t asennuksen kohdehakemistosta."
LangString InstPageHeaderText ${LANG_ENGLISH} "Choose OpenOffice.org 2.0 Installation"
LangString InstPageHeaderText ${LANG_FINNISH} "Valitse OpenOffice.org 2.0:n asennushakemisto"
LangString InstPageHeaderSubtext ${LANG_ENGLISH} "Choose the OpenOffice.org 2.0 folder in which to install ${INSTALLER_NAME}."
LangString InstPageHeaderSubtext ${LANG_FINNISH} "Valitse OpenOffice.org 2.0:n asennushakemisto johon ${INSTALLER_NAME} asennetaan."
LangString InstDirectorypageTextTop ${LANG_ENGLISH} \
   "Setup will install ${INSTALLER_NAME} to the following OpenOffice.org 2.0 \
    installation. To install to a different installation, click \
    Browse and select another installation folder. Click Next to continue.$\n$\n\
    Note that the Next button will be activated only if the given installation folder contains \
    a valid OpenOffice.org 2.0 installation."
LangString InstDirectorypageTextTop ${LANG_FINNISH} \
   "Asennusohjelman asentaa ohjelman ${INSTALLER_NAME} seuraavaan OpenOffice.org 2.0:n asennukseen. \
    Jos haluat asentaa jonnekin muualle, paina Selaa ja valitse toinen kansio. Paina Seuraava jatkaaksesi.$\n$\n\
    Huomaa, ett‰ painike Seuraava on aktiivinen vain, jos annettu asennushakemisto sis‰lt‰‰ kelvollisen \
    OpenOffice.org 2.0:n asennuksen."


;--------------------------------
; Macros
!macro CheckRunningApp
    FindWindow $0 ${OO_WINDOW_CLASS_APPLICATION}
    StrCmp $0 0 _openOfficeClosed
      MessageBox MB_ICONSTOP|MB_OK "OpenOffice.org is running. Close it and try again."
      Abort
  _openOfficeClosed:

    FindWindow $0 ${OO_WINDOW_CLASS_QUICKSTART}
    StrCmp $0 0 _quickStartClosed
      MessageBox MB_ICONSTOP|MB_OK "OpenOffice.org QuickStart is running. Close it and try again."
      Abort
  _quickStartClosed:
  
!macroend

;--------------------------------
  !include WinMessages.nsh  ; Windows Message definitions for closing applications
  !include "MUI.nsh"        ; Modern UI
  !include "helper-functions.nsh"

;--------------------------------
  !define MUI_CUSTOMPAGECOMMANDS

;--------------------------------
; Pages 
; Installer

  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT $(InstWelcomepageText)
  !insertmacro MUI_PAGE_WELCOME

  !define MUI_LICENSEPAGE_TEXT_BOTTOM $(InstLicensepageTextBottom)
  !insertmacro MUI_PAGE_LICENSE ${LICENSE_FILE}

  !define MUI_DIRECTORYPAGE_VERIFYONLEAVE
  !insertmacro MUI_PAGE_DIRECTORY

  !define MUI_PAGE_HEADER_TEXT $(InstPageHeaderText)
  !define MUI_PAGE_HEADER_SUBTEXT $(InstPageHeaderSubtext)
  !define MUI_DIRECTORYPAGE_VARIABLE $OO_PATH
  !define MUI_PAGE_CUSTOMFUNCTION_PRE OOInstallationPre
  !define MUI_DIRECTORYPAGE_TEXT_TOP $(InstDirectorypageTextTop)
  !define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Folder of the OpenOffice.org 2.0 installation"
  !insertmacro MUI_PAGE_DIRECTORY

  !insertmacro MUI_PAGE_INSTFILES

; Uninstaller
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT \
    "This wizard will guide you through the uninstallation of ${INSTALLER_NAME}.\r\n\r\n\
     Before starting the uninstallation, close all OpenOffice.org applications including the QuickStart.\r\n\r\n\
     Click Next to continue."
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  
;--------------------------------
;Installer Sections
 
; The stuff to install
Section "Oo2-soikko"
    SectionIn RO
  
    CreateDirectory "$INSTDIR"
    CreateDirectory "$INSTDIR\logs" ; Folder for log files
    
    SetOutPath "$INSTDIR"
    
    WriteUninstaller "$INSTDIR\${UNINSTALLER_FILE}"
  
; Add all files from the source directory 
    DetailPrint "Copying files to $INSTDIR folder ..."
    File /a "${SRCDIR}\*"
  
; Path needs to be set in order to have correct DLL's for the configurator 
    SetOutPath "$OO_PATH\program"
  
; Remove old installation
    Call RemoveExistingPackages
    
; Install soikko to OpenOffice.org
; ExecWait '"$OO_PATH\program\unopkg" add "$INSTDIR\${LF_PACKAGEFILE}"' $0
    DetailPrint "Adding ${LF_PACKAGEFILE} to OpenOffice.org ..."
    ExecDos::exec "/BRAND=Installing ${LF_PACKAGEFILE}" \
                  '"$OO_PATH\program\unopkg" add --verbose "$INSTDIR\${LF_PACKAGEFILE}"' \
                  "" "$INSTDIR\logs\adding_${LF_PACKAGEFILE}.txt"

; Check for the errors
    Pop $0 # return value
    IntCmp $0 0 packageSuccess packageFailed packageFailed

  packageFailed:
    DetailPrint "Failed to add ${LF_PACKAGEFILE} to the OpenOffice.org. Exit code = $0"
    MessageBox MB_ICONSTOP|MB_OK \
      "Failed to add ${LF_PACKAGEFILE} to the OpenOffice.org.$\n$\n\
      Close the installer, make sure that the OpenOffice.org starts normally$\n\
      and start the installer again.$\n$\n\
      The package can be added also manually from OpenOffice.org."
    Goto end
    
  packageSuccess:
  
; Configure package to the OpenOffice.org
    Call ConfigurePackage
    
; Add uninstaller to the add/remove programs
    Call AddUninstaller

; Dump the details to the log file    
  end:
    Push "$INSTDIR\logs\details.txt"
    Call DumpLog

SectionEnd

;--------------------------------
Function .onInit
; Check that the OpenOffice.org is not running
  !insertmacro CheckRunningApp
  
; Check if there is previous uninstallers that are not uninstalled
  Call CheckExistingUninstaller
  
; Check that output dir has been set properly
    StrCmp ${OUTDIR} "" noOutDir outDirDefined
  noOutDir:
      MessageBox MB_ICONSTOP|MB_OK "Internal error. Default destination folder is not defined. Exiting."
      Abort
    
  outDirDefined:
      StrCpy $INSTDIR "$PROGRAMFILES\${OUTDIR}"
FunctionEnd

;--------------------------------
Function .onVerifyInstDir
    IfFileExists $OO_PATH\program\unopkg.exe PathGood
; If the given path is not OpenOffice.org installation then the path is not valid
      Abort 
  PathGood:
FunctionEnd

;--------------------------------
Function OOInstallationPre
; Try to find the OpenOffice.org from registry  
    ReadRegStr $OO_PATH HKLM \
               "Software\Microsoft\Windows\CurrentVersion\App Paths\soffice.exe" \
               "Path"

    StrCmp $OO_PATH "" useDefaultPath pathOk
  useDefaultPath:
      ; Use default OpenOffice.org installation path
      StrCpy $OO_PATH "${OO_PATH_DEFAULT}"

  pathOk:
FunctionEnd

;--------------------------------
Function RemoveExistingPackages

    DetailPrint "Searching for existing packages ..."
    ExecDos::exec "/BRAND=Searching for existing packages"  \
                  '"$OO_PATH\program\unopkg" list'  \
                  "" "$INSTDIR\logs\packages.txt"

    ClearErrors
    FileOpen $0 "$INSTDIR\logs\packages.txt" r
  readNextLine:
    IfErrors done
      FileRead $0 $1
      Push $1 ; Line from the file
      Push "Name: oo2-soikko-" ; Text that is searched
      Call StrStr
      Pop $R0
      StrCmp $R0 "" readNextLine
        Push $1 ; Line from the file
        Push "oo2-soikko-" ; Text that is searched
        Call StrStr
        ; Pop $R0
        ; Push $R0
        Call TrimNewLines
        Pop $R0
        DetailPrint "Removing $R0 from OpenOffice.org ..."
        ExecDos::exec "/BRAND=Removing $R0" \
                      '"$OO_PATH\program\unopkg" remove --verbose "$R0"' \
                      "" "$INSTDIR\logs\removing_$R0.txt"
        
        Goto readNextLine 
  done:
    FileClose $0
FunctionEnd

;--------------------------------
Function ConfigurePackage

; Start OpenOffice.org so that the configurator can connect to it
    SetErrorLevel 0
    
    DetailPrint "Starting OpenOffice.org for setting configuration ..."
    Exec '"$OO_PATH\program\soffice.exe" "-nologo"  \
          "-accept=socket,host=localhost,port=${LF_PORT};urp;StartOffice.ServiceManager"'
    
; Check for errors
   GetErrorLevel $0
   IntCmp $0 0 oo2Success oo2Failed oo2Failed
   
  oo2Failed:
    DetailPrint "Failed to start $OO_PATH\program\soffice.exe."
    MessageBox MB_OK \
      "Failed to start the OpenOffice.org.$\n$\n\
      Configure ${LF_PACKAGEFILE} manually after the installation."
    Goto openOfficeClosed
    
  oo2Success:
  
; Start the configurator that connect to the OpenOffice.org and configures the soikko
    ExecDos::exec "/BRAND=Configuring OpenOffice.org" \
      '"$INSTDIR\${LF_CONFIGURATOR}" ${LF_PORT} 30 1 ${LF_NAMESPACE}.Hyphenator ${LF_NAMESPACE}.SpellChecker fi FI' \
      "" "$INSTDIR\logs\configurator.txt"
      
; Check for the errors
    Pop $0 # return value
    IntCmp $0 0 packageSuccess packageFailed packageFailed

  packageFailed:
    DetailPrint "Failed to configure ${LF_PACKAGEFILE} to the OpenOffice.org. Exit code = $0"
    MessageBox MB_OK \
      "Failed to configure ${LF_PACKAGEFILE} to the OpenOffice.org.$\n$\n\
       Configure ${LF_PACKAGEFILE} manually after the installation."
    
  packageSuccess:
  
; Close the OpenOffice that was started before configurator
    DetailPrint "Closing OpenOffice.org application ..."
    FindWindow $0 ${OO_WINDOW_CLASS_APPLICATION}
    StrCmp $0 0 openOfficeClosed
      SendMessage $0 ${WM_CLOSE} 0 0
    openOfficeClosed:
FunctionEnd

;--------------------------------
Function AddUninstaller
    DetailPrint "Adding uninstaller to Add/remove Programs ..."
    WriteRegStr HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}"   "DisplayName"        "${INSTALLER_NAME}"
    WriteRegStr HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}"   "UninstallString"    "$INSTDIR\${UNINSTALLER_FILE}"
    WriteRegStr HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}"   "InstallLocation"    "$INSTDIR"
    WriteRegDWORD HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}" "NoModify"           1 
    WriteRegDWORD HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}" "NoRepair"           1
    WriteRegStr HKLM "${WINDOWS_UNINSTALLERS}\${INSTALLER_NAME}"   "OpenOffice.orgPath" "$OO_PATH"
FunctionEnd

;--------------------------------
Function CheckExistingUninstaller
    StrCpy $0 0
  loop:
      EnumRegKey $1 HKLM "${WINDOWS_UNINSTALLERS}" $0
      StrCmp $1 "" done
      Push $1 ; Registry key
      Push "oo2-soikko-" ; Text that is searched
      Call StrStr
      Pop $R0
      IntOp $0 $0 + 1
      StrCmp $R0 "" loop
        MessageBox MB_ICONSTOP|MB_OK "Uninstall $R0 from Add/remove programs and try again."
        Abort     
  done:
FunctionEnd

;--------------------------------
; Uninstaller
Section "Uninstall"
; Try to get the path of the OpenOffice.org that was used in installation phase
    DetailPrint "Searching for OpenOffice.org installation ..."
    ReadRegStr $OO_PATH HKLM  \
               "Software\Microsoft\Windows\CurrentVersion\Uninstall\${INSTALLER_NAME}" \
               "OpenOffice.orgPath" 
    IfFileExists "$OO_PATH\program\unopkg.exe" OOFound
    
; Try to find the installation location from the OS
      ReadRegStr $OO_PATH HKLM  \
                 "Software\Microsoft\Windows\CurrentVersion\App Paths\soffice.exe" \
                 "Path"
      IfFileExists "$OO_PATH\program\unopkg.exe" OOFound OONotFound
  OOFound:
  
; OpenOffice.org was found
; Remove the package from the OpenOffice.org
        DetailPrint "Removing ${LF_PACKAGEFILE} from OpenOffice.org  ..."
        ExecDos::exec "/BRAND=Removing ${LF_PACKAGEFILE}"  \
                      '"$OO_PATH\program\unopkg.exe" remove "${LF_PACKAGEFILE}"'  \
                      "" "c:\removing_${LF_PACKAGEFILE}.txt"
  
  OONotFound:  
; Remove uninstaller from the add/remove programs
    DetailPrint "Removing uninstaller from Add/remove programs ..."
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${INSTALLER_NAME}" 

; Check that there is valid path for deleting
    StrCmp $INSTDIR "" noOutDir outDirDefined
noOutDir:
      DetailPrint "Installation folder was not defined. Folder is not deleted."
      MessageBox MB_ICONSTOP|MB_OK "Installation folder was not defined. Folder is not deleted."
      Abort
 outDirDefined:
      DetailPrint "Deleting installation folder $INSTDIR ..."
      RMDir /r $INSTDIR
SectionEnd

;--------------------------------
Function un.onInit
; Check that the OpenOffice.org is not running
    !insertmacro CheckRunningApp
FunctionEnd


