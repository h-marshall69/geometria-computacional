#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <iostream>
#include <math.h>

HINSTANCE hInst;
class Punto {
public:
    double x = 0;
    double y = 0;
};

double productoCruz(const Punto& A, const Punto& B, const Punto& P) {
    return (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
}

bool productoCruz2(const Punto& A, const Punto& B, const Punto& P) {
    double pc = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    return (pc > 0) ? true: false;
}

Punto A, B, P;
Punto X, Y, Z;

long activo;
long activo1;

bool pivot_uno;
bool pivot_dos;
bool segmento_uno;
bool segmento_dos;
bool scroll_master;

static int mouseX = 0;
static int mouseY = 0;


BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


    switch(uMsg)
    {
    case WM_LBUTTONDOWN: {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);
        activo = 1;
        A.x = B.x = xPos;
        A.y = B.y = yPos;
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_LBUTTONUP:
    {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);

        activo = 0;

        segmento_uno = false;
        pivot_dos = false;
        pivot_uno = false;
        scroll_master = false;

        B.x = xPos;
        B.y = yPos;
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_RBUTTONDOWN: {
        if(pivot_uno) {
            pivot_dos = true;
        }
        if(!scroll_master) {
            long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);
            activo1 = 1;
            X.x = Y.x = xPos;
            X.y = Y.y = yPos;
        }
        else {
            //activo1 = 1;
            pivot_uno = true;
        }
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_RBUTTONUP: {
        if(!scroll_master) {
            long xPos = GET_X_LPARAM(lParam);
            long yPos = GET_Y_LPARAM(lParam);

            activo1 = 0;

            Y.x = xPos;
            Y.y = yPos;
        }
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_MOUSEWHEEL:
    {
        scroll_master = true;
        short delta = GET_WHEEL_DELTA_WPARAM(wParam);
        double movementFactor = 0.1; // Ajusta este valor según tu preferencia de movimiento
        // Calcula el vector de dirección entre A y B
        double directionX = B.x - A.x;
        double directionY = B.y - A.y;

        // Normaliza el vector de dirección para mantener el desplazamiento uniforme
        double length = sqrt(directionX * directionX + directionY * directionY);
        directionX /= length;
        directionY /= length;

        if(!segmento_uno) {
            X.x = (A.x + B.x) / 2.0;
            X.y = (A.y + B.y) / 2.0;
            Y.x = (A.x + B.x) / 2.0;
            Y.y = (A.y + B.y) / 2.0;
            segmento_uno = !segmento_uno;
        }
        if(!pivot_uno) {
            X.x += directionX * movementFactor * delta;
            X.y += directionY * movementFactor * delta;
        }

        if(!pivot_dos) {
            Y.x += directionX * movementFactor * delta;
            Y.y += directionY * movementFactor * delta;
        }

        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_MOUSEMOVE:
    {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);


        SetDlgItemInt(hwndDlg,EDITA,(int)xPos,FALSE);
        SetDlgItemInt(hwndDlg,EDITB,(int)yPos,FALSE);

        SetDlgItemInt(hwndDlg,EDITX,(int)xPos,FALSE);
        SetDlgItemInt(hwndDlg,EDITY,(int)yPos,FALSE);



        if(activo == 1) {
            B.x = xPos;
            B.y = yPos;
            InvalidateRect(hwndDlg, NULL, TRUE);
        }
        else {
            P.x = xPos;
            P.y = yPos;
            double val=productoCruz(A, B, P);
            char cadena[50];
            sprintf(cadena, "%.2f", val);
            SetDlgItemText(hwndDlg,POSX,cadena);
        }
        if(activo1 == 1) {
            Y.x = xPos;
            Y.y = yPos;
            InvalidateRect(hwndDlg, NULL, TRUE);
        }
        else {
            Z.x = xPos;
            Z.y = yPos;
            double val=productoCruz(X, Y, Z);
            char cadena[50];
            sprintf(cadena, "%.2f", val);
            SetDlgItemText(hwndDlg,POSY,cadena);
        }

        bool p1 = productoCruz2(A, B, X);
        bool p2 = productoCruz2(A, B, Y);
        bool p3 = productoCruz2(X, Y, A);
        bool p4 = productoCruz2(X, Y, B);
        if((p1 != p2) && (p3 != p4)) {
            char cadena[50] = "Segmentos crusados";
            SetDlgItemText(hwndDlg, RESUL, cadena);
        }else {
            char cadena[50] = "Segmentos no crusados";
            SetDlgItemText(hwndDlg, RESUL, cadena);
        }
    }
    return TRUE;
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);



        // Crear un nuevo pincel con el color deseado
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Color rojo (R, G, B)

        // Seleccionar el pincel en el contexto del dispositivo
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

        MoveToEx(hdc,A.x,A.y,NULL);
        LineTo(hdc,B.x,B.y);

        MoveToEx(hdc, X.x, X.y, NULL);
        LineTo(hdc, Y.x, Y.y);


        Ellipse(hdc, A.x - 10, A.y - 10, A.x + 10, A.y + 10);
        Ellipse(hdc, B.x - 10, B.y - 10, B.x + 10, B.y + 10);

        // Restaurar el pincel original y liberar el pincel creado
        SelectObject(hdc, hOldBrush);
        DeleteObject(hBrush);

        Ellipse(hdc, X.x - 10, X.y - 10, X.x + 10, X.y + 10);
        Ellipse(hdc, Y.x - 10, Y.y - 10, Y.x + 10, Y.y + 10);



        EndPaint(hwndDlg, &ps);
    }
    return TRUE;
    case WM_INITDIALOG:
    {
        activo = 0;
        activo1 = 0;

        segmento_uno = false;
        scroll_master = false;
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
            case IDC_BUTTON: {
            }
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
