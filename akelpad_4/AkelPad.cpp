#define WIN32_LEAN_AND_MEAN
#define WINVER 0x0500
#define _WIN32_IE 0x0400
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlobj.h>
#include <richedit.h>
#include "StackFunc.h"
#include "WideFunc.h"
#include "AkelEdit\StrFunc.h"
#include "AkelEdit\AkelBuild.h"
#include "AkelFiles\Langs\Resources\resource.h"
#include "AkelFiles\Langs\Resources\version.h"
#include "AkelFiles\Plugs\AkelDLL\AkelDLL.h"
#include "AkelPad.h"
#include "Edit.h"


//Include stack functions
#define StackGetElement
#define StackInsertAfter
#define StackInsertBefore
#define StackInsertIndex
#define StackMoveAfter
#define StackMoveBefore
#define StackMoveIndex
#define StackDelete
#define StackClear
#include "StackFunc.h"

//Include string functions
#ifndef AKELEDIT_STATICBUILD
  #define WideCharLower
  #define xmemcpy
  #define xmemcmp
  #define xmemset
  #define xstrcpynW
#else
  #define WideCharLower_INCLUDED
  #define xmemcpy_INCLUDED
  #define xmemcmp_INCLUDED
  #define xmemset_INCLUDED
  #define xstrcpynW_INCLUDED
#endif
#define WideCharUpper
#define xarraysizeA
#define xarraysizeW
#define xstrcpyA
#define xstrcpyW
#define xstrstrW
#define xstrcmpW
#define xstrcmpinW
#define xstrcmpiW
#define xatoiA
#define xatoiW
#define xuitoaW
#define xitoaW
#define hex2decW
#define dec2hexW
#define hex2binW
#define bin2hexW
#define xprintfW
#include "AkelEdit\StrFunc.h"

//Include wide functions
#define ComboBox_AddStringWide
#define ComboBox_FindStringExactWide
#define ComboBox_FindStringWide
#define ComboBox_GetLBTextLenWide
#define ComboBox_GetLBTextWide
#define ComboBox_InsertStringWide
#define CreateDirectoryWide
#define CreateFileWide
#define CreateFontIndirectWide
#define CreateMDIWindowWide
#define CreateProcessWide
#define CreateWindowExWide
#define DeleteFileWide
#define DispatchMessageWide
#define DragQueryFileWide
#define ExpandEnvironmentStringsWide
#define ExtractIconExWide
#define FileExistsAnsi
#define FileExistsWide
#define FindFirstFileWide
#define FindNextFileWide
#define FindWindowExWide
#define GetCPInfoExWide
#define GetCurrentDirectoryWide
#define GetDateFormatWide
#define GetDlgItemTextWide
#define GetFileAttributesWide
#define GetFullPathNameWide
#define GetKeyNameTextWide
#define GetLongPathNameWide
#define GetMessageWide
#define GetModuleFileNameWide
#define GetModuleHandleWide
#define GetOpenFileNameWide
#define GetSaveFileNameWide
#define GetTimeFormatWide
#define GetWindowLongWide
#define GetWindowTextLengthWide
#define GetWindowTextWide
#define InsertMenuWide
#define IsDialogMessageWide
#define ListBox_AddStringWide
#define ListBox_FindStringWide
#define ListBox_GetTextWide
#define ListBox_InsertStringWide
#define ListView_GetItemWide
#define ListView_InsertColumnWide
#define ListView_InsertItemWide
#define ListView_SetItemWide
#define LoadLibraryWide
#define LoadStringWide
#define ModifyMenuWide
#define PeekMessageWide
#define RegClearKeyAnsi
#define RegClearKeyWide
#define RegCreateKeyExWide
#define RegDeleteKeyWide
#define RegDeleteValueWide
#define RegEnumValueWide
#define RegisterClassWide
#define RegOpenKeyExWide
#define RegQueryValueExWide
#define RegSetValueExWide
#define SearchPathWide
#define SendMessageWide
#define SetCurrentDirectoryWide
#define SetDlgItemTextWide
#define SetFileAttributesWide
#define SetWindowLongWide
#define SetWindowTextWide
#define SHBrowseForFolderWide
#define SHGetPathFromIDListWide
#define StartDocWide
#define StatusBar_SetTextWide
#define TabCtrl_GetItemWide
#define TabCtrl_InsertItemWide
#define TabCtrl_SetItemWide
#define TranslateAcceleratorWide
#include "WideFunc.h"

//Include AEC functions
#define AEC_FUNCTIONS
#include "AkelEdit\AkelEdit.h"


//Process
HANDLE hHeap;
HANDLE hMutex=0;

//WinMain
HINSTANCE hInstance;
DWORD dwCmdShow;
BOOL bNotepadCommandLine=TRUE;

//Identification
DWORD dwExeVersion=0;
BOOL bOldWindows;
BOOL bOldRichEdit=FALSE;
BOOL bOldComctl32;
BOOL bAkelEdit=TRUE;
BOOL bRichEditClass=FALSE;
BOOL bWindowsNT=FALSE;

//Buffers
char szCmdLine[32768];
wchar_t wszCmdLine[32768];
wchar_t wszCmdFile[MAX_PATH]=L"";
unsigned char pcTranslateBuffer[TRANSLATE_BUFFER_SIZE];
char buf[BUFFER_SIZE];
wchar_t wbuf[BUFFER_SIZE];
char buf2[BUFFER_SIZE];
wchar_t wbuf2[BUFFER_SIZE];

//Language
char szLangModule[MAX_PATH]="";
wchar_t wszLangModule[MAX_PATH]=L"";
int nLangModules=0;
HMODULE hLangLib;
DWORD dwLangSystem;
DWORD dwLangCodepageRecognition;

//Procedures
HSTACK hMainProcStack={0};
HSTACK hMainProcRetStack={0};
HSTACK hEditProcStack={0};
HSTACK hEditProcRetStack={0};
HSTACK hFrameProcStack={0};
HSTACK hFrameProcRetStack={0};
WNDPROC lpfnMainProc;
WNDPROCRET lpfnMainProcRet;
WNDPROC lpfnFrameProc;
WNDPROCRET lpfnFrameProcRet;
WNDPROC lpfnEditProc;
WNDPROCRET lpfnEditProcRet;

//Plugins
HSTACK hPluginsStack={0};
HSTACK hPluginListStack={0};
HSTACK hHandlesStack={0};
RECT rcPluginsInitDialog={0};
RECT rcPluginsCurrentDialog={0};
BOOL bSavePluginsStackOnExit=FALSE;
WNDPROC OldHotkeyInputProc=NULL;

//INI
HSTACK hIniStack={0};
char szIniFile[MAX_PATH];
wchar_t wszIniFile[MAX_PATH];
int nSaveSettings=SS_REGISTRY;
int nRegSaveSettings=SS_REGISTRY;

//Main Window
HWND hMainWnd;
HWND hDummyWindow;
RECT rcMainWindowRestored={CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT};
DWORD dwMainStyle=0;
DWORD dwLastMainSize=0;
HACCEL hGlobalAccel;
HACCEL hMainAccel;
HICON hMainIcon;
HCURSOR hCursorDragMove;
HCURSOR hCursorHandOpen;
HCURSOR hCursorHandClose;
HBITMAP hBitmapClose;
HMENU hMainMenu;
HMENU hPopupMenu;
HMENU hPopupEdit;
HMENU hPopupView;
HMENU hPopupCodepage;
HMENU hPopupOpenCodepage;
HMENU hPopupSaveCodepage;
HMENU hPopupHeadline;
HMENU hMenuRecentFiles=NULL;
HMENU hMenuLanguage=NULL;
BOOL bMenuPopupCodepage=TRUE;
BOOL bMenuRecentFiles=FALSE;
BOOL bMenuLanguage=FALSE;
BOOL bMainOnStartFinish=FALSE;
BOOL bEditOnFinish=FALSE;

//Status window
STATUSSTATE ssStatus;
HWND hStatus;
HWND hProgress;
BOOL bStatusBar=TRUE;
int nStatusHeight=0;
int nProgressWidth=0;
DWORD dwStatusPosType=0;

//Clones
HCURSOR hCursorSizeWE;
HCURSOR hCursorSizeNS;
HCURSOR hCursorSizeALL;
HCURSOR hCursorClone=NULL;

//Docks
HDOCK hDocksStack={0};
NSIZE nsSize;
WNDPROC OldDockProc=NULL;
WNDPROC OldCloseButtonProc=NULL;

//Codepages
int *lpCodepageList=NULL;
int nCodepageListLen=0;
int *lpCodepageTable;
int nCodepageTableCount;
BOOL bDefaultBOM;
int nDefaultCodePage;
int nAnsiCodePage;
int nOemCodePage;
DWORD dwCodepageRecognitionBuffer=DETECT_CODEPAGE_SIZE;

//Recent files
wchar_t (*lpwszRecentNames)[MAX_PATH]=NULL;
DWORD *lpdwRecentPositions=NULL;
DWORD *lpdwRecentCodepages=NULL;
int nRecentFiles=RECENTFILES_AMOUNT;
BOOL bSavePositions=TRUE;
BOOL bSaveCodepages=TRUE;

//Date and time
wchar_t wszDateLogFormat[MAX_PATH]=L"";
wchar_t wszDateInsertFormat[MAX_PATH]=L"";
BOOL bDateLog=FALSE;

//Open/Save document
wchar_t wszLastDir[MAX_PATH]=L"";
wchar_t wszDefaultSaveExt[MAX_PATH];
wchar_t wszFilter[MAX_PATH];
int nFilterLen;
BOOL bAutodetect=TRUE;
BOOL bSaveDlg;
DWORD dwOfnFlags;
BOOL bOfnBOM=FALSE;
int nOfnCodePage;
int nMsgCreate=AUTOANSWER_ASK;
int nMsgBinary=AUTOANSWER_ASK;
POINT ptDocumentPos;
BOOL bSaveInReadOnlyMsg=FALSE;
WNDPROC OldFilePreviewProc;

//Modeless
HWND hDlgModeless=NULL;
int nModelessType=MLT_NONE;

//Recode dialog
RECT rcRecodeDlg={0};

//Find/Replace dialog
RECT rcFindAndReplaceDlg={0};
DWORD ftflags=FR_DOWN;
BOOL bReplaceAllAndClose=FALSE;
int nSearchStrings=SEARCHSTRINGS_AMOUNT;
wchar_t *wszFindText_orig=NULL;
wchar_t *wszReplaceText_orig=NULL;
wchar_t *wszFindText=NULL;
wchar_t *wszReplaceText=NULL;
int nFindTextLen=0;
int nReplaceTextLen=0;
WNDPROC OldComboboxEdit;

//Go to line dialog
RECT rcGotoLineDlg={0};
int nGotoType=NT_LINE;

//Options dialog
HHOOK hHookOptions;
BOOL bOptionsSave;
BOOL bOptionsRestart;

//Font/Color
HSTACK hFontsStack={0};
HSTACK hThemesStack={0};
COLORREF crCustColors[16]={0};
BOOL bEditFontChanged=FALSE;
BOOL bColorsChanged=FALSE;
RECT rcColorsInitDialog={0};
RECT rcColorsCurrentDialog={0};

//Print
HWND hWndPreviewEdit=NULL;
HWND hWndPreviewDlg=NULL;
HWND hWndZoomEdit;
HSTACK hPreviewAllPagesStack={0};
HSTACK hPreviewSelPagesStack={0};
RECT rcPreviewDialog={0};
RECT rcPreviewWindow={10, 50, 0, 0};
RECT rcPreviewPaper={0};
RECT rcPreviewZoomed={0};
POINT ptPreviewScroll;
DWORD dwPreviewShowDialog=SW_MAXIMIZE;
int lpZoom[]={25, 50, 75, 100, 150, 200, 300, 400, 500, PREVIEWZOOM_FIT, PREVIEWZOOM_WIDTH};
int nPreviewZoomMaxIndex=8;
int nPreviewZoomValue=PREVIEWZOOM_FIT;
int nPreviewZoomPercent;
int nPreviewCharHeight;
int nPreviewAveCharWidth;
int nPreviewPageCur;
int nPreviewAllPageSum;
int nPreviewSelPageSum;
BOOL bPreviewSelection;
BOOL bPreviewRedrawLock;
HHOOK hHookKeys=NULL;
AEPRINT prn;
PRINTINFO prninfo={0};
LOGFONTW lfPrintFont;
char szPrintHeader[MAX_PATH];
wchar_t wszPrintHeader[MAX_PATH];
char szPrintFooter[MAX_PATH];
wchar_t wszPrintFooter[MAX_PATH];
DWORD dwPrintColor=0;
BOOL bPrintFontEnable=FALSE;
BOOL bPrintHeaderEnable=FALSE;
BOOL bPrintFooterEnable=FALSE;
BOOL bGlobalPrint=FALSE;
BOOL bPrintFontChanged=FALSE;

//Edit state
AECHARRANGE crSel={0};
AECHARRANGE crPrevSel={0};
AECHARINDEX ciCaret={0};
int nDefaultNewLine;
BOOL bOnTop=FALSE;
DWORD dwShowModify=SM_STATUSBAR|SM_TABTITLE_MDI;
BOOL bSaveTime=FALSE;
BOOL bWatchFile=FALSE;
BOOL bSingleOpenFile=FALSE;
BOOL bSingleOpenProgram=TRUE;
BOOL bKeepSpace=FALSE;
int nSelSubtract=0;
int nLoopCase=0;
int nClickURL=2;
char szWordDelimiters[WORD_DELIMITERS_SIZE];
DWORD dwCustomWordBreak=AEWB_LEFTWORDSTART|AEWB_RIGHTWORDEND;
DWORD dwDefaultWordBreak=0;
DWORD dwPaintOptions=0;
HWND hWndReopen=NULL;
WNDPROC OldEditProc;

//Execute
char szCommand[BUFFER_SIZE]="";
wchar_t wszCommand[BUFFER_SIZE]=L"";
char szWorkDir[MAX_PATH]="";
wchar_t wszWorkDir[MAX_PATH]=L"";
char szExeDir[MAX_PATH]="";
wchar_t wszExeDir[MAX_PATH]=L"";

//Associations
char szFileTypesOpen[MAX_PATH];
wchar_t wszFileTypesOpen[MAX_PATH];
char szFileTypesEdit[MAX_PATH];
wchar_t wszFileTypesEdit[MAX_PATH];
char szFileTypesPrint[MAX_PATH];
wchar_t wszFileTypesPrint[MAX_PATH];
DWORD dwFileTypesAssociated=0;

//Mdi
HSTACK hFramesStack={0};
FRAMEDATA fdInit={0};
FRAMEDATA fdLast={0};
FRAMEDATA *lpFrameCurrent=&fdInit;
int nMDI=WMD_SDI;
int nRegMDI=WMD_SDI;
HWND hMdiClient=NULL;
BOOL bMdiMaximize=-1;
BOOL bMdiNoWindows=FALSE;
BOOL bMdiClientRedraw=TRUE;
HWND hTab=NULL;
DWORD dwTabOpenTimer=0;
int nTabOpenItem=-1;
DWORD dwTabOptionsMDI=TAB_VIEW_TOP|TAB_TYPE_STANDARD|TAB_SWITCH_NEXTPREV;
BOOL bKeybLayoutMDI=FALSE;
HSTACK hIconsStack={0};
HIMAGELIST hImageList;
HICON hIconEmpty;
BOOL bTabPressed=FALSE;
RECT rcMdiListInitDialog={0};
RECT rcMdiListCurrentDialog={0};
DWORD dwMdiStyle=WS_MAXIMIZE;
WNDPROC OldMdiClientProc;
WNDPROC OldTabProc;
FRAMEDATA *lpFrame;

//GetProcAddress
HMONITOR (WINAPI *MonitorFromPointPtr)(POINT, DWORD);
BOOL (WINAPI *GetMonitorInfoAPtr)(HMONITOR, LPMONITORINFO);

//GCC
int main()
{
  return 0;
}

//Entry point
extern "C" void _WinMain()
{
  WNDCLASSW wndclassW={0};
  MSG msg;
  const wchar_t *wpCmdLine;
  HMODULE hUser32=NULL;
#ifndef AKELEDIT_STATICBUILD
  HMODULE hAkelLib=NULL;
#endif
  HWND hWndFriend=NULL;
  BOOL bMsgStatus;
  BOOL bExit=FALSE;
  int nMajor;
  int nMinor;
  int nRelease;
  int nBuild;

  //Process
  hHeap=GetProcessHeap();

  //Is unicode Windows
  bOldWindows=!GetWindowsDirectoryW(NULL, 0);

  //Is Windows NT4?
  if (!bOldWindows)
  {
    OSVERSIONINFO ovi;

    ovi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
    GetVersionEx(&ovi);
    if (ovi.dwMajorVersion == 4 && ovi.dwMinorVersion == 0 && ovi.dwPlatformId == VER_PLATFORM_WIN32_NT)
      bWindowsNT=TRUE;
  }

  //Get program version
  {
    DWORD ver[4]={AKELPAD_ID};

    dwExeVersion=MAKE_IDENTIFIER(ver[0], ver[1], ver[2], ver[3]);
  }

  //Initialize WideFunc.h header
  WideInitialize();

  //Set default options before reading from registry/ini
  nAnsiCodePage=GetACP();
  nOemCodePage=GetOEMCP();
  nDefaultCodePage=nAnsiCodePage;
  bDefaultBOM=FALSE;
  nDefaultNewLine=NEWLINE_WIN;
  dwLangCodepageRecognition=dwLangSystem=GetUserDefaultLangID();

  //fdInit.hWndEditParent=NULL;
  //fdInit.ei.hWndEdit=NULL;
  fdInit.ei.pFile=bOldWindows?(LPBYTE)fdInit.szFile:(LPBYTE)fdInit.wszFile;
  fdInit.ei.szFile=fdInit.szFile;
  fdInit.ei.wszFile=fdInit.wszFile;
  fdInit.ei.nCodePage=nDefaultCodePage;
  fdInit.ei.bBOM=bDefaultBOM;
  fdInit.ei.nNewLine=nDefaultNewLine;
  //fdInit.ei.bModified=FALSE;
  //fdInit.ei.bReadOnly=FALSE;
  //fdInit.ei.bWordWrap=FALSE;
  //fdInit.ei.bOvertypeMode=FALSE;
  //fdInit.ei.hWndMaster=NULL;
  //fdInit.ei.hWndClone1=NULL;
  //fdInit.ei.hWndClone2=NULL;
  //fdInit.ei.hWndClone3=NULL;
  //fdInit.szFile[0]='\0';
  //fdInit.wszFile[0]=L'\0';
  //fdInit.nFileLen=0;
  fdInit.hIcon=hIconEmpty;

  //Font
  if (bOldWindows)
  {
    LOGFONTA lfA;

    GetObjectA(GetStockObject(SYSTEM_FONT), sizeof(LOGFONTA), &lfA);
    LogFontAtoW(&lfA, &fdInit.lf);
  }
  else GetObjectW(GetStockObject(SYSTEM_FONT), sizeof(LOGFONTW), &fdInit.lf);
  fdInit.lf.lfHeight=-mod(fdInit.lf.lfHeight);
  fdInit.lf.lfWidth=0;

  //fdInit.lpEditProc=NULL;
  //fdInit.hDataEdit=NULL;
  //fdInit.hDataMaster=NULL;
  //fdInit.hDataClone1=NULL;
  //fdInit.hDataClone2=NULL;
  //fdInit.hDataClone3=NULL;
  //fdInit.ft.dwLowDateTime=0;
  //fdInit.ft.dwHighDateTime=0;
  fdInit.aec.dwFlags=AECLR_ALL;
  fdInit.aec.crCaret=RGB(0x00, 0x00, 0x00);
  fdInit.aec.crBasicText=GetSysColor(COLOR_WINDOWTEXT);
  fdInit.aec.crBasicBk=GetSysColor(COLOR_WINDOW);
  fdInit.aec.crSelText=GetSysColor(COLOR_HIGHLIGHTTEXT);
  fdInit.aec.crSelBk=GetSysColor(COLOR_HIGHLIGHT);
  fdInit.aec.crActiveLineText=fdInit.aec.crBasicText;
  fdInit.aec.crActiveLineBk=fdInit.aec.crBasicBk;
  fdInit.aec.crUrlText=RGB(0x00, 0x00, 0xFF);
  fdInit.aec.crActiveColumn=RGB(0x00, 0x00, 0x00);
  fdInit.aec.crColumnMarker=GetSysColor(COLOR_BTNFACE);
  //fdInit.rcEditWindow.left=0;
  //fdInit.rcEditWindow.top=0;
  //fdInit.rcEditWindow.right=0;
  //fdInit.rcEditWindow.bottom=0;
  //fdInit.rcMasterWindow.left=0;
  //fdInit.rcMasterWindow.top=0;
  //fdInit.rcMasterWindow.right=0;
  //fdInit.rcMasterWindow.bottom=0;
  fdInit.dwInputLocale=(DWORD)-1;

  fdInit.dwEditMargins=EDIT_MARGINS;
  fdInit.nTabStopSize=EDIT_TABSTOPS;
  fdInit.bTabStopAsSpaces=FALSE;
  fdInit.nUndoLimit=EDIT_UNDOLIMIT;
  fdInit.bDetailedUndo=FALSE;
  fdInit.dwWrapType=AEWW_WORD;
  //fdInit.dwWrapLimit=0;
  //fdInit.dwMarker=0;
  //fdInit.dwMappedPrintWidth=0;
  //fdInit.dwCaretOptions=0;
  fdInit.nCaretWidth=1;
  fdInit.dwMouseOptions=MO_LEFTMARGINSELECTION|MO_RICHEDITMOUSE|MO_MOUSEDRAGGING;
  fdInit.dwLineGap=1;
  fdInit.bShowURL=TRUE;
  fdInit.nClickURL=2;
  //fdInit.bUrlPrefixesEnable=FALSE;
  //fdInit.bUrlDelimitersEnable=FALSE;
  fdInit.bWordDelimitersEnable=TRUE;
  //fdInit.bWrapDelimitersEnable=FALSE;
  xstrcpyW(fdInit.wszUrlPrefixes, STR_URL_PREFIXESW);
  xstrcpyW(fdInit.wszUrlLeftDelimiters, STR_URL_LEFTDELIMITERSW);
  xstrcpyW(fdInit.wszUrlRightDelimiters, STR_URL_RIGHTDELIMITERSW);
  xstrcpyW(fdInit.wszWordDelimiters, STR_WORD_DELIMITERSW);
  xstrcpyW(fdInit.wszWrapDelimiters, STR_WRAP_DELIMITERSW);

  xstrcpyW(wszPrintHeader, STR_PRINT_HEADERW);
  xstrcpyW(wszPrintFooter, STR_PRINT_FOOTERW);
  xstrcpyW(wszDefaultSaveExt, STR_DEFAULTSAVEEXTW);
  xstrcpyW(wszFileTypesOpen, STR_ASSOCIATE_OPENW);
  xstrcpyW(wszFileTypesEdit, STR_ASSOCIATE_EDITW);
  xstrcpyW(wszFileTypesPrint, STR_ASSOCIATE_PRINTW);
  xmemcpy(&lfPrintFont, &fdInit.lf, sizeof(LOGFONTW));

  if (GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_IMEASURE, buf, BUFFER_SIZE))
  {
    if (*buf == '0')
    {
      prninfo.dwPageFlags=PSD_INHUNDREDTHSOFMILLIMETERS;
      prninfo.rtMargin.left=1000;
      prninfo.rtMargin.right=1000;
      prninfo.rtMargin.top=1000;
      prninfo.rtMargin.bottom=1000;
    }
    else
    {
      prninfo.dwPageFlags=PSD_INTHOUSANDTHSOFINCHES;
      prninfo.rtMargin.left=500;
      prninfo.rtMargin.right=500;
      prninfo.rtMargin.top=500;
      prninfo.rtMargin.bottom=500;
    }
    prninfo.hDC=NULL;
    prninfo.dwPrintFlags=PD_ALLPAGES;
    prninfo.nFromPage=1;
    prninfo.nToPage=1;
    prninfo.nCopies=1;
  }

  //Get program HINSTANCE
  hInstance=GetModuleHandleWide(NULL);

  //Get program directory
  GetExeDirW(hInstance, wszExeDir, MAX_PATH);
  WideCharToMultiByte(CP_ACP, 0, wszExeDir, -1, szExeDir, MAX_PATH, NULL, NULL);

  //Read options
  xprintfW(wszIniFile, L"%s\\AkelPad.ini", wszExeDir);
  if (OpenIniW(&hIniStack, wszIniFile, FALSE))
  {
    IniGetValueW(&hIniStack, L"Options", L"SaveSettings", INI_DWORD, (LPBYTE)&nSaveSettings, sizeof(DWORD));
    nRegSaveSettings=nSaveSettings;
  }
  ReadOptions();
  RegisterPluginsHotkeys();
  ReadThemes();
  StackFreeIni(&hIniStack);

  if (nDefaultCodePage == CP_UNICODE_UCS2_LE || nDefaultCodePage == CP_UNICODE_UCS2_BE || nDefaultCodePage == CP_UNICODE_UTF8)
    bDefaultBOM=TRUE;
  fdInit.ei.bBOM=bDefaultBOM;
  fdInit.ei.nCodePage=nDefaultCodePage;
  nMDI=nRegMDI;
  if (!lpCodepageList) nCodepageListLen=EnumCodepageList(&lpCodepageList);

  //Normalize search flags
  ftflags=(ftflags & AEFR_DOWN) |
          (ftflags & AEFR_WHOLEWORD) |
          (ftflags & AEFR_MATCHCASE) |
          (ftflags & AEFR_UP) |
          (ftflags & AEFR_BEGINNING) |
          (ftflags & AEFR_SELECTION) |
          (ftflags & AEFR_ESCAPESEQ) |
          (ftflags & AEFR_ALLFILES);

  //Get startup info
  {
    STARTUPINFOA lpStartupInfoA;

    lpStartupInfoA.cb=sizeof(STARTUPINFOA);
    GetStartupInfoA(&lpStartupInfoA);
    dwCmdShow=lpStartupInfoA.wShowWindow;
    if (!dwCmdShow) dwCmdShow=SW_SHOWNORMAL;

    //Mutex
    if (!(lpStartupInfoA.dwFlags & STARTF_NOMUTEX))
    {
      if (hMutex=CreateMutexA(NULL, TRUE, APP_MUTEXA))
      {
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
          WaitForSingleObject(hMutex, INFINITE);
          ReleaseMutex(hMutex);
          CloseHandle(hMutex);
          hMutex=0;
        }
      }
    }
  }

  //Get command line
  wpCmdLine=GetCommandLineParamsW();

  //Get common controls version
  GetFileVersionA("comctl32.dll", &nMajor, &nMinor, &nRelease, &nBuild);
  if (nMajor < 4 || (nMajor == 4 && nMinor < 71))
    bOldComctl32=TRUE;
  else
    bOldComctl32=FALSE;

  //Get functions addresses
  hUser32=GetModuleHandleA("user32.dll");

  MonitorFromPointPtr=(HMONITOR (WINAPI *)(POINT, DWORD))GetProcAddress(hUser32, "MonitorFromPoint");
  GetMonitorInfoAPtr=(BOOL (WINAPI *)(HMONITOR, LPMONITORINFO))GetProcAddress(hUser32, "GetMonitorInfoA");

  //Get command line arguments
  if (nMDI)
  {
    if (bSingleOpenProgram)
    {
      if (hWndFriend=FindWindowExWide(NULL, NULL, APP_MAIN_CLASSW, NULL))
      {
        ActivateWindow(hWndFriend);
        SendMessage(hWndFriend, AKDN_ACTIVATE, 0, 0);
        bExit=TRUE;
      }
    }
  }

  if (*wpCmdLine)
  {
    while (GetCommandLineArgW(wpCmdLine, wbuf, BUFFER_SIZE, NULL, NULL, &wpCmdLine, bNotepadCommandLine))
    {
      if (wbuf[0] == '/')
      {
        if (!xstrcmpiW(wbuf, L"/P"))
        {
          bGlobalPrint=TRUE;
        }
        else if (!xstrcmpiW(wbuf, L"/C+"))
        {
          nMsgCreate=AUTOANSWER_YES;
        }
        else if (!xstrcmpiW(wbuf, L"/C-"))
        {
          nMsgCreate=AUTOANSWER_NO;
        }
        else if (!xstrcmpiW(wbuf, L"/B+"))
        {
          nMsgBinary=AUTOANSWER_YES;
        }
        else if (!xstrcmpiW(wbuf, L"/B-"))
        {
          nMsgBinary=AUTOANSWER_NO;
        }
        else if (!xstrcmpiW(wbuf, L"/X"))
        {
          bNotepadCommandLine=FALSE;
        }
        else if (!xstrcmpiW(wbuf, L"/REASSOC"))
        {
          if (dwFileTypesAssociated & AE_OPEN)
          {
            AssociateFileTypesW(hInstance, wszFileTypesOpen, AE_OPEN|AE_ASSOCIATE);
          }
          else if (dwFileTypesAssociated & AE_EDIT)
          {
            AssociateFileTypesW(hInstance, wszFileTypesEdit, AE_EDIT|AE_ASSOCIATE);
          }
          else if (dwFileTypesAssociated & AE_PRINT)
          {
            AssociateFileTypesW(hInstance, wszFileTypesPrint, AE_PRINT|AE_ASSOCIATE);
          }
          if (dwFileTypesAssociated) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
        }
        else if (!xstrcmpiW(wbuf, L"/DEASSOC"))
        {
          if (dwFileTypesAssociated & AE_OPEN)
          {
            AssociateFileTypesW(hInstance, wszFileTypesOpen, AE_OPEN|AE_DEASSOCIATE);
          }
          else if (dwFileTypesAssociated & AE_EDIT)
          {
            AssociateFileTypesW(hInstance, wszFileTypesEdit, AE_EDIT|AE_DEASSOCIATE);
          }
          else if (dwFileTypesAssociated & AE_PRINT)
          {
            AssociateFileTypesW(hInstance, wszFileTypesPrint, AE_PRINT|AE_DEASSOCIATE);
          }
          if (dwFileTypesAssociated) SHChangeNotify(SHCNE_ASSOCCHANGED, 0, 0, 0);
        }
        else if (!xstrcmpiW(wbuf, L"/QUIT"))
        {
          goto Quit;
        }
        else if (!xstrcmpiW(wbuf, L"/END"))
        {
          break;
        }
        continue;
      }
      if (!*wbuf) continue;
      xstrcpynW(wszCmdFile, wbuf, MAX_PATH);

      if (nMDI)
      {
        if (!hWndFriend)
        {
          if (!*wpCmdLine) break;
          if (hWndFriend=DoFileNewWindow(STARTF_NOMUTEX))
            bExit=TRUE;
        }
      }
      else
      {
        if (bSingleOpenFile)
        {
          if (GetFullNameW(wszCmdFile, wszCmdFile, MAX_PATH))
          {
            if ((hWndFriend=FindWindowExWide(NULL, NULL, APP_SDI_CLASSW, wszCmdFile)) &&
                (hWndFriend=GetParent(hWndFriend)))
            {
              ActivateWindow(hWndFriend);
              SendMessage(hWndFriend, AKDN_ACTIVATE, 0, 0);
              bExit=TRUE;
              goto OpenSendW;
            }
          }
        }
        bExit=FALSE;
        if (!*wpCmdLine) break;

        hWndFriend=DoFileNewWindow(STARTF_NOMUTEX);
      }

      OpenSendW:
      if (hWndFriend)
      {
        if (bGlobalPrint) SendMessage(hWndFriend, AKD_SETFILEPRINT, TRUE, 0);
        if (nMsgCreate != AUTOANSWER_ASK) SendMessage(hWndFriend, AKD_SETMSGCREATE, (WPARAM)nMsgCreate, 0);
        if (nMsgBinary != AUTOANSWER_ASK) SendMessage(hWndFriend, AKD_SETMSGBINARY, (WPARAM)nMsgBinary, 0);

        OpenDocumentSend(hWndFriend, NULL, wszCmdFile, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, 0, TRUE);
      }
    }
  }
  if (bExit) goto Quit;

  //Load DLL's
  hLangLib=hInstance;
  WideCharToMultiByte(CP_ACP, 0, wszLangModule, -1, szLangModule, MAX_PATH, NULL, NULL);

  if (*wszLangModule)
  {
    BOOL bResult;

    xprintfW(wbuf, L"%s\\AkelFiles\\Langs\\%s", wszExeDir, wszLangModule);
    if (bOldWindows)
    {
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, buf, MAX_PATH, NULL, NULL);
      bResult=GetFileVersionA(buf, &nMajor, &nMinor, &nRelease, &nBuild);
    }
    else bResult=GetFileVersionW(wbuf, &nMajor, &nMinor, &nRelease, &nBuild);

    if (bResult && MAKE_IDENTIFIER(nMajor, nMinor, nRelease, nBuild) == dwExeVersion)
    {
      if (!(hLangLib=LoadLibraryWide(wbuf)))
      {
        hLangLib=hInstance;
        LoadStringWide(hLangLib, MSG_ERROR_LOAD_DLL, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, wszLangModule);
        MessageBoxW(NULL, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
    }
    else
    {
      LoadStringWide(hLangLib, MSG_UPDATE_LANGMODULE, wbuf, BUFFER_SIZE);
      xprintfW(wbuf2, wbuf, wszLangModule,
                           nMajor, nMinor, nRelease, nBuild,
                           LOBYTE(dwExeVersion), HIBYTE(dwExeVersion), LOBYTE(HIWORD(dwExeVersion)), HIBYTE(HIWORD(dwExeVersion)));
      MessageBoxW(NULL, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
    }
  }

#ifdef AKELEDIT_STATICBUILD
  OleInitialize(0);
  AE_RegisterClassA(hInstance, TRUE);
  AE_RegisterClassW(hInstance, TRUE);
#else
  if (!(hAkelLib=LoadLibraryWide(L"AkelEdit.dll")))
  {
    LoadStringWide(hLangLib, MSG_ERROR_LOAD_DLL, wbuf, BUFFER_SIZE);
    xprintfW(wbuf2, wbuf, L"AkelEdit.dll");
    MessageBoxW(NULL, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    goto Quit;
  }
#endif

  //GetOpenFileName dialog file filter
  LoadStringWide(hLangLib, STR_FILE_FILTER, wszFilter, MAX_PATH);
  for (nFilterLen=0; wszFilter[nFilterLen]; ++nFilterLen)
    if (wszFilter[nFilterLen] == '|') wszFilter[nFilterLen]='\0';
  wszFilter[++nFilterLen]='\0';

  if (bOldWindows)
  {
    hGlobalAccel=API_LoadAcceleratorsA(hLangLib, MAKEINTRESOURCEA(IDA_ACCEL_GLOBAL));
    hMainAccel=API_LoadAcceleratorsA(hLangLib, MAKEINTRESOURCEA(IDA_ACCEL_MAIN));
    hMainIcon=API_LoadIconA(hLangLib, MAKEINTRESOURCEA(IDI_ICON_MAIN));
    hMainMenu=API_LoadMenuA(hLangLib, MAKEINTRESOURCEA(IDM_MENU_MAIN));
    hPopupMenu=API_LoadMenuA(hLangLib, MAKEINTRESOURCEA(IDM_MENU_POPUP));
    hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
    hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
    hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);
    hCursorHandOpen=(HCURSOR)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDC_CURSOR_HANDOPEN), IMAGE_CURSOR, 0, 0, 0);
    hCursorHandClose=(HCURSOR)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDC_CURSOR_HANDCLOSE), IMAGE_CURSOR, 0, 0, 0);
    hBitmapClose=(HBITMAP)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDB_BITMAP_CLOSE), IMAGE_BITMAP, 0, 0, 0);
    if (nMDI)
    {
      hIconEmpty=(HICON)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDI_ICON_EMPTY), IMAGE_ICON, 0, 0, 0);
      hCursorDragMove=(HCURSOR)API_LoadImageA(hLangLib, MAKEINTRESOURCEA(IDC_CURSOR_DRAGMOVE), IMAGE_CURSOR, 0, 0, 0);
    }
  }
  else
  {
    hGlobalAccel=API_LoadAcceleratorsW(hLangLib, MAKEINTRESOURCEW(IDA_ACCEL_GLOBAL));
    hMainAccel=API_LoadAcceleratorsW(hLangLib, MAKEINTRESOURCEW(IDA_ACCEL_MAIN));
    hMainIcon=API_LoadIconW(hLangLib, MAKEINTRESOURCEW(IDI_ICON_MAIN));
    hMainMenu=API_LoadMenuW(hLangLib, MAKEINTRESOURCEW(IDM_MENU_MAIN));
    hPopupMenu=API_LoadMenuW(hLangLib, MAKEINTRESOURCEW(IDM_MENU_POPUP));
    hCursorSizeWE=LoadCursor(NULL, IDC_SIZEWE);
    hCursorSizeNS=LoadCursor(NULL, IDC_SIZENS);
    hCursorSizeALL=LoadCursor(NULL, IDC_SIZEALL);
    hCursorHandOpen=(HCURSOR)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDC_CURSOR_HANDOPEN), IMAGE_CURSOR, 0, 0, 0);
    hCursorHandClose=(HCURSOR)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDC_CURSOR_HANDCLOSE), IMAGE_CURSOR, 0, 0, 0);
    hBitmapClose=(HBITMAP)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDB_BITMAP_CLOSE), IMAGE_BITMAP, 0, 0, 0);
    if (nMDI)
    {
      hIconEmpty=(HICON)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDI_ICON_EMPTY), IMAGE_ICON, 0, 0, 0);
      hCursorDragMove=(HCURSOR)API_LoadImageW(hLangLib, MAKEINTRESOURCEW(IDC_CURSOR_DRAGMOVE), IMAGE_CURSOR, 0, 0, 0);
    }
  }



  lpfnMainProc=MainProc;
  lpfnMainProcRet=NULL;
  lpfnFrameProc=FrameProc;
  lpfnFrameProcRet=NULL;
  lpfnEditProc=EditProc;
  lpfnEditProcRet=NULL;
  StackProcSet(&hMainProcStack, MainProc, NULL, NULL);
  StackProcSet(&hEditProcStack, EditProc, NULL, NULL);
  StackProcSet(&hFrameProcStack, FrameProc, NULL, NULL);

  InitCommonControls();

  wndclassW.style        =CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
  wndclassW.lpfnWndProc  =CommonMainProc;
  wndclassW.cbClsExtra   =0;
  wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
  wndclassW.hInstance    =hInstance;
  wndclassW.hIcon        =hMainIcon;
  wndclassW.hCursor      =LoadCursor(NULL, IDC_ARROW);
  wndclassW.hbrBackground=(HBRUSH)GetStockObject(HOLLOW_BRUSH);
  wndclassW.lpszMenuName =MAKEINTRESOURCEW(IDM_MENU_MAIN);
  wndclassW.lpszClassName=APP_MAIN_CLASSW;
  if (!RegisterClassWide(&wndclassW)) goto Quit;

  if (nMDI == WMD_MDI)
  {
    wndclassW.style        =CS_HREDRAW|CS_VREDRAW;
    wndclassW.lpfnWndProc  =CommonFrameProc;
    wndclassW.cbClsExtra   =0;
    wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
    wndclassW.hInstance    =hInstance;
    wndclassW.hIcon        =NULL;
    wndclassW.hCursor      =NULL;
    wndclassW.hbrBackground=NULL;
    wndclassW.lpszMenuName =NULL;
    wndclassW.lpszClassName=APP_MDI_CLASSW;
    if (!RegisterClassWide(&wndclassW)) goto Quit;
  }
  else
  {
    wndclassW.style        =0;
    wndclassW.lpfnWndProc  =DummyProc;
    wndclassW.cbClsExtra   =0;
    wndclassW.cbWndExtra   =DLGWINDOWEXTRA;
    wndclassW.hInstance    =hInstance;
    wndclassW.hIcon        =NULL;
    wndclassW.hCursor      =NULL;
    wndclassW.hbrBackground=NULL;
    wndclassW.lpszMenuName =NULL;
    wndclassW.lpszClassName=APP_SDI_CLASSW;
    if (!RegisterClassWide(&wndclassW)) goto Quit;
  }
  EnsureWindowInMonitor(&rcMainWindowRestored);

  hMainWnd=CreateWindowExWide(0,
                              APP_MAIN_CLASSW,              // window class name
                              APP_MAIN_TITLEW,              // window caption
                              WS_OVERLAPPEDWINDOW,          // window style
                              rcMainWindowRestored.left,    // initial x position
                              rcMainWindowRestored.top,     // initial y position
                              rcMainWindowRestored.right,   // initial x size
                              rcMainWindowRestored.bottom,  // initial y size
                              NULL,                         // parent window handle
                              hMainMenu,                    // window menu handle
                              hInstance,                    // program instance handle
                              NULL);                        // creation parameters
  if (!hMainWnd) goto Quit;

  while ((bMsgStatus=GetMessageWide(&msg, NULL, 0, 0)) && bMsgStatus != -1)
  {
    if (!TranslateAccelerator(hMainWnd, hGlobalAccel, &msg))
    {
      if (!TranslateDialog(&hDocksStack, &msg))
      {
        if (!TranslatePlugin(&msg))
        {
          if (!TranslateHotkey(&hPluginsStack, &msg))
          {
            if (!TranslateAcceleratorWide(hMainWnd, hMainAccel, &msg))
            {
              TranslateMessage(&msg);
              DispatchMessageWide(&msg);
            }
          }
        }
      }
    }
    if (bMainOnStartFinish)
    {
      if (GetQueueStatus(QS_ALLINPUT) == 0)
      {
        bMainOnStartFinish=FALSE;
        SendMessage(hMainWnd, AKDN_MAIN_ONSTART_IDLE, 0, 0);
      }
    }
  }
  if (bMsgStatus == -1)
  {
    LoadStringWide(hLangLib, MSG_ERROR_IN_MESSAGE_QUEUE, wbuf, BUFFER_SIZE);
    MessageBoxW(NULL, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
  }


  Quit:
  if (hMutex)
  {
    ReleaseMutex(hMutex);
    CloseHandle(hMutex);
    hMutex=0;
  }
  CodepageListFree(&lpCodepageList);
  FreeMemoryRecentFiles();
  FreeMemorySearch();
  StackDockFree(&hDocksStack);
  StackThemeFree(&hThemesStack);
  StackFramesFree(&hFramesStack);
  StackPluginFree(&hPluginsStack);
  StackProcFree(&hMainProcStack);
  StackProcFree(&hMainProcRetStack);
  StackProcFree(&hEditProcStack);
  StackProcFree(&hEditProcRetStack);
  StackProcFree(&hFrameProcStack);
  StackProcFree(&hFrameProcRetStack);
  if (hLangLib && hLangLib != hInstance) FreeLibrary(hLangLib);
#ifdef AKELEDIT_STATICBUILD
  AE_UnregisterClassA(hInstance);
  AE_UnregisterClassW(hInstance);
  OleUninitialize();
#else
  if (hAkelLib) FreeLibrary(hAkelLib);
#endif
  ExitProcess(0);
}

LRESULT CALLBACK CommonMainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (uMsg >= WM_USER)
  {
    if (uMsg == AKD_GETMAINPROC)
    {
      return StackProcGet(&hMainProcStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETMAINPROC)
    {
      return StackProcSet(&hMainProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnMainProc);
    }
    else if (uMsg == AKD_GETMAINPROCRET)
    {
      return StackProcGet(&hMainProcRetStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETMAINPROCRET)
    {
      return StackProcSet(&hMainProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnMainProcRet);
    }
    else if (uMsg == AKD_GETEDITPROC)
    {
      return StackProcGet(&hEditProcStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETEDITPROC)
    {
      return StackProcSet(&hEditProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnEditProc);
    }
    else if (uMsg == AKD_GETEDITPROCRET)
    {
      return StackProcGet(&hEditProcRetStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETEDITPROCRET)
    {
      return StackProcSet(&hEditProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnEditProcRet);
    }
    else if (uMsg == AKD_GETFRAMEPROC)
    {
      return StackProcGet(&hFrameProcStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETFRAMEPROC)
    {
      return StackProcSet(&hFrameProcStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, &lpfnFrameProc);
    }
    else if (uMsg == AKD_GETFRAMEPROCRET)
    {
      return StackProcGet(&hFrameProcRetStack, wParam, (WNDPROCDATA **)lParam);
    }
    else if (uMsg == AKD_SETFRAMEPROCRET)
    {
      return StackProcSet(&hFrameProcRetStack, (WNDPROC)wParam, (WNDPROCDATA **)lParam, (WNDPROC *)&lpfnFrameProcRet);
    }
    else if (uMsg == AKD_DLLCALL ||
             uMsg == AKD_DLLCALLA ||
             uMsg == AKD_DLLCALLW)
    {
      PLUGINCALLSENDW *lpCallSend=(PLUGINCALLSENDW *)lParam;
      PLUGINCALLSENDW pcsW;
      wchar_t wszPluginFunction[MAX_PATH];

      if (uMsg == AKD_DLLCALLA || (bOldWindows && uMsg == AKD_DLLCALL))
        xprintfW(wszPluginFunction, L"%S", (char *)lpCallSend->pFunction);
      else
        xprintfW(wszPluginFunction, L"%s", (wchar_t *)lpCallSend->pFunction);
      pcsW.pFunction=wszPluginFunction;
      pcsW.lParam=lpCallSend->lParam;
      pcsW.lpbAutoLoad=lpCallSend->lpbAutoLoad;
      return (LRESULT)CallPluginSend(NULL, &pcsW, FALSE);
    }
    else if (uMsg == AKD_DLLFIND ||
             uMsg == AKD_DLLFINDA ||
             uMsg == AKD_DLLFINDW)
    {
      wchar_t wszPluginFunction[MAX_PATH];
      int nPluginFunctionLen;

      if (wParam)
      {
        if (uMsg == AKD_DLLFINDA || (bOldWindows && uMsg == AKD_DLLFIND))
          nPluginFunctionLen=xprintfW(wszPluginFunction, L"%S", (char *)wParam);
        else
          nPluginFunctionLen=xprintfW(wszPluginFunction, L"%s", (wchar_t *)wParam);
        return (LRESULT)StackPluginFind(&hPluginsStack, wszPluginFunction, nPluginFunctionLen);
      }
      if (lParam)
      {
        return (LRESULT)StackHotkeyFind(&hPluginsStack, (WORD)lParam);
      }
      return 0;
    }
    else if (uMsg == AKD_DLLADD ||
             uMsg == AKD_DLLADDA ||
             uMsg == AKD_DLLADDW)
    {
      PLUGINADDW *pa=(PLUGINADDW *)lParam;
      wchar_t wszPluginFunction[MAX_PATH];
      int nPluginFunctionLen;

      if (uMsg == AKD_DLLADDA || (bOldWindows && uMsg == AKD_DLLADD))
        nPluginFunctionLen=xprintfW(wszPluginFunction, L"%S", (char *)pa->pFunction);
      else
        nPluginFunctionLen=xprintfW(wszPluginFunction, L"%s", (wchar_t *)pa->pFunction);
      return (LRESULT)StackPluginAdd(&hPluginsStack, wszPluginFunction, nPluginFunctionLen, pa->wHotkey, pa->bAutoLoad, pa->PluginProc, pa->lpParameter);
    }
    else if (uMsg == AKD_DLLDELETE)
    {
      StackPluginDelete(&hPluginsStack, (void *)lParam);
      return 0;
    }
    else if (uMsg == AKD_DLLSAVE)
    {
      if (lParam == DLLS_ONEXIT)
      {
        bSavePluginsStackOnExit=TRUE;
        return TRUE;
      }
      return StackPluginSave(&hPluginsStack);
    }
  }

  lResult=lpfnMainProc(hWnd, uMsg, wParam, lParam);

  if (uMsg == AKDN_MAIN_ONFINISH)
  {
    //Unload plugins
    StackHandleFree(&hHandlesStack);

    //Destroy windows
    if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
    {
      //Destroy virtual edits
      SplitDestroy(lpFrameCurrent, CN_CLONE1|CN_CLONE2|CN_CLONE3);

      if (nMDI == WMD_PMDI)
      {
        //Destroy last empty tab
        SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->hDataEdit);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_SETWINDOWDATA, (WPARAM)fdInit.hDataEdit, FALSE);
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_DELETEWINDOWDATA, (WPARAM)lpFrameCurrent->hDataEdit, 0);
      }
      StackFrameDelete(&hFramesStack, lpFrameCurrent);

      //Destroy real edits
      SplitDestroy(&fdInit, CN_CLONE1|CN_CLONE2|CN_CLONE3);

      SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)fdInit.ei.hWndEdit, 0);
      DestroyWindow(fdInit.ei.hWndEdit);
      fdInit.ei.hWndEdit=NULL;
      fdInit.hDataEdit=NULL;
    }
    else if (nMDI == WMD_MDI)
    {
      DestroyWindow(hMdiClient);
      hMdiClient=NULL;
    }
    DestroyWindow(hMainWnd);
    hMainWnd=NULL;
  }

  if (lpfnMainProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnMainProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK MainProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_CREATE)
  {
    int iSBParts[]={110, 220, 250, 280, -1};
    int iBorders[3];
    CLIENTCREATESTRUCT ccs;
    DWORD dwClassStyle;
    RECT rcRect;
    HMENU hMenu;

    hPopupEdit=GetSubMenu(hPopupMenu, MENU_POPUP_EDIT);
    hPopupView=GetSubMenu(hPopupMenu, MENU_POPUP_VIEW);
    hPopupCodepage=GetSubMenu(hPopupMenu, MENU_POPUP_CODEPAGE);
    hPopupOpenCodepage=GetSubMenu(hPopupCodepage, MENU_POPUP_CODEPAGE_OPEN);
    hPopupSaveCodepage=GetSubMenu(hPopupCodepage, MENU_POPUP_CODEPAGE_SAVE);
    hPopupHeadline=GetSubMenu(hPopupMenu, MENU_POPUP_HEADLINE);

    hMenu=GetSubMenu(hMainMenu, MENU_FILE_POSITION);
    hMenuRecentFiles=GetSubMenu(hMenu, MENU_FILE_RECENTFILES_4X);
    hMenu=GetSubMenu(hMainMenu, MENU_VIEW_POSITION);
    hMenuLanguage=GetSubMenu(hMenu, MENU_VIEW_LANGUAGE_4X);

    if (!nMDI)
    {
      DeleteMenu(hMainMenu, IDM_FILE_SAVEALL, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, IDM_FILE_SAVEALLAS, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, IDM_OPTIONS_SINGLEOPEN_PROGRAM, MF_BYCOMMAND);
      DeleteMenu(hMainMenu, MENU_MDI_POSITION, MF_BYPOSITION);
      DrawMenuBar(hWnd);
      DragAcceptFiles(hWnd, TRUE);

      hDummyWindow=CreateWindowExWide(0,
                                      APP_SDI_CLASSW,        // window class name
                                      APP_SDI_TITLEW,        // window caption
                                      WS_POPUP,              // window style
                                      CW_USEDEFAULT,         // initial x position
                                      CW_USEDEFAULT,         // initial y position
                                      CW_USEDEFAULT,         // initial x size
                                      CW_USEDEFAULT,         // initial y size
                                      hWnd,                  // parent window handle
                                      NULL,                  // window menu handle
                                      hInstance,             // program instance handle
                                      NULL);                 // creation parameters
    }
    else
    {
      GetClientRect(hWnd, &rcRect);

      //MDI Client
      if (nMDI == WMD_MDI)
      {
        ccs.hWindowMenu=GetSubMenu(hMainMenu, MENU_MDI_POSITION);
        ccs.idFirstChild=ID_FIRSTMDI;

        hMdiClient=CreateWindowExWide(0,
                                      L"MdiClient",
                                      NULL,
                                      WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VSCROLL|WS_HSCROLL,
                                      rcRect.left, rcRect.top, rcRect.right, rcRect.bottom,
                                      hWnd,
                                      NULL,
                                      hInstance,
                                      (LPVOID)&ccs);

        dwClassStyle=GetClassLongA(hMdiClient, GCL_STYLE);
        SetClassLongA(hMdiClient, GCL_STYLE, dwClassStyle|CS_DBLCLKS);

        OldMdiClientProc=(WNDPROC)GetWindowLongWide(hMdiClient, GWL_WNDPROC);
        SetWindowLongWide(hMdiClient, GWL_WNDPROC, (LONG)NewMdiClientProc);

        DeleteMenu(hMainMenu, IDM_WINDOW_MDILIST, MF_BYCOMMAND);
        DragAcceptFiles(hMdiClient, TRUE);
      }

      //PMDI
      if (nMDI == WMD_PMDI)
      {
        DeleteMenu(hMainMenu, IDM_WINDOW_TILEHORIZONTAL, MF_BYCOMMAND);
        DeleteMenu(hMainMenu, IDM_WINDOW_TILEVERTICAL, MF_BYCOMMAND);
        DeleteMenu(hMainMenu, IDM_WINDOW_CASCADE, MF_BYCOMMAND);
        DragAcceptFiles(hWnd, TRUE);
      }

      //Tab Control
      hTab=CreateWindowExWide(0,
                              L"SysTabControl32",
                              NULL,
                              WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|TCS_SINGLELINE|TCS_FOCUSNEVER|TCS_BUTTONS|TCS_FLATBUTTONS,
                              rcRect.left, rcRect.top, rcRect.right, TAB_HEIGHT,
                              hWnd,
                              (HMENU)ID_TAB,
                              hInstance,
                              0);

      OldTabProc=(WNDPROC)GetWindowLongWide(hTab, GWL_WNDPROC);
      SetWindowLongWide(hTab, GWL_WNDPROC, (LONG)NewTabProc);

      SendMessage(hTab, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
      hImageList=ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_COLOR32|ILC_MASK, 0, 0);
      ImageList_SetBkColor(hImageList, GetSysColor(COLOR_BTNFACE));
      SendMessage(hTab, TCM_SETIMAGELIST, 0, (LPARAM)hImageList);
      SendMessage(hTab, TCM_SETEXTENDEDSTYLE, 0, TCS_EX_FLATSEPARATORS|TCS_EX_REGISTERDROP);
    }

    //Status Bar
    hStatus=CreateWindowExWide(0,
                               L"msctls_statusbar32",
                               NULL,
                               WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SBARS_SIZEGRIP,
                               0, 0, 0, 0,
                               hWnd,
                               (HMENU)ID_STATUS,
                               hInstance,
                               NULL);

    SendMessage(hStatus, SB_SIMPLE, FALSE, 0);
    SendMessage(hStatus, SB_SETPARTS, STATUS_PARTS, (LPARAM)&iSBParts);
    GetWindowRect(hStatus, &rcRect);
    nStatusHeight=rcRect.bottom - rcRect.top;

    //Progress Bar
    SendMessage(hStatus, SB_GETBORDERS, 0, (LPARAM)&iBorders);
    nProgressWidth=(iSBParts[1] - iSBParts[0]) - iBorders[2];

    hProgress=CreateWindowExWide(0,
                                 L"msctls_progress32",
                                 NULL,
                                 WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|PBS_SMOOTH,
                                 iSBParts[0] + iBorders[2], iBorders[1], nProgressWidth, nStatusHeight - iBorders[1],
                                 hStatus,
                                 (HMENU)ID_PROGRESS,
                                 hInstance,
                                 NULL);

    //Initialize status
    ssStatus.bModified=-1;
    ssStatus.bOvertypeMode=-1;
    ssStatus.nNewLine=-1;
    ssStatus.nCodePage=-1;
    ssStatus.bBOM=-1;

    PostMessage(hWnd, AKDN_MAIN_ONSTART, 0, 0);
  }

  if (uMsg >= WM_USER)
  {
    if (uMsg == AKDN_MAIN_ONSTART)
    {
      NMAINSHOW nms;
      RECT rcRect;
      wchar_t *wpFileName;
      int i=0;

      //Allocate and read search string
      if (nSearchStrings)
      {
        RegReadSearch();
      }

      //Allocate and read recent files
      if (nRecentFiles)
      {
        if (RecentFilesAlloc())
        {
          RecentFilesZero();
          RecentFilesRead();
        }
      }
      bMenuRecentFiles=TRUE;

      //Call plugins on start
      CallPluginsOnStart(&hPluginsStack);

      //AkelPad support 3 window modes:
      //SDI:
      //  - Maximum edit windows in SDI mode is 4. At next step we will create first one, later when window will be split
      //  we will create another 3 windows. Those 1 or 4 windows will be destroyed only on program exit.
      //  - SDI mode has one allocated FRAMEDATA.
      //  - fdInit structure keeps default settings and also all created edit windows.
      //MDI:
      //  - Maximum edit windows in MDI mode is limited by GDI buffer. During tests this number is about 800-1000 windows.
      //  - MDI mode allocated FRAMEDATA count is equal to number of tabs (MDI client frame windows).
      //  - Each tab is 1 MDI client frame window and 1 edit window (or 4 if window split).
      //  - fdInit structure keeps default settings, but not edit windows.
      //PMDI:
      //  - Allows you to open an unlimited number of files. In contrast to the MDI mode, opening a new tab does not lead
      //  creation of new graphical objects, thus avoid GDI buffer overflow.
      //  - Maximum edit windows in PMDI mode is 4. At next step we will create first one, later when window will be split
      //  we will create another 3 windows. Those 1 or 4 windows will be destroyed only on program exit.
      //  - PMDI mode allocated FRAMEDATA count is equal to number of tabs.
      //  - fdInit structure keeps default settings and also all created real edit windows.

      if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
      {
        //Create edit window
        fdInit.hWndEditParent=hMainWnd;
        CreateEditWindow(&fdInit, NULL, &fdInit.ei.hWndEdit, &fdInit.hDataEdit);

        if (nMDI == WMD_SDI)
        {
          if (lpFrameCurrent=CreateFrameData(hMainWnd, lpFrameCurrent))
            lpFrameCurrent->ei.hWndEdit=fdInit.ei.hWndEdit;
          SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrameCurrent->ei.hWndEdit, (LPARAM)lpFrameCurrent->hDataEdit);
          RestoreFrameData(lpFrameCurrent);
        }
        else if (nMDI == WMD_PMDI)
        {
          CreateMdiFrameWindow(NULL);
        }
      }
      else if (nMDI == WMD_MDI)
      {
        //Create MDI client frame window. In WM_CREATE of the frame procedure edit window will be created.
        if (dwMainStyle == WS_MAXIMIZE)
        {
          rcRect.right=GetSystemMetrics(SM_CXMAXIMIZED);
          rcRect.bottom=GetSystemMetrics(SM_CYMAXIMIZED);
        }
        else GetClientRect(hMdiClient, &rcRect);

        i=MulDiv(rcRect.bottom, 30, 100);
        rcRect.left=0;
        rcRect.top=0;
        rcRect.right-=i;
        rcRect.bottom-=i;
        CreateMdiFrameWindow(&rcRect);
      }

      //Apply settings
      SetFocus(lpFrameCurrent->ei.hWndEdit);

      DoViewOnTop(bOnTop, TRUE);
      DoViewShowStatusBar(bStatusBar, TRUE);
      DoSettingsSaveTime(bSaveTime);
      DoSettingsKeepSpace(bKeepSpace);
      DoSettingsWatchFile(bWatchFile);
      DoSettingsSingleOpenFile(bSingleOpenFile);
      if (!SearchPathWide(NULL, L"charmap.exe", NULL, BUFFER_SIZE, wbuf, &wpFileName))
        EnableMenuItem(hMainMenu, IDM_EDIT_INSERTCHAR, MF_GRAYED);

      if (nRegMDI == WMD_SDI)
        i=IDM_OPTIONS_SDI;
      else if (nRegMDI == WMD_MDI)
        i=IDM_OPTIONS_MDI;
      else if (nRegMDI == WMD_PMDI)
        i=IDM_OPTIONS_PMDI;
      CheckMenuRadioItem(hMainMenu, IDM_OPTIONS_SDI, IDM_OPTIONS_PMDI, i, MF_BYCOMMAND);

      if (nMDI)
      {
        DoSettingsSingleOpenProgram(bSingleOpenProgram);
        DoWindowTabView(dwTabOptionsMDI, TRUE);
        DoWindowTabType(dwTabOptionsMDI, TRUE);
        if (bOldComctl32) EnableMenuItem(hMainMenu, IDM_WINDOW_TABTYPE_FLATBUTTONS, MF_GRAYED);
        CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABSWITCH_NEXTPREV, IDM_WINDOW_TABSWITCH_RIGHTLEFT, (dwTabOptionsMDI & TAB_SWITCH_NEXTPREV)?IDM_WINDOW_TABSWITCH_NEXTPREV:IDM_WINDOW_TABSWITCH_RIGHTLEFT, MF_BYCOMMAND);
      }

      //PreShow
      nms.dwStyle=&dwMainStyle;
      nms.dwShow=&dwCmdShow;
      nms.bProcess=TRUE;
      SendMessage(hMainWnd, AKDN_MAIN_ONSTART_PRESHOW, 0, (LPARAM)&nms);
      if (nms.bProcess)
      {
        //Show main window
        ShowWindow(hMainWnd, (dwMainStyle == WS_MAXIMIZE)?SW_SHOWMAXIMIZED:SW_SHOW);

        //Shortcut
        if (dwCmdShow != SW_SHOWNORMAL)
          ShowWindow(hMainWnd, dwCmdShow);

        //Update main window
        UpdateWindow(hMainWnd);
      }
      SendMessage(hMainWnd, AKDN_MAIN_ONSTART_SHOW, 0, 0);

      if (*wszCmdFile) OpenDocument(lpFrameCurrent->ei.hWndEdit, wszCmdFile, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);
      SetCurrentDirectoryWide(wszExeDir);

      if (hMutex)
      {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
        hMutex=0;
      }

      SendMessage(hMainWnd, AKDN_MAIN_ONSTART_FINISH, 0, 0);
      bMainOnStartFinish=TRUE;
      return 0;
    }

    //Text retrieval and modification
    if (uMsg == AKD_DETECTCODEPAGE ||
        uMsg == AKD_DETECTCODEPAGEA ||
        uMsg == AKD_DETECTCODEPAGEW)
    {
      DETECTCODEPAGEW *dc=(DETECTCODEPAGEW *)lParam;
      wchar_t *wpFile=AllocWideStr(MAX_PATH);
      int nResult;

      if (uMsg == AKD_DETECTCODEPAGEA || (bOldWindows && uMsg == AKD_DETECTCODEPAGE))
        xprintfW(wpFile, L"%S", (char *)dc->pFile);
      else
        xprintfW(wpFile, L"%s", (wchar_t *)dc->pFile);
      nResult=AutodetectCodePage(wpFile, dc->dwBytesToCheck, dc->dwFlags, &dc->nCodePage, &dc->bBOM);

      FreeWideStr(wpFile);
      return nResult;
    }
    if (uMsg == AKD_READFILECONTENT)
    {
      FILECONTENT *fc=(FILECONTENT *)lParam;

      return ReadFileContent(fc->hFile, fc->dwBytesMax, fc->nCodePage, fc->bBOM, &fc->wpContent);
    }
    if (uMsg == AKD_OPENDOCUMENT ||
        uMsg == AKD_OPENDOCUMENTA ||
        uMsg == AKD_OPENDOCUMENTW)
    {
      OPENDOCUMENTW *od=(OPENDOCUMENTW *)lParam;
      wchar_t *wpFile=AllocWideStr(MAX_PATH);
      wchar_t *wpWorkDir=AllocWideStr(MAX_PATH);
      int nResult=0;

      if (((HWND)wParam && !IsEditActive((HWND)wParam)) || nMDI || SaveChanged())
      {
        if (uMsg == AKD_OPENDOCUMENTA || (bOldWindows && uMsg == AKD_OPENDOCUMENT))
        {
          xprintfW(wpFile, L"%S", (char *)od->pFile);
          xprintfW(wpWorkDir, L"%S", (char *)od->pWorkDir);
        }
        else
        {
          xprintfW(wpFile, L"%s", (wchar_t *)od->pFile);
          xprintfW(wpWorkDir, L"%s", (wchar_t *)od->pWorkDir);
        }

        if (*wpWorkDir) SetCurrentDirectoryWide(wpWorkDir);
        nResult=OpenDocument((HWND)wParam, wpFile, od->dwFlags, od->nCodePage, od->bBOM);
        if (*wpWorkDir) SetCurrentDirectoryWide(wszExeDir);
      }
      FreeWideStr(wpFile);
      FreeWideStr(wpWorkDir);
      return nResult;
    }
    if (uMsg == AKD_SAVEDOCUMENT ||
        uMsg == AKD_SAVEDOCUMENTA ||
        uMsg == AKD_SAVEDOCUMENTW)
    {
      SAVEDOCUMENTW *sd=(SAVEDOCUMENTW *)lParam;
      wchar_t *wpFile=AllocWideStr(MAX_PATH);
      int nResult;

      if (uMsg == AKD_SAVEDOCUMENTA || (bOldWindows && uMsg == AKD_SAVEDOCUMENT))
        xprintfW(wpFile, L"%S", (char *)sd->pFile);
      else
        xprintfW(wpFile, L"%s", (wchar_t *)sd->pFile);
      nResult=SaveDocument((HWND)wParam, wpFile, sd->nCodePage, sd->bBOM, sd->dwFlags);

      FreeWideStr(wpFile);
      return nResult;
    }
    if (uMsg == AKD_GETTEXTLENGTH)
    {
      return GetTextLength((HWND)wParam);
    }
    if (uMsg == AKD_GETTEXTRANGE ||
        uMsg == AKD_GETTEXTRANGEA ||
        uMsg == AKD_GETTEXTRANGEW)
    {
      GETTEXTRANGE *gtr=(GETTEXTRANGE *)lParam;

      if (uMsg == AKD_GETTEXTRANGEA || (bOldWindows && uMsg == AKD_GETTEXTRANGE))
        return GetRangeTextA((HWND)wParam, gtr->cpMin, gtr->cpMax, (char **)&gtr->pText);
      else
        return GetRangeTextW((HWND)wParam, gtr->cpMin, gtr->cpMax, (wchar_t **)&gtr->pText);
    }
    if (uMsg == AKD_GETSELTEXTW)
    {
      int *nResultLen=(int *)lParam;
      AECHARRANGE cr;
      wchar_t *wpText=NULL;
      int nTextLen;
      BOOL bColumnSel=FALSE;

      GetSel((HWND)wParam, &cr, &bColumnSel, NULL);
      nTextLen=ExGetRangeTextW((HWND)wParam, &cr.ciMin, &cr.ciMax, bColumnSel, &wpText, AELB_R, TRUE);
      if (nResultLen) *nResultLen=nTextLen;
      return (LRESULT)wpText;
    }
    if (uMsg == AKD_FREETEXT)
    {
      return FreeText((LPVOID)lParam);
    }
    if (uMsg == AKD_REPLACESEL ||
        uMsg == AKD_REPLACESELA ||
        uMsg == AKD_REPLACESELW)
    {
      if (uMsg == AKD_REPLACESELA || (bOldWindows && uMsg == AKD_REPLACESEL))
        ReplaceSelA((HWND)wParam, (char *)lParam, -1, -1, NULL, NULL);
      else
        ReplaceSelW((HWND)wParam, (wchar_t *)lParam, -1, -1, NULL, NULL);
      return 0;
    }
    if (uMsg == AKD_PASTE)
    {
      if (lParam == PASTE_SINGLELINE)
        return PasteInEditAsRichEdit((HWND)wParam);
      else
        return DoEditPaste((HWND)wParam, lParam);
    }
    if (uMsg == AKD_COPY)
    {
      DoEditCopy((HWND)wParam);
      return 0;
    }
    if (uMsg == AKD_TEXTFIND ||
        uMsg == AKD_TEXTFINDA ||
        uMsg == AKD_TEXTFINDW)
    {
      TEXTFINDW *tf=(TEXTFINDW *)lParam;

      if (uMsg == AKD_TEXTFINDA || (bOldWindows && uMsg == AKD_TEXTFIND))
      {
        wchar_t *wpFindIt;
        int nFindItLen;
        int nResult=-1;

        nFindItLen=MultiByteToWideChar(CP_ACP, 0, (char *)tf->pFindIt, tf->nFindItLen, NULL, 0);
        if (wpFindIt=(wchar_t *)GlobalAlloc(GPTR, nFindItLen * sizeof(wchar_t)))
        {
          MultiByteToWideChar(CP_ACP, 0, (char *)tf->pFindIt, tf->nFindItLen, wpFindIt, nFindItLen);

          nResult=FindTextW((HWND)wParam, tf->dwFlags, wpFindIt, tf->nFindItLen);
          GlobalFree((HGLOBAL)wpFindIt);
        }
        return nResult;
      }
      return FindTextW((HWND)wParam, tf->dwFlags, tf->pFindIt, tf->nFindItLen);
    }
    if (uMsg == AKD_TEXTREPLACE ||
        uMsg == AKD_TEXTREPLACEA ||
        uMsg == AKD_TEXTREPLACEW)
    {
      TEXTREPLACEW *tr=(TEXTREPLACEW *)lParam;

      if (uMsg == AKD_TEXTREPLACEA || (bOldWindows && uMsg == AKD_TEXTREPLACE))
      {
        wchar_t *wpFindIt;
        wchar_t *wpReplaceWith;
        int nFindItLen;
        int nReplaceWithLen;
        int nResult=-1;

        nFindItLen=MultiByteToWideChar(CP_ACP, 0, (char *)tr->pFindIt, tr->nFindItLen, NULL, 0);
        if (wpFindIt=(wchar_t *)GlobalAlloc(GPTR, nFindItLen * sizeof(wchar_t)))
        {
          MultiByteToWideChar(CP_ACP, 0, (char *)tr->pFindIt, tr->nFindItLen, wpFindIt, nFindItLen);

          nReplaceWithLen=MultiByteToWideChar(CP_ACP, 0, (char *)tr->pReplaceWith, tr->nReplaceWithLen, NULL, 0);
          if (wpReplaceWith=(wchar_t *)GlobalAlloc(GPTR, nReplaceWithLen * sizeof(wchar_t)))
          {
            MultiByteToWideChar(CP_ACP, 0, (char *)tr->pReplaceWith, tr->nReplaceWithLen, wpReplaceWith, nReplaceWithLen);

            nResult=ReplaceTextW((HWND)wParam, tr->dwFlags, wpFindIt, tr->nFindItLen, wpReplaceWith, tr->nReplaceWithLen, tr->bAll, &tr->nChanges);
            GlobalFree((HGLOBAL)wpReplaceWith);
          }
          GlobalFree((HGLOBAL)wpFindIt);
        }
        return nResult;
      }
      return ReplaceTextW((HWND)wParam, tr->dwFlags, tr->pFindIt, tr->nFindItLen, tr->pReplaceWith, tr->nReplaceWithLen, tr->bAll, &tr->nChanges);
    }
    if (uMsg == AKD_RECODESEL)
    {
      TEXTRECODE *tr=(TEXTRECODE *)lParam;

      RecodeTextW((HWND)wParam, tr->nCodePageFrom, tr->nCodePageTo);
      return 0;
    }
    if (uMsg == AKD_GETCHARCOLOR)
    {
      CHARCOLOR *cc=(CHARCOLOR *)lParam;

      return GetCharColor((HWND)wParam, cc);
    }

    //Print
    if (uMsg == AKD_GETFILEPRINT)
    {
      return bGlobalPrint;
    }
    if (uMsg == AKD_SETFILEPRINT)
    {
      bGlobalPrint=(BOOL)wParam;
      return 0;
    }
    if (uMsg == AKD_GETPRINTINFO)
    {
      PRINTINFO *info=(PRINTINFO *)wParam;

      xmemcpy(info, &prninfo, sizeof(PRINTINFO));
      return 0;
    }
    if (uMsg == AKD_SETPRINTINFO)
    {
      PRINTINFO *info=(PRINTINFO *)wParam;

      xmemcpy(&prninfo, info, sizeof(PRINTINFO));
      return 0;
    }

    //Options
    if (uMsg == AKD_PROGRAMVERSION)
    {
      return dwExeVersion;
    }
    if (uMsg == AKD_PROGRAMARCHITECTURE)
    {
      return AKELDLL;
    }
    if (uMsg == AKD_GETMAININFO)
    {
      PLUGINDATA *pd=(PLUGINDATA *)lParam;

      pd->cb=sizeof(PLUGINDATA);
      pd->pFunction=NULL;
      pd->szFunction=NULL;
      pd->wszFunction=NULL;
      pd->hInstanceDLL=NULL;
      pd->lpPluginFunction=NULL;
      pd->lpbAutoLoad=NULL;
      pd->nUnload=0;
      pd->bInMemory=0;
      pd->lParam=0;
      pd->pAkelDir=bOldWindows?(LPBYTE)szExeDir:(LPBYTE)wszExeDir;
      pd->szAkelDir=szExeDir;
      pd->wszAkelDir=wszExeDir;
      pd->hInstanceEXE=hInstance;
      pd->hPluginsStack=&hPluginsStack;
      pd->hMainWnd=hMainWnd;
      pd->hWndEdit=lpFrameCurrent->ei.hWndEdit;
      pd->hStatus=hStatus;
      pd->hMdiClient=hMdiClient;
      pd->hTab=hTab;
      pd->hMainMenu=hMainMenu;
      pd->hMenuRecentFiles=hMenuRecentFiles;
      pd->hMenuLanguage=hMenuLanguage;
      pd->hPopupMenu=hPopupMenu;
      pd->hMainIcon=hMainIcon;
      pd->hGlobalAccel=hGlobalAccel;
      pd->hMainAccel=hMainAccel;
      pd->bOldWindows=bOldWindows;
      pd->bOldRichEdit=bOldRichEdit;
      pd->bOldComctl32=bOldComctl32;
      pd->bAkelEdit=bAkelEdit;
      pd->nMDI=nMDI;
      pd->nSaveSettings=nSaveSettings;
      pd->pLangModule=bOldWindows?(LPBYTE)szLangModule:(LPBYTE)wszLangModule;
      pd->szLangModule=szLangModule;
      pd->wszLangModule=wszLangModule;
      pd->wLangSystem=(WORD)dwLangSystem;

      return 0;
    }
    if (uMsg == AKD_GETEDITINFO)
    {
      return GetEditInfo((HWND)wParam, (EDITINFO *)lParam);
    }
    if (uMsg == AKD_SETMODIFY)
    {
      SetModifyStatus((HWND)wParam, lParam);
      return 0;
    }
    if (uMsg == AKD_SETNEWLINE)
    {
      SetNewLineStatus((HWND)wParam, lParam, AENL_INPUT|AENL_OUTPUT);
      return 0;
    }
    if (uMsg == AKD_GETFONT ||
        uMsg == AKD_GETFONTA ||
        uMsg == AKD_GETFONTW)
    {
      FRAMEDATA *lpFrame;

      if (lParam)
      {
        if (lpFrame=GetFrameDataFromEdit((HWND)wParam))
        {
          if (uMsg == AKD_GETFONTA || (bOldWindows && uMsg == AKD_GETFONT))
            LogFontWtoA(&lpFrame->lf, (LOGFONTA *)lParam);
          else
            xmemcpy((LOGFONTW *)lParam, &lpFrame->lf, sizeof(LOGFONTW));
          return (LRESULT)lParam;
        }
      }
      return (LRESULT)NULL;
    }
    if (uMsg == AKD_SETFONT ||
        uMsg == AKD_SETFONTA ||
        uMsg == AKD_SETFONTW)
    {
      FRAMEDATA *lpFrame;
      LOGFONTW lfW;

      if (uMsg == AKD_GETFONTA || (bOldWindows && uMsg == AKD_GETFONT))
        LogFontAtoW((LOGFONTA *)lParam, &lfW);
      else
        xmemcpy(&lfW, (LOGFONTW *)lParam, sizeof(LOGFONTW));

      if (SetChosenFont((HWND)wParam, &lfW))
      {
        if (lpFrame=GetFrameDataFromEdit((HWND)wParam))
          xmemcpy(&lpFrame->lf, &lfW, sizeof(LOGFONTW));
        bEditFontChanged=TRUE;
        return TRUE;
      }
      return FALSE;
    }
    if (uMsg == AKD_GETMSGCREATE)
    {
      return nMsgCreate;
    }
    if (uMsg == AKD_SETMSGCREATE)
    {
      nMsgCreate=(int)wParam;
      return 0;
    }
    if (uMsg == AKD_GETMSGBINARY)
    {
      return nMsgBinary;
    }
    if (uMsg == AKD_SETMSGBINARY)
    {
      nMsgBinary=(int)wParam;
      return 0;
    }
    if (uMsg == AKD_GETCODEPAGELIST)
    {
      int *nCodePage=(int *)wParam;

      if (nCodePage)
      {
        *nCodePage=nDefaultCodePage;
      }
      return (LRESULT)lpCodepageList;
    }
    if (uMsg == AKD_RECENTFILES)
    {
      if (wParam == RF_GET)
      {
        RECENTFILES *rf=(RECENTFILES *)lParam;

        if (rf)
        {
          rf->lpszRecentNames=lpwszRecentNames;
          rf->lpdwRecentPositions=lpdwRecentPositions;
          rf->lpdwRecentCodepages=lpdwRecentCodepages;
        }
      }
      else if (wParam == RF_CLEAR)
      {
        RecentFilesZero();
        RecentFilesSave();
        bMenuRecentFiles=TRUE;
      }
      return nRecentFiles;
    }
    if (uMsg == AKD_SEARCHHISTORY)
    {
      if (wParam == SH_GET)
      {
      }
      else if (wParam == SH_CLEAR)
      {
        wchar_t wszRegKey[MAX_PATH];

        xprintfW(wszRegKey, L"%s\\Search", APP_REGHOMEW);
        RegClearKeyWide(HKEY_CURRENT_USER, wszRegKey);
      }
      return nSearchStrings;
    }

    //Windows
    if (uMsg == AKD_GETMODELESS)
    {
      BOOL *bType=(BOOL *)lParam;

      if (bType) *bType=nModelessType;
      return (LRESULT)hDlgModeless;
    }
    if (uMsg == AKD_SETMODELESS)
    {
      hDlgModeless=(HWND)wParam;
      if (hDlgModeless)
        nModelessType=MLT_CUSTOM;
      else
        nModelessType=MLT_NONE;
      return 0;
    }
    if (uMsg == AKD_RESIZE)
    {
      UpdateSize();
      return 0;
    }
    if (uMsg == AKD_DOCK)
    {
      DOCK *lpDock=(DOCK *)lParam;
      DOCK *lpResult=NULL;

      if (wParam & DK_ADD)
      {
        lpResult=StackDockAdd(&hDocksStack, lpDock);
        lpDock=lpResult;
      }
      if (wParam & DK_SUBCLASS)
      {
        if (lpDock->hWnd)
        {
          OldDockProc=(WNDPROC)GetWindowLongWide(lpDock->hWnd, GWL_WNDPROC);
          SetWindowLongWide(lpDock->hWnd, GWL_WNDPROC, (LONG)DockProc);
        }
      }
      if (wParam & DK_UNSUBCLASS)
      {
        if (lpDock->hWnd)
        {
          SetWindowLongWide(lpDock->hWnd, GWL_WNDPROC, (LONG)OldDockProc);
        }
      }
      if ((wParam & DK_SETLEFT) ||
          (wParam & DK_SETRIGHT) ||
          (wParam & DK_SETTOP) ||
          (wParam & DK_SETBOTTOM))
      {
        int nSide=0;

        if (wParam & DK_SETLEFT) nSide=DKS_LEFT;
        else if (wParam & DK_SETRIGHT) nSide=DKS_RIGHT;
        else if (wParam & DK_SETTOP) nSide=DKS_TOP;
        else if (wParam & DK_SETBOTTOM) nSide=DKS_BOTTOM;

        if (nSide) DockSetSide(&hDocksStack, lpDock, nSide);
      }
      if (wParam & DK_HIDE)
      {
        if (lpDock->hWnd)
        {
          lpDock->dwFlags|=DKF_HIDDEN;
          ShowWindow(lpDock->hWnd, SW_HIDE);
          UpdateSize();
        }
      }
      if (wParam & DK_SHOW)
      {
        if (lpDock->hWnd)
        {
          lpDock->dwFlags&=~DKF_HIDDEN;
          ShowWindow(lpDock->hWnd, SW_SHOW);
          UpdateSize();
        }
      }
      if (wParam & DK_DELETE)
      {
        StackDockDelete(&hDocksStack, lpDock);
      }
      return (LRESULT)lpResult;
    }
    if (uMsg == AKD_SETCLOSEBUTTON)
    {
      HBITMAP hBitmap=(HBITMAP)lParam;

      if (!hBitmap) hBitmap=hBitmapClose;
      SendMessage((HWND)wParam, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);

      OldCloseButtonProc=(WNDPROC)GetWindowLongWide((HWND)wParam, GWL_WNDPROC);
      SetWindowLongWide((HWND)wParam, GWL_WNDPROC, (LONG)NewCloseButtonProc);
      return 0;
    }
    if (uMsg == AKD_SETHOTKEYINPUT)
    {
      SetWindowLongWide((HWND)wParam, GWL_USERDATA, lParam);
      OldHotkeyInputProc=(WNDPROC)GetWindowLongWide((HWND)wParam, GWL_WNDPROC);
      SetWindowLongWide((HWND)wParam, GWL_WNDPROC, (LONG)NewHotkeyInputProc);
      return 0;
    }
    if (uMsg == AKD_DIALOGRESIZE)
    {
      DIALOGRESIZEMSG *drsm=(DIALOGRESIZEMSG *)lParam;

      return DialogResizeMessages(drsm->drs, drsm->rcInit, drsm->rcCurrent, drsm->dwFlags, drsm->hDlg, drsm->uMsg, drsm->wParam, drsm->lParam);
    }

    //Frames
    if (uMsg == AKD_FRAMEFIND ||
        uMsg == AKD_FRAMEFINDA ||
        uMsg == AKD_FRAMEFINDW)
    {
      if (wParam == FWF_CURRENT)
        return (LRESULT)lpFrameCurrent;
      if (wParam == FWF_NEXT)
      {
        FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

        return (LRESULT)(lpFrame?lpFrame->next:NULL);
      }
      if (wParam == FWF_PREV)
      {
        FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

        return (LRESULT)(lpFrame?lpFrame->prev:NULL);
      }
      if (wParam == FWF_BYINDEX)
        return (LRESULT)StackFrameGetByIndex(&hFramesStack, lParam);
      if (wParam == FWF_BYFILENAME)
      {
        wchar_t *wpFileName=AllocWideStr(MAX_PATH);
        FRAMEDATA *lpResult;
        int nFileNameLen;

        if (uMsg == AKD_FRAMEFINDA || (bOldWindows && uMsg == AKD_FRAMEFIND))
          nFileNameLen=xprintfW(wpFileName, L"%S", (char *)lParam);
        else
          nFileNameLen=xprintfW(wpFileName, L"%s", (wchar_t *)lParam);
        lpResult=StackFrameGetByName(&hFramesStack, wpFileName, nFileNameLen);

        FreeWideStr(wpFileName);
        return (LRESULT)lpResult;
      }
      if (wParam == FWF_BYEDITWINDOW)
        return (LRESULT)GetFrameDataFromEdit((HWND)lParam);
      if (wParam == FWF_BYEDITDATA)
        return (LRESULT)StackFrameGetByHandle(&hFramesStack, (HANDLE)lParam);
      return 0;
    }
    if (uMsg == AKD_FRAMEACTIVATE)
    {
      FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

      ActivateMdiFrameWindow(lpFrame, TRUE);
      return 0;
    }
    if (uMsg == AKD_FRAMENEXT)
    {
      FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

      return (LRESULT)NextMdiFrameWindow(lpFrame, wParam);
    }
    if (uMsg == AKD_FRAMEDESTROY)
    {
      FRAMEDATA *lpFrame=(FRAMEDATA *)lParam;

      return DestroyMdiFrameWindow(lpFrame, -1);
    }

    //Thread
    if (uMsg == AKD_GLOBALALLOC)
    {
      return (LRESULT)GlobalAlloc(wParam, lParam);
    }
    if (uMsg == AKD_GLOBALLOCK)
    {
      return (LRESULT)GlobalLock((HGLOBAL)wParam);
    }
    if (uMsg == AKD_GLOBALUNLOCK)
    {
      return (LRESULT)GlobalUnlock((HGLOBAL)wParam);
    }
    if (uMsg == AKD_GLOBALFREE)
    {
      return (LRESULT)GlobalFree((HGLOBAL)wParam);
    }
    if (uMsg == AKD_STRLENA)
    {
      return lstrlenA((char *)wParam);
    }
    if (uMsg == AKD_STRLENW)
    {
      return lstrlenW((wchar_t *)wParam);
    }
    if (uMsg == AKD_CREATEWINDOW)
    {
      CREATEWINDOWW *cw=(CREATEWINDOWW *)lParam;

      return (LRESULT)CreateWindowExWide(0, cw->szClassName, cw->szWindowName, cw->dwStyle, cw->x, cw->y, cw->nWidth, cw->nHeight, cw->hWndParent, cw->hMenu, cw->hInstance, cw->lpParam);
    }
    if (uMsg == AKD_WAITKEYBOARD)
    {
      BYTE lpKeyState[256];
      MSG msg;
      int i;

      Loop:
      Sleep(0);

      if (GetKeyboardState(lpKeyState))
      {
        for (i=0; i < 256; ++i)
        {
          if (lpKeyState[i] & 0x80)
          {
            if (wParam) return 1;

            while (PeekMessageWide(&msg, NULL, 0, 0, PM_REMOVE))
            {
              TranslateMessage(&msg);
              DispatchMessageWide(&msg);
            }
            goto Loop;
          }
        }
      }
      return 0;
    }
    if (uMsg == AKD_GETQUEUE)
    {
      return GetQueueStatus(wParam);
    }

    //Plugin options
    if (uMsg == AKD_BEGINOPTIONS ||
        uMsg == AKD_BEGINOPTIONSA ||
        uMsg == AKD_BEGINOPTIONSW)
    {
      if (nSaveSettings == SS_REGISTRY)
      {
        REGHANDLE *rh;

        if (rh=(REGHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(REGHANDLE)))
        {
          rh->dwType=wParam;

          if (lParam)
          {
            if (uMsg == AKD_BEGINOPTIONSA || (bOldWindows && uMsg == AKD_BEGINOPTIONS))
              xprintfW(rh->wszKey, L"%s\\Plugs\\%S", APP_REGHOMEW, (char *)lParam);
            else
              xprintfW(rh->wszKey, L"%s\\Plugs\\%s", APP_REGHOMEW, (wchar_t *)lParam);
          }
          else xprintfW(rh->wszKey, L"%s\\Options", APP_REGHOMEW);

          if (rh->dwType & POB_READ)
          {
            if (RegOpenKeyExWide(HKEY_CURRENT_USER, rh->wszKey, 0, KEY_READ, &rh->hKey) != ERROR_SUCCESS)
            {
              API_HeapFree(hHeap, 0, (LPVOID)rh);
              rh=NULL;
            }
          }
          else if (rh->dwType & POB_SAVE)
          {
            if (rh->dwType & POB_CLEAR)
            {
              RegClearKeyWide(HKEY_CURRENT_USER, rh->wszKey);
            }
            if (RegCreateKeyExWide(HKEY_CURRENT_USER, rh->wszKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rh->hKey, NULL) != ERROR_SUCCESS)
            {
              API_HeapFree(hHeap, 0, (LPVOID)rh);
              rh=NULL;
            }
          }
        }
        return (LRESULT)rh;
      }
      else
      {
        INIHANDLE *ih;

        if (ih=(INIHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(INIHANDLE)))
        {
          ih->dwType=wParam;

          if (lParam)
          {
            if (uMsg == AKD_BEGINOPTIONSA || (bOldWindows && uMsg == AKD_BEGINOPTIONS))
              xprintfW(ih->wszIniFile, L"%s\\AkelFiles\\Plugs\\%S.ini", wszExeDir, (char *)lParam);
            else
              xprintfW(ih->wszIniFile, L"%s\\AkelFiles\\Plugs\\%s.ini", wszExeDir, (wchar_t *)lParam);
          }
          else xprintfW(ih->wszIniFile, L"%s\\AkelPad.ini", wszExeDir);

          if (ih->dwType & POB_READ)
          {
            if (!OpenIniW(&ih->hStack, ih->wszIniFile, FALSE))
            {
              API_HeapFree(hHeap, 0, (LPVOID)ih);
              ih=NULL;
            }
          }
          else if (ih->dwType & POB_SAVE)
          {
            if (!OpenIniW(&ih->hStack, ih->wszIniFile, TRUE))
            {
              API_HeapFree(hHeap, 0, (LPVOID)ih);
              ih=NULL;
            }
            else
            {
              if (ih->dwType & POB_CLEAR)
              {
                HINISECTION *lpIniSection;

                if (lpIniSection=StackOpenIniSectionW(&ih->hStack, L"Options", lstrlenW(L"Options"), FALSE))
                  StackDeleteIniSection(&ih->hStack, lpIniSection, TRUE);
              }
            }
          }
        }
        return (LRESULT)ih;
      }
    }
    if (uMsg == AKD_OPTION ||
        uMsg == AKD_OPTIONA ||
        uMsg == AKD_OPTIONW)
    {
      PLUGINOPTIONW *po=(PLUGINOPTIONW *)lParam;
      wchar_t *wpOptionName=AllocWideStr(MAX_PATH);
      DWORD dwType;
      DWORD dwResult=0;

      if (uMsg == AKD_OPTIONA || (bOldWindows && uMsg == AKD_OPTION))
        xprintfW(wpOptionName, L"%S", (char *)po->pOptionName);
      else
        xprintfW(wpOptionName, L"%s", (wchar_t *)po->pOptionName);

      if (nSaveSettings == SS_REGISTRY)
      {
        REGHANDLE *rh=(REGHANDLE *)wParam;
        DWORD dwSize;

        if (po->dwType == PO_DWORD)
          dwType=REG_DWORD;
        else if (po->dwType == PO_BINARY)
          dwType=REG_BINARY;
        else if (po->dwType == PO_STRING)
          dwType=REG_SZ;

        if (rh->dwType & POB_READ)
        {
          dwSize=po->dwData;
          if (RegQueryValueExWide(rh->hKey, wpOptionName, NULL, &dwType, (LPBYTE)po->lpData, &dwSize) == ERROR_SUCCESS)
            dwResult=dwSize;
        }
        else if (rh->dwType & POB_SAVE)
        {
          if (RegSetValueExWide(rh->hKey, wpOptionName, 0, dwType, (LPBYTE)po->lpData, po->dwData) == ERROR_SUCCESS)
            dwResult=TRUE;
        }
      }
      else
      {
        INIHANDLE *ih=(INIHANDLE *)wParam;

        if (po->dwType == PO_DWORD)
          dwType=INI_DWORD;
        else if (po->dwType == PO_BINARY)
          dwType=INI_BINARY;
        else if (po->dwType == PO_STRING)
          dwType=INI_STRINGUNICODE;

        if (ih->dwType & POB_READ)
          dwResult=IniGetValueW(&ih->hStack, L"Options", wpOptionName, dwType, (LPBYTE)po->lpData, po->dwData);
        else if (ih->dwType & POB_SAVE)
          dwResult=IniSetValueW(&ih->hStack, L"Options", wpOptionName, dwType, (LPBYTE)po->lpData, po->dwData);
      }
      FreeWideStr(wpOptionName);
      return dwResult;
    }
    if (uMsg == AKD_ENDOPTIONS)
    {
      BOOL bResult=FALSE;

      if (wParam)
      {
        if (nSaveSettings == SS_REGISTRY)
        {
          REGHANDLE *rh=(REGHANDLE *)wParam;

          if (RegCloseKey(rh->hKey) == ERROR_SUCCESS)
            bResult=TRUE;
          API_HeapFree(hHeap, 0, (LPVOID)rh);
        }
        else
        {
          INIHANDLE *ih=(INIHANDLE *)wParam;

          if (ih->dwType & POB_READ)
            bResult=TRUE;
          else if (ih->dwType & POB_SAVE)
            bResult=SaveIniW(&ih->hStack, ih->wszIniFile);
          StackFreeIni(&ih->hStack);
          API_HeapFree(hHeap, 0, (LPVOID)ih);
        }
      }
      return bResult;
    }
    if (uMsg == AKD_INIOPEN ||
        uMsg == AKD_INIOPENA ||
        uMsg == AKD_INIOPENW)
    {
      INIHANDLE *ih;
      BOOL bCreate=FALSE;

      if (ih=(INIHANDLE *)API_HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(INIHANDLE)))
      {
        ih->dwType=wParam;
        if (ih->dwType & POB_READ)
          bCreate=FALSE;
        else if (ih->dwType & POB_SAVE)
          bCreate=TRUE;

        if (uMsg == AKD_INIOPENA || (bOldWindows && uMsg == AKD_INIOPEN))
          xprintfW(ih->wszIniFile, L"%S", (char *)lParam);
        else
          xprintfW(ih->wszIniFile, L"%s", (wchar_t *)lParam);

        if (!OpenIniW(&ih->hStack, ih->wszIniFile, bCreate))
        {
          API_HeapFree(hHeap, 0, (LPVOID)ih);
          ih=NULL;
        }
      }
      return (LRESULT)ih;
    }
    if (uMsg == AKD_INIGETSECTION ||
        uMsg == AKD_INIGETSECTIONA ||
        uMsg == AKD_INIGETSECTIONW)
    {
      INIHANDLE *ih=(INIHANDLE *)wParam;
      wchar_t *wpSection=AllocWideStr(MAX_PATH);
      int nSectionLen;
      HINISECTION *lpResult;

      if (uMsg == AKD_INIGETSECTIONA || (bOldWindows && uMsg == AKD_INIGETSECTION))
        nSectionLen=xprintfW(wpSection, L"%S", (char *)lParam);
      else
        nSectionLen=xprintfW(wpSection, L"%s", (wchar_t *)lParam);
      lpResult=StackOpenIniSectionW(&ih->hStack, wpSection, nSectionLen, FALSE);

      FreeWideStr(wpSection);
      return (LRESULT)lpResult;
    }
    if (uMsg == AKD_INICLEARSECTION ||
        uMsg == AKD_INIDELETESECTION)
    {
      INIHANDLE *ih=(INIHANDLE *)wParam;
      HINISECTION *lpIniSection=(HINISECTION *)lParam;

      StackDeleteIniSection(&ih->hStack, lpIniSection, (uMsg == AKD_INICLEARSECTION)?TRUE:FALSE);
      return 0;
    }
    if (uMsg == AKD_INIGETKEY ||
        uMsg == AKD_INIGETKEYA ||
        uMsg == AKD_INIGETKEYW)
    {
      HINISECTION *lpIniSection=(HINISECTION *)wParam;
      wchar_t *wpKey=AllocWideStr(MAX_PATH);
      int nKeyLen;
      HINIKEY *lpResult;

      if (uMsg == AKD_INIGETKEYA || (bOldWindows && uMsg == AKD_INIGETKEY))
        nKeyLen=xprintfW(wpKey, L"%S", (char *)lParam);
      else
        nKeyLen=xprintfW(wpKey, L"%s", (wchar_t *)lParam);
      lpResult=StackOpenIniKeyW(lpIniSection, wpKey, nKeyLen, FALSE);

      FreeWideStr(wpKey);
      return (LRESULT)lpResult;
    }
    if (uMsg == AKD_INIDELETEKEY)
    {
      HINISECTION *lpIniSection=(HINISECTION *)wParam;
      HINIKEY *lpIniKey=(HINIKEY *)lParam;

      StackDeleteIniKey(lpIniSection, lpIniKey);
      return 0;
    }
    if (uMsg == AKD_INIGETVALUE ||
        uMsg == AKD_INIGETVALUEA ||
        uMsg == AKD_INIGETVALUEW)
    {
      INIHANDLE *ih=(INIHANDLE *)wParam;
      INIVALUEW *iv=(INIVALUEW *)lParam;
      wchar_t *wpSection=AllocWideStr(MAX_PATH);
      wchar_t *wpKey=AllocWideStr(MAX_PATH);
      int nResult;

      if (uMsg == AKD_INIGETVALUEA || (bOldWindows && uMsg == AKD_INIGETVALUE))
      {
        xprintfW(wpSection, L"%S", (char *)iv->pSection);
        xprintfW(wpKey, L"%S", (char *)iv->pKey);
      }
      else
      {
        xprintfW(wpSection, L"%s", (wchar_t *)iv->pSection);
        xprintfW(wpKey, L"%s", (wchar_t *)iv->pKey);
      }
      nResult=IniGetValueW(&ih->hStack, wpSection, wpKey, iv->dwType, (LPBYTE)iv->lpData, iv->dwData);

      FreeWideStr(wpSection);
      FreeWideStr(wpKey);
      return (LRESULT)nResult;
    }
    if (uMsg == AKD_INISETVALUE ||
        uMsg == AKD_INISETVALUEA ||
        uMsg == AKD_INISETVALUEW)
    {
      INIHANDLE *ih=(INIHANDLE *)wParam;
      INIVALUEW *iv=(INIVALUEW *)lParam;
      wchar_t *wpSection=AllocWideStr(MAX_PATH);
      wchar_t *wpKey=AllocWideStr(MAX_PATH);
      int nResult;

      if (uMsg == AKD_INISETVALUEA || (bOldWindows && uMsg == AKD_INISETVALUE))
      {
        xprintfW(wpSection, L"%S", (char *)iv->pSection);
        xprintfW(wpKey, L"%S", (char *)iv->pKey);
      }
      else
      {
        xprintfW(wpSection, L"%s", (wchar_t *)iv->pSection);
        xprintfW(wpKey, L"%s", (wchar_t *)iv->pKey);
      }
      nResult=IniSetValueW(&ih->hStack, wpSection, wpKey, iv->dwType, (LPBYTE)iv->lpData, iv->dwData);

      FreeWideStr(wpSection);
      FreeWideStr(wpKey);
      return (LRESULT)nResult;
    }
    if (uMsg == AKD_INICLOSE)
    {
      INIHANDLE *ih=(INIHANDLE *)wParam;
      BOOL bResult=FALSE;

      if (ih)
      {
        if (ih->dwType & POB_READ)
          bResult=TRUE;
        else if (ih->dwType & POB_SAVE)
          bResult=SaveIniW(&ih->hStack, ih->wszIniFile);
        StackFreeIni(&ih->hStack);
        API_HeapFree(hHeap, 0, (LPVOID)ih);
      }
      return bResult;
    }

    //AkelPad 4.x only messages
    if (uMsg == AKD_EXGETTEXTLENGTH)
    {
      return -IndexSubtract((HWND)wParam, NULL, NULL, lParam, FALSE);
    }
    if (uMsg == AKD_EXGETTEXTRANGE ||
        uMsg == AKD_EXGETTEXTRANGEA ||
        uMsg == AKD_EXGETTEXTRANGEW)
    {
      EXGETTEXTRANGE *tr=(EXGETTEXTRANGE *)lParam;

      if (uMsg == AKD_EXGETTEXTRANGEA || (bOldWindows && uMsg == AKD_EXGETTEXTRANGE))
        return ExGetRangeTextA((HWND)wParam, tr->nCodePage, tr->lpDefaultChar, tr->lpUsedDefChar, &tr->cr.ciMin, &tr->cr.ciMax, tr->bColumnSel, (char **)&tr->pText, tr->nNewLine, TRUE);
      else
        return ExGetRangeTextW((HWND)wParam, &tr->cr.ciMin, &tr->cr.ciMax, tr->bColumnSel, (wchar_t **)&tr->pText, tr->nNewLine, TRUE);
    }
    if (uMsg == AKD_GETSTATUSPOSTYPE)
    {
      return dwStatusPosType;
    }
  }

  if (uMsg == WM_COPYDATA)
  {
    COPYDATASTRUCT *cds=(COPYDATASTRUCT *)lParam;
    int nResult=0;

    if (cds->dwData == CD_OPENDOCUMENT ||
        cds->dwData == CD_OPENDOCUMENTA ||
        cds->dwData == CD_OPENDOCUMENTW)
    {
      OPENDOCUMENTPOSTA *odpA=(OPENDOCUMENTPOSTA *)cds->lpData;
      OPENDOCUMENTPOSTW *odpW=(OPENDOCUMENTPOSTW *)cds->lpData;
      wchar_t *wpFile=AllocWideStr(MAX_PATH);
      wchar_t *wpWorkDir=AllocWideStr(MAX_PATH);
      int nResult=0;

      if ((odpA->hWnd && !IsEditActive(odpA->hWnd)) || nMDI || SaveChanged())
      {
        if (cds->dwData == CD_OPENDOCUMENTA || (bOldWindows && cds->dwData == CD_OPENDOCUMENT))
        {
          xprintfW(wpFile, L"%S", (char *)odpA->szFile);
          xprintfW(wpWorkDir, L"%S", (char *)odpA->szWorkDir);
        }
        else
        {
          xprintfW(wpFile, L"%s", (wchar_t *)odpW->szFile);
          xprintfW(wpWorkDir, L"%s", (wchar_t *)odpW->szWorkDir);
        }

        if (*wpWorkDir) SetCurrentDirectoryWide(wpWorkDir);
        nResult=OpenDocument(odpA->hWnd, wpFile, odpA->dwFlags, odpA->nCodePage, odpA->bBOM);
        if (*wpWorkDir) SetCurrentDirectoryWide(wszExeDir);
      }
      FreeWideStr(wpFile);
      FreeWideStr(wpWorkDir);
    }
    return nResult;
  }
  else if (uMsg == WM_INITMENU)
  {
    LPARAM lMenuState;

    if (!lParam || (lParam & IMENU_EDIT))
    {
      if (lpFrameCurrent->ei.hWndEdit)
      {
        EnableMenuItem(hMainMenu, IDM_FILE_CREATENEW, (nMDI && bSingleOpenProgram)?MF_GRAYED:MF_ENABLED);
        EnableMenuItem(hMainMenu, IDM_FILE_REOPEN, (lpFrameCurrent->wszFile[0])?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_FILE_SAVE, lpFrameCurrent->ei.bModified?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_UNDO, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANUNDO, 0, 0))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_REDO, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANREDO, 0, 0))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_PASTE, (!lpFrameCurrent->ei.bReadOnly && SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANPASTE, 0, 0))?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_CUT, (!lpFrameCurrent->ei.bReadOnly && AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_CLEAR, (!lpFrameCurrent->ei.bReadOnly && AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))?MF_ENABLED:MF_GRAYED);

        EnableMenuItem(hMainMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_FIRST_CHAR_MENU, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);

        lMenuState=(!DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK, NULL))?MF_GRAYED:MF_ENABLED;
        EnableMenuItem(hMainMenu, IDM_EDIT_INSERT_TAB_MENU, lMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_TAB_MENU, lMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_INSERT_SPACE_MENU, lMenuState);
        EnableMenuItem(hMainMenu, IDM_EDIT_DELETE_SPACE_MENU, lMenuState);
      }
      EnableMenuItem(hMainMenu, IDM_OPTIONS_EXEC, (*wszCommand)?MF_ENABLED:MF_GRAYED);
    }
    if (!lParam || (lParam & IMENU_CHECKS))
    {
      if (lpFrameCurrent->ei.nNewLine == NEWLINE_WIN)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_WIN, MF_BYCOMMAND);
      else if (lpFrameCurrent->ei.nNewLine == NEWLINE_UNIX)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_UNIX, MF_BYCOMMAND);
      else if (lpFrameCurrent->ei.nNewLine == NEWLINE_MAC)
        CheckMenuRadioItem(hMainMenu, IDM_EDIT_NEWLINE_WIN, IDM_EDIT_NEWLINE_MAC, IDM_EDIT_NEWLINE_MAC, MF_BYCOMMAND);
      CheckMenuItem(hMainMenu, IDM_VIEW_READONLY, lpFrameCurrent->ei.bReadOnly?MF_CHECKED:MF_UNCHECKED);
      CheckMenuItem(hMainMenu, IDM_VIEW_WORDWRAP, lpFrameCurrent->ei.bWordWrap?MF_CHECKED:MF_UNCHECKED);
      CheckMenuItem(hMainMenu, IDM_VIEW_SPLIT_WINDOW_ALL, lpFrameCurrent->ei.hWndMaster?MF_CHECKED:MF_UNCHECKED);
    }
    bMenuLanguage=TRUE;
    if (lParam) return TRUE;
  }
  else if (uMsg == WM_INITMENUPOPUP)
  {
    if ((HMENU)wParam == hMenuLanguage)
    {
      if (bMenuLanguage)
      {
        bMenuLanguage=FALSE;
        LanguageMenu();
      }
    }
    else if ((HMENU)wParam == hMenuRecentFiles)
    {
      if (bMenuRecentFiles)
      {
        bMenuRecentFiles=FALSE;
        RecentFilesMenu();
      }
    }
    else if ((HMENU)wParam == hPopupOpenCodepage ||
             (HMENU)wParam == hPopupSaveCodepage)
    {
      if (bMenuPopupCodepage)
      {
        bMenuPopupCodepage=FALSE;
        FillMenuPopupCodepage();
      }

      //Check radio item
      {
        int nCurrentCodePageIndex;

        nCurrentCodePageIndex=CodepageListFind(lpCodepageList, lpFrameCurrent->ei.nCodePage);
        CheckMenuRadioItem(hPopupOpenCodepage, 0, nCodepageListLen - 1, nCurrentCodePageIndex, MF_BYPOSITION);
        CheckMenuRadioItem(hPopupSaveCodepage, 0, nCodepageListLen - 1, nCurrentCodePageIndex, MF_BYPOSITION);
      }
    }
  }
  else if (uMsg == WM_MOVE)
  {
    DWORD dwStyle;

    dwStyle=GetWindowLongWide(hWnd, GWL_STYLE);

    if (!(dwStyle & WS_MAXIMIZE) && !(dwStyle & WS_MINIMIZE))
    {
      GetWindowPos(hWnd, NULL, &rcMainWindowRestored);
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (wParam != SIZE_MINIMIZED)
    {
      if (wParam != SIZE_MAXIMIZED)
      {
        GetWindowPos(hWnd, NULL, &rcMainWindowRestored);
      }
      dwLastMainSize=wParam;
      SendMessage(hStatus, WM_SIZE, wParam, lParam);
      UpdateSize();
      return TRUE;
    }
  }
  else if (uMsg == WM_DROPFILES)
  {
    if (uMsg == WM_DROPFILES)
    {
      DropFiles((HDROP)wParam);
      return TRUE;
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    //WM_COMMAND (SDI & MDI)
    if (LOWORD(wParam) == IDM_RECENT_FILES)
    {
      int nDead;
      BOOL bMessageShow=TRUE;

      RecentFilesRead();
      nDead=RecentFilesDeleteDead();
      RecentFilesSave();
      bMenuRecentFiles=TRUE;
      SendMessage(hMainWnd, AKDN_RECENTFILESDELETE, nDead, (LPARAM)&bMessageShow);

      if (bMessageShow)
      {
        LoadStringWide(hLangLib, MSG_RECENTFILES_DELETED, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, nDead);
        MessageBoxW(hWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONINFORMATION);
      }
    }
    else if (LOWORD(wParam) > IDM_RECENT_FILES && LOWORD(wParam) <= (IDM_RECENT_FILES + nRecentFiles))
    {
      if (nMDI || SaveChanged())
      {
        wchar_t *wpFile=AllocWideStr(MAX_PATH);

        xstrcpynW(wpFile, lpwszRecentNames[LOWORD(wParam) - IDM_RECENT_FILES - 1], MAX_PATH);
        OpenDocument(lpFrameCurrent->ei.hWndEdit, wpFile, OD_ADT_BINARY_ERROR|OD_ADT_REG_CODEPAGE, 0, FALSE);

        FreeWideStr(wpFile);
      }
    }
    else if (LOWORD(wParam) >= IDM_LANGUAGE && LOWORD(wParam) <= (IDM_LANGUAGE + nLangModules + 1))
    {
      if (LOWORD(wParam) == IDM_LANGUAGE)
      {
        if (!*wszLangModule) return TRUE;
        wszLangModule[0]='\0';
      }
      else
      {
        GetMenuStringW(hMainMenu, LOWORD(wParam), wbuf, BUFFER_SIZE, MF_BYCOMMAND);
        xprintfW(wbuf2, L"%s.dll", wbuf);
        if (!xstrcmpiW(wszLangModule, wbuf2)) return TRUE;
        xstrcpynW(wszLangModule, wbuf2, MAX_PATH);
      }
      LoadStringWide(hLangLib, MSG_RESTART_PROGRAM, wbuf, BUFFER_SIZE);
      MessageBoxW(hWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
    }
    else if (LOWORD(wParam) >= IDM_POPUP_OPENAS && LOWORD(wParam) < IDM_POPUP_OPENAS + nCodepageListLen)
    {
      return DoFileReopenAs(OD_ADT_DETECT_BOM, lpCodepageList[LOWORD(wParam) - IDM_POPUP_OPENAS], TRUE);
    }
    else if (LOWORD(wParam) >= IDM_POPUP_SAVEAS && LOWORD(wParam) < IDM_POPUP_SAVEAS + nCodepageListLen)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, lpCodepageList[LOWORD(wParam) - IDM_POPUP_SAVEAS], TRUE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_FILE_NEW)
    {
      return DoFileNew();
    }
    else if (LOWORD(wParam) == IDM_FILE_CREATENEW)
    {
      if (!nMDI || !bSingleOpenProgram)
      {
        SaveOptions();
      }
      return (int)DoFileNewWindow(0);
    }
    else if (LOWORD(wParam) == IDM_FILE_OPEN)
    {
      return DoFileOpen();
    }
    else if (LOWORD(wParam) == IDM_FILE_REOPEN)
    {
      return DoFileReopenAs(0, lpFrameCurrent->ei.nCodePage, lpFrameCurrent->ei.bBOM);
    }
    else if (LOWORD(wParam) == IDM_FILE_SAVE)
    {
      return DoFileSave();
    }
    else if (LOWORD(wParam) == IDM_FILE_SAVEAS)
    {
      return DoFileSaveAs(-1, -1);
    }
    else if (LOWORD(wParam) == IDM_FILE_SAVEALL)
    {
      if (!nMDI)
      {
        return DoFileSave();
      }
      else
      {
        FRAMEDATA *lpFrameInit=lpFrameCurrent;

        do
        {
          if (!DoFileSave()) return FALSE;
          lpFrameCurrent=NextMdiFrameWindow(lpFrameCurrent, FALSE);
        }
        while (lpFrameCurrent != lpFrameInit);

        return TRUE;
      }
    }
    else if (LOWORD(wParam) == IDM_FILE_CLOSEALL)
    {
      if (!nMDI)
      {
        return DoFileClose();
      }
      else
      {
        while (lpFrameCurrent->hWndEditParent)
        {
          if (DestroyMdiFrameWindow(lpFrameCurrent, -1) != FWD_SUCCESS)
            return FALSE;
        }
        return TRUE;
      }
    }
    else if (LOWORD(wParam) == IDM_FILE_CLOSEALL_BUTACTIVE)
    {
      if (nMDI)
      {
        FRAMEDATA *lpFrameInit=lpFrameCurrent;

        while (1)
        {
          lpFrameCurrent=NextMdiFrameWindow(lpFrameCurrent, FALSE);
          if (lpFrameCurrent == lpFrameInit) break;

          if (DestroyMdiFrameWindow(lpFrameCurrent, -1) != FWD_SUCCESS)
            return FALSE;
        }
        return TRUE;
      }
    }
    else if (LOWORD(wParam) == IDM_FILE_SAVEALLAS)
    {
      DoFileSaveAllAs();
    }
    else if (LOWORD(wParam) == IDM_FILE_PAGESETUP)
    {
      return DoFilePageSetup(hMainWnd);
    }
    else if (LOWORD(wParam) == IDM_FILE_PRINT)
    {
      return DoFilePrint((lParam?(HWND)lParam:lpFrameCurrent->ei.hWndEdit), FALSE);
    }
    else if (LOWORD(wParam) == IDM_FILE_PRINTPREVIEW)
    {
      DoFilePreview((lParam?(HWND)lParam:lpFrameCurrent->ei.hWndEdit));
      return 0;
    }
    else if (LOWORD(wParam) == IDM_FILE_SILENTPRINT)
    {
      return DoFilePrint((lParam?(HWND)lParam:lpFrameCurrent->ei.hWndEdit), TRUE);
    }
    else if (LOWORD(wParam) == IDM_FILE_EXIT)
    {
      PostMessage(hWnd, WM_CLOSE, 0, 0);
    }
    else if (LOWORD(wParam) == IDM_EDIT_UNDO)
    {
      DoEditUndo(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_REDO)
    {
      DoEditRedo(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_CUT)
    {
      DoEditCut(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_COPY)
    {
      DoEditCopy(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_PASTE)
    {
      return DoEditPaste(lpFrameCurrent->ei.hWndEdit, FALSE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_CLEAR)
    {
      DoEditClear(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_SELECTALL)
    {
      DoEditSelectAll(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_INSERTCHAR)
    {
      DoEditInsertChar();
    }
    else if (LOWORD(wParam) == IDM_EDIT_INSERTDATE)
    {
      DoEditInsertDate(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_RECODE)
    {
      DoEditRecode(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_INSERT_TAB_MENU ||
             LOWORD(wParam) == IDM_EDIT_INSERT_TAB)
    {
      if (!DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK, NULL))
        InsertTabStopW(lpFrameCurrent->ei.hWndEdit);
      else
        IndentTabStopW(lpFrameCurrent->ei.hWndEdit, STRSEL_INSERT|STRSEL_TAB);
    }
    else if (LOWORD(wParam) == IDM_EDIT_DELETE_TAB_MENU ||
             LOWORD(wParam) == IDM_EDIT_DELETE_TAB)
    {
      if (!DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK, NULL))
        InsertTabStopW(lpFrameCurrent->ei.hWndEdit);
      else
        IndentTabStopW(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_TAB);
    }
    else if (LOWORD(wParam) == IDM_EDIT_INSERT_SPACE_MENU ||
             LOWORD(wParam) == IDM_EDIT_INSERT_SPACE)
    {
      if (!DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK, NULL))
      {
        if (!IsReadOnly(NULL))
          SendMessage(lpFrameCurrent->ei.hWndEdit, WM_CHAR, (WPARAM)' ', 0);
      }
      else DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_INSERT, L" ");
    }
    else if (LOWORD(wParam) == IDM_EDIT_DELETE_SPACE_MENU ||
             LOWORD(wParam) == IDM_EDIT_DELETE_SPACE)
    {
      if (!DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_CHECK, NULL))
      {
        if (!IsReadOnly(NULL)) SendMessage(lpFrameCurrent->ei.hWndEdit, WM_CHAR, (WPARAM)' ', 0);
      }
      else
        DoEditInsertStringInSelectionW(lpFrameCurrent->ei.hWndEdit, STRSEL_DELETE|STRSEL_SPACE, L" ");
    }
    else if (LOWORD(wParam) == IDM_EDIT_DELETE_FIRST_CHAR_MENU ||
             LOWORD(wParam) == IDM_EDIT_DELETE_FIRST_CHAR)
    {
      DoEditDeleteFirstCharW(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_DELETE_TRAILING_WHITESPACES)
    {
      DoEditDeleteTrailingWhitespacesW(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_UPPERCASE)
    {
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, UPPERCASE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_LOWERCASE)
    {
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, LOWERCASE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_SENTENCECASE)
    {
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, SENTENCECASE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_TITLECASE)
    {
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, TITLECASE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_INVERTCASE)
    {
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, INVERTCASE);
    }
    else if (LOWORD(wParam) == IDM_EDIT_LOOPCASE)
    {
      int nCase=nLoopCase;

      if (nCase >= INVERTCASE)
        nCase=UPPERCASE;
      else
        ++nCase;
      DoEditChangeCaseW(lpFrameCurrent->ei.hWndEdit, nCase);
      nLoopCase=nCase;
    }
    else if (LOWORD(wParam) == IDM_EDIT_NEWLINE_WIN)
    {
      if (!IsReadOnly(NULL))
      {
        if (!(lpFrameCurrent->ei.nNewLine == NEWLINE_WIN))
        {
          SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_WIN, AENL_INPUT|AENL_OUTPUT);
          SetModifyStatus(lpFrameCurrent->ei.hWndEdit, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDM_EDIT_NEWLINE_UNIX)
    {
      if (!IsReadOnly(NULL))
      {
        if (!(lpFrameCurrent->ei.nNewLine == NEWLINE_UNIX))
        {
          SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_UNIX, AENL_INPUT|AENL_OUTPUT);
          SetModifyStatus(lpFrameCurrent->ei.hWndEdit, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDM_EDIT_NEWLINE_MAC)
    {
      if (!IsReadOnly(NULL))
      {
        if (!(lpFrameCurrent->ei.nNewLine == NEWLINE_MAC))
        {
          SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_MAC, AENL_INPUT|AENL_OUTPUT);
          SetModifyStatus(lpFrameCurrent->ei.hWndEdit, TRUE);
        }
      }
    }
    else if (LOWORD(wParam) == IDM_EDIT_FIND)
    {
      DoEditFind();
    }
    else if (LOWORD(wParam) == IDM_EDIT_FINDNEXTDOWN)
    {
      DoEditFindNextDown(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_FINDNEXTUP)
    {
      DoEditFindNextUp(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_EDIT_REPLACE)
    {
      DoEditReplace();
    }
    else if (LOWORD(wParam) == IDM_EDIT_GOTOLINE)
    {
      DoEditGoToLine();
    }
    else if (LOWORD(wParam) == IDM_VIEW_FONT)
    {
      if (DoViewFont(hMainWnd, &lpFrameCurrent->lf))
      {
        if (SetChosenFont(lpFrameCurrent->ei.hWndEdit, &lpFrameCurrent->lf))
        {
          bEditFontChanged=TRUE;
          return TRUE;
        }
      }
      return FALSE;
    }
    else if (LOWORD(wParam) == IDM_VIEW_COLORS)
    {
      DoViewColors();
      return FALSE;
    }
    else if (LOWORD(wParam) == IDM_VIEW_INCREASE_FONT)
    {
      DoViewFontSize(lpFrameCurrent->ei.hWndEdit, INCREASE_FONT);
      bEditFontChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDM_VIEW_DECREASE_FONT)
    {
      DoViewFontSize(lpFrameCurrent->ei.hWndEdit, DECREASE_FONT);
      bEditFontChanged=TRUE;
    }
    else if (LOWORD(wParam) == IDM_VIEW_WORDWRAP)
    {
      DoViewWordWrap(lpFrameCurrent->ei.hWndEdit, !lpFrameCurrent->ei.bWordWrap, FALSE);
    }
    else if (LOWORD(wParam) == IDM_VIEW_SPLIT_WINDOW_ALL ||
             LOWORD(wParam) == IDM_VIEW_SPLIT_WINDOW_WE ||
             LOWORD(wParam) == IDM_VIEW_SPLIT_WINDOW_NS)
    {
      DoViewSplitWindow(!lpFrameCurrent->ei.hWndMaster, LOWORD(wParam));
    }
    else if (LOWORD(wParam) == IDM_VIEW_ONTOP)
    {
      DoViewOnTop(!bOnTop, FALSE);
    }
    else if (LOWORD(wParam) == IDM_VIEW_SHOW_STATUSBAR)
    {
      DoViewShowStatusBar(!bStatusBar, FALSE);
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_EXEC)
    {
      return DoSettingsExec();
    }
    else if (LOWORD(wParam) == IDM_VIEW_READONLY)
    {
      DoSettingsReadOnly(lpFrameCurrent->ei.hWndEdit, !lpFrameCurrent->ei.bReadOnly, FALSE);
      if (hDlgModeless) PostMessage(hDlgModeless, WM_COMMAND, IDC_SETREADONLY, 0);
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_SAVETIME)
    {
      DoSettingsSaveTime(!bSaveTime);
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_KEEPSPACE)
    {
      DoSettingsKeepSpace(!bKeepSpace);
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_WATCHFILE)
    {
      DoSettingsWatchFile(!bWatchFile);
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_SINGLEOPEN_FILE)
    {
      DoSettingsSingleOpenFile(!bSingleOpenFile);
      SaveOptions();
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_SINGLEOPEN_PROGRAM)
    {
      DoSettingsSingleOpenProgram(!bSingleOpenProgram);
      SaveOptions();
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_SDI ||
             LOWORD(wParam) == IDM_OPTIONS_MDI ||
             LOWORD(wParam) == IDM_OPTIONS_PMDI)
    {
      int nMode=WMD_SDI;

      if (LOWORD(wParam) == IDM_OPTIONS_SDI)
        nMode=WMD_SDI;
      else if (LOWORD(wParam) == IDM_OPTIONS_MDI)
        nMode=WMD_MDI;
      else if (LOWORD(wParam) == IDM_OPTIONS_PMDI)
        nMode=WMD_PMDI;

      if (nRegMDI != nMode)
      {
        CheckMenuRadioItem(hMainMenu, IDM_OPTIONS_SDI, IDM_OPTIONS_PMDI, IDM_OPTIONS_SDI + nMode, MF_BYCOMMAND);

        LoadStringWide(hLangLib, MSG_RESTART_PROGRAM, wbuf, BUFFER_SIZE);
        MessageBoxW(hWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
        nRegMDI=nMode;
      }
    }
    else if (LOWORD(wParam) == IDM_OPTIONS)
    {
      DoSettingsOptions();
    }
    else if (LOWORD(wParam) == IDM_OPTIONS_PLUGINS)
    {
      DoSettingsPlugins(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_ABOUT)
    {
      DoHelpAbout();
    }
    else if (LOWORD(wParam) == IDM_NONMENU_CHANGESIZE)
    {
      ShowWindow(hWnd, (GetWindowLongWide(hWnd, GWL_STYLE) & WS_MAXIMIZE)?SW_RESTORE:SW_MAXIMIZE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_DELLINE)
    {
      DoNonMenuDelLine(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_PASTEANSI)
    {
      return DoEditPaste(lpFrameCurrent->ei.hWndEdit, TRUE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REDETECT)
    {
      return DoFileReopenAs(OD_ADT_BINARY_ERROR|OD_ADT_DETECT_CODEPAGE|OD_ADT_DETECT_BOM, 0, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_ANSI)
    {
      return DoFileReopenAs(0, nAnsiCodePage, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_OEM)
    {
      return DoFileReopenAs(0, nOemCodePage, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_UTF16LE)
    {
      return DoFileReopenAs(OD_ADT_DETECT_BOM, CP_UNICODE_UCS2_LE, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_UTF16BE)
    {
      return DoFileReopenAs(OD_ADT_DETECT_BOM, CP_UNICODE_UCS2_BE, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_UTF8)
    {
      return DoFileReopenAs(OD_ADT_DETECT_BOM, CP_UNICODE_UTF8, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_REOPENAS_KOIR)
    {
      return DoFileReopenAs(0, CP_KOI8_R, FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_ANSI)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, nAnsiCodePage, FALSE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_OEM)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, nOemCodePage, FALSE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_UTF16LE)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, CP_UNICODE_UCS2_LE, TRUE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_UTF16BE)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, CP_UNICODE_UCS2_BE, TRUE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_UTF8)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, CP_UNICODE_UTF8, TRUE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_UTF8_NOBOM)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, CP_UNICODE_UTF8, FALSE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_SAVEAS_KOIR)
    {
      return SaveDocument(lpFrameCurrent->ei.hWndEdit, lpFrameCurrent->wszFile, CP_KOI8_R, FALSE, SD_UPDATE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_INSERTMODE)
    {
      if (lpFrameCurrent->ei.hWndEdit) SetOvertypeStatus(lpFrameCurrent->ei.hWndEdit, !lpFrameCurrent->ei.bOvertypeMode);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_FILECLOSE)
    {
      return DoFileClose();
    }
    else if (LOWORD(wParam) == IDM_NONMENU_DLGNEXT)
    {
      return (LRESULT)NextDialog(FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_DLGPREV)
    {
      return (LRESULT)NextDialog(TRUE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_AUTOINDENT)
    {
      BOOL bResult;

      if (!(bResult=AutoIndent(lpFrameCurrent->ei.hWndEdit, &crSel)))
        ReplaceSelW(lpFrameCurrent->ei.hWndEdit, L"\n", -1, FALSE, NULL, NULL);
      return bResult;
    }
    else if (LOWORD(wParam) == IDM_NONMENU_CLONENEXT)
    {
      return (LRESULT)NextClone(FALSE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_CLONEPREV)
    {
      return (LRESULT)NextClone(TRUE);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_COLUMNPASTE)
    {
      return ColumnPaste(lpFrameCurrent->ei.hWndEdit);
    }
    else if (LOWORD(wParam) == IDM_NONMENU_PASTEAFTER)
    {
      return PasteAfter(lpFrameCurrent->ei.hWndEdit, FALSE);
    }
    else if (LOWORD(wParam) == IDM_INTERNAL_REOPEN_MSG)
    {
      if (IsEditActive((HWND)lParam))
      {
        LoadStringWide(hLangLib, MSG_FILE_CHANGED, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, lpFrameCurrent->wszFile);
        if (MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, lpFrameCurrent->ei.bModified?MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2:MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
          EDITINFO ei;

          if (GetEditInfo((HWND)lParam, &ei))
          {
            OpenDocument(ei.hWndEdit, (wchar_t *)ei.pFile, OD_REOPEN, ei.nCodePage, ei.bBOM);
          }
        }
      }
      hWndReopen=NULL;
    }
    else if (LOWORD(wParam) == IDM_INTERNAL_CANTOPEN_MSG)
    {
      if (IsEditActive((HWND)lParam))
      {
        LoadStringWide(hLangLib, MSG_CANNOT_OPEN_FILE, wbuf, BUFFER_SIZE);
        xprintfW(wbuf2, wbuf, lpFrameCurrent->wszFile);
        MessageBoxW(hMainWnd, wbuf2, APP_MAIN_TITLEW, MB_OK|MB_ICONEXCLAMATION);
      }
    }
    else if (LOWORD(wParam) == IDM_INTERNAL_ERRORIO_MSG)
    {
      LoadStringWide(hLangLib, MSG_ERROR_IO, wbuf, BUFFER_SIZE);
      MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
    }
    else if (LOWORD(wParam) == IDM_POPUP_CODEPAGEMENU)
    {
      RECT rc;

      SendMessage(hStatus, SB_GETRECT, STATUS_CODEPAGE, (LPARAM)&rc);
      ClientToScreen(hStatus, (POINT *)&rc);
      ShowMenuPopupCodepage((POINT *)&rc);
    }

    //WM_COMMAND (MDI)
    if (nMDI)
    {
      if (LOWORD(wParam) == IDM_SELECTWINDOW ||
          LOWORD(wParam) == IDM_WINDOW_MDILIST)
      {
        DoWindowSelectWindow();
        return 0;
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABVIEW_TOP)
      {
        DoWindowTabView(TAB_VIEW_TOP, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABVIEW_BOTTOM)
      {
        DoWindowTabView(TAB_VIEW_BOTTOM, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABVIEW_NONE)
      {
        DoWindowTabView(TAB_VIEW_NONE, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABTYPE_STANDARD)
      {
        DoWindowTabType(TAB_TYPE_STANDARD, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABTYPE_BUTTONS)
      {
        DoWindowTabType(TAB_TYPE_BUTTONS, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABTYPE_FLATBUTTONS)
      {
        DoWindowTabType(TAB_TYPE_FLATBUTTONS, FALSE);
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABSWITCH_NEXTPREV)
      {
        if (dwTabOptionsMDI & TAB_SWITCH_RIGHTLEFT)
        {
          CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABSWITCH_NEXTPREV, IDM_WINDOW_TABSWITCH_RIGHTLEFT, IDM_WINDOW_TABSWITCH_NEXTPREV, MF_BYCOMMAND);
          dwTabOptionsMDI&=~TAB_SWITCH_RIGHTLEFT;
          dwTabOptionsMDI|=TAB_SWITCH_NEXTPREV;
        }
      }
      else if (LOWORD(wParam) == IDM_WINDOW_TABSWITCH_RIGHTLEFT)
      {
        if (dwTabOptionsMDI & TAB_SWITCH_NEXTPREV)
        {
          CheckMenuRadioItem(hMainMenu, IDM_WINDOW_TABSWITCH_NEXTPREV, IDM_WINDOW_TABSWITCH_RIGHTLEFT, IDM_WINDOW_TABSWITCH_RIGHTLEFT, MF_BYCOMMAND);
          dwTabOptionsMDI&=~TAB_SWITCH_NEXTPREV;
          dwTabOptionsMDI|=TAB_SWITCH_RIGHTLEFT;
        }
      }
      if (nMDI == WMD_MDI)
      {
        //WMD_MDI only
        if (LOWORD(wParam) == IDM_WINDOW_TILEHORIZONTAL)
        {
          SendMessage(hMdiClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
        }
        else if (LOWORD(wParam) == IDM_WINDOW_TILEVERTICAL)
        {
          SendMessage(hMdiClient, WM_MDITILE, MDITILE_VERTICAL, 0);
        }
        else if (LOWORD(wParam) == IDM_WINDOW_CASCADE)
        {
          SendMessage(hMdiClient, WM_MDICASCADE, 0, 0);
        }
      }

      if (LOWORD(wParam) == IDM_NONMENU_MDINEXT)
      {
        NextMdiFrameWindow(lpFrameCurrent, FALSE);
      }
      else if (LOWORD(wParam) == IDM_NONMENU_MDIPREV)
      {
        NextMdiFrameWindow(lpFrameCurrent, TRUE);
      }
      else if (LOWORD(wParam) == IDM_NONMENU_MDICLOSE)
      {
        return !DestroyMdiFrameWindow(lpFrameCurrent, -1);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    //WM_NOTIFY (SDI & MDI)
    if (wParam == ID_STATUS && (nMDI == WMD_SDI || nMDI == WMD_PMDI || lpFrameCurrent->hWndEditParent))
    {
      if ((int)((NMHDR *)lParam)->code == NM_DBLCLK)
      {
        if (((NMMOUSE *)lParam)->dwItemSpec == 0)
        {
          DoEditGoToLine();
        }
        else if (((NMMOUSE *)lParam)->dwItemSpec == 2)
        {
          SetOvertypeStatus(lpFrameCurrent->ei.hWndEdit, !lpFrameCurrent->ei.bOvertypeMode);
        }
        else if (((NMMOUSE *)lParam)->dwItemSpec == 3)
        {
          if (!IsReadOnly(NULL))
          {
            if (lpFrameCurrent->ei.nNewLine == NEWLINE_WIN)
              SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_UNIX, AENL_INPUT|AENL_OUTPUT);
            else if (lpFrameCurrent->ei.nNewLine == NEWLINE_UNIX)
              SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_MAC, AENL_INPUT|AENL_OUTPUT);
            else if (lpFrameCurrent->ei.nNewLine == NEWLINE_MAC)
              SetNewLineStatus(lpFrameCurrent->ei.hWndEdit, NEWLINE_WIN, AENL_INPUT|AENL_OUTPUT);
            SetModifyStatus(lpFrameCurrent->ei.hWndEdit, TRUE);
          }
        }
      }
      else if ((int)((NMHDR *)lParam)->code == NM_RCLICK)
      {
        if (((NMMOUSE *)lParam)->dwItemSpec == 4)
        {
          POINT pt;

          GetCursorPos(&pt);
          ShowMenuPopupCodepage(&pt);
        }
      }
    }

    //WM_NOTIFY (MDI)
    if (nMDI)
    {
      if (wParam == ID_TAB)
      {
        if (((NMHDR *)lParam)->code == TCN_GETOBJECT)
        {
          if (((LPNMOBJECTNOTIFY)lParam)->iItem != -1)
          {
            if (!dwTabOpenTimer)
            {
              dwTabOpenTimer=SetTimer(hTab, TIMERID_TABOPEN, TIME_TABOPEN, NULL);
              nTabOpenItem=((LPNMOBJECTNOTIFY)lParam)->iItem;
            }
            else if (nTabOpenItem != ((LPNMOBJECTNOTIFY)lParam)->iItem)
            {
              KillTimer(hTab, dwTabOpenTimer);
              dwTabOpenTimer=0;
              nTabOpenItem=-1;
            }
          }
          return 0;
        }
        else if (((NMHDR *)lParam)->code == TCN_SELCHANGING)
        {
          if (dwTabOpenTimer)
            return TRUE;
        }
        else if (((NMHDR *)lParam)->code == TCN_SELCHANGE)
        {
          TCITEMW tcItemW;
          FRAMEDATA *lpFrame;
          int nItem;

          bTabPressed=TRUE;
          nItem=SendMessage(hTab, TCM_GETCURSEL, 0, 0);
          tcItemW.mask=TCIF_PARAM;
          TabCtrl_GetItemWide(hTab, nItem, &tcItemW);
          lpFrame=(FRAMEDATA *)tcItemW.lParam;

          ActivateMdiFrameWindow(lpFrame, TRUE);
          bTabPressed=FALSE;
        }
      }
    }
  }
  else if (uMsg == WM_CLOSE)
  {
    if (!nMDI)
    {
      if (!DoFileExit()) return TRUE;
      CopyFrameData(&fdLast, lpFrameCurrent);
    }
    else
    {
      int nDestroyResult;

      bMdiClientRedraw=FALSE;
      CopyFrameData(&fdLast, lpFrameCurrent);

      while (lpFrameCurrent->hWndEditParent)
      {
        nDestroyResult=DestroyMdiFrameWindow(lpFrameCurrent, -1);

        if (nDestroyResult == FWD_ABORT)
        {
          bMdiClientRedraw=TRUE;
          return TRUE;
        }
        else if (nDestroyResult != FWD_SUCCESS)
          break;
      }
    }

    //Close modeless dialog
    if (hDlgModeless)
      SendMessage(hDlgModeless, WM_CLOSE, 0, 0);

    //Main window will be destroyed
    PostMessage(hWnd, AKDN_MAIN_ONFINISH, 0, 0);

    return 0;
  }
  else if (uMsg == WM_DESTROY)
  {
    //Save options
    SaveOptions();

    //Save plugin stack
    if (bSavePluginsStackOnExit)
      StackPluginSave(&hPluginsStack);

    //Clean up
    if (nMDI)
    {
      ImageList_Destroy(hImageList);
      DestroyCursor(hCursorDragMove);
      DestroyIcon(hIconEmpty);
      StackIconsFree(&hIconsStack);
    }
    DestroyAcceleratorTable(hMainAccel);
    DestroyAcceleratorTable(hGlobalAccel);
    DestroyCursor(hCursorHandOpen);
    DestroyCursor(hCursorHandClose);
    DeleteObject(hBitmapClose);
    DestroyIcon(hMainIcon);
    DestroyMenu(hMainMenu);
    DestroyMenu(hPopupMenu);
    StackFontItemsFree(&hFontsStack);

    PostQuitMessage(0);
    return 0;
  }

  //Special messages
  if (nMDI)
  {
    if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MBUTTONDOWN)
    {
      CreateMdiFrameWindow(NULL);
    }
  }
  if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
  {
    LRESULT lResult;

    if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  if (bOldWindows)
  {
    if (nMDI == WMD_MDI)
      return DefFrameProcA(hWnd, hMdiClient, uMsg, wParam, lParam);
    else
      return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  }
  else
  {
    if (nMDI == WMD_MDI)
      return DefFrameProcW(hWnd, hMdiClient, uMsg, wParam, lParam);
    else
      return DefWindowProcW(hWnd, uMsg, wParam, lParam);
  }
}

LRESULT CALLBACK EditParentMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == AKDN_FRAME_ACTIVATE ||
      uMsg == WM_SETFOCUS)
  {
    if (bEditOnFinish)
      return FALSE;

    if (uMsg == WM_SETFOCUS)
    {
      if (lpFrameCurrent->ei.hWndEdit)
        SetFocus(lpFrameCurrent->ei.hWndEdit);
    }

    //Check modification time
    CheckModificationTime(lpFrameCurrent);
  }
  else if (uMsg == WM_CONTEXTMENU)
  {
    if ((HWND)wParam != lpFrameCurrent->ei.hWndEdit)
    {
      if (GetWindowLongWide((HWND)wParam, GWL_ID) == ID_EDIT)
      {
        SetFocus((HWND)wParam);
      }
    }

    if ((HWND)wParam == lpFrameCurrent->ei.hWndEdit)
    {
      NCONTEXTMENU ncm;
      RECT rc;
      POINT pt;
      HMENU hTrackMenu;

      if (lParam == -1)
      {
        SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_POSFROMCHAR, (WPARAM)&pt, (LPARAM)&ciCaret);
        pt.y-=lpFrameCurrent->lf.lfHeight;

        ClientToScreen(lpFrameCurrent->ei.hWndEdit, &pt);
        GetWindowRect(lpFrameCurrent->ei.hWndEdit, &rc);
        if (pt.x > rc.right || pt.y > rc.bottom)
        {
          pt.x=rc.left;
          pt.y=rc.top;
        }
      }
      else
      {
        GetCursorPos(&pt);
      }

      ncm.hWnd=lpFrameCurrent->ei.hWndEdit;
      ncm.uType=NCM_EDIT;
      ncm.pt=pt;
      ncm.bProcess=TRUE;
      SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
      if (ncm.bProcess)
      {
        if (lpFrameCurrent->ei.bReadOnly)
        {
          hTrackMenu=hPopupView;
          EnableMenuItem(hTrackMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);
        }
        else
        {
          hTrackMenu=hPopupEdit;
          EnableMenuItem(hTrackMenu, IDM_EDIT_UNDO, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANUNDO, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_REDO, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANREDO, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_PASTE, SendMessage(lpFrameCurrent->ei.hWndEdit, AEM_CANPASTE, 0, 0)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_CUT, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_CLEAR, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);
          EnableMenuItem(hTrackMenu, IDM_EDIT_COPY, AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax)?MF_ENABLED:MF_GRAYED);
        }
        TrackPopupMenu(hTrackMenu, TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hMainWnd, NULL);
      }
    }
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == ID_EDIT)
    {
      if (((NMHDR *)lParam)->code == AEN_ERRSPACE)
      {
        LoadStringWide(hLangLib, MSG_ERROR_NOT_ENOUGH_MEMORY_FOR_EDIT, wbuf, BUFFER_SIZE);
        MessageBoxW(hMainWnd, wbuf, APP_MAIN_TITLEW, MB_OK|MB_ICONERROR);
      }
      else if (((NMHDR *)lParam)->code == AEN_TEXTCHANGING)
      {
        nSelSubtract=0;
      }
      else if (((NMHDR *)lParam)->code == AEN_SELCHANGED)
      {
        AENSELCHANGE *aensc=(AENSELCHANGE *)lParam;

        SetSelectionStatus(aensc->hdr.hwndFrom, &aensc->aes.crSel, &aensc->ciCaret);
      }
      else if (((NMHDR *)lParam)->code == AEN_MODIFY)
      {
        AENMODIFY *aenm=(AENMODIFY *)lParam;

        //Synchronize changed state
        if (dwShowModify & SM_MAINTITLE_SDI)
        {
          if (nMDI == WMD_SDI || nMDI == WMD_PMDI)
          {
            wchar_t *wpMainName=AllocWideStr(MAX_PATH);
            const wchar_t *wpFileName=GetFileNameW(lpFrameCurrent->wszFile);

            if (aenm->bModified)
              xprintfW(wpMainName, L"* %s - %s", wpFileName, APP_MAIN_TITLEW);
            else
              xprintfW(wpMainName, L"%s - %s", wpFileName, APP_MAIN_TITLEW);
            SetWindowTextWide(hMainWnd, wpMainName);

            FreeWideStr(wpMainName);
          }
        }
        if (dwShowModify & SM_TABTITLE_MDI)
        {
          if (nMDI)
          {
            FRAMEDATA *lpFrame;
            int nItem;

            if (lpFrame=GetFrameDataFromEdit(aenm->hdr.hwndFrom))
            {
              if ((nItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
              {
                wchar_t *wpTabName=AllocWideStr(MAX_PATH);
                TCITEMW tcItemW;

                tcItemW.mask=TCIF_TEXT;
                tcItemW.pszText=wpTabName;
                tcItemW.cchTextMax=MAX_PATH;
                TabCtrl_GetItemWide(hTab, nItem, &tcItemW);

                if (aenm->bModified)
                  xprintfW(wpTabName, L"%s *", wpTabName);
                else
                  TrimModifyStateW(wpTabName);
                TabCtrl_SetItemWide(hTab, nItem, &tcItemW);

                FreeWideStr(wpTabName);
              }
            }
          }
        }
        if (dwShowModify & SM_FRAMETITLE_MDI)
        {
          if (nMDI == WMD_MDI)
          {
            wchar_t *wpFrameName=AllocWideStr(MAX_PATH);
            FRAMEDATA *lpFrame;

            if (lpFrame=GetFrameDataFromEdit(aenm->hdr.hwndFrom))
            {
              if (aenm->bModified)
                xprintfW(wpFrameName, L"%s *", lpFrame->wszFile);
              else
                xstrcpynW(wpFrameName, lpFrame->wszFile, MAX_PATH);
              SetWindowTextWide(lpFrame->hWndEditParent, wpFrameName);
            }
            FreeWideStr(wpFrameName);
          }
        }
        SetModifyStatus(aenm->hdr.hwndFrom, aenm->bModified);
      }
      else if (((NMHDR *)lParam)->code == AEN_LINK)
      {
        AENLINK *aenl=(AENLINK *)lParam;
        static BOOL bDownURL=FALSE;

        if (aenl->hdr.hwndFrom == lpFrameCurrent->ei.hWndEdit)
        {
          if (nClickURL == 1 && aenl->uMsg == WM_LBUTTONDOWN)
          {
            bDownURL=TRUE;
          }
          else
          {
            if ((nClickURL == 1 && aenl->uMsg == WM_LBUTTONUP && bDownURL) ||
                (nClickURL == 2 && aenl->uMsg == WM_LBUTTONDBLCLK))
            {
              if (!AEC_IndexCompare(&crSel.ciMin, &crSel.ciMax))
              {
                if (bOldWindows)
                {
                  char *szURL;

                  if (ExGetRangeTextA(lpFrameCurrent->ei.hWndEdit, CP_ACP, NULL, NULL, &aenl->crLink.ciMin, &aenl->crLink.ciMax, FALSE, &szURL, AELB_ASIS, FALSE))
                  {
                    ShellExecuteA(lpFrameCurrent->ei.hWndEdit, "open", szURL, NULL, NULL, SW_SHOWNORMAL);
                    FreeText(szURL);
                  }
                }
                else
                {
                  wchar_t *wszURL;

                  if (ExGetRangeTextW(lpFrameCurrent->ei.hWndEdit, &aenl->crLink.ciMin, &aenl->crLink.ciMax, FALSE, &wszURL, AELB_ASIS, FALSE))
                  {
                    ShellExecuteW(lpFrameCurrent->ei.hWndEdit, L"open", wszURL, NULL, NULL, SW_SHOWNORMAL);
                    FreeText(wszURL);
                  }
                }
                return TRUE;
              }
            }
            bDownURL=FALSE;
          }
        }
      }
      else if (((NMHDR *)lParam)->code == AEN_PROGRESS)
      {
        AENPROGRESS *aenp=(AENPROGRESS *)lParam;
        MSG msg;
        BOOL bStop=FALSE;
        static DWORD dwProgressType=0;
        static int nIncrement;
        static int nBarrier;
        static DWORD dwSeconds;

        if (!dwProgressType || dwProgressType == aenp->dwType)
        {
          //Start indication if elapsed time more than 0,5 second
          if (aenp->dwTimeElapsed > 500)
          {
            //Start progress
            if (!dwProgressType)
            {
              dwProgressType=aenp->dwType;

              SendMessage(hProgress, PBM_SETRANGE32, 0, aenp->nMaximum);
              SendMessage(hProgress, PBM_SETPOS, 0, 0);
              ShowWindow(hProgress, SW_SHOW);
              nIncrement=aenp->nMaximum / nProgressWidth;
              if (!nIncrement)
                nBarrier=0;
              else
                nBarrier=(aenp->nCurrent / nIncrement) * nIncrement;
              dwSeconds=1;
            }

            //Change position
            if (aenp->nCurrent >= nBarrier)
            {
              SendMessage(hProgress, PBM_SETPOS, aenp->nCurrent, 0);
              nBarrier+=nIncrement;
            }

            if (aenp->dwTimeElapsed / 500 > dwSeconds)
            {
              dwSeconds=aenp->dwTimeElapsed / 500;

              //Check for progress update
              while (PeekMessageWide(&msg, hProgress, 0, 0, PM_REMOVE))
              {
                TranslateMessage(&msg);
                DispatchMessageWide(&msg);
              }

              //Check for stop processing
              //if (dwProgressType == AEPGS_SETTEXT || dwProgressType == AEPGS_STREAMIN)
              {
                while (PeekMessageWide(&msg, hMainWnd, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE))
                {
                  if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
                  {
                    bStop=TRUE;
                  }
                }
              }
            }

            //End progress
            if (aenp->nCurrent == aenp->nMaximum || bStop)
            {
              ShowWindow(hProgress, SW_HIDE);
              UpdateWindow(hStatus);
              dwProgressType=0;

              if (bStop)
              {
                PostMessage(hMainWnd, WM_COMMAND, IDM_FILE_EXIT, 0);
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return FALSE;
}

LRESULT CALLBACK CommonFrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=lpfnFrameProc(hWnd, uMsg, wParam, lParam);

  if (lpfnFrameProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnFrameProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK FrameProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  FRAMEDATA *lpFrame;

  if (uMsg == WM_CREATE)
  {
    //CREATESTRUCT *cs=(CREATESTRUCT *)lParam;
    //MDICREATESTRUCT *mcs=(MDICREATESTRUCT *)cs->lpCreateParams;
    //lpFrame=(FRAMEDATA *)mcs->lParam

    if (lpFrame=CreateFrameData(hWnd, lpFrameCurrent))
    {
      SetWindowLongWide(hWnd, GWL_USERDATA, (LONG)lpFrame);
      CreateEditWindow(lpFrame, NULL, &lpFrame->ei.hWndEdit, &lpFrame->hDataEdit);
      SendMessage(hMainWnd, AKDN_EDIT_ONSTART, (WPARAM)lpFrame->ei.hWndEdit, (LPARAM)lpFrame->hDataEdit);

      AddTabItem(hTab, lpFrame->hIcon, (LPARAM)lpFrame);
      SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)lpFrame->hIcon);
    }
  }
  else if (uMsg == WM_SIZE)
  {
    if (wParam != SIZE_MINIMIZED)
    {
      FRAMEDATA *lpFrame;

      if (lpFrame=(FRAMEDATA *)GetWindowLongWide(hWnd, GWL_USERDATA))
      {
        ResizeEdit(lpFrame, 0, 0, LOWORD(lParam), HIWORD(lParam), FALSE);
      }
    }
  }
  else if (uMsg == WM_MDIACTIVATE)
  {
    int nMessages[]={IDM_FILE_REOPEN,
                     IDM_FILE_SAVE,
                     IDM_FILE_SAVEAS,
                     IDM_FILE_SAVEALL,
                     IDM_FILE_SAVEALLAS,
                     IDM_FILE_PAGESETUP,
                     IDM_FILE_PRINT,
                     IDM_FILE_PRINTPREVIEW,
                     IDM_EDIT_UNDO,
                     IDM_EDIT_REDO,
                     IDM_EDIT_CUT,
                     IDM_EDIT_COPY,
                     IDM_EDIT_PASTE,
                     IDM_EDIT_CLEAR,
                     IDM_EDIT_SELECTALL,
                     IDM_EDIT_FIND,
                     IDM_EDIT_FINDNEXTDOWN,
                     IDM_EDIT_FINDNEXTUP,
                     IDM_EDIT_REPLACE,
                     IDM_EDIT_GOTOLINE,
                     IDM_EDIT_INSERTDATE,
                     IDM_EDIT_INSERT_TAB_MENU,
                     IDM_EDIT_DELETE_TAB_MENU,
                     IDM_EDIT_INSERT_SPACE_MENU,
                     IDM_EDIT_DELETE_SPACE_MENU,
                     IDM_EDIT_DELETE_FIRST_CHAR_MENU,
                     IDM_EDIT_DELETE_TRAILING_WHITESPACES,
                     IDM_EDIT_UPPERCASE,
                     IDM_EDIT_LOWERCASE,
                     IDM_EDIT_SENTENCECASE,
                     IDM_EDIT_TITLECASE,
                     IDM_EDIT_INVERTCASE,
                     IDM_EDIT_RECODE,
                     IDM_EDIT_NEWLINE_WIN,
                     IDM_EDIT_NEWLINE_UNIX,
                     IDM_EDIT_NEWLINE_MAC,
                     IDM_VIEW_FONT,
                     IDM_VIEW_COLORS,
                     IDM_VIEW_WORDWRAP,
                     IDM_VIEW_SPLIT_WINDOW_ALL,
                     IDM_WINDOW_TILEHORIZONTAL,
                     IDM_WINDOW_TILEVERTICAL,
                     IDM_WINDOW_CASCADE,
                     0};
    int i;

    if (!lParam)
    {
      for (i=0; nMessages[i]; ++i)
        EnableMenuItem(hMainMenu, nMessages[i], MF_GRAYED);
      bMdiNoWindows=TRUE;

      //Clear status
      ssStatus.bModified=-1;
      ssStatus.bOvertypeMode=-1;
      ssStatus.nNewLine=-1;
      ssStatus.nCodePage=-1;
      ssStatus.bBOM=-1;
      for (i=0; i < STATUS_PARTS; ++i)
        StatusBar_SetTextWide(hStatus, i, L"");

      SendMessage(hMainWnd, AKDN_FRAME_NOWINDOWS, 0, 0);
    }
    else
    {
      if (bMdiNoWindows)
      {
        for (i=0; nMessages[i]; ++i)
          EnableMenuItem(hMainMenu, nMessages[i], MF_ENABLED);
        bMdiNoWindows=FALSE;

        SetNewLineStatus(NULL, nDefaultNewLine, 0);
        SetOvertypeStatus(NULL, FALSE);
        SetModifyStatus(NULL, FALSE);
        SetCodePageStatus(NULL, nDefaultCodePage, bDefaultBOM);
      }

      //Variants:
      //A. MDI window just created - MDI client has now one window. WM_MDIACTIVATE sended 1 time:
      //  1. On enter lpFrameCurrent == &fdInit, wParam == NULL, lParam != NULL
      //B. Another MDI window created or switching between MDI windows. WM_MDIACTIVATE sended 2 times:
      //  1. On enter lpFrameCurrent == wParam, wParam != NULL, lParam != NULL
      //  2. On leave lpFrameCurrent == lParam, wParam != NULL, lParam != NULL
      //C. Active MDI window just received WM_MDIDESTROY - MDI client has still one or more windows. WM_MDIACTIVATE sended 2 times:
      //  1. On enter lpFrameCurrent == &fdInit, wParam != NULL, lParam != NULL
      //  2. On leave lpFrameCurrent == lParam, wParam != NULL, lParam != NULL
      //D. Active MDI window just received WM_MDIDESTROY - MDI client has no more windows. WM_MDIACTIVATE not sended.

      if (hWnd == (HWND)wParam)
      {
        //Save deactivated MDI window settings
        if (lpFrame=(FRAMEDATA *)GetWindowLongWide((HWND)wParam, GWL_USERDATA))
          SaveFrameData(lpFrame);
      }
      else if (hWnd == (HWND)lParam)
      {
        //Change current frame handle in WM_MDIACTIVATE because WM_SETFOCUS can be uncalled if WM_MDINEXT used.
        if (lpFrameCurrent->hWndEditParent != (HWND)lParam)
          if (lpFrame=(FRAMEDATA *)GetWindowLongWide((HWND)lParam, GWL_USERDATA))
            lpFrameCurrent=lpFrame;

        //Restore activated MDI window settings
        RestoreFrameData(lpFrameCurrent);

        SendMessage(hMainWnd, AKDN_FRAME_ACTIVATE, 0, (LPARAM)lpFrameCurrent->hWndEditParent);
      }
    }
  }
  else if (uMsg == WM_SETFOCUS)
  {
    //Change current frame handle in WM_SETFOCUS because WM_MDIACTIVATE sended later.
    if (lpFrame=(FRAMEDATA *)GetWindowLongWide(hWnd, GWL_USERDATA))
      lpFrameCurrent=lpFrame;
  }

  //Special messages
  {
    LRESULT lResult;

    if (lResult=EditParentMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  if (bOldWindows)
    return DefMDIChildProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefMDIChildProcW(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CommonEditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  lResult=lpfnEditProc(hWnd, uMsg, wParam, lParam);

  if (lpfnEditProcRet)
  {
    CWPRETSTRUCT cwprs;

    cwprs.lResult=lResult;
    cwprs.lParam=lParam;
    cwprs.wParam=wParam;
    cwprs.message=uMsg;
    cwprs.hwnd=hWnd;
    lpfnEditProcRet(&cwprs);
  }
  return lResult;
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_SETFOCUS)
  {
    if (nMDI == WMD_MDI && !IsEditActive(hWnd))
    {
      FRAMEDATA *lpFrame;

      if (lpFrame=GetFrameDataFromEdit(hWnd))
        ActivateMdiFrameWindow(lpFrame, TRUE);
    }
    else SetSelectionStatus(lpFrameCurrent->ei.hWndEdit, NULL, NULL);

    //Assign current window. Need for split windows.
    lpFrameCurrent->ei.hWndEdit=hWnd;
  }
  else if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_RETURN)
    {
      if (!lpFrameCurrent->ei.bReadOnly)
      {
        if (bKeepSpace)
        {
          if (GetKeyState(VK_MENU) >= 0 &&
              GetKeyState(VK_SHIFT) >= 0 &&
              GetKeyState(VK_CONTROL) >= 0)
          {
            if (AutoIndent(hWnd, &crSel))
              return TRUE;
          }
        }
      }
    }
  }
  else if (uMsg == WM_MOUSEWHEEL)
  {
    if (LOWORD(wParam) == MK_CONTROL)
    {
      if ((short)HIWORD(wParam) < 0)
        DoViewFontSize(hWnd, DECREASE_FONT);
      else
        DoViewFontSize(hWnd, INCREASE_FONT);
      bEditFontChanged=TRUE;
      return TRUE;
    }
  }

  //Clone messages
  {
    LRESULT lResult;

    if (lResult=CloneDragAndDropMessages(hWnd, uMsg, wParam, lParam))
      return lResult;
  }

  if (bOldWindows)
    return CallWindowProcA(OldEditProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldEditProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CloneDragAndDropMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static POINT ptMouseDown;
  static int nMouseMove;
  static BOOL bMouseDown=FALSE;
  LRESULT lResult=0;

  if (uMsg == WM_SETCURSOR)
  {
    if (lpFrameCurrent->ei.hWndMaster && LOWORD(lParam) == HTBORDER)
    {
      POINT pt;
      RECT rc;
      DWORD dwSide=0;

      GetCursorPos(&pt);
      GetWindowRect(hWnd, &rc);
      if (pt.x >= rc.left && pt.x <= rc.left + 2)
        dwSide|=SIDE_LEFT;
      if (pt.x >= rc.right - 2 && pt.x <= rc.right)
        dwSide|=SIDE_RIGHT;
      if (pt.y >= rc.top && pt.y <= rc.top + 2)
        dwSide|=SIDE_TOP;
      if (pt.y >= rc.bottom - 2 && pt.y <= rc.bottom)
        dwSide|=SIDE_BOTTOM;

      if ((hWnd == lpFrameCurrent->ei.hWndMaster && dwSide == SIDE_RIGHT && lpFrameCurrent->ei.hWndClone1) ||
          (hWnd == lpFrameCurrent->ei.hWndClone1 && dwSide == SIDE_LEFT && lpFrameCurrent->ei.hWndMaster) ||
          (hWnd == lpFrameCurrent->ei.hWndClone2 && dwSide == SIDE_RIGHT && lpFrameCurrent->ei.hWndClone3) ||
          (hWnd == lpFrameCurrent->ei.hWndClone3 && dwSide == SIDE_LEFT && lpFrameCurrent->ei.hWndClone2))
      {
        SetCursor(hCursorSizeWE);
        hCursorClone=hCursorSizeWE;
        return TRUE;
      }
      else if ((hWnd == lpFrameCurrent->ei.hWndMaster && dwSide == SIDE_BOTTOM && lpFrameCurrent->ei.hWndClone2) ||
               (hWnd == lpFrameCurrent->ei.hWndClone1 && dwSide == SIDE_BOTTOM && lpFrameCurrent->ei.hWndClone3) ||
               (hWnd == lpFrameCurrent->ei.hWndClone2 && dwSide == SIDE_TOP && lpFrameCurrent->ei.hWndMaster) ||
               (hWnd == lpFrameCurrent->ei.hWndClone3 && dwSide == SIDE_TOP && lpFrameCurrent->ei.hWndClone1))
      {
        SetCursor(hCursorSizeNS);
        hCursorClone=hCursorSizeNS;
        return TRUE;
      }
      else if (lpFrameCurrent->ei.hWndMaster && lpFrameCurrent->ei.hWndClone1 && lpFrameCurrent->ei.hWndClone2 && lpFrameCurrent->ei.hWndClone3 &&
               ((hWnd == lpFrameCurrent->ei.hWndMaster && (dwSide & SIDE_RIGHT) && (dwSide & SIDE_BOTTOM)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone1 && (dwSide & SIDE_LEFT) && (dwSide & SIDE_BOTTOM)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone2 && (dwSide & SIDE_RIGHT) && (dwSide & SIDE_TOP)) ||
                (hWnd == lpFrameCurrent->ei.hWndClone3 && (dwSide & SIDE_LEFT) && (dwSide & SIDE_TOP))))
      {
        SetCursor(hCursorSizeALL);
        hCursorClone=hCursorSizeALL;
        return TRUE;
      }
    }
    hCursorClone=NULL;
  }
  else if (uMsg == WM_LBUTTONDBLCLK ||
           uMsg == WM_NCLBUTTONDBLCLK)
  {
    if (hCursorClone)
    {
      if (hCursorClone == hCursorSizeWE || hCursorClone == hCursorSizeALL)
        SplitDestroy(lpFrameCurrent, CN_CLONE1|CN_CLONE3);
      if (hCursorClone == hCursorSizeNS || hCursorClone == hCursorSizeALL)
        SplitDestroy(lpFrameCurrent, CN_CLONE2|CN_CLONE3);
      SetFocus(lpFrameCurrent->ei.hWndEdit);

      if (lpFrameCurrent->ei.hWndClone1 || lpFrameCurrent->ei.hWndClone2 || lpFrameCurrent->ei.hWndClone3)
      {
        UpdateShowHScroll(lpFrameCurrent);
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, FALSE);
      }
      else DoViewSplitWindow(FALSE, 0);

      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONDOWN ||
           uMsg == WM_NCLBUTTONDOWN)
  {
    if (!bMouseDown)
    {
      if (hCursorClone)
      {
        GetCursorPos(&ptMouseDown);
        bMouseDown=TRUE;
        nMouseMove=1;
        SetCapture(hWnd);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (bMouseDown)
    {
      RECT rcMasterInitial=lpFrameCurrent->rcMasterWindow;
      POINT ptPos;

      if (nMouseMove > 0)
      {
        if (--nMouseMove == 0)
        {
          ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, TRUE);
        }
      }
      else
      {
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, TRUE);
        GetCursorPos(&ptPos);
        if (hCursorClone == hCursorSizeWE || hCursorClone == hCursorSizeALL)
          lpFrameCurrent->rcMasterWindow.right+=(ptPos.x - ptMouseDown.x);
        if (hCursorClone == hCursorSizeNS || hCursorClone == hCursorSizeALL)
          lpFrameCurrent->rcMasterWindow.bottom+=(ptPos.y - ptMouseDown.y);
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, TRUE);

        ptMouseDown.x+=(lpFrameCurrent->rcMasterWindow.right - rcMasterInitial.right);
        ptMouseDown.y+=(lpFrameCurrent->rcMasterWindow.bottom - rcMasterInitial.bottom);
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONUP)
  {
    if (bMouseDown)
    {
      bMouseDown=FALSE;
      ReleaseCapture();

      if (nMouseMove == 0)
      {
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, TRUE);
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, FALSE);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_CAPTURECHANGED)
  {
    if (bMouseDown)
    {
      bMouseDown=FALSE;
      ReleaseCapture();

      if (nMouseMove == 0)
      {
        ResizeEdit(lpFrameCurrent, lpFrameCurrent->rcEditWindow.left, lpFrameCurrent->rcEditWindow.top, lpFrameCurrent->rcEditWindow.right, lpFrameCurrent->rcEditWindow.bottom, TRUE);
      }
    }
  }
  return lResult;
}

LRESULT CALLBACK NewMdiClientProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_DROPFILES)
  {
    DropFiles((HDROP)wParam);
    return TRUE;
  }
  else if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MBUTTONDOWN)
  {
    CreateMdiFrameWindow(NULL);
  }
  else if (uMsg == WM_MDINEXT)
  {
    if (dwTabOptionsMDI & TAB_SWITCH_RIGHTLEFT)
    {
      NextMdiFrameWindow(lpFrameCurrent, lParam);
      return TRUE;
    }
  }
  else if (uMsg == WM_MDIACTIVATE)
  {
    //Remove blinking frame windows effect (from Mark editor)
    if (lpFrameCurrent->hWndEditParent)
    {
      SendMessage(hWnd, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);

      if (bMdiMaximize)
      {
        LRESULT lResult;

        SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
        if (bOldWindows)
          lResult=CallWindowProcA(OldMdiClientProc, hWnd, uMsg, wParam, lParam);
        else
          lResult=CallWindowProcW(OldMdiClientProc, hWnd, uMsg, wParam, lParam);
        SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
        InvalidateRect((HWND)wParam, NULL, TRUE);
        return lResult;
      }
    }
  }
  else if (uMsg == WM_MDIDESTROY)
  {
    //WM_MDIDESTROY should not be called directly. Use DestroyMdiFrameWindow function.
    if ((HWND)wParam)
    {
      FRAMEDATA *lpFrame;
      int nTabItem;

      if (lpFrame=(FRAMEDATA *)GetWindowLongWide((HWND)wParam, GWL_USERDATA))
      {
        //Activate frame
        ActivateMdiFrameWindow(lpFrame, TRUE);

        //Ask if document unsaved
        if (!DoFileExit()) return TRUE;

        //Save closed frame settings
        if (bMdiClientRedraw) CopyFrameData(&fdLast, lpFrame);

        if ((nTabItem=GetTabItemFromParam(hTab, (LPARAM)lpFrame)) != -1)
        {
          //Avoid program exit blinking on last frame close
          if (!bMdiClientRedraw)
          {
            if (SendMessage(hTab, TCM_GETITEMCOUNT, 0, 0) == 1)
              SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);
          }

          //Remove tab item
          DeleteTabItem(hTab, nTabItem);

          //Get frame window maximize state
          SendMessage(hMdiClient, WM_MDIGETACTIVE, 0, (LPARAM)&bMdiMaximize);

          //Destroy edits
          SplitDestroy(lpFrame, CN_CLONE1|CN_CLONE2|CN_CLONE3);
          SendMessage(hMainWnd, AKDN_EDIT_ONFINISH, (WPARAM)lpFrame->ei.hWndEdit, 0);
          DestroyWindow(lpFrame->ei.hWndEdit);
          lpFrame->ei.hWndEdit=NULL;
          lpFrame->hDataEdit=NULL;

          //Delete frame data
          SetWindowLongWide((HWND)wParam, GWL_USERDATA, (LONG)0);
          StackFrameDelete(&hFramesStack, lpFrame);
        }
      }
    }
  }

  if (bOldWindows)
    return CallWindowProcA(OldMdiClientProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldMdiClientProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewTabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static int nDragItem;
  static int nDropItem;
  static int nMouseMove;
  static BOOL bMouseDown=FALSE;

  if (uMsg == WM_CONTEXTMENU)
  {
    if (lParam != -1)
    {
      NCONTEXTMENU ncm;
      TCHITTESTINFO thti;
      POINT pt;
      HMENU hMdiChildMenu;
      int nItem;
      int i;

      GetCursorPos(&pt);
      thti.pt=pt;
      ScreenToClient(hWnd, &thti.pt);
      nItem=SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);

      if (nItem != -1)
      {
        SelectTabItem(hWnd, nItem);

        ncm.hWnd=hWnd;
        ncm.uType=NCM_TAB;
        ncm.pt=pt;
        ncm.bProcess=TRUE;
        SendMessage(hMainWnd, AKDN_CONTEXTMENU, 0, (LPARAM)&ncm);
        if (ncm.bProcess)
        {
          hMdiChildMenu=GetSystemMenu(lpFrameCurrent->hWndEditParent, FALSE);
          if (i=TrackPopupMenu(hMdiChildMenu, TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, 0, lpFrameCurrent->hWndEditParent, NULL))
            PostMessage(lpFrameCurrent->hWndEditParent, WM_SYSCOMMAND, i, 0);
        }
        return TRUE;
      }
    }
    return TRUE;
  }
  else if (uMsg == WM_MBUTTONDOWN)
  {
    TCHITTESTINFO thti;
    int nItem;

    thti.pt.x=LOWORD(lParam);
    thti.pt.y=HIWORD(lParam);
    nItem=SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);

    if (nItem != -1)
    {
      SelectTabItem(hWnd, nItem);
      DestroyMdiFrameWindow(lpFrameCurrent, nItem);
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONDBLCLK)
  {
    DestroyMdiFrameWindow(lpFrameCurrent, -1);
    return TRUE;
  }
  else if (uMsg == WM_TIMER)
  {
    if (wParam == TIMERID_TABOPEN)
    {
      if (dwTabOpenTimer)
      {
        TCITEMW tcItemW;
        FRAMEDATA *lpFrame;
        int nItem;

        //Remove timer
        KillTimer(hTab, dwTabOpenTimer);
        dwTabOpenTimer=0;
        nTabOpenItem=-1;

        //Activate tab
        if ((nItem=GetTabItemFromCursorPos(hTab)) != -1)
        {
          SelectTabItem(hTab, nItem);

          if (nMDI == WMD_MDI)
          {
            //Restore minimized frame
            tcItemW.mask=TCIF_PARAM;
            TabCtrl_GetItemWide(hTab, nItem, &tcItemW);
            lpFrame=(FRAMEDATA *)tcItemW.lParam;

            if (GetWindowLongWide(lpFrame->hWndEditParent, GWL_STYLE) & WS_MINIMIZE)
            {
              SendMessage(hMdiClient, WM_MDIRESTORE, (WPARAM)lpFrame->hWndEditParent, 0);
            }
          }
        }
      }
    }
    return 0;
  }

  //Drag'n'Drop
  if (uMsg == WM_LBUTTONDOWN)
  {
    if (!bMouseDown)
    {
      TCHITTESTINFO thti;

      thti.pt.x=LOWORD(lParam);
      thti.pt.y=HIWORD(lParam);
      nDragItem=SendMessage(hWnd, TCM_HITTEST, 0, (LPARAM)&thti);

      if (nDragItem != -1)
      {
        SelectTabItem(hWnd, nDragItem);
        bMouseDown=TRUE;
        nMouseMove=4;
        SetCapture(hWnd);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (bMouseDown)
    {
      if (nMouseMove > 0)
      {
        if (--nMouseMove == 0)
        {
          SetCursor(hCursorDragMove);
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_LBUTTONUP)
  {
    if (bMouseDown)
    {
      POINT pt;

      bMouseDown=FALSE;
      ReleaseCapture();

      if (nMouseMove == 0)
      {
        GetCursorPos(&pt);
        nDropItem=GetTabItemForDrop(hWnd, &pt);

        if (nDropItem != -1 && nDropItem != nDragItem)
        {
          MoveTabItem(hWnd, nDragItem, nDropItem);
        }
      }
      return TRUE;
    }
  }
  else if (uMsg == WM_CAPTURECHANGED)
  {
    if (bMouseDown)
    {
      bMouseDown=FALSE;
      ReleaseCapture();
    }
  }

  if (bOldWindows)
    return CallWindowProcA(OldTabProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldTabProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK DockProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult;

  if (lResult=DockMessages(hWnd, uMsg, wParam, lParam))
    return lResult;

  if (bOldWindows)
    return CallWindowProcA(OldDockProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldDockProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK DockMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static MINMAXINFO mmi;
  static RECT rcEdge;
  static RECT rcDrop;
  static DOCK *dkDragSource;
  static DOCK *dkDropTarget;
  static int nMouseEdge;
  static int nMouseDown=0;
  static int nMouseMove;
  static int nDropSide;
  DOCK *dkData;
  POINT pt;
  RECT rc;

  if (uMsg == WM_LBUTTONDOWN ||
      uMsg == WM_MOUSEMOVE ||
      uMsg == WM_LBUTTONUP ||
      uMsg == WM_CAPTURECHANGED)
  {
    if (dkData=StackDockFindWindow(&hDocksStack, hWnd, FALSE))
    {
      if (uMsg == WM_LBUTTONDOWN)
      {
        if (!nMouseDown)
        {
          GetCursorPos(&pt);

          if ((nMouseEdge=GetMouseEdge(hWnd, &pt)) == GetOppEdge(dkData->nSide))
          {
            if (!(dkData->dwFlags & DKF_FIXEDSIZE))
            {
              xmemset(&mmi, 0, sizeof(MINMAXINFO));
              mmi.ptMinTrackSize.x=0;
              mmi.ptMinTrackSize.y=0;
              mmi.ptMaxTrackSize.x=0x0FFFFFFF;
              mmi.ptMaxTrackSize.y=0x0FFFFFFF;
              SendMessage(hMainWnd, AKDN_DOCK_GETMINMAXINFO, (WPARAM)dkData, (LPARAM)&mmi);

              SetEdgeCursor(nMouseEdge);
              GetMovingRect(dkData, &pt, &mmi, &rcEdge);
              DrawMovingRect(&rcEdge);

              nMouseDown=DKC_SIZING;
              SetCapture(hWnd);

              SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONSTART, (WPARAM)dkData, DKC_SIZING);
            }
          }
          else
          {
            if (dkData->dwFlags & DKF_DRAGDROP)
            {
              rc=dkData->rcDragDrop;

              if (ClientToScreenRect(dkData->hWnd, &rc))
              {
                if (PtInRect(&rc, pt))
                {
                  nDropSide=0;
                  dkDragSource=dkData;
                  dkDropTarget=dkData;

                  nMouseDown=DKC_DRAGDROP;
                  nMouseMove=4;
                  SetCapture(hWnd);

                  SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONSTART, (WPARAM)dkData, DKC_DRAGDROP);
                }
              }
            }
          }
        }
      }
      else if (uMsg == WM_MOUSEMOVE)
      {
        GetCursorPos(&pt);

        if (nMouseDown)
        {
          if (nMouseDown == DKC_SIZING)
          {
            DrawMovingRect(&rcEdge);
            GetMovingRect(dkData, &pt, &mmi, &rcEdge);
            DrawMovingRect(&rcEdge);
          }
          else if (nMouseDown == DKC_DRAGDROP)
          {
            if (nMouseMove > 0)
            {
              if (--nMouseMove == 0)
              {
                GetWindowRect(dkDropTarget->hWnd, &rcDrop);
                DrawMovingRect(&rcDrop);
                SetEdgeCursor(0);
              }
            }
            if (nMouseMove == 0)
            {
              DOCK *dkPoint;
              int nSide=0;

              if ((dkPoint=StackDockFromPoint(&hDocksStack, &pt)) && !(dkPoint->dwFlags & DKF_HIDDEN))
              {
                if (dkPoint != dkDropTarget)
                {
                  if ((dkPoint->nSide == DKS_LEFT && !(dkDragSource->dwFlags & DKF_NODROPLEFT)) ||
                      (dkPoint->nSide == DKS_TOP && !(dkDragSource->dwFlags & DKF_NODROPTOP)) ||
                      (dkPoint->nSide == DKS_RIGHT && !(dkDragSource->dwFlags & DKF_NODROPRIGHT)) ||
                      (dkPoint->nSide == DKS_BOTTOM && !(dkDragSource->dwFlags & DKF_NODROPBOTTOM)))
                  {
                    nDropSide=0;
                    dkDropTarget=dkPoint;
                    DrawMovingRect(&rcDrop);
                    GetWindowRect(dkDropTarget->hWnd, &rcDrop);
                    DrawMovingRect(&rcDrop);
                  }
                }
              }
              else
              {
                rc=nsSize.rcCurrent;

                if (ClientToScreenRect(hMainWnd, &rc))
                {
                  if (PtInRect(&rc, pt))
                  {
                    if (pt.x >= rc.left && pt.x <= rc.left + DOCK_MAINMIN_X)
                    {
                      rc.right=rc.left + dkDragSource->rcSize.right;
                      nSide=DKS_LEFT;
                    }
                    else if (pt.y >= rc.top && pt.y <= rc.top + DOCK_MAINMIN_Y)
                    {
                      rc.bottom=rc.top + dkDragSource->rcSize.bottom;
                      nSide=DKS_TOP;
                    }
                    else if (pt.x <= rc.right && pt.x >= rc.right - DOCK_MAINMIN_X)
                    {
                      rc.left=rc.right - dkDragSource->rcSize.right;
                      nSide=DKS_RIGHT;
                    }
                    else if (pt.y <= rc.bottom && pt.y >= rc.bottom - DOCK_MAINMIN_Y)
                    {
                      rc.top=rc.bottom - dkDragSource->rcSize.bottom;
                      nSide=DKS_BOTTOM;
                    }

                    if (nSide && nSide != nDropSide && nSide != dkDragSource->nSide)
                    {
                      if ((nSide == DKS_LEFT && !(dkDragSource->dwFlags & DKF_NODROPLEFT)) ||
                          (nSide == DKS_TOP && !(dkDragSource->dwFlags & DKF_NODROPTOP)) ||
                          (nSide == DKS_RIGHT && !(dkDragSource->dwFlags & DKF_NODROPRIGHT)) ||
                          (nSide == DKS_BOTTOM && !(dkDragSource->dwFlags & DKF_NODROPBOTTOM)))
                      {
                        nDropSide=nSide;
                        dkDropTarget=NULL;
                        DrawMovingRect(&rcDrop);
                        rcDrop=rc;
                        DrawMovingRect(&rcDrop);
                      }
                    }
                  }
                }
              }
            }
          }
        }
        else
        {
          if (!(dkData->dwFlags & DKF_FIXEDSIZE))
          {
            if ((nMouseEdge=GetMouseEdge(hWnd, &pt)) == GetOppEdge(dkData->nSide))
            {
              SetEdgeCursor(nMouseEdge);
            }
          }
        }
      }
      else if (uMsg == WM_LBUTTONUP)
      {
        if (nMouseDown)
        {
          if (nMouseDown == DKC_SIZING)
          {
            nMouseDown=0;
            DrawMovingRect(&rcEdge);
            ReleaseCapture();

            if (ScreenToClientRect(hMainWnd, &rcEdge))
            {
              if (dkData->nSide == DKS_LEFT ||
                  dkData->nSide == DKS_RIGHT)
              {
                dkData->rcSize.right=rcEdge.right;
              }
              else if (dkData->nSide == DKS_TOP ||
                       dkData->nSide == DKS_BOTTOM)
              {
                dkData->rcSize.bottom=rcEdge.bottom;
              }
              hDocksStack.nSizingSide=dkData->nSide;
              UpdateSize();
            }
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_SIZING);
          }
          else if (nMouseDown == DKC_DRAGDROP)
          {
            if (nMouseMove == 0)
              DrawMovingRect(&rcDrop);
            nMouseDown=0;
            ReleaseCapture();

            if (dkDropTarget != dkDragSource)
            {
              if (dkDropTarget)
                nDropSide=dkDropTarget->nSide;
              rc=rcDrop;

              if (ScreenToClientRect(hMainWnd, &rc))
              {
                if (nDropSide == DKS_LEFT ||
                    nDropSide == DKS_RIGHT)
                {
                  dkDragSource->rcSize.left=rc.left;
                }
                else if (nDropSide == DKS_TOP ||
                         nDropSide == DKS_BOTTOM)
                {
                  dkDragSource->rcSize.top=rc.top;
                }
                DockSetSide(&hDocksStack, dkDragSource, nDropSide);
                hDocksStack.nSizingSide=dkDragSource->nSide;
                UpdateSize();
              }
            }
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_DRAGDROP);
          }
        }
      }
      else if (uMsg == WM_CAPTURECHANGED)
      {
        if (nMouseDown)
        {
          if (nMouseDown == DKC_SIZING)
          {
            nMouseDown=0;
            DrawMovingRect(&rcEdge);
            ReleaseCapture();
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_SIZING);
          }
          else if (nMouseDown == DKC_DRAGDROP)
          {
            if (nMouseMove == 0)
              DrawMovingRect(&rcDrop);
            nMouseDown=0;
            ReleaseCapture();
            SendMessage(hMainWnd, AKDN_DOCK_CAPTURE_ONFINISH, (WPARAM)dkData, DKC_DRAGDROP);
          }
        }
      }
    }
  }
  return 0;
}

LRESULT CALLBACK NewCloseButtonProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static BOOL bMouseDown=FALSE;

  if (uMsg == WM_MOUSEACTIVATE)
  {
    return MA_NOACTIVATE;
  }
  else if (uMsg == WM_SETFOCUS)
  {
    return 0;
  }
  else if (uMsg == BM_SETSTYLE)
  {
    wParam&=~BS_DEFPUSHBUTTON;
  }
  else if (uMsg == WM_LBUTTONDOWN ||
           uMsg == WM_LBUTTONDBLCLK)
  {
    SendMessage(hWnd, BM_SETSTATE, BST_PUSHED, 0);
    SetCapture(hWnd);
    bMouseDown=TRUE;
    return 0;
  }
  else if (uMsg == WM_MOUSEMOVE)
  {
    if (bMouseDown)
    {
      RECT rc;
      POINT pt;

      GetCursorPos(&pt);
      GetWindowRect(hWnd, &rc);

      if (PtInRect(&rc, pt))
        SendMessage(hWnd, BM_SETSTATE, BST_PUSHED, 0);
      else
        SendMessage(hWnd, BM_SETSTATE, BST_UNCHECKED, 0);
    }
  }
  else if (uMsg == WM_LBUTTONUP ||
           uMsg == WM_CAPTURECHANGED)
  {
    if (bMouseDown)
    {
      bMouseDown=FALSE;
      ReleaseCapture();

      if (SendMessage(hWnd, BM_GETSTATE, 0, 0) & BST_PUSHED)
      {
        SendMessage(hWnd, BM_SETSTATE, BST_UNCHECKED, 0);
        PostMessage(GetParent(hWnd), WM_COMMAND, MAKELONG(GetDlgCtrlID(hWnd), 0), 0);
      }
    }
    return 0;
  }

  if (!IsWindowUnicode(hWnd))
    return CallWindowProcA(OldCloseButtonProc, hWnd, uMsg, wParam, lParam);
  else
    return CallWindowProcW(OldCloseButtonProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK NewHotkeyInputProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndToolTip=NULL;
  static TOOLINFOA tiA;
  static TOOLINFOW tiW;
  static HWND hWndLButtonClick=NULL;
  static WORD wInitHotkey;
  LRESULT lResult=0;

  wInitHotkey=(WORD)GetWindowLongWide(hWnd, GWL_USERDATA);

  if (uMsg == HKM_SETHOTKEY)
  {
    wInitHotkey=wParam;
    SetWindowLongWide(hWnd, GWL_USERDATA, wInitHotkey);
  }
  else if (uMsg == WM_LBUTTONDOWN)
  {
    hWndLButtonClick=hWnd;
  }
  else if (uMsg == WM_KILLFOCUS ||
           uMsg == WM_DESTROY)
  {
    if (hWndLButtonClick == hWnd)
      hWndLButtonClick=NULL;

    if (hWndToolTip)
    {
      if (GetFocus() == hWndToolTip)
        PostMessage(hWnd, WM_SETFOCUS, (WPARAM)NULL, 0);
      DestroyWindow(hWndToolTip);
      hWndToolTip=NULL;
    }
  }
  else if (uMsg == WM_GETDLGCODE)
  {
    if (lParam)
    {
      if (((MSG *)lParam)->message == WM_KEYDOWN ||
          ((MSG *)lParam)->message == WM_SYSKEYDOWN)
      {
        int nVk=(int)((MSG *)lParam)->wParam;
        BYTE nMod=0;

        if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
        if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
        if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;

        if (nVk == VK_ESCAPE ||
            nVk == VK_SPACE ||
            nVk == VK_RETURN ||
            nVk == VK_BACK ||
            nVk == VK_DELETE ||
            (nVk == VK_TAB && (nMod & HOTKEYF_CONTROL)) ||
            (nVk == VK_TAB && hWndLButtonClick == hWnd))
        {
          if (wParam == VK_ESCAPE && !(nMod & HOTKEYF_CONTROL) && !(nMod & HOTKEYF_ALT) && !(nMod & HOTKEYF_SHIFT))
          {
            if ((WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0) == wInitHotkey)
            {
              return 0;
            }
          }
          return DLGC_WANTMESSAGE;
        }
      }
    }
  }
  else if (uMsg == WM_KEYDOWN ||
           uMsg == WM_SYSKEYDOWN)
  {
    RECT rcWindow;
    BOOL bOwnKey=FALSE;
    BOOL bHotkeyExist=FALSE;
    BYTE nMod=0;
    WORD wHotkey;

    if (wParam == VK_ESCAPE ||
        wParam == VK_SPACE ||
        wParam == VK_RETURN ||
        wParam == VK_BACK ||
        wParam == VK_DELETE ||
        wParam == VK_TAB)
    {
      if ((lParam >> 24) & 1) nMod|=HOTKEYF_EXT;
      if (GetKeyState(VK_CONTROL) & 0x80) nMod|=HOTKEYF_CONTROL;
      if (GetKeyState(VK_MENU) & 0x80) nMod|=HOTKEYF_ALT;
      if (GetKeyState(VK_SHIFT) & 0x80) nMod|=HOTKEYF_SHIFT;

      if (wParam == VK_ESCAPE && !(nMod & HOTKEYF_CONTROL) && !(nMod & HOTKEYF_ALT) && !(nMod & HOTKEYF_SHIFT))
      {
        if ((WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0) != wInitHotkey)
        {
          //Reset to initial hotkey
          if (!IsWindowUnicode(hWnd))
            CallWindowProcA(OldHotkeyInputProc, hWnd, HKM_SETHOTKEY, wInitHotkey, 0);
          else
            CallWindowProcW(OldHotkeyInputProc, hWnd, HKM_SETHOTKEY, wInitHotkey, 0);
          bOwnKey=TRUE;
        }
      }
      else if (wParam == VK_SPACE ||
               wParam == VK_RETURN ||
               wParam == VK_ESCAPE ||
               (wParam == VK_BACK && ((nMod & HOTKEYF_CONTROL) || (nMod & HOTKEYF_ALT) || (nMod & HOTKEYF_SHIFT))) ||
               (wParam == VK_DELETE && ((nMod & HOTKEYF_CONTROL) || (nMod & HOTKEYF_ALT) || (nMod & HOTKEYF_SHIFT))) ||
               (wParam == VK_TAB && (nMod & HOTKEYF_CONTROL)) ||
               (wParam == VK_TAB && hWndLButtonClick == hWnd))
      {
        if (!IsWindowUnicode(hWnd))
          CallWindowProcA(OldHotkeyInputProc, hWnd, HKM_SETHOTKEY, MAKEWORD(wParam, nMod), 0);
        else
          CallWindowProcW(OldHotkeyInputProc, hWnd, HKM_SETHOTKEY, MAKEWORD(wParam, nMod), 0);
        bOwnKey=TRUE;
      }
    }
    if (!bOwnKey)
    {
      if (!IsWindowUnicode(hWnd))
        lResult=CallWindowProcA(OldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
      else
        lResult=CallWindowProcW(OldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
    }

    //Show tooltip if hotkey already exist
    {
      PLUGINFUNCTION *pfElement=NULL;

      if (wHotkey=(WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0))
      {
        if (wHotkey != wInitHotkey)
        {
          if (pfElement=StackHotkeyFind(&hPluginsStack, wHotkey))
          {
            bHotkeyExist=TRUE;

            if (!hWndToolTip)
            {
              hWndToolTip=CreateWindowExWide(WS_EX_TOPMOST,
                                             L"tooltips_class32",
                                             NULL,
                                             WS_POPUP|TTS_NOPREFIX,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             CW_USEDEFAULT,
                                             hWnd,
                                             NULL,
                                             hInstance,
                                             NULL);

              if (bOldWindows)
              {
                tiA.cbSize=sizeof(TOOLINFOA);
                tiA.uFlags=TTF_ABSOLUTE|TTF_TRACK;
                tiA.hwnd=hWnd;
                tiA.hinst=hInstance;
                tiA.uId=0;
                tiA.lpszText=pfElement->szFunction;
                SendMessage(hWndToolTip, TTM_ADDTOOLA, 0, (LPARAM)&tiA);
              }
              else
              {
                tiW.cbSize=sizeof(TOOLINFOW);
                tiW.uFlags=TTF_ABSOLUTE|TTF_TRACK;
                tiW.hwnd=hWnd;
                tiW.hinst=hInstance;
                tiW.uId=0;
                tiW.lpszText=pfElement->wszFunction;
                SendMessage(hWndToolTip, TTM_ADDTOOLW, 0, (LPARAM)&tiW);
              }
            }
            else
            {
              if (bOldWindows)
              {
                tiA.lpszText=pfElement->szFunction;
                SendMessage(hWndToolTip, TTM_SETTOOLINFOA, 0, (LPARAM)&tiA);
              }
              else
              {
                tiW.lpszText=pfElement->wszFunction;
                SendMessage(hWndToolTip, TTM_SETTOOLINFOW, 0, (LPARAM)&tiW);
              }
            }
            GetWindowRect(hWnd, &rcWindow);
            SendMessage(hWndToolTip, TTM_TRACKPOSITION, 0, MAKELONG(rcWindow.left, rcWindow.top + RectH(&rcWindow)));
            SendMessage(hWndToolTip, TTM_TRACKACTIVATE, TRUE, (LPARAM)&tiW);
          }
        }
      }
      if (!bHotkeyExist)
      {
        if (hWndToolTip) SendMessage(hWndToolTip, TTM_TRACKACTIVATE, FALSE, (LPARAM)&tiW);
      }
    }
    return lResult;
  }

  if (!IsWindowUnicode(hWnd))
    lResult=CallWindowProcA(OldHotkeyInputProc, hWnd, uMsg, wParam, lParam);
  else
    lResult=CallWindowProcW(OldHotkeyInputProc, hWnd, uMsg, wParam, lParam);

  //Draw color rectangle if hotkey already exist
  if (uMsg == WM_PAINT)
  {
    HPEN hPen;
    HPEN hPenOld;
    HDC hDC;
    RECT rcWindow;
    WORD wHotkey;
    BOOL bDrawRect=FALSE;

    if (bOldWindows)
    {
      PLUGINFUNCTION *pfElement=NULL;

      if (wHotkey=(WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0))
        if (wHotkey != wInitHotkey)
          if (pfElement=StackHotkeyFind(&hPluginsStack, wHotkey))
            bDrawRect=TRUE;
    }
    else
    {
      PLUGINFUNCTION *pfElement=NULL;

      if (wHotkey=(WORD)SendMessage(hWnd, HKM_GETHOTKEY, 0, 0))
        if (wHotkey != wInitHotkey)
          if (pfElement=StackHotkeyFind(&hPluginsStack, wHotkey))
            bDrawRect=TRUE;
    }

    if (bDrawRect)
    {
      if (hDC=GetDC(hWnd))
      {
        GetClientRect(hWnd, &rcWindow);
        hPen=CreatePen(PS_SOLID, 0, RGB(0xFF, 0x00, 0x00));
        hPenOld=(HPEN)SelectObject(hDC, hPen);

        MoveToEx(hDC, rcWindow.left, rcWindow.top, NULL);
        LineTo(hDC, rcWindow.left, rcWindow.bottom - 1);
        LineTo(hDC, rcWindow.right - 1, rcWindow.bottom - 1);
        LineTo(hDC, rcWindow.right - 1, rcWindow.top);
        LineTo(hDC, rcWindow.left, rcWindow.top);

        SelectObject(hDC, hPenOld);
        DeleteObject(hPen);
        ReleaseDC(hWnd, hDC);
      }
    }
  }
  return lResult;
}

LRESULT CALLBACK DummyProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (bOldWindows)
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
  else
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
