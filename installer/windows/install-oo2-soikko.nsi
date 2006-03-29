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

; These settings are given by the makefile in the command line.
; In order to use the script through Eclipse plugin definitions are n
; are given here.
  !ifndef INSTALLER_NAME
  !define INSTALLER_NAME "oo2-soikko-Windows-1.1"
  !endif

  !ifndef OUTDIR
  !define OUTDIR "oo2-soikko-Windows-1.1"
  !endif

  !ifndef SRCDIR
  !define SRCDIR "..\..\build\oo2-soikko-Windows-1.1"
  !endif

  !ifndef LICENSE_FILE_EN
  !define LICENSE_FILE_EN "..\..\build\oo2-soikko-Windows-1.1\COPYING"
  !endif
  
  !ifndef LICENSE_FILE_FI
  !define LICENSE_FILE_FI "..\..\build\oo2-soikko-Windows-1.1\COPYING_FI"
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
  
; Remember the installer language in uninstaller
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\${INSTALLER_NAME}" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

; Path of the OpenOffice.org installation
  var /GLOBAL OO_PATH

; The name of the installer
  Name ${INSTALLER_NAME}
  OutFile ${INSTALLER_FILE}
  
;--------------------------------
  !include WinMessages.nsh  ; Windows Message definitions for closing applications
  !include "MUI.nsh"        ; Modern UI
  !include "helper-functions.nsh"

;--------------------------------
  !define MUI_CUSTOMPAGECOMMANDS

;--------------------------------
; Pages 
; Installer

  !define MUI_CUSTOMFUNCTION_GUIINIT myGuiInit

  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT $(InstWelcomepageText)
  !insertmacro MUI_PAGE_WELCOME

  !define MUI_LICENSEPAGE_TEXT_BOTTOM $(InstLicensepageTextBottom)
  !insertmacro MUI_PAGE_LICENSE $(MUILicense)

  !define MUI_DIRECTORYPAGE_VERIFYONLEAVE
  !insertmacro MUI_PAGE_DIRECTORY

  !define MUI_PAGE_HEADER_TEXT $(InstPageHeaderText)
  !define MUI_PAGE_HEADER_SUBTEXT $(InstPageHeaderSubtext)
  !define MUI_DIRECTORYPAGE_VARIABLE $OO_PATH
  !define MUI_PAGE_CUSTOMFUNCTION_PRE OOInstallationPre
  !define MUI_DIRECTORYPAGE_TEXT_TOP $(InstDirectorypageTextTop)
  !define MUI_DIRECTORYPAGE_TEXT_DESTINATION $(InstDirectorypageTextDestination)
  !insertmacro MUI_PAGE_DIRECTORY
  
  !insertmacro MUI_PAGE_COMPONENTS

  !insertmacro MUI_PAGE_INSTFILES

; Uninstaller
  !define MUI_WELCOMEPAGE_TITLE_3LINES
  !define MUI_WELCOMEPAGE_TEXT $(UninstWelcomepageText)
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
; Support for languages  
  !include "locale_english.nsh"
  !include "locale_finnish.nsh"
  
  !insertmacro MUI_RESERVEFILE_LANGDLL
  
;--------------------------------
; Installation types
  InstType $(InstTypeFull)
  InstType $(IntsTypeMinimal)
  
;--------------------------------
; Macros
!macro CheckRunningApp
    FindWindow $0 ${OO_WINDOW_CLASS_APPLICATION}
    StrCmp $0 0 _openOfficeClosed
      MessageBox MB_ICONSTOP|MB_OK $(OpenOfficeRunningText)
      Abort
  _openOfficeClosed:

    FindWindow $0 ${OO_WINDOW_CLASS_QUICKSTART}
    StrCmp $0 0 _quickStartClosed
      MessageBox MB_ICONSTOP|MB_OK $(OpenOfficeQuickStartRunningText)
      Abort
  _quickStartClosed:
!macroend

;--------------------------------
;Installer Sections
 
; The stuff to install
Section "Core" SecCore
    SectionIn RO
   
    CreateDirectory "$INSTDIR"
    CreateDirectory "$INSTDIR\logs" ; Folder for log files
    
    SetOutPath "$INSTDIR"
    
    WriteUninstaller "$INSTDIR\${UNINSTALLER_FILE}"
  
; Add all files from the source directory 
    DetailPrint $(InstCopyingFilesDetailText)
    File /a "${SRCDIR}\*"
  
; Path needs to be set in order to have correct DLL's for the configurator 
    SetOutPath "$OO_PATH\program"
  
; Remove old installation
    Call RemoveExistingPackages
    
; Install soikko to OpenOffice.org
    DetailPrint $(InstAddingPackageDetailText)
    ExecDos::exec "/BRAND=$(InstAddingPackageProgressBarText) Installing ${LF_PACKAGEFILE}" \
                  '"$OO_PATH\program\unopkg" add --verbose "$INSTDIR\${LF_PACKAGEFILE}"' \
                  "" "$INSTDIR\logs\adding_${LF_PACKAGEFILE}.txt"

; Check for the errors
    Pop $0 # return value
    IntCmp $0 0 packageSuccess packageFailed packageFailed

  packageFailed:
    DetailPrint $(InstAddingPackageFailedDetailText)
    MessageBox MB_ICONSTOP|MB_OK $(InstAddingPackageFailedText)
    Goto end
    
  packageSuccess:
    
; Add uninstaller to the add/remove programs
    Call AddUninstaller

; Dump the details to the log file    
  end:
SectionEnd
 
Section "Config" SecConfig
    SectionIn 1
    
; Configure package to the OpenOffice.org
    Call ConfigurePackage
SectionEnd  

Section -Finalize
    Push "$INSTDIR\logs\details.txt"
    Call DumpLog
SectionEnd

;--------------------------------
; Add section descriptions
 !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCore}   $(SecCoreDesc)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecConfig} $(SecConfigDesc)
 !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
Function .onInit    
; Select language.
  !define MUI_LANGDLL_WINDOWTITLE $(InstLanguageDialogWindowTitle)
  !define MUI_LANGDLL_INFO $(InstLanguageDialogInfoText)
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

;--------------------------------
Function myGUIInit
; These checkings has to be done here instead of .onInit-function because
; language texts are modified only after the .onInit-function.

; Check that the OpenOffice.org is not running
  !insertmacro CheckRunningApp
  
; Check if there is previous uninstallers that are not uninstalled
  Call CheckExistingUninstaller
  
; Check that output dir has been set properly
    StrCmp ${OUTDIR} "" noOutDir outDirDefined
  noOutDir:
      MessageBox MB_ICONSTOP|MB_OK $(InstOnInitInternalErrorText)
      Abort
    
  outDirDefined:
      StrCpy $INSTDIR "$PROGRAMFILES\${OUTDIR}"
      
  SectionSetText ${SecCore}   $(SecCoreText)
  SectionSetText ${SecConfig} $(SecConfigText) 
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

    DetailPrint $(InstSearchingExistingPackagesDetailText)
    ExecDos::exec "/BRAND=$(InstSearchingExistingPackagesProgressBarText)"  \
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
        DetailPrint $(InstRemovingPackageDetailText)
        ExecDos::exec "/BRAND=$(InstRemovingPackageProgressBarText)" \
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
    
    DetailPrint $(InstStartingOpenOfficeDetailText)
    Exec '"$OO_PATH\program\soffice.exe" "-nologo"  \
          "-accept=socket,host=localhost,port=${LF_PORT};urp;StartOffice.ServiceManager"'
    
; Check for errors
   GetErrorLevel $0
   IntCmp $0 0 oo2Success oo2Failed oo2Failed
   
  oo2Failed:
    DetailPrint $(InstStartingOpenOfficeFailedDetailText)
    MessageBox MB_OK $(InstStartingOpenOfficeFailedText)
    Goto openOfficeClosed
    
  oo2Success:
  
; Start the configurator that connect to the OpenOffice.org and configures the soikko
    ExecDos::exec "/BRAND=$(InstConfiguringOpenOfficeOrgProgressBarText)" \
      '"$INSTDIR\${LF_CONFIGURATOR}" ${LF_PORT} 30 1 ${LF_NAMESPACE}.Hyphenator ${LF_NAMESPACE}.SpellChecker fi FI' \
      "" "$INSTDIR\logs\configurator.txt"
      
; Check for the errors
    Pop $0 # return value
    IntCmp $0 0 packageSuccess packageFailed packageFailed

  packageFailed:
    DetailPrint $(InstConfiguringOpenOfficeOrgFailedDetailText)
    MessageBox MB_OK $(InstConfiguringOpenOfficeFailedText)
    
  packageSuccess:
  
; Close the OpenOffice that was started before configurator
    DetailPrint $(InstClosingOpenOfficeFailedDetailText)
    FindWindow $0 ${OO_WINDOW_CLASS_APPLICATION}
    StrCmp $0 0 openOfficeClosed
      SendMessage $0 ${WM_CLOSE} 0 0
    openOfficeClosed:
FunctionEnd

;--------------------------------
Function AddUninstaller
    DetailPrint $(InstAddingUninstallerToAddRemoveProgramsDetailText)
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
        MessageBox MB_ICONSTOP|MB_OK $(InstUninstallPreviousInstallationText)
        Abort     
  done:
FunctionEnd

;--------------------------------
; Uninstaller
Section "Uninstall"
; Try to get the path of the OpenOffice.org that was used in installation phase
    DetailPrint $(UninstSearchingOpenOfficeInstallationFromRegistryDetailText)
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
        DetailPrint $(UninstRemovingPackageDetailText)
        ExecDos::exec "/BRAND=$(UninstRemovingPackageProgressBarText) ${LF_PACKAGEFILE}"  \
                      '"$OO_PATH\program\unopkg.exe" remove "${LF_PACKAGEFILE}"'  \
                      "" "c:\removing_${LF_PACKAGEFILE}.txt"
  
  OONotFound:  
; Remove uninstaller from the add/remove programs
    DetailPrint $(UninstRemovingUninstallerDetailText)
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${INSTALLER_NAME}" 
    
; Remove installer language
    DeleteRegKey HKCU ${MUI_LANGDLL_REGISTRY_KEY}

; Check that there is valid path for deleting
    StrCmp $INSTDIR "" noOutDir outDirDefined
noOutDir:
      DetailPrint $(UninstInstallationFolderNotDefinedDetailText)
      MessageBox MB_ICONSTOP|MB_OK $(UninstInstallationFolderNotDefinedText)
      Abort
 outDirDefined:
      DetailPrint $(UninstDeletingInstallationFolderDetailText)
      RMDir /r $INSTDIR
SectionEnd

;--------------------------------
Function un.onInit
; Select language. 
  !insertmacro MUI_UNGETLANGUAGE
  
; Check that the OpenOffice.org is not running
  !insertmacro CheckRunningApp
FunctionEnd

;--------------------------------
; UI strings and translations

