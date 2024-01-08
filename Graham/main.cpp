#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>
#include "resource.h"
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

HINSTANCE hInst;

class Punto {
public:
    double x = 0, y = 0;
    Punto() {
        x = 0;
        y = 0;
    }
    Punto(double x, double y) {
        this->x = x;
        this->y = y;
    }
    // Sobrecarga del operador !=
    bool operator!=(const Punto& otro) const {
        return (x != otro.x || y != otro.y);
    }
};

Punto p0;
std::vector<Punto> gh;
std::vector<Punto> aristas;

Punto punto_menor(std::vector<Punto> &puntos) {
    Punto menor = puntos[0];
    for(const auto& it: puntos) {
        if(it.y < menor.y || (it.y == menor.y && it.x < menor.x)) {
            menor = it;
        }
    }
    return menor;
}

Punto nexToTop(std::stack<Punto> &hull ) {
    Punto top = hull.top();
    hull.pop();
    Punto next_top = hull.top();
    hull.push(top);
    return next_top;
}

int productoCruz(const Punto& P, const Punto& Q, const Punto& R) {
    int val = (Q.y - P.y) * (R.x - Q.x) - (Q.x - P.x) * (R.y - Q.y);
    if(val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

int squaredDist(const Punto& p1, const Punto& p2) {
    return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);
}

bool comparar_angulos(const Punto& p1,const Punto& p2) {
    int orient = productoCruz(p0, p1, p2);
    if(orient == 0)
        return squaredDist(p0, p2) >= squaredDist(p0, p1);
    return orient == 2;
}

std::vector<Punto> convexHull(std::vector<Punto> &puntos) {
    int n = puntos.size();
    Punto menor = punto_menor(puntos);
    p0 = menor;

    std::sort(puntos.begin(), puntos.end(), comparar_angulos);

    std::stack<Punto> hull;
    hull.push(puntos[0]);
    hull.push(puntos[1]);
    hull.push(puntos[2]);

    for(int i = 3; i < n; i++) {
        while(productoCruz(nexToTop(hull), hull.top(), puntos[i]) != 2)
            hull.pop();
        hull.push(puntos[i]);
    }

    std::vector<Punto> result;
    while(!hull.empty()) {
        result.push_back(hull.top());
        hull.pop();
    }

    // Lógica para cerrar la envolvente convexa si no está cerrada
    if (result.front() != result.back()) {
        result.push_back(result.front());
    }

    std::reverse(result.begin(), result.end());

    return result;

}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg) {
    case WM_MOUSEMOVE: {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);

        SetDlgItemInt(hwndDlg,EDITA,(int)xPos,FALSE);
        SetDlgItemInt(hwndDlg,EDITB,(int)yPos,FALSE);
    }
    return TRUE;
    case WM_LBUTTONDOWN: {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);
        Punto A(xPos, yPos);
        gh.push_back(A);
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_LBUTTONUP: {
        long xPos = GET_X_LPARAM(lParam);
        long yPos = GET_Y_LPARAM(lParam);
        if(gh.size() < 3) {
            Punto A(xPos, yPos);
            aristas.push_back(A);
        }
        else
            aristas = convexHull(gh);
        InvalidateRect(hwndDlg, NULL, TRUE);
    }
    return TRUE;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwndDlg, &ps);
        for(int i = 1; i < aristas.size(); i++) {
            MoveToEx(hdc, aristas[i - 1].x, aristas[i - 1].y,NULL);
            LineTo(hdc, aristas[i].x, aristas[i].y);
        }
        for(auto it: gh) {
            Ellipse(hdc, it.x - 10, it.y - 10, it.x + 10, it.y + 10);
        }
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
