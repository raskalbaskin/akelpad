!define MUI_UI "Pages\Modern.exe"
!define PRODUCT_NAME "AkelPad"
!define PRODUCT_VERSION "4.1.1"

;_____________________________________________________________________________________________
;
;                                     Install
;_____________________________________________________________________________________________

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}-${PRODUCT_VERSION}-setup.exe"
SetCompressor /SOLID lzma
SubCaption 3 ' '
BrandingText "${PRODUCT_NAME} ${PRODUCT_VERSION}"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"

############  Functions  ############
!include "FileFunc.nsh"
!insertmacro GetFileName
!insertmacro un.GetFileName
!insertmacro GetParent
!insertmacro un.GetParent

!include "WordFunc.nsh"
!insertmacro WordReplace

############  MUI  ############
!include "MUI.nsh"
!define MUI_ICON "Graphics\IconInstall.ico"
!define MUI_UNICON "Graphics\IconUninstall.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "Graphics\HeaderInstall.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "Graphics\HeaderUninstall.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "Graphics\WizardFinish.bmp"
!define MUI_FINISHPAGE_RUN "$SETUPEXE"

############  Defines  ############
!define LANG_ENGLISH              1033
!define LANG_RUSSIAN              1049
!define LANG_GERMAN               1043
!define LANG_SPANISH              1034
!define LANG_UKRAINIAN            1058
!define LANG_BELORUSSIAN          1059
!define LANG_FRENCH               1036
!define LANG_JAPANESE             1041
!define LANG_BRAZILIAN            1046
!define LANG_ITALIAN              1040
!define LANG_KOREAN               1042
!define LANG_CHINESE_TRADITIONAL  1028
!define LANG_CHINESE_SIMPLIFIED   2052
!define LANG_KYRGYZ               1088
!define LANG_POLISH               1045
!define LANG_TURKISH              1055
!define LANG_TATAR                1092
!define LANG_SLOVAK               1051
!define LANG_ROMANIAN             1048
!define LANG_DUTCH                1043

!define INSTTYPE_STANDARD 1
!define INSTTYPE_TOTALCMD 2
!define INSTTYPE_NOTEPAD  3

############  Variables  ############
Var INI
Var HWND
Var REDCTL
Var INSTTYPE
Var SETUPDIR
Var SETUPEXE
Var TCDIR
Var TCINI
Var SYSLANGUAGE
Var UNSETTINGS
Var UNRESULT

############  Pages  ############
Page Custom CustomShow CustomLeave

PageEx directory
	Caption ' '
	PageCallbacks '' DirectoryShow DirectoryLeave
PageExEnd

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

PageEx un.uninstConfirm
	Caption ' '
	PageCallbacks '' un.uninstConfirmShow un.uninstConfirmLeave
PageExEnd

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Russian"

ChangeUI IDD_INST "${MUI_UI}"

############  Strings  ############
LangString Help ${LANG_ENGLISH} 'Help'
LangString Help ${LANG_RUSSIAN} '�������'
LangString Delete ${LANG_ENGLISH} 'Uninstall'
LangString Delete ${LANG_RUSSIAN} '�������'
LangString WelcomeInfoTitle ${LANG_ENGLISH} 'Welcome to the $(^Name) Setup Wizard'
LangString WelcomeInfoTitle ${LANG_RUSSIAN} '��� ������������ ������ ��������� $(^Name)'
LangString WelcomeInfoText ${LANG_ENGLISH} 'Choose install type.'
LangString WelcomeInfoText ${LANG_RUSSIAN} '�������� ��� ���������.'
LangString TypeStandard ${LANG_ENGLISH} 'Standard install'
LangString TypeStandard ${LANG_RUSSIAN} '����������� ���������'
LangString TypeTotalcmd ${LANG_ENGLISH} 'Editor for Total Commander'
LangString TypeTotalcmd ${LANG_RUSSIAN} '�������� ��� Total Commander'
LangString TypeNotepad ${LANG_ENGLISH} 'Windows notepad replacement'
LangString TypeNotepad ${LANG_RUSSIAN} '������ �������� Windows'
LangString TypeStandardText ${LANG_ENGLISH} 'Program will be installed to the specified directory.'
LangString TypeStandardText ${LANG_RUSSIAN} '��������� ����� ����������� � ��������� ����������.'
LangString TypeTotalcmdText ${LANG_ENGLISH} 'Program will be installed as external editor for Total Commander file manager.'
LangString TypeTotalcmdText ${LANG_RUSSIAN} '��������� ����� ����������� ��� ������� �������� ��� ��������� ��������� Total Commander.'
LangString TypeNotepadText ${LANG_ENGLISH} 'Windows notepad will be replaced with program. Copy of the notepad will be restored after program uninstall.'
LangString TypeNotepadText ${LANG_RUSSIAN} '������� Windows ����� ������� ����������. ����� �������� ����� ������������� ����� �������� ���������.'
LangString DirectoryInfoTitle ${LANG_ENGLISH} 'Choose Install Location'
LangString DirectoryInfoTitle ${LANG_RUSSIAN} '����� ����� ���������'
LangString DirectoryInfoText ${LANG_ENGLISH} 'Choose the folder in which to install $(^Name).'
LangString DirectoryInfoText ${LANG_RUSSIAN} '�������� ����� ��� ��������� $(^Name).'
LangString DirectoryTextStandard ${LANG_ENGLISH} 'Setup will install $(^Name) in the following folder. To install in a different folder, click Browse and select another folder.'
LangString DirectoryTextStandard ${LANG_RUSSIAN} '��������� ��������� $(^Name) � ��������� �������. ����� ���������� ��������� � ������ �������, ������� �� ������ "�����".'
LangString DirectoryTextTotalcmd ${LANG_ENGLISH} 'Select Total Commander folder.'
LangString DirectoryTextTotalcmd ${LANG_RUSSIAN} '�������� ������� Total Commander.'
LangString DirectoryTextNotepad ${LANG_ENGLISH} 'System folder.'
LangString DirectoryTextNotepad ${LANG_RUSSIAN} '��������� �������.'
LangString ShortcutGroup ${LANG_ENGLISH} 'Shortcuts'
LangString ShortcutGroup ${LANG_RUSSIAN} '������'
LangString ShortcutQuickLaunch ${LANG_ENGLISH} 'Quick launch'
LangString ShortcutQuickLaunch ${LANG_RUSSIAN} '������� ������'
LangString ShortcutDesktop ${LANG_ENGLISH} 'Desktop'
LangString ShortcutDesktop ${LANG_RUSSIAN} '������� ����'
LangString ShortcutStartMenu ${LANG_ENGLISH} 'Start menu'
LangString ShortcutStartMenu ${LANG_RUSSIAN} '���� ����'
LangString ConfirmInfoTitle ${LANG_ENGLISH} 'Uninstall $(^Name)'
LangString ConfirmInfoTitle ${LANG_RUSSIAN} '�������� $(^Name)'
LangString ConfirmInfoText ${LANG_ENGLISH} 'Remove $(^Name) from your computer.'
LangString ConfirmInfoText ${LANG_RUSSIAN} '�������� $(^Name) �� ����������.'
LangString ConfirmDeleteSettings ${LANG_ENGLISH} 'Delete program settings'
LangString ConfirmDeleteSettings ${LANG_RUSSIAN} '������� ��������� ���������'
LangString InstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort installation, select Cancel.'
LangString InstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} �������.$\n$\n����� ����, ��� �� �������� ${PRODUCT_NAME}, �������� ������.$\n$\n���� �� ������ �������� ���������, �������� ������.'
LangString UninstallAlreadyRun ${LANG_ENGLISH} '${PRODUCT_NAME} running.$\n$\nAfter closing ${PRODUCT_NAME}, select Retry.$\n$\nIf you want abort uninstallation, select Cancel.'
LangString UninstallAlreadyRun ${LANG_RUSSIAN} '${PRODUCT_NAME} �������.$\n$\n����� ����, ��� �� �������� ${PRODUCT_NAME}, �������� ������.$\n$\n���� �� ������ �������� ��������, �������� ������.'
LangString UninstallSuccess ${LANG_ENGLISH} 'Uninstall was completed successfully.'
LangString UninstallSuccess ${LANG_RUSSIAN} '�������� ��������� ������� ���������.'

Function .onInit
	CheckWindow:
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(InstallAlreadyRun)" IDRETRY CheckWindow
	quit

	# Custom page #
	InitPluginsDir

	GetTempFileName $INI $PLUGINSDIR
	File /oname=$INI "Pages\InstallType.ini"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeStandard.ico"
	WriteINIStr "$INI" "Field 7" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeTotalcmd.ico"
	WriteINIStr "$INI" "Field 8" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\IconTypeNotepad.ico"
	WriteINIStr "$INI" "Field 9" "Text" "$0"

	GetTempFileName $0 $PLUGINSDIR
	File /oname=$0 "Graphics\WizardWelcome.bmp"
	WriteINIStr "$INI" "Field 10" "Text" "$0"

	StrCpy $REDCTL 0
	StrCpy $SYSLANGUAGE $LANGUAGE
FunctionEnd

Function CustomShow
	InstallOptions::initDialog /NOUNLOAD "$INI"
	Pop $HWND

	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(WelcomeInfoText)'

	ReadINIStr $0 "$INI" "Field 1" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandard)'

	ReadINIStr $0 "$INI" "Field 2" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmd)'

	ReadINIStr $0 "$INI" "Field 3" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepad)'

	ReadINIStr $0 "$INI" "Field 4" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeStandardText)'

	ReadINIStr $0 "$INI" "Field 5" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeTotalcmdText)'

	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(TypeNotepadText)'

	StrCmp $REDCTL 1 0 +2
	SetCtlColors $0 /BRANDING 0xFF0000

	InstallOptions::show
	Pop $0
FunctionEnd

Function CustomLeave
	ReadINIStr $0 "$INI" "Settings" "State"
	StrCmp $0 0 next
	StrCmp $0 1 color_standard
	StrCmp $0 2 color_totalcmd
	StrCmp $0 3 color_notepad
	abort

	color_standard:
	color_totalcmd:
	StrCmp $REDCTL 0 0 +2
	abort

	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0x000000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 0
	abort

	color_notepad:
	ReadINIStr $0 "$INI" "Field 6" "HWND"
	SetCtlColors $0 /BRANDING 0xFF0000
	ShowWindow $0 ${SW_HIDE}
	ShowWindow $0 ${SW_SHOW}
	StrCpy $REDCTL 1
	abort

	next:

#	_standard:
	ReadINIStr $0 "$INI" "Field 1" "State"
	StrCmp $0 1 0 _totalcmd
	StrCpy $INSTTYPE ${INSTTYPE_STANDARD}

	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	StrCmp $0 '' 0 +3
	StrCpy $INSTDIR "$PROGRAMFILES\${PRODUCT_NAME}"
	goto end
	${GetParent} "$0" $0
	${GetParent} "$0" $INSTDIR
	goto end

	_totalcmd:
	ReadINIStr $0 "$INI" "Field 2" "State"
	StrCmp $0 1 0 _notepad
	StrCpy $INSTTYPE ${INSTTYPE_TOTALCMD}

	ReadRegStr $0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString"
	StrCmp $0 '' Ghisler
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	${GetParent} "$0" $0
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
	goto end

	Ghisler:
	ReadRegStr $0 HKCU "SOFTWARE\Ghisler\Total Commander" "InstallDir"
	StrCmp $0 '' +4
	IfFileExists "$0\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "$0\${PRODUCT_NAME}"
	goto end
	IfFileExists "C:\TotalCmd\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TotalCmd\${PRODUCT_NAME}"
	goto end
	IfFileExists "C:\TC\TotalCmd.exe" 0 +3
	StrCpy $INSTDIR "C:\TC\${PRODUCT_NAME}"
	goto end
	StrCpy $INSTDIR ""
	goto end

	_notepad:
	ReadINIStr $0 "$INI" "Field 3" "State"
	StrCmp $0 1 0 end
	StrCpy $INSTTYPE ${INSTTYPE_NOTEPAD}

	ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion" "VersionNumber"
	StrCmp $0 '' 0 +3
	StrCpy $INSTDIR "$SYSDIR"
	goto end
	StrCpy $INSTDIR "$WINDIR"

	end:
FunctionEnd

Function DirectoryShow
	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryInfoText)'

	FindWindow $R0 '#32770' '' $HWNDPARENT

	GetDlgItem $0 $R0 1006
	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextStandard)'
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextTotalcmd)'
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 +2
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(DirectoryTextNotepad)'

	GetDlgItem $0 $R0 1050
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutGroup)'

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutQuickLaunch)'

	GetDlgItem $0 $R0 1052
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutDesktop)'

	GetDlgItem $0 $R0 1053
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ShortcutStartMenu)'

	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 +2
	SendMessage $0 ${BM_SETCHECK} 1 0
FunctionEnd

Function .onVerifyInstDir
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 end
	StrCpy $TCDIR $INSTDIR
	IfFileExists "$TCDIR\totalcmd.exe" end
	${GetParent} "$INSTDIR" $TCDIR
	IfFileExists "$TCDIR\totalcmd.exe" end
	Abort

	end:
FunctionEnd

Function DirectoryLeave
	StrCpy $SETUPDIR "$INSTDIR"
	StrCpy $SETUPEXE "$SETUPDIR\AkelPad.exe"
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 quicklaunch
	StrCmp $SETUPDIR $WINDIR +2
	StrCmp $SETUPDIR $SYSDIR 0 +2
	StrCpy $SETUPEXE "$SETUPDIR\notepad.exe"

	quicklaunch:
	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 desktop
	SetOutPath "$SETUPDIR"
	CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$SETUPEXE"

	desktop:
	GetDlgItem $0 $R0 1052
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 startmenu
	SetOutPath "$SETUPDIR"
	CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$SETUPEXE"

	startmenu:
	GetDlgItem $0 $R0 1053
	SendMessage $0 ${BM_GETSTATE} 0 0 $1
	StrCmp $1 1 0 end
	SetOutPath "$SETUPDIR"
	CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$SETUPEXE"
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk" "$SETUPDIR\AkelFiles\Uninstall.exe"
	StrCmp $SYSLANGUAGE ${LANG_RUSSIAN} 0 +3
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Rus.htm"
	goto +2
	CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk" "$SETUPDIR\AkelFiles\Docs\AkelHelp-Eng.htm"

	end:
FunctionEnd

Section
	SetOutPath "$SETUPDIR"
	File /r /x Justify*.* /x PlainText*.* /x PrintClassic*.* "Files\*.*"

	IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
	RegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

#	_standard:
	StrCmp $INSTTYPE ${INSTTYPE_STANDARD} 0 _totalcmd
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	goto RegInfo

	_totalcmd:
	StrCmp $INSTTYPE ${INSTTYPE_TOTALCMD} 0 _notepad
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	StrCpy $TCINI "$TCDIR\Wincmd.ini"
	IfFileExists "$TCINI" +3
	SearchPath $TCINI "Wincmd.ini"
	StrCmp $TCINI '' RegInfo
	ReadINIStr $1 "$TCINI" "Configuration" "Editor"
	StrCmp $1 '' WriteINIEditor
	${GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" WriteINIEditor
	WriteINIStr "$TCINI" "Configuration" "Editor_AkelUndo" "$1"
	WriteINIEditor:
	${WordReplace} "$SETUPDIR\AkelPad.exe" "$TCDIR" "%COMMANDER_PATH%" "+" $1
	WriteINIStr "$TCINI" "Configuration" "Editor" "$1"
	goto RegInfo

	_notepad:
	StrCmp $INSTTYPE ${INSTTYPE_NOTEPAD} 0 RegInfo
	IfFileExists "$WINDIR\notepad_AkelUndo.exe" +2
	CopyFiles /SILENT "$WINDIR\notepad.exe" "$WINDIR\notepad_AkelUndo.exe"
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" +2
	CopyFiles /SILENT "$SYSDIR\notepad.exe" "$SYSDIR\notepad_AkelUndo.exe"
	IfFileExists "$SYSDIR\DLLCACHE\notepad.exe" 0 +3
	Delete "$SYSDIR\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$SETUPDIR\AkelPad.exe" "$SYSDIR\DLLCACHE\notepad.exe"

#	__windir:
	StrCmp $SETUPDIR $WINDIR 0 __sysdir
	IfFileExists "$SETUPDIR\notepad.exe" 0 +2
	Delete "$SETUPDIR\notepad.exe"
	Rename "$SETUPDIR\AkelPad.exe" "$SETUPDIR\notepad.exe"
	ExecWait '"$SETUPDIR\notepad.exe" /reassoc /quit'
	SetOutPath "$SYSDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\notepad.exe"
	goto RegInfo

	__sysdir:
	StrCmp $SETUPDIR $SYSDIR 0 __nonsystem
	IfFileExists "$SETUPDIR\notepad.exe" 0 +2
	Delete "$SETUPDIR\notepad.exe"
	Rename "$SETUPDIR\AkelPad.exe" "$SETUPDIR\notepad.exe"
	ExecWait '"$SETUPDIR\notepad.exe" /reassoc /quit'
	SetOutPath "$WINDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\notepad.exe"
	goto RegInfo

	__nonsystem:
	ExecWait '"$SETUPDIR\AkelPad.exe" /reassoc /quit'
	SetOutPath "$WINDIR"
	File "Redirect\notepad.exe"
	SetOutPath "$SYSDIR"
	File "Redirect\notepad.exe"
	WriteRegStr HKLM "Software\Akelsoft\AkelPad" "Path" "$SETUPDIR\AkelPad.exe"

	RegInfo:
	WriteUninstaller "$SETUPDIR\AkelFiles\Uninstall.exe"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayName" "$(^Name)"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayIcon" "$SETUPEXE"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "DisplayVersion" "${PRODUCT_VERSION}"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}" "UninstallString" "$SETUPDIR\AkelFiles\Uninstall.exe"

	ClearErrors
	ReadRegStr $0 HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule"
	IfErrors 0 end
	StrCmp $SYSLANGUAGE ${LANG_RUSSIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Russian.dll"
	StrCmp $SYSLANGUAGE ${LANG_GERMAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "German.dll"
	StrCmp $SYSLANGUAGE ${LANG_SPANISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Spanish.dll"
	StrCmp $SYSLANGUAGE ${LANG_UKRAINIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Ukrainian.dll"
	StrCmp $SYSLANGUAGE ${LANG_BELORUSSIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Belorussian.dll"
	StrCmp $SYSLANGUAGE ${LANG_FRENCH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "French.dll"
	StrCmp $SYSLANGUAGE ${LANG_JAPANESE} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Japanese.dll"
	StrCmp $SYSLANGUAGE ${LANG_BRAZILIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Brazilian.dll"
	StrCmp $SYSLANGUAGE ${LANG_ITALIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Italian.dll"
	StrCmp $SYSLANGUAGE ${LANG_KOREAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Korean.dll"
	StrCmp $SYSLANGUAGE ${LANG_CHINESE_TRADITIONAL} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Traditional).dll"
	StrCmp $SYSLANGUAGE ${LANG_CHINESE_SIMPLIFIED} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Chinese (Simplified).dll"
	StrCmp $SYSLANGUAGE ${LANG_KYRGYZ} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Kyrgyz.dll"
	StrCmp $SYSLANGUAGE ${LANG_POLISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Polish.dll"
	StrCmp $SYSLANGUAGE ${LANG_TURKISH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Turkish.dll"
	StrCmp $SYSLANGUAGE ${LANG_TATAR} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Tatar.dll"
	StrCmp $SYSLANGUAGE ${LANG_SLOVAK} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Slovak.dll"
	StrCmp $SYSLANGUAGE ${LANG_ROMANIAN} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Romanian.dll"
	StrCmp $SYSLANGUAGE ${LANG_DUTCH} 0 +2
	WriteRegStr HKCU "SOFTWARE\Akelsoft\AkelPad\Options" "LanguageModule" "Dutch.dll"

	end:
SectionEnd


;_____________________________________________________________________________________________
;
;                                     Uninstall
;_____________________________________________________________________________________________

Function un.onInit
	CheckWindow:
	FindWindow $0 "AkelPad4"
	IsWindow $0 +5
	FindWindow $0 "AkelPad3"
	IsWindow $0 +3
	FindWindow $0 "AkelPad2"
	IsWindow $0 0 +3
	MessageBox MB_RETRYCANCEL|MB_ICONEXCLAMATION "$(UninstallAlreadyRun)" IDRETRY CheckWindow
	quit
FunctionEnd

Function un.uninstConfirmShow
	GetDlgItem $0 $HWNDPARENT 1037
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmInfoTitle)'

	GetDlgItem $0 $HWNDPARENT 1038
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmInfoText)'

	FindWindow $R0 '#32770' '' $HWNDPARENT

	GetDlgItem $0 $R0 1051
	SendMessage $0 ${WM_SETTEXT} 1 'STR:$(ConfirmDeleteSettings)'
	SendMessage $0 ${BM_SETCHECK} 1 0
FunctionEnd

Function un.uninstConfirmLeave
	GetDlgItem $0 $R0 1051
	SendMessage $0 ${BM_GETSTATE} 0 0 $UNSETTINGS
FunctionEnd

Section un.install
	${un.GetParent} "$INSTDIR" $SETUPDIR

	IfFileExists "$SETUPDIR\AkelFiles\Plugs\Scripts.dll" 0 +2
	UnRegDLL "$SETUPDIR\AkelFiles\Plugs\Scripts.dll"

#	_notepad:
	IfFileExists "$WINDIR\notepad_AkelUndo.exe" 0 +3
	ExecWait '"$WINDIR\notepad.exe" /deassoc /quit'
	goto +3
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" 0 _totalcmd
	ExecWait '"$SYSDIR\notepad.exe" /deassoc /quit'

	IfFileExists "$WINDIR\notepad_AkelUndo.exe" 0 +3
	Delete "$WINDIR\notepad.exe"
	Rename "$WINDIR\notepad_AkelUndo.exe" "$WINDIR\notepad.exe"
	IfFileExists "$SYSDIR\notepad_AkelUndo.exe" 0 +3
	Delete "$SYSDIR\notepad.exe"
	Rename "$SYSDIR\notepad_AkelUndo.exe" "$SYSDIR\notepad.exe"

	IfFileExists "$SYSDIR\DLLCACHE\notepad.exe" 0 +3
	Delete "$SYSDIR\DLLCACHE\notepad.exe"
	CopyFiles /SILENT "$SYSDIR\notepad.exe" "$SYSDIR\DLLCACHE\notepad.exe"
	DeleteRegValue HKLM "Software\Akelsoft\AkelPad" "Path"

	_totalcmd:
	StrCpy $TCDIR $SETUPDIR
	IfFileExists "$TCDIR\totalcmd.exe" +3
	${un.GetParent} "$SETUPDIR" $TCDIR
	IfFileExists "$TCDIR\totalcmd.exe" 0 _standard
	ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'
	StrCpy $TCINI "$TCDIR\Wincmd.ini"
	IfFileExists "$TCINI" +3
	SearchPath $TCINI "Wincmd.ini"
	StrCmp $TCINI '' DeleteSettings
	ReadINIStr $1 "$TCINI" "Configuration" "Editor"
	StrCmp $1 '' RestoreAkelUndo
	${un.GetFileName} "$1" $2
	StrCmp $2 "Akelpad.exe" 0 DeleteSettings
	DeleteINIStr "$TCINI" "Configuration" "Editor"
	RestoreAkelUndo:
	ReadINIStr $1 "$TCINI" "Configuration" "Editor_AkelUndo"
	StrCmp $1 '' DeleteSettings
	WriteINIStr "$TCINI" "Configuration" "Editor" "$1"
	DeleteINIStr "$TCINI" "Configuration" "Editor_AkelUndo"
	goto DeleteSettings

	_standard:
	ExecWait '"$SETUPDIR\AkelPad.exe" /deassoc /quit'

	DeleteSettings:
	StrCmp $UNSETTINGS 0 DeleteUninstallKey
	DeleteRegKey HKCU "SOFTWARE\Akelsoft\${PRODUCT_NAME}"
	IfFileExists "$SETUPDIR\AkelPad.ini" 0 +2
	Delete "$SETUPDIR\AkelPad.ini"

	DeleteUninstallKey:
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

	SetOutPath "$TEMP"
	Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
	Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Help).lnk"
	Delete "$SMPROGRAMS\${PRODUCT_NAME}\$(Delete).lnk"
	RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

	Delete "$SETUPDIR\AkelFiles\Uninstall.exe"
	;Generate list and include it in script at compile-time
	!execute 'unList\unList.exe /DATE=0 /INSTDIR="Files" /LOG=unList.txt /UNDIR_VAR=$SETUPDIR /MB=0'
	!include 'unList\unList.txt'
	!delfile 'unList\unList.txt'
	RMDir "$SETUPDIR"

	StrCpy $UNRESULT SuccessUninstall
	quit
SectionEnd

Function un.onGUIEnd
	StrCmp $UNRESULT SuccessUninstall 0 +2
	MessageBox MB_ICONINFORMATION|MB_OK "$(UninstallSuccess)"
FunctionEnd
