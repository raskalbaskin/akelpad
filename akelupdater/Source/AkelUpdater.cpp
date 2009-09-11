/*****************************************************************
 *                 AkelUpdater NSIS plugin v1.8                  *
 *                                                               *
 * 2009 Shengalts Aleksander aka Instructor (Shengalts@mail.ru)  *
 *****************************************************************/

#define _WIN32_IE 0x0400
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include "ConvFunc.h"
#include "Resources\Resource.h"


/* Include conversion functions */
#define xatoiA
#define xitoaA
#include "ConvFunc.h"

/* Defines */
#define NSIS_MAX_STRLEN 1024

//Plugins list
#define LVSI_NAME     0
#define LVSI_LATEST   1
#define LVSI_CURRENT  2

//String IDs
#define STRID_PROGRAM  0
#define STRID_PLUGIN   1
#define STRID_LATEST   2
#define STRID_CURRENT  3
#define STRID_MIRROR   4
#define STRID_LANGUAGE 5
#define STRID_SELECT   6
#define STRID_UPDATE   7
#define STRID_CANCEL   8

//DIALOGRESIZE
#define DRS_SIZE  0x1 //Resize control
#define DRS_MOVE  0x2 //Move control
#define DRS_X     0x4 //X value
#define DRS_Y     0x8 //Y value

typedef struct {
  HWND *lpWnd;   //Control window
  DWORD dwType;  //See DRS_* defines
} DIALOGRESIZE;

/* ExDll */
typedef struct _stack_t {
  struct _stack_t *next;
  char text[1];
} stack_t;

stack_t **g_stacktop;
char *g_variables;
unsigned int g_stringsize;

#define EXDLL_INIT()        \
{                           \
  g_stacktop=stacktop;      \
  g_variables=variables;    \
  g_stringsize=string_size; \
}

/* NSIS variables */
enum
{
INST_0,         // $0
INST_1,         // $1
INST_2,         // $2
INST_3,         // $3
INST_4,         // $4
INST_5,         // $5
INST_6,         // $6
INST_7,         // $7
INST_8,         // $8
INST_9,         // $9
INST_R0,        // $R0
INST_R1,        // $R1
INST_R2,        // $R2
INST_R3,        // $R3
INST_R4,        // $R4
INST_R5,        // $R5
INST_R6,        // $R6
INST_R7,        // $R7
INST_R8,        // $R8
INST_R9,        // $R9
INST_CMDLINE,   // $CMDLINE
INST_INSTDIR,   // $INSTDIR
INST_OUTDIR,    // $OUTDIR
INST_EXEDIR,    // $EXEDIR
INST_LANG,      // $LANGUAGE
__INST_LAST
};

/* Global variables */
char szBuf[NSIS_MAX_STRLEN];
char szBuf2[NSIS_MAX_STRLEN];
HINSTANCE hInstanceDLL=NULL;
WORD wLangSystem;
RECT rcInitDialog={0};
RECT rcMainDialog={0};

/* Funtions prototypes and macros */
BOOL CALLBACK SetupDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetCommandLineArgA(char *pCmdLine, char **pArgStart, char **pArgEnd, char **pNextArg);
BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc);
BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcDialog, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
char* GetLangStringA(LANGID wLangID, int nStringID);
char* getuservariable(const int varnum);
void setuservariable(const int varnum, const char *var);
int popinteger();
void pushinteger(int integer);
int popstring(char *str, int len);
void pushstring(const char *str, int len);

/* NSIS functions code */
extern "C" void __declspec(dllexport) List(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
  EXDLL_INIT();
  {
    popstring(szBuf, MAX_PATH);
    wLangSystem=PRIMARYLANGID(GetUserDefaultLangID());
    DialogBoxA(hInstanceDLL, MAKEINTRESOURCEA(IDD_SETUP), hwndParent, (DLGPROC)SetupDlgProcA);
  }
}

extern "C" void __declspec(dllexport) Collapse(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
  EXDLL_INIT();
  {
    HWND hWnd;

    hWnd=(HWND)popinteger();
    MoveWindow(hWnd, 0, 0, 0, 0, TRUE);
  }
}

extern "C" void __declspec(dllexport) ParseAndPush(HWND hwndParent, int string_size, char *variables, stack_t **stacktop)
{
  EXDLL_INIT();
  {
    char *pCmdLine;
    char *pArgStart;
    char *pArgEnd;
    char *pNextArg;
    int nArgLen;

    popstring(szBuf, MAX_PATH);
    pCmdLine=szBuf;

    while (nArgLen=GetCommandLineArgA(pCmdLine, &pArgStart, &pArgEnd, &pNextArg))
    {
      lstrcpynA(szBuf2, pArgStart, nArgLen + 1);
      pushstring(szBuf2, MAX_PATH);
      pCmdLine=pNextArg;
    }
  }
}

BOOL WINAPI DllMain(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved)
{
  hInstanceDLL=(HINSTANCE)hInst;
  return TRUE;
}

BOOL CALLBACK SetupDlgProcA(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWndGroupExe;
  static HWND hWndListExe;
  static HWND hWndLanguage;
  static HWND hWndMirrorLabel;
  static HWND hWndMirror;
  static HWND hWndListDll;
  static HWND hWndListInfo;
  static HWND hWndSeleted;
  static HWND hWndUpdate;
  static HWND hWndCancel;
  static BOOL bSelectAllExe=TRUE;
  static BOOL bSelectAllDll=TRUE;
  static int nAllItemsCount=0;
  static int nSelItemsCount=0;
  static LVITEMA lviA;
  static DIALOGRESIZE drs[]={&hWndGroupExe,    DRS_SIZE|DRS_X,
                             &hWndListExe,     DRS_SIZE|DRS_X,
                             &hWndMirrorLabel, DRS_MOVE|DRS_X,
                             &hWndMirror,      DRS_MOVE|DRS_X,
                             &hWndListDll,     DRS_SIZE|DRS_X|DRS_Y,
                             &hWndListInfo,    DRS_MOVE|DRS_Y,
                             &hWndSeleted,     DRS_MOVE|DRS_X|DRS_Y,
                             &hWndUpdate,      DRS_MOVE|DRS_Y,
                             &hWndCancel,      DRS_MOVE|DRS_X|DRS_Y,
                             0, 0};

  if (uMsg == WM_INITDIALOG)
  {
    char szName[MAX_PATH];
    char szLatestVer[MAX_PATH];
    char szCurrentVer[MAX_PATH];
    char szCompareResult[MAX_PATH];
    HWND hWndList;
    LVCOLUMNA lvcA;
    int nCompareResult;
    int nIndex;

    SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIconA(hInstanceDLL, MAKEINTRESOURCEA(IDI_ICON)));
    hWndGroupExe=GetDlgItem(hDlg, IDC_GROUP_EXE);
    hWndListExe=GetDlgItem(hDlg, IDC_LIST_EXE);
    hWndLanguage=GetDlgItem(hDlg, IDC_LANGUAGE);
    hWndMirrorLabel=GetDlgItem(hDlg, IDC_MIRROR_LABEL);
    hWndMirror=GetDlgItem(hDlg, IDC_MIRROR);
    hWndListDll=GetDlgItem(hDlg, IDC_LIST_DLL);
    hWndListInfo=GetDlgItem(hDlg, IDC_LIST_INFO);
    hWndSeleted=GetDlgItem(hDlg, IDC_SELECTED);
    hWndUpdate=GetDlgItem(hDlg, IDOK);
    hWndCancel=GetDlgItem(hDlg, IDCANCEL);
    SendMessage(hWndListExe, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
    SendMessage(hWndListDll, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

    SetDlgItemTextA(hDlg, IDC_MIRROR_LABEL, GetLangStringA(wLangSystem, STRID_MIRROR));
    SetDlgItemTextA(hDlg, IDC_LANGUAGE_LABEL, GetLangStringA(wLangSystem, STRID_LANGUAGE));
    SetDlgItemTextA(hDlg, IDC_LIST_INFO, GetLangStringA(wLangSystem, STRID_SELECT));
    SetDlgItemTextA(hDlg, IDOK, GetLangStringA(wLangSystem, STRID_UPDATE));
    SetDlgItemTextA(hDlg, IDCANCEL, GetLangStringA(wLangSystem, STRID_CANCEL));

    SendMessageA(hWndMirror, CB_ADDSTRING, 0, (LPARAM)"osdn");
    SendMessageA(hWndMirror, CB_ADDSTRING, 0, (LPARAM)"heanet");
    SendMessageA(hWndMirror, CB_ADDSTRING, 0, (LPARAM)"switch");
    SendMessageA(hWndMirror, CB_ADDSTRING, 0, (LPARAM)"nchc");
    SendMessage(hWndMirror, CB_SETCURSEL, (WPARAM)0, 0);

    SendMessageA(hWndLanguage, CB_ADDSTRING, 0, (LPARAM)"eng");
    SendMessageA(hWndLanguage, CB_ADDSTRING, 0, (LPARAM)"rus");
    if (!lstrcmpA(szBuf, "rus"))
      SendMessage(hWndLanguage, CB_SETCURSEL, (WPARAM)1, 0);
    else
      SendMessage(hWndLanguage, CB_SETCURSEL, (WPARAM)0, 0);

    EnableWindow(hWndMirror, FALSE);
    EnableWindow(hWndUpdate, FALSE);

    //Columns EXE
    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_PROGRAM);
    lvcA.cx=188;
    lvcA.iSubItem=LVSI_NAME;
    SendMessage(hWndListExe, LVM_INSERTCOLUMNA, LVSI_NAME, (LPARAM)&lvcA);

    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_LATEST);
    lvcA.cx=105;
    lvcA.iSubItem=LVSI_LATEST;
    SendMessage(hWndListExe, LVM_INSERTCOLUMNA, LVSI_LATEST, (LPARAM)&lvcA);

    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_CURRENT);
    lvcA.cx=63;
    lvcA.iSubItem=LVSI_CURRENT;
    SendMessage(hWndListExe, LVM_INSERTCOLUMNA, LVSI_CURRENT, (LPARAM)&lvcA);

    //Columns DLL
    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_PLUGIN);
    lvcA.cx=210;
    lvcA.iSubItem=LVSI_NAME;
    SendMessage(hWndListDll, LVM_INSERTCOLUMNA, LVSI_NAME, (LPARAM)&lvcA);

    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_LATEST);
    lvcA.cx=105;
    lvcA.iSubItem=LVSI_LATEST;
    SendMessage(hWndListDll, LVM_INSERTCOLUMNA, LVSI_LATEST, (LPARAM)&lvcA);

    lvcA.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    lvcA.pszText=GetLangStringA(wLangSystem, STRID_CURRENT);
    lvcA.cx=62;
    lvcA.iSubItem=LVSI_CURRENT;
    SendMessage(hWndListDll, LVM_INSERTCOLUMNA, LVSI_CURRENT, (LPARAM)&lvcA);

    //Rows
    while (!popstring(szName, MAX_PATH) &&
           !popstring(szLatestVer, MAX_PATH) &&
           !popstring(szCurrentVer, MAX_PATH) &&
           !popstring(szCompareResult, MAX_PATH))
    {
      if (!memcmp(szName, "AkelPad", lstrlenA("AkelPad")))
        hWndList=hWndListExe;
      else
        hWndList=hWndListDll;
      ++nAllItemsCount;
      nCompareResult=xatoiA(szCompareResult);

      lviA.mask=LVIF_TEXT;
      lviA.pszText=szName;
      lviA.iItem=0;
      lviA.iSubItem=LVSI_NAME;
      nIndex=SendMessage(hWndList, LVM_INSERTITEMA, 0, (LPARAM)&lviA);

      lviA.mask=LVIF_STATE|LVIF_PARAM;
      lviA.iItem=nIndex;
      lviA.iSubItem=LVSI_NAME;
      lviA.state=(((nCompareResult == 1) + 1) << 12);
      lviA.stateMask=LVIS_STATEIMAGEMASK;
      lviA.lParam=nCompareResult;
      SendMessage(hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);

      if (nCompareResult == 1)
      {
        if (hWndList == hWndListExe)
          bSelectAllExe=FALSE;
        else if (hWndList == hWndListDll)
          bSelectAllDll=FALSE;
      }

      lviA.mask=LVIF_TEXT;
      lviA.pszText=szLatestVer;
      lviA.iItem=nIndex;
      lviA.iSubItem=LVSI_LATEST;
      SendMessage(hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);

      lviA.mask=LVIF_TEXT;
      lviA.pszText=szCurrentVer;
      lviA.iItem=nIndex;
      lviA.iSubItem=LVSI_CURRENT;
      SendMessage(hWndList, LVM_SETITEMA, 0, (LPARAM)&lviA);
    }

    wsprintfA(szBuf, "%d / %d", nSelItemsCount, nAllItemsCount);
    SetWindowTextA(hWndSeleted, szBuf);
  }
  else if (uMsg == WM_NOTIFY)
  {
    if (wParam == IDC_LIST_EXE)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_COLUMNCLICK)
      {
        if (((NMLISTVIEW *)lParam)->iSubItem == LVSI_NAME)
        {
          lviA.mask=LVIF_STATE;
          lviA.iItem=0;
          lviA.iSubItem=LVSI_NAME;
          lviA.state=((bSelectAllExe + 1) << 12);
          lviA.stateMask=LVIS_STATEIMAGEMASK;
          SendMessage(hWndListExe, LVM_SETITEMA, 0, (LPARAM)&lviA);

          bSelectAllExe=!bSelectAllExe;
        }
      }
      else if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGING)
      {
        BOOL bNewState;
        BOOL bOldState;

        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            EnableWindow(hWndMirror, bNewState);

            if (bNewState)
            {
              if (!nSelItemsCount)
                EnableWindow(hWndUpdate, TRUE);
              ++nSelItemsCount;
            }
            else
            {
              --nSelItemsCount;
              if (!nSelItemsCount)
                EnableWindow(hWndUpdate, FALSE);
            }
            wsprintfA(szBuf, "%d / %d", nSelItemsCount, nAllItemsCount);
            SetWindowTextA(hWndSeleted, szBuf);
          }
        }
      }
    }
    else if (wParam == IDC_LIST_DLL)
    {
      if (((NMLISTVIEW *)lParam)->hdr.code == LVN_COLUMNCLICK)
      {
        int nIndex=0;

        if (((NMLISTVIEW *)lParam)->iSubItem == LVSI_NAME)
        {
          while (1)
          {
            lviA.mask=LVIF_STATE;
            lviA.iItem=nIndex;
            lviA.iSubItem=LVSI_NAME;
            lviA.state=((bSelectAllDll + 1) << 12);
            lviA.stateMask=LVIS_STATEIMAGEMASK;
            if (!SendMessage(hWndListDll, LVM_SETITEMA, 0, (LPARAM)&lviA)) break;

            ++nIndex;
          }
          bSelectAllDll=!bSelectAllDll;
        }
      }
      else if (((NMLISTVIEW *)lParam)->hdr.code == LVN_ITEMCHANGING)
      {
        BOOL bNewState;
        BOOL bOldState;

        if (((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK)
        {
          bNewState=((((NMLISTVIEW *)lParam)->uNewState & LVIS_STATEIMAGEMASK) >> 12) - 1;
          bOldState=((((NMLISTVIEW *)lParam)->uOldState & LVIS_STATEIMAGEMASK) >> 12) - 1;

          if (bNewState >=0 && bOldState >=0 && bNewState != bOldState)
          {
            if (bNewState)
            {
              if (!nSelItemsCount)
                EnableWindow(hWndUpdate, TRUE);
              ++nSelItemsCount;
            }
            else
            {
              --nSelItemsCount;
              if (!nSelItemsCount)
                EnableWindow(hWndUpdate, FALSE);
            }
            wsprintfA(szBuf, "%d / %d", nSelItemsCount, nAllItemsCount);
            SetWindowTextA(hWndSeleted, szBuf);
          }
        }
      }
    }

    if (wParam == IDC_LIST_EXE ||
        wParam == IDC_LIST_DLL)
    {
      if ((int)((NMHDR *)lParam)->code == NM_CUSTOMDRAW)
      {
        LPNMLVCUSTOMDRAW lplvcd=(LPNMLVCUSTOMDRAW)lParam;
        LRESULT lResult;

        if (lplvcd->nmcd.dwDrawStage == CDDS_PREPAINT)
        {
          lResult=CDRF_NOTIFYITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
        {
          lResult=CDRF_NOTIFYSUBITEMDRAW;
        }
        else if (lplvcd->nmcd.dwDrawStage == (CDDS_SUBITEM|CDDS_ITEMPREPAINT))
        {
          lviA.mask=LVIF_PARAM;
          lviA.iItem=lplvcd->nmcd.dwItemSpec;
          lviA.iSubItem=LVSI_NAME;

          if (SendMessage(((NMLISTVIEW *)lParam)->hdr.hwndFrom, LVM_GETITEMA, 0, (LPARAM)&lviA))
          {
            if (lplvcd->iSubItem == LVSI_NAME ||
               lplvcd->iSubItem == LVSI_LATEST)
            {
              if (lviA.lParam == 3) //Not installed
              {
                lplvcd->clrText=RGB(0xC0, 0xC0, 0xC0);
                lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
              }
              else if (lviA.lParam == 2) //Installed version is higher than on site
              {
                lplvcd->clrText=RGB(0xFF, 0x00, 0x00);
                lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
              }
              else if (lviA.lParam == 1) //New version available on site
              {
                lplvcd->clrText=RGB(0x00, 0x00, 0xFF);
                lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
              }
            }
            else
            {
              lplvcd->clrText=RGB(0x00, 0x00, 0x00);
              lplvcd->clrTextBk=RGB(0xFF, 0xFF, 0xFF);
            }
          }
          lResult=CDRF_DODEFAULT;
        }
        else lResult=CDRF_DODEFAULT;

        SetWindowLongA(hDlg, DWL_MSGRESULT, (LONG)lResult);
        return TRUE;
      }
    }
  }
  else if (uMsg == WM_COMMAND)
  {
    if (LOWORD(wParam) == IDOK)
    {
      char szExeVersion[MAX_PATH]="0";
      char szName[MAX_PATH];
      int nIndex;
      int nCountDLL=0;
      int nSelection;

      // EXE
      for (nIndex=0; 1; ++nIndex)
      {
        lviA.mask=LVIF_STATE;
        lviA.iItem=nIndex;
        lviA.iSubItem=LVSI_NAME;
        lviA.stateMask=LVIS_STATEIMAGEMASK;
        if (!SendMessage(hWndListExe, LVM_GETITEMA, 0, (LPARAM)&lviA)) break;

        if (((lviA.state & LVIS_STATEIMAGEMASK) >> 12) - 1)
        {
          lviA.mask=LVIF_TEXT;
          lviA.pszText=szExeVersion;
          lviA.cchTextMax=MAX_PATH;
          lviA.iItem=nIndex;
          lviA.iSubItem=LVSI_LATEST;
          lviA.stateMask=LVIS_STATEIMAGEMASK;
          SendMessage(hWndListExe, LVM_GETITEMA, 0, (LPARAM)&lviA);
          break;
        }
      }

      // DLL
      for (nIndex=0; 1; ++nIndex)
      {
        lviA.mask=LVIF_TEXT|LVIF_STATE;
        lviA.pszText=szName;
        lviA.cchTextMax=MAX_PATH;
        lviA.iItem=nIndex;
        lviA.iSubItem=LVSI_NAME;
        lviA.stateMask=LVIS_STATEIMAGEMASK;
        if (!SendMessage(hWndListDll, LVM_GETITEMA, 0, (LPARAM)&lviA)) break;

        if (((lviA.state & LVIS_STATEIMAGEMASK) >> 12) - 1)
        {
          pushstring(szName, MAX_PATH);
          ++nCountDLL;
        }
      }
      wsprintfA(szBuf, "%s|%d", szExeVersion, nCountDLL);
      setuservariable(INST_0, szBuf);

      szBuf[0]='\0';
      if ((nSelection=SendMessage(hWndMirror, CB_GETCURSEL, 0, 0)) != CB_ERR)
        SendMessageA(hWndMirror, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)szBuf);
      setuservariable(INST_1, szBuf);

      szBuf[0]='\0';
      if ((nSelection=SendMessage(hWndLanguage, CB_GETCURSEL, 0, 0)) != CB_ERR)
        SendMessageA(hWndLanguage, CB_GETLBTEXT, (WPARAM)nSelection, (LPARAM)szBuf);
      setuservariable(INST_2, szBuf);

      EndDialog(hDlg, 0);
      return TRUE;
    }
    else if (LOWORD(wParam) == IDCANCEL)
    {
      pushstring("0|0", MAX_PATH);

      EndDialog(hDlg, 0);
      return TRUE;
    }
  }

  DialogResizeMessages(&drs[0], &rcMainDialog, hDlg, uMsg, wParam, lParam);

  return FALSE;
}

int GetCommandLineArgA(char *pCmdLine, char **pArgStart, char **pArgEnd, char **pNextArg)
{
  char *pCount;
  char chStopChar;

  while (*pCmdLine == ' ') ++pCmdLine;

  if (*pCmdLine == '\"' || *pCmdLine == '\'' || *pCmdLine == '`')
  {
    //Parse: "param" or 'param' or `param`
    chStopChar=*pCmdLine;
    pCount=++pCmdLine;
    while (*pCount != '\0' && *pCount != chStopChar)
      ++pCount;

    if (pNextArg)
    {
      *pNextArg=pCount;
      if (*pCount == chStopChar)
        ++*pNextArg;
      while (**pNextArg == ' ') ++*pNextArg;
    }
  }
  else
  {
    pCount=pCmdLine;

    //Parse: /O or /O="param" or /O='param' or /O=`param`
    while (1)
    {
      if (*pCount != '\"' && *pCount != '\'' && *pCount != '`')
      {
        while (*pCount != '\0' && *pCount != ' ' && *pCount != '\"' && *pCount != '\'' && *pCount != '`')
          ++pCount;
        if (*pCount == '\0' || *pCount == ' ')
          break;
      }
      chStopChar=*pCount;
      while (*++pCount != '\0' && *pCount != chStopChar);
      if (*pCount == '\0')
        break;
      ++pCount;
    }
    if (pNextArg)
    {
      *pNextArg=pCount;
      while (**pNextArg == ' ') ++*pNextArg;
    }
  }
  if (pArgStart) *pArgStart=pCmdLine;
  if (pArgEnd) *pArgEnd=pCount;
  return pCount - pCmdLine;
}

BOOL GetWindowPos(HWND hWnd, HWND hWndOwner, RECT *rc)
{
  if (GetWindowRect(hWnd, rc))
  {
    rc->right-=rc->left;
    rc->bottom-=rc->top;

    if (hWndOwner)
    {
      if (!ScreenToClient(hWndOwner, (POINT *)&rc->left))
        return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL DialogResizeMessages(DIALOGRESIZE *drs, RECT *rcDialog, HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static RECT rcInitDialog;
  static RECT rcTempDialog;
  static BOOL bRectChanged;

  if (!rcDialog) rcDialog=&rcTempDialog;

  if (uMsg == WM_INITDIALOG)
  {
    RECT rcTemplate;

    GetWindowPos(hDlg, NULL, &rcInitDialog);
    rcTemplate=*rcDialog;
    *rcDialog=rcInitDialog;

    if (rcTemplate.right && rcTemplate.bottom)
    {
      rcTemplate.left=rcInitDialog.left + (rcInitDialog.right - rcTemplate.right) / 2;
      rcTemplate.top=rcInitDialog.top + (rcInitDialog.bottom - rcTemplate.bottom) / 2;
      SetWindowPos(hDlg, 0, rcTemplate.left, rcTemplate.top, rcTemplate.right, rcTemplate.bottom, SWP_NOZORDER);
    }
    bRectChanged=FALSE;
  }
  else if (uMsg == WM_GETMINMAXINFO)
  {
    MINMAXINFO *mmi=(MINMAXINFO *)lParam;

    mmi->ptMinTrackSize.x=rcInitDialog.right;
    mmi->ptMinTrackSize.y=rcInitDialog.bottom;
  }
  else if (uMsg == WM_SIZE)
  {
    if (lParam)
    {
      RECT rcTemplate;
      RECT rcControl;
      DWORD dwFlags;
      POINT pt;
      int i;

      GetWindowPos(hDlg, NULL, &rcTemplate);
      pt.x=rcTemplate.right - rcDialog->right;
      pt.y=rcTemplate.bottom - rcDialog->bottom;
      *rcDialog=rcTemplate;

      for (i=0; drs[i].lpWnd; ++i)
      {
        if (*drs[i].lpWnd)
        {
          dwFlags=0;
          if (drs[i].dwType & DRS_SIZE)
            dwFlags|=SWP_NOMOVE;
          else if (drs[i].dwType & DRS_MOVE)
            dwFlags|=SWP_NOSIZE;
          GetWindowPos(*drs[i].lpWnd, hDlg, &rcControl);
          SetWindowPos(*drs[i].lpWnd, 0, rcControl.left + ((drs[i].dwType & DRS_X)?pt.x:0), rcControl.top + ((drs[i].dwType & DRS_Y)?pt.y:0), rcControl.right + ((drs[i].dwType & DRS_X)?pt.x:0), rcControl.bottom + ((drs[i].dwType & DRS_Y)?pt.y:0), dwFlags|SWP_NOZORDER);
        }
      }
      InvalidateRect(hDlg, NULL, TRUE);
      bRectChanged=TRUE;
    }
  }
  else if (uMsg == WM_PAINT)
  {
    PAINTSTRUCT ps;
    RECT rcGrip;
    HDC hDC;

    if (hDC=BeginPaint(hDlg, &ps))
    {
      GetClientRect(hDlg, &rcGrip);
      rcGrip.left=rcGrip.right - GetSystemMetrics(SM_CXVSCROLL);
      rcGrip.top=rcGrip.bottom - GetSystemMetrics(SM_CYVSCROLL);
      DrawFrameControl(hDC, &rcGrip, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
      EndPaint(hDlg, &ps);
    }
  }
  return bRectChanged;
}

char* GetLangStringA(LANGID wLangID, int nStringID)
{
  if (wLangID == LANG_RUSSIAN)
  {
    if (nStringID == STRID_PROGRAM)
      return "\xCF\xF0\xEE\xE3\xF0\xE0\xEC\xEC\xE0";
    if (nStringID == STRID_PLUGIN)
      return "\xCF\xEB\xE0\xE3\xE8\xED";
    if (nStringID == STRID_LATEST)
      return "\xCF\xEE\xF1\xEB\xE5\xE4\xED\xFF\xFF";
    if (nStringID == STRID_CURRENT)
      return "\xD2\xE5\xEA\xF3\xF9\xE0\xFF";
    if (nStringID == STRID_MIRROR)
      return "\xC7\xE5\xF0\xEA\xE0\xEB\xEE";
    if (nStringID == STRID_LANGUAGE)
      return "\xDF\xE7\xFB\xEA";
    if (nStringID == STRID_SELECT)
      return "\xD3\xF1\xF2\xE0\xED\xEE\xE2\xE8\xF2\xE5\x20\xE3\xE0\xEB\xEE\xF7\xEA\xF3\x20\xE4\xEB\xFF\x20\xE7\xE0\xE3\xF0\xF3\xE7\xEA\xE8\x2E";
    if (nStringID == STRID_UPDATE)
      return "\xCE\xE1\xED\xEE\xE2\xE8\xF2\xFC";
    if (nStringID == STRID_CANCEL)
      return "\xCE\xF2\xEC\xE5\xED\xE0";
  }
  else
  {
    if (nStringID == STRID_PROGRAM)
      return "Program";
    if (nStringID == STRID_PLUGIN)
      return "Function";
    if (nStringID == STRID_LATEST)
      return "Latest";
    if (nStringID == STRID_CURRENT)
      return "Current";
    if (nStringID == STRID_MIRROR)
      return "Mirror";
    if (nStringID == STRID_LANGUAGE)
      return "Language";
    if (nStringID == STRID_SELECT)
      return "Select checkbox for download.";
    if (nStringID == STRID_UPDATE)
      return "Update";
    if (nStringID == STRID_CANCEL)
      return "Cancel";
  }
  return "";
}

char* getuservariable(const int varnum)
{
  if (varnum < 0 || varnum >= __INST_LAST) return NULL;
  return g_variables+varnum*g_stringsize;
}

void setuservariable(const int varnum, const char *var)
{
  if (var != NULL && varnum >= 0 && varnum < __INST_LAST)
    lstrcpyA(g_variables + varnum*g_stringsize, var);
}

int popinteger()
{
  char szInt[32];

  popstring(szInt, 31);
  return xatoiA(szInt);
}

void pushinteger(int integer)
{
  char szInt[32];

  xitoaA(integer, szInt, 0);
  pushstring(szInt, 32);
}

//Function: Removes the element from the top of the NSIS stack and puts it in the buffer
int popstring(char *str, int len)
{
  stack_t *th;

  if (!g_stacktop || !*g_stacktop) return 1;
  th=(*g_stacktop);
  lstrcpyn(str, th->text, len);
  *g_stacktop=th->next;
  GlobalFree((HGLOBAL)th);
  return 0;
}

//Function: Adds an element to the top of the NSIS stack
void pushstring(const char *str, int len)
{
  stack_t *th;

  if (!g_stacktop) return;
  th=(stack_t*)GlobalAlloc(GPTR, sizeof(stack_t) + len);
  lstrcpyn(th->text, str, len);
  th->next=*g_stacktop;
  *g_stacktop=th;
}
