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

int currentString = -1;
int cycleString = -1;
int toneLength = 1000;
BOOL loopPlayback = FALSE;

tuning currentTuning;

int setTuning(tuning tune)
{
    float freq[] = {tune.STRING_1_FREQ, tune.STRING_2_FREQ, tune.STRING_3_FREQ, tune.STRING_4_FREQ, tune.STRING_5_FREQ, tune.STRING_6_FREQ};
    int freqInput[] = {ID_FREQ_1, ID_FREQ_2, ID_FREQ_3, ID_FREQ_4, ID_FREQ_5, ID_FREQ_6};

    for(int i = 0; i < 6; i++)
    {
        char freqs[64];
        sprintf(freqs, "%.3f", freq[i]);
        SetDlgItemText(hwnd, MAKEINTRESOURCE(freqInput[i]), freqs);
    }

    currentTuning = tune;
    return 0;
}

tuning getTuning()
{
    return currentTuning;
}

long stringNoToFreq(int stringNo)
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

    dropDTuning = standardTuning;
    dropDTuning.name = "Drop D";
    dropDTuning.STRING_6_FREQ /= 1.0595;

    setTuning(eFlatTuning);

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
