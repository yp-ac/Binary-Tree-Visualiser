#include <Windows.h>
#include <gdiplus.h>
#include <stdio.h>
#include <math.h>
#include "headerres.h"

using namespace Gdiplus;

#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib") 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdiplus.lib")

#define TREE_HEIGHT_MULTIPLIER 5

#define EXIT_IF(cond, lpszMsg, lpszCaption)         if((cond))\
                                                    {\
                                                        MessageBox((HWND)NULL,\
                                                                    TEXT(lpszMsg),\
                                                                    TEXT(lpszCaption),\
                                                                    MB_ICONERROR);\
                                                        ExitProcess(EXIT_FAILURE);\
                                                    }

#define SUCCESS             1
#define FAILURE             0
#define LEFT_NODE_EMPTY     -1
#define RIGHT_NODE_EMPTY    -2

struct bst
{
    INT Pos;
    INT Data;
    INT isEmpty;
};

typedef struct bst bst;
typedef bst* PBST;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void addMenuBar(HWND hwnd);
void MyDrawText2(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B, INT SIZE);
void SetBackgroundColor(HWND hWnd, HDC hdc);
int range_rand(int Start, int End);

//GUI FUNCTION DECLARATION
VOID MakeTreeStructure(HDC hdc, PAINTSTRUCT ps, HWND hWnd);
BOOL InsertNode(HWND hWnd, INT Data);

//Global variables
//Graphical Tree Variables
LONG NodeYLenght = 5;
LONG NodeXLenght;
const INT HeightOfNode = 20;
const INT LenghtOfNode = 50;

INT* NodePositionArray = NULL;
PBST inBst = NULL;

BOOL Flag = 1;
BOOL NotToAddFlag = 0;

//Size Variables
INT cxsize;
INT cysize;
INT key = 0;
int NUMBER;
BOOL isStartWindow = 0;
INT ColorChangeR = 255;
INT ColorChangeG = 255;
INT ColorChangeB = 255;


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    static TCHAR szAppName[] = TEXT("Tree Visualizer"); 
    static TCHAR szWindowClassName[] = TEXT("Binary Search Tree Visualizer"); 

    HBRUSH hBrush = NULL; 
    HCURSOR hCursor = NULL; 
    HICON hIcon = NULL; 
    HICON hIconSm = NULL; 
    HWND hWnd = NULL; 

    ATOM bClassRegistrationStatus = 0; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory((void*)&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory((void*)&msg, sizeof(MSG)); 

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    EXIT_IF(hBrush == NULL, "Error in loading brush", "GetStockObject"); 

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW); 
    EXIT_IF(hCursor == NULL, "Error in Loading cursor", "LoadCursor"); 

    hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIcon == NULL, "Error in loading icon", "LoadIcon"); 

    hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIconSm == NULL, "Error in loading icon", "LoadIcon"); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = hBrush; 
    wnd.hCursor = hCursor; 
    wnd.hIcon = hIcon; 
    wnd.hInstance = hInstance; 
    wnd.hIconSm = hIconSm; 
    wnd.lpfnWndProc = WndProc; 
    wnd.lpszClassName = szWindowClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    bClassRegistrationStatus = RegisterClassEx(&wnd); 
    EXIT_IF(bClassRegistrationStatus == 0, "Failed to register a window class",
            "RegisterClassEx"); 
    
    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClassName, szAppName, WS_OVERLAPPEDWINDOW, 
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                            (HWND)NULL, (HMENU)NULL, hInstance, NULL); 
    EXIT_IF(hWnd == NULL, "Failed to create a window in memory", "CreateWindowEx"); 

    ShowWindow(hWnd, SW_MAXIMIZE);
    UpdateWindow(hWnd); 

    while(GetMessage(&msg, (HWND)NULL, 0U, 0U))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (msg.wParam); 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc; 
    PAINTSTRUCT ps; 
    static int cxChar, cyChar, cxCaps; 
    TEXTMETRIC tm;
    int array[] = {25, 20, 10, 5, 12, 22, 36, 30, 28, 40, 38, 48};
    int i;

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd); 
            
            addMenuBar(hwnd);
            isStartWindow = 1;

            GetTextMetrics(hdc, &tm); 
            cxChar = tm.tmAveCharWidth; 
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar/2; 
            cyChar = tm.tmHeight; 

            NodePositionArray = (int*)malloc((LONG)pow((double)2, (double)NodeYLenght) * sizeof(int));
            EXIT_IF(NodePositionArray == NULL, TEXT("Memory Allocation Failed!"), TEXT("malloc()"))

            inBst= (PBST)malloc((LONG)pow((double)2, (double)NodeYLenght) * sizeof(inBst));
            EXIT_IF(inBst == NULL, TEXT("Memory Allocation Failed!"), TEXT("malloc()"))
            memset(inBst, 0, (LONG)pow((double)2, (double)NodeYLenght) * sizeof(inBst));

            ReleaseDC(hwnd, hdc); 
            hdc = NULL;    
            break;

        case WM_SIZE:
            cxsize = LOWORD(lParam);
            cysize = HIWORD(lParam);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            SetBackgroundColor(hwnd, hdc);
            if(isStartWindow == 1)
            {
                MyDrawText2(hdc, hwnd, ps, L"Binary Tree Visualiser", 
                    cxsize/2 - 400, cysize/2, 0, 0, 0, 60);
            }
            MakeTreeStructure(hdc, ps, hwnd);
            EndPaint(hwnd, &ps);
            break;

        case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case ID_ABOUT:
                MessageBox(hwnd, "Binary Tree Visualiser\nLearn how data is Arranged in Binary Search Tree\n\nMade by Yash Thakare", "About", MB_OK);
                break;
            
            case ID_EXIT:
                PostQuitMessage(0);
                break;
            
            case ID_ADD:
                if(key < 32)
                {
                    DialogBox(NULL, MAKEINTRESOURCE(DATAENTRY), hwnd, DlgProc);
                    if(NotToAddFlag != 1)
                    {
                        InsertNode(hwnd, NUMBER);
                        key++;
                        MessageBeep(MB_OK);
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
                else
                {
                    MessageBox(hwnd, "Only Limited Nodes can be inserted", "Error", MB_ICONERROR);
                }
                break;
            
            case ID_DEL:
                MessageBox(hwnd, "This Functionality is yet to implement!", "Delete", MB_OK);
                break;
            
            case ID_DELETE_TREE:
                for(i = 0; i < (LONG)pow((double)2, (double)NodeYLenght); i++)
                {
                    inBst[i].isEmpty = 0;
                    inBst[i].Data = 0;
                    inBst[i].Pos = 0;
                }
                isStartWindow = 1;
                ColorChangeR = 255;
                ColorChangeG = 255;
                ColorChangeB = 255;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            
            case ID_CHANGE_COLOR:
                ColorChangeR = range_rand(0, 255);
                ColorChangeG = range_rand(0, 255);
                ColorChangeB = range_rand(0, 255);
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            
            case ID_DARK_MODE:
                ColorChangeR = 0;
                ColorChangeG = 0;
                ColorChangeB = 0;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
            
            case ID_LIGHT_MODE:
                ColorChangeR = 255;
                ColorChangeG = 255;
                ColorChangeB = 255;
                InvalidateRect(hwnd, NULL, TRUE);
                break;
        }
        break;

        
        case WM_KEYDOWN:
            switch(wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
            }
            break;

        case WM_DESTROY :
            PostQuitMessage (0);
            break;
    }

    return (DefWindowProc(hwnd, uMsg, wParam, lParam));
}

void SetBackgroundColor(HWND hWnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    SetDCBrushColor(hdc, RGB(ColorChangeR, ColorChangeG, ColorChangeB));
    HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);

    FillRect(hdc, &rect, brush);
}

void MyDrawText2(HDC hdc, HWND hWnd, PAINTSTRUCT ps, 
                WCHAR* STRING,INT X, INT Y, INT R, INT G, INT B, INT SIZE)
{
    Graphics graphics(hdc);

   Font myFont(L"Times New Roman", SIZE);
   PointF origin((float)X, (float)Y);
   SolidBrush myBrush(Color(255, R, G, B));

   graphics.DrawString(STRING, -1, &myFont, origin, &myBrush);
}


VOID MakeTreeStructure(HDC hdc, PAINTSTRUCT ps, HWND hWnd)
{
    Graphics graphics(hdc);

    INT ScreenNodeFitX, ScreenNodeFitY;
    LONG i, j;
    INT CurrentBox;
    INT Counter = 1;
    INT ArrayCounter = 0;

    CHAR BUFFER[10];

    if(ColorChangeR <= 10 && ColorChangeG <= 10 && ColorChangeB <= 10)
    {
        DWORD pen_style = PS_SOLID;
        COLORREF col = RGB(255, 255, 255);
        HPEN hPen2 = CreatePen(pen_style, 5, col);
        HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);
    }
    else
    {
        DWORD pen_style = PS_SOLID;
        COLORREF col = RGB(0, 0, 0);
        HPEN hPen2 = CreatePen(pen_style, 5, col);
        HPEN holdPen2 = (HPEN)SelectObject(hdc, hPen2);
    }

    POINT pt[2];

    NodeXLenght = (LONG)pow((double)2, (double)(NodeYLenght - 1));

    if((NodeXLenght * HeightOfNode * 2 + 1 ) > cxsize)
    {
        NodeYLenght--;
        NodeXLenght = (LONG)pow((double)2, (double)(NodeYLenght - 1));
    }

    ScreenNodeFitX = NodeXLenght * 2 + 1;
    ScreenNodeFitY = NodeXLenght * TREE_HEIGHT_MULTIPLIER + 1;

    //Loop for calculating position for each node on screen
    for(i = 0; i < NodeYLenght; i++)
    {
        for(j = 0; j < (LONG)pow((double)2, (double)(i)); j++)
        {
            CurrentBox = (Counter) * (LONG)pow((double)2, (double)(NodeYLenght - i));            
            NodePositionArray[ArrayCounter] = CurrentBox;
            inBst[ArrayCounter].Pos = CurrentBox;
            ArrayCounter++;
            Counter+=2;
        }
        Counter = 1;
    }

    SetDCBrushColor(hdc, RGB(255, 255, 255));
    SelectObject(ps.hdc, GetStockObject(DC_BRUSH));

    ArrayCounter = 0;
    INT ArrayCounter2 = 1;
    INT k;

    for(i = 0; i < NodeYLenght; i++)
    {
        for(j = 0; j < (LONG)pow((double)2, (double)(i)); j++)
        {
            if(inBst[ArrayCounter].isEmpty != 1/*NodePositionArray[ArrayCounter] == 0 || inBst[ArrayCounter].Pos == 0*/)
            {
                ArrayCounter2 += 2;
                ArrayCounter++;
                continue;
            }
            
            if(i != NodeYLenght - 1)
            {
                pt[0] = {NodePositionArray[ArrayCounter] * HeightOfNode + HeightOfNode/2, ((i * TREE_HEIGHT_MULTIPLIER) + 1) * HeightOfNode + HeightOfNode};
                for(k = 0; k < 2; k++)
                {
                    if(inBst[ArrayCounter2 + k].isEmpty != 1/*NodePositionArray[ArrayCounter2 + k] == 0 || inBst[ArrayCounter2 + k].Pos == 0*/)
                    {
                        continue;
                    }
                    pt[1] = {NodePositionArray[ArrayCounter2 + k] * HeightOfNode + HeightOfNode/2, (((i + 1) * TREE_HEIGHT_MULTIPLIER) + 1) * HeightOfNode};
                    Polyline(hdc, pt, 2);
                }
                ArrayCounter2 = ArrayCounter2 + 2;
            }

            Ellipse(hdc, NodePositionArray[ArrayCounter] * HeightOfNode - 25,
                     ((i * TREE_HEIGHT_MULTIPLIER) + 1) * HeightOfNode - 25, 
                     NodePositionArray[ArrayCounter] * HeightOfNode + HeightOfNode + 25, 
                     ((i * TREE_HEIGHT_MULTIPLIER) + 1) * HeightOfNode + HeightOfNode + 25);

            sprintf(BUFFER, "%d", inBst[ArrayCounter].Data);
            TextOut(hdc,  NodePositionArray[ArrayCounter] * HeightOfNode + HeightOfNode/2,
                     ((i * TREE_HEIGHT_MULTIPLIER) + 1) * HeightOfNode + HeightOfNode/2, BUFFER, strlen(BUFFER));
            SetTextAlign(hdc, TA_CENTER);

            ArrayCounter++;
        }
    }

    Flag = 0;  
    isStartWindow = 0; 
}


BOOL InsertNode(HWND hWnd, INT Data)
{
    INT ParentPos = 0;
    INT i;
    
    if(inBst[0].isEmpty == 0)
    {
        inBst[0].Data = Data;
        inBst[0].isEmpty = 1;
        return SUCCESS;
    }

    CHAR BUFFER[256];

    while(1)
    {
        // if(ParentPos * 2 > (INT)pow((double)2, (double)NodeYLenght))
        // {
        //     NodeYLenght++;
        //     NodePositionArray = (int*)realloc(NodePositionArray, (LONG)pow((double)2, (double)NodeYLenght) * sizeof(int));
        //     EXIT_IF(NodePositionArray == NULL, TEXT("Memory Allocation Failed!"), TEXT("malloc()"))

        //     inBst= (PBST)realloc(inBst, (LONG)pow((double)2, (double)NodeYLenght) * sizeof(inBst));
        //     EXIT_IF(inBst == NULL, TEXT("Memory Allocation Failed!"), TEXT("malloc()"))
        // }
        if(Data <= inBst[ParentPos].Data)
        {
            if(inBst[ParentPos * 2 + 1].isEmpty != 1)
            {
                inBst[ParentPos * 2 + 1].Data = Data;
                inBst[ParentPos * 2 + 1].isEmpty = 1;
                break;
            }
            else
                ParentPos = ParentPos * 2 + 1;
        }
        else
        {
            if(inBst[ParentPos * 2 + 2].isEmpty != 1)
            {
                inBst[ParentPos * 2 + 2].Data = Data;
                inBst[ParentPos * 2 + 2].isEmpty = 1;
                break;
            }
            else
                ParentPos = ParentPos * 2 + 2;
        }
    }

    return(SUCCESS);
}

void addMenuBar(HWND hwnd)
{
	HMENU hMenuBar;
	HMENU hMenu;
    HMENU hMenu2;

	hMenuBar = CreateMenu();
	hMenu = CreateMenu();
    hMenu2 = CreateMenu();
    HMENU hMenu3 = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, ID_ABOUT, L"About");
    AppendMenuW(hMenu, MF_STRING, ID_EXIT, L"Exit");
	AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hMenu, L"Application");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hMenu2, L"Edit");
    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hMenu3, L"Color Modes");
    AppendMenuW(hMenu3, MF_STRING, ID_CHANGE_COLOR, L"Change Color");
    AppendMenuW(hMenu3, MF_STRING, ID_DARK_MODE, L"Dark Mode");
    AppendMenuW(hMenu3, MF_STRING, ID_LIGHT_MODE, L"Light Mode");
    AppendMenuW(hMenu2, MF_STRING, ID_ADD, L"Add Node");
    AppendMenuW(hMenu2, MF_STRING, ID_DEL, L"Delete Node");
    AppendMenuW(hMenu2, MF_STRING, ID_DELETE_TREE, L"Delete Tree");

	SetMenu(hwnd, hMenuBar);
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    CHAR BUFFER[3];
    switch(iMsg)
    {
        case WM_INITDIALOG:
            SetFocus(GetDlgItem(hDlg, ID_USERINPUT));
            SendMessage(GetDlgItem(hDlg, ID_USERINPUT), EM_SETLIMITTEXT, 3, 0);
            SetDlgItemText(hDlg, ID_USERINPUT, "Enter 3 Digit Number");
            return(TRUE);

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
                    NotToAddFlag = 0;
                    NUMBER = GetDlgItemInt(hDlg, ID_USERINPUT, NULL, TRUE);
                    EndDialog(hDlg, 0);
                    key++;
                    // sprintf(BUFFER, "%d", NUMBER);
                    // MessageBox(hDlg, BUFFER, BUFFER, MB_OK);
                    break;

                case IDCANCEL:      
                    NotToAddFlag = 1;          
                    EndDialog(hDlg, 0);
                    break;
            }
            return TRUE;

    }
    return FALSE;
}

int range_rand(int Start, int End)
{
    int num;
    num = rand() % End + Start;
    return(num);
}


