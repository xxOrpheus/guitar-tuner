#include <windows.h>
#include <stdio.h>
#include <dos.h>
#include "resource.h"
#include <SDL.h>
#include <SDL_audio.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "tunings.h"
#include "GUI.h"
#include <pthread.h>

// uses code from https://github.com/psycotica0/tone-generator/blob/master/generator.c#L11
// thank you friendo :)

const char g_szClassName[] = "guitarTuner";

WNDCLASSEX wc;
HWND hwnd;
MSG Msg;
BOOL ret;
long currFreq = 0;
int currentString = -1;
int cycleString = -1;
int toneLength = 1000;
BOOL loopPlayback = FALSE;

struct tuning currentTuning;

struct tuning standardTuning =
{
    "E Standard",
    329.63, 246.94, 196, 146.83, 110, 82.41
};

struct tuning eFlatTuning;
struct tuning dropDTuning;

int setTuning(struct tuning tune)
{
    char STRING_6_FREQ[128];
    char STRING_5_FREQ[128];
    char STRING_4_FREQ[128];
    char STRING_3_FREQ[128];
    char STRING_2_FREQ[128];
    char STRING_1_FREQ[128];
    sprintf(STRING_6_FREQ, "%.3f", tune.STRING_6_FREQ);
    sprintf(STRING_5_FREQ, "%.3f", tune.STRING_5_FREQ);
    sprintf(STRING_4_FREQ, "%.3f", tune.STRING_4_FREQ);
    sprintf(STRING_3_FREQ, "%.3f", tune.STRING_3_FREQ);
    sprintf(STRING_2_FREQ, "%.3f", tune.STRING_2_FREQ);
    sprintf(STRING_1_FREQ, "%.3f", tune.STRING_1_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_6, STRING_6_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_5, STRING_5_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_4, STRING_4_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_3, STRING_3_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_2, STRING_2_FREQ);
    SetDlgItemText(hwnd, ID_FREQ_1, STRING_1_FREQ);
    currentTuning = tune;
    return 0;
}

struct tuning getTuning()
{
    return currentTuning;
}

int stringNoToFreq(int stringNo)
{
    switch(stringNo)
    {
    case 1:
        return getTuning().STRING_1_FREQ;
    case 2:
        return getTuning().STRING_2_FREQ;
    case 3:
        return getTuning().STRING_3_FREQ;
    case 4:
        return getTuning().STRING_4_FREQ;
    case 5:
        return getTuning().STRING_5_FREQ;
    case 6:
        return getTuning().STRING_6_FREQ;
    default:
        return 0;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon  = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON));
    wc.hIconSm  = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPICON), IMAGE_ICON, 16, 16, 0);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName =  MAKEINTRESOURCE(IDR_MENU);
    wc.lpszClassName = g_szClassName;

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window register failed", "Fuck dude", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Fucking shitty man it couldnt boot up", "shit the pants", MB_ICONASTERISK | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    eFlatTuning = standardTuning;
    eFlatTuning.name = "Half-step Down";
    eFlatTuning.STRING_1_FREQ /= 1.0595;
    eFlatTuning.STRING_2_FREQ /= 1.0595;
    eFlatTuning.STRING_3_FREQ /= 1.0595;
    eFlatTuning.STRING_4_FREQ /= 1.0595;
    eFlatTuning.STRING_5_FREQ /= 1.0595;
    eFlatTuning.STRING_6_FREQ /= 1.0595;

    dropDTuning = standardTuning;
    dropDTuning.name = "Drop D";
    dropDTuning.STRING_6_FREQ /= 1.0595,

    setTuning(standardTuning);

    while((ret = GetMessage(&Msg, NULL, 0, 0) > 0))
    {
        if(ret == -1)
        {
            return -1;
        }
        if(!IsDialogMessage(hwnd, &Msg))
        {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }

    return Msg.wParam;
}
