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

; These settings are given by the makefile in the command line
  !ifndef INSTALLER_NAME
  !define INSTALLER_NAME "oo2-soikko-Windows-1.1pre1"
  !endif

  !ifndef OUTDIR
  !define OUTDIR "oo2-soikko-Windows-1.1pre1"
  !endif

  !ifndef SRCDIR
  !define SRCDIR "..\..\build\oo2-soikko-Windows-1.1pre1"
  !endif

  !ifndef LICENSE_FILE
  !define LICENSE_FILE "..\..\build\oo2-soikko-Windows-1.1pre1\COPYING"
  !endif

  !ifndef INSTALLER_FILE
  !define INSTALLER_FILE "..\..\build\oo2-soikko-Windows-1.1pre1.exe"
  !endif

  !ifndef UNINSTALLER_FILE
  !define UNINSTALLER_FILE "uninstall-oo2-soikko-Windows-1.1pre1.exe"
  !endif

  !ifndef LF_PORT
  !define LF_PORT 2083
  !endif

  !ifndef LF_CONFIGURATOR
  !define LF_CONFIGURATOR "oo2-soikko-configurator.exe"
  !endif

  !ifndef LF_PACKAGEFILE
  !define LF_PACKAGEFILE "oo2-soikko-Windows-1.1pre1.uno.pkg"
  !endif
  
  !ifndef LF_NAMESPACE 
  !define LF_NAMESPACE "soikko"
  !endif 

; Window class of the OpenOffice.org application
  !define OO_WINDOW_CLASS "SALFRAME"
  
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
; Macros
!macro CheckRunningApp
    FindWindow $0 ${OO_WINDOW_CLASS}
    StrCmp $0 0 openOfficeClosed
      MessageBox MB_ICONSTOP|MB_OK "OpenOffice.org is running. Close the application and try again."
      Abort
  openOfficeClosed:
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

  !insertmacro MUI_PAGE_WELCOME

  !define MUI_LICENSEPAGE_TEXT_BOTTOM \
    "If you accept the terms of the agreement, click I Agree to continue. \
    You must accept the agreement to install ${INSTALLER_NAME}.$\n\
    License files can be found in the destination folder."
  !insertmacro MUI_PAGE_LICENSE ${LICENSE_FILE}

  !define MUI_DIRECTORYPAGE_VERIFYONLEAVE
  !insertmacro MUI_PAGE_DIRECTORY

  !define MUI_DIRECTORYPAGE_VARIABLE $OO_PATH
  !define MUI_PAGE_CUSTOMFUNCTION_PRE OOInstallationPre
  !define MUI_DIRECTORYPAGE_TEXT_TOP \
    "Setup will install ${INSTALLER_NAME} to the following OpenOffice.org 2.0 \
    installation. To install to a different installation, click \
    Browse and select another installation folder. Click Next to Continue.$\n$\n\
    Note that the Next button will be activated only if the given installation folder contains \
    a valid OpenOffice.org 2.0 installation."
  
  !define MUI_DIRECTORYPAGE_TEXT_DESTINATION "Folder of the OpenOffice.org 2.0 installation"
  !insertmacro MUI_PAGE_DIRECTORY

  !insertmacro MUI_PAGE_INSTFILES

; Uninstaller
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
    ExecDos::exec "/BRAND=Installing ${LF_PACKAGEFILE}" '"$OO_PATH\program\unopkg" add --verbose "$INSTDIR\${LF_PACKAGEFILE}"' "" "$INSTDIR\logs\adding_${LF_PACKAGEFILE}.txt"
  
; Configure package to the OpenOffice.org
    Call ConfigurePackage
    
; Add uninstaller to the add/remove programs
    Call AddUninstaller

; Dump the details to the log file    
    Push "$INSTDIR\logs\details.txt"
    Call DumpLog

SectionEnd

;--------------------------------
Function .onInit
; Check that the OpenOffice.org is not running
  !insertmacro CheckRunningApp
  
; Check if there is previous uninstallers that are not uninstalled
  Call CheckExistingUninstaller
  
; Nasty way to kill the OpenOffice QuickStart
    KillProcDLL::KillProc "soffice.bin"
   
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
      Abort ; If the given path is not OpenOffice.org installation then the path is not valid
  PathGood:
FunctionEnd

;--------------------------------
Function OOInstallationPre
; Try to find the OpenOffice.org from registry  
    ReadRegStr $OO_PATH HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\soffice.exe" "Path"

    StrCmp $OO_PATH "" useDefaultPath pathOk
  useDefaultPath:
      ; Use default OpenOffice.org installation path
      StrCpy $OO_PATH "${OO_PATH_DEFAULT}"

  pathOk:
FunctionEnd

;--------------------------------
Function RemoveExistingPackages

    DetailPrint "Searching for existing packages ..."
    ExecDos::exec "/BRAND=Searching for existing packages" '"$OO_PATH\program\unopkg" list' "" "$INSTDIR\logs\packages.txt"

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
        ExecDos::exec "/BRAND=Removing $R0" '"$OO_PATH\program\unopkg" remove --verbose "$R0"' "" "$INSTDIR\logs\removing_$R0.txt"
        
        Goto readNextLine 
  done:
    FileClose $0
FunctionEnd

;--------------------------------
Function ConfigurePackage

; Start OpenOffice.org so that the configurator can connect to it
    DetailPrint "Starting OpenOffice.org for setting configuration ..."
    Exec '"$OO_PATH\program\soffice.exe" "-nologo" "-accept=socket,host=localhost,port=${LF_PORT};urp;StartOffice.ServiceManager"'
  
; Start the configurator that connect to the OpenOffice.org and configures the soikko
; ExecWait '"$INSTDIR\${LF_CONFIGURATOR}" ${LF_PORT} 30 1' 
    ExecDos::exec "/BRAND=Configuring OpenOffice.org" \
      '"$INSTDIR\${LF_CONFIGURATOR}" ${LF_PORT} 30 1 ${LF_NAMESPACE}.Hyphenator ${LF_NAMESPACE}.SpellChecker fi FI' \
      "" "$INSTDIR\logs\configurator.txt"
  
; Close the OpenOffice that was started before configurator
    DetailPrint "Closing OpenOffice.org application ..."
    FindWindow $0 ${OO_WINDOW_CLASS}
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
    ReadRegStr $OO_PATH HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${INSTALLER_NAME}" "OpenOffice.orgPath" 
    IfFileExists "$OO_PATH\program\unopkg.exe" OOFound
; Try to find the installation location from the OS
      ReadRegStr $OO_PATH HKLM "Software\Microsoft\Windows\CurrentVersion\App Paths\soffice.exe" "Path"
      IfFileExists "$OO_PATH\program\unopkg.exe" OOFound OONotFound
  OOFound:
; OpenOffice.org was found
; Remove the package from the OpenOffice.org
        DetailPrint "Removing ${LF_PACKAGEFILE} from OpenOffice.org  ..."
;        ExecWait '"$OO_PATH\program\unopkg" remove "${LF_PACKAGEFILE}"' $0
        ExecDos::exec "/BRAND=Removing ${LF_PACKAGEFILE}" '"$OO_PATH\program\unopkg.exe" remove "${LF_PACKAGEFILE}"' "" "c:\removing_${LF_PACKAGEFILE}.txt"
  
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
   
; Nasty way to kill the OpenOffice QuickStart
    KillProcDLL::KillProc "soffice.bin"
FunctionEnd


