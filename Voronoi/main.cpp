#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include "delaunay.h"
#include <iostream>
#include <vector>
#include <utility>
#include <limits>

using namespace delaunay;
HINSTANCE hInst;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static std::vector<Point<float>> voronoi;
    static Delaunay<float> puntosMedios;
    switch(uMsg)
    {
    case WM_LBUTTONDOWN: {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);
        Point<float> A(xPos, yPos);
        voronoi.push_back(A);
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_LBUTTONUP: {
        puntosMedios = triangulate(voronoi);
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_MOUSEMOVE: {
    }
    return TRUE;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);
        for (const auto& it : voronoi)
            Ellipse(hdc, it.x - 10, it.y - 10, it.x + 10, it.y + 10);

        for (const auto& tri : puntosMedios.triangles) {
            // Mover al punto inicial del segmento
            MoveToEx(hdc, tri.p0.x, tri.p0.y, NULL);

            // Dibujar el segmento hasta el punto final
            LineTo(hdc, tri.p1.x, tri.p1.y);

            // Mover al punto inicial del segmento
            MoveToEx(hdc, tri.p1.x, tri.p1.y, NULL);

            // Dibujar el segmento hasta el punto final
            LineTo(hdc, tri.p2.x, tri.p2.y);

            // Mover al punto inicial del segmento
            MoveToEx(hdc, tri.p2.x, tri.p2.y, NULL);

            // Dibujar el segmento hasta el punto final
            LineTo(hdc, tri.p0.x, tri.p0.y);
        }
        for (const auto& it : voronoi)
            Ellipse(hdc, it.x - 10, it.y - 10, it.x + 10, it.y + 10);
        EndPaint(hwndDlg, &ps);
    }
    return TRUE;
    case WM_INITDIALOG:
    {
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
