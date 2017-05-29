/* 
** $Id: createicon.c 741 2009-03-31 07:16:18Z weiym $
**
** Listing 10.1
**
** createicon.c: Sample program for MiniGUI Programming Guide
**      Demo of creating and destroying icon
**
** Copyright (C) 2004 ~ 2007 Feynman Software.
**
** License: GPL
*/

#include <minigui/common.h>
#include <minigui/minigui.h>
#include <minigui/gdi.h>
#include <minigui/window.h>
#include <minigui/control.h>

#ifdef _LANG_ZHCN
#include "createicon_res_cn.h"
#elif defined _LANG_ZHTW
#include "createicon_res_tw.h"
#else
#include "createicon_res_en.h"
#endif

static BYTE ANDmaskIcon[] = {
    0xff, 0x9f, 0x00, 0x00, 
    0xff, 0x1f, 0x00, 0x00, 
    0xfc, 0x1f, 0x00, 0x00, 
    0xf0, 0x1f, 0x00, 0x00, 

    0xe0, 0x0f, 0x00, 0x00, 
    0xc0, 0x07, 0x00, 0x00, 
    0x80, 0x03, 0x00, 0x00, 
    0x80, 0x03, 0x00, 0x00, 

    0x00, 0x01, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 

    0x80, 0x03, 0x00, 0x00, 
    0x80, 0x03, 0x00, 0x00, 
    0xc0, 0x07, 0x00, 0x00, 
    0xf0, 0x1f, 0x00, 0x00
};

static BYTE XORmaskIcon[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x8f, 0xff, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x8f, 0xff, 0xff, 0x00, 0x00, 0x00, 
    0x00, 0x08, 0xff, 0xf8, 0xff, 0xf8, 0x00, 0x00, 
    0x00, 0xff, 0xff, 0x80, 0x8f, 0xff, 0xf0, 0x00, 
    0x00, 0xff, 0xff, 0xf8, 0xff, 0xff, 0xf0, 0x00, 

    0x0f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0x00, 
    0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0x00, 
    0x0f, 0xff, 0xff, 0xf8, 0x00, 0xff, 0xff, 0x00, 
    0x0f, 0xff, 0xf0, 0x0f, 0x00, 0xff, 0xff, 0x00, 
    0x00, 0xff, 0xf8, 0x00, 0x08, 0xff, 0xf0, 0x00, 
    0x00, 0x8f, 0xff, 0x80, 0x8f, 0xff, 0xf0, 0x00, 
    0x00, 0x00, 0x8f, 0xff, 0xff, 0xf0, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    0xff, 0x9f, 0x00, 0x00, 0xff, 0x1f, 0x00, 0x00, 
    0xfc, 0x1f, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 
    0xe0, 0x0f, 0x00, 0x00, 0xc0, 0x07, 0x00, 0x00, 
    0x80, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 
    0x80, 0x03, 0x00, 0x00, 0x80, 0x03, 0x00, 0x00, 
    0xc0, 0x07, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 

    0x26, 0x00, 0x00, 0x00, 0xf4, 0xd9, 0x04, 0x08, 
    0xa8, 0xf8, 0xff, 0xbf, 0xc0, 0xf7, 0xff, 0xbf, 
    0x20, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
    0xc0, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 
    0x28, 0x01, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 
    0x10, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 
    0xf0, 0x10, 0x04, 0x00, 0x70, 0xe1, 0x04, 0x08, 
    0xd8, 0xf8, 0xff, 0xbf, 0x41, 0x90, 0x04, 0x08
};

static int CreateiconWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
    static HICON new_icon;
    HDC hdc;

    switch (message) {
    case MSG_CREATE:
        new_icon = CreateIcon(HDC_SCREEN, 16, 16, ANDmaskIcon, XORmaskIcon, 4);
        break;

    case MSG_PAINT:
        hdc = BeginPaint(hWnd);
        if (new_icon != 0) {
            DrawIcon(hdc, 0, 0, 0, 0, new_icon);
            DrawIcon(hdc, 50, 50, 64, 64, new_icon);
        }
        EndPaint(hWnd, hdc);
        return 0;

    case MSG_CLOSE:
        DestroyIcon(new_icon);
        DestroyMainWindow (hWnd);
        PostQuitMessage (hWnd);
        return 0;
    }

    return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int args, const char* arg[])
{
    MSG Msg;
    HWND hMainWnd;
    MAINWINCREATE CreateInfo;

#ifdef _MGRM_PROCESSES
    JoinLayer(NAME_DEF_LAYER , "createicon" , 0 , 0);
#endif
    
    CreateInfo.dwStyle = WS_VISIBLE | WS_BORDER | WS_CAPTION;
    CreateInfo.dwExStyle = WS_EX_NONE;
    CreateInfo.spCaption = CI_ST_CAP;
    CreateInfo.hMenu = 0;
    CreateInfo.hCursor = GetSystemCursor(0);
    CreateInfo.hIcon = 0;
    CreateInfo.MainWindowProc = CreateiconWinProc;
    CreateInfo.lx = 0;
    CreateInfo.ty = 0;
    CreateInfo.rx = 320;
    CreateInfo.by = 240;
    CreateInfo.iBkColor = COLOR_lightwhite;
    CreateInfo.dwAddData = 0;
    CreateInfo.hHosting = HWND_DESKTOP;
    
    hMainWnd = CreateMainWindow (&CreateInfo);
    
    if (hMainWnd == HWND_INVALID)
        return -1;

    ShowWindow(hMainWnd, SW_SHOWNORMAL);

    while (GetMessage(&Msg, hMainWnd)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MainWindowThreadCleanup (hMainWnd);

    return 0;
}

#ifdef _MGRM_THREADS
#include <minigui/dti.c>
#endif
