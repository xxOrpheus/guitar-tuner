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
#include "main.h"
#include "commctrl.h"
#include <pthread.h>

int stringDlgIds[6] = {ID_FREQ_1, ID_FREQ_2, ID_FREQ_3, ID_FREQ_4, ID_FREQ_5, ID_FREQ_6};
int noteBoxDlgIds[6] = {ID_NOTE_1, ID_NOTE_2, ID_NOTE_3, ID_NOTE_4, ID_NOTE_5, ID_NOTE_6};

int getInput(int id)
{
    switch(id)
    {
        case ID_NOTE_1: return ID_FREQ_1;
        case ID_NOTE_2: return ID_FREQ_2;
        case ID_NOTE_3: return ID_FREQ_3;
        case ID_NOTE_4: return ID_FREQ_4;
        case ID_NOTE_5: return ID_FREQ_5;
        case ID_NOTE_6: return ID_FREQ_6;
    }

    return 0;
}

void* processGUI(void *arg)
{
    if(currentString > 0)
    {
        if(loopPlayback)
        {
            while(loopPlayback)
            {
                generateTone(stringNoToFreq(currentString), toneLength);
                Sleep(1000);
            }
        }
        else
        {
            generateTone(stringNoToFreq(currentString), toneLength);
            currentString = -1;
        }
    }
}

// generates a range of notes from 3 1/2 steps up and down, based from standard tuning
int populateNoteTables(HWND hwnd)
{
    tuning tune = standardTuning;
    float* notes = getFrequencies(tune);
    for(int i = 0; i < 6; i++)
    {
        HWND list = GetDlgItem(hwnd, noteBoxDlgIds[i]);
        float currentFreq = notes[i];
        float originalFreq = currentFreq;
        float freq = originalFreq;

        int y; // move up seven half steps (three and 1/2 half steps) (extra half (8) to compensate for original note being counted)
        for(y = 0; y < 8; y++)
        {
            freq = halfStepUp(freq);
        }
        int pos;
        for(y = 0; y < 15; y++)
        {
            freq = halfStepDown(freq);
            char *str[16];
            char name[16];
            notationByFrequency(freq, name, 16);
            sprintf(str, "%s \[%.3f\]", name, freq);
            pos = (int) SendMessage(list, LB_ADDSTRING, 0, (LPARAM) str);
            SendMessage(list, LB_SETITEMDATA, pos, freq*1000);
            //  printf("a %.2f\n", freq);
        }
        printf("%.3f \n", (float)SendMessage(list, LB_GETITEMDATA, pos, NULL)/1000);
        currentFreq = originalFreq;

    }
    return 0;
}

char* getText(HWND hwnd, int ctrlid)
{
    int buflen = GetWindowTextLength(GetDlgItem(hwnd, ctrlid));
    if(buflen > 0)
    {
        int i;
        char* buf;
        buf = (char*)GlobalAlloc(GPTR, buflen + 1);
        GetDlgItemText(hwnd, ctrlid, buf, buflen + 1);
        return buf;
    }
}


float getFrequencyByInput(HWND hwnd, int ctrlid)
{
    char *e;
    double freq;
    char *text;
    text = getText(hwnd, ctrlid);
    freq = strtod(text, &e);
    GlobalFree((HANDLE) text);
    if(*e != '\0')
    {
        printf("bad frequency data\n");
    }

    return (float) freq;
}


INT_PTR CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lparam)
{
    HWND textHwnd;
    HWND hwndTB;
    int i;
    tuning tune;
    switch(uMsg)
    {
    case WM_INITDIALOG:
        hwndTB = GetDlgItem(hwnd, ID_TONE_LENGTH);
        SendMessage(hwndTB, TBM_SETRANGE, TRUE, MAKELONG(1000, 5000));
        populateNoteTables(hwnd);
        break;

    case WM_NCHITTEST:
        ReleaseCapture();
        SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        break;

    case WM_CLOSE:
        if(MessageBox(hwnd, TEXT("Really close?"), TEXT("Close?"), MB_ICONQUESTION | MB_YESNO) == IDYES)
        {
            DestroyWindow(hwnd);
            return TRUE;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_HSCROLL:
        textHwnd = GetDlgItem(hwnd, ID_TIME_MS);
        int ctrlId = GetDlgCtrlID((HWND) lparam);
        int requestId = LOWORD(wParam);
        int pos = SendMessage((HWND) lparam, TBM_GETPOS, 0, 0);
        switch(ctrlId)
        {
        case ID_TONE_LENGTH:
            toneLength = pos;
            char timeMs[32];
            sprintf(timeMs, "%d ms", toneLength);
            SetWindowText(textHwnd, timeMs);
            break;
        }
        break;

    case WM_COMMAND:

        switch(LOWORD(wParam))
        {
        case ID_NOTE_1:
        case ID_NOTE_2:
        case ID_NOTE_3:
        case ID_NOTE_4:
        case ID_NOTE_5:
        case ID_NOTE_6:
            switch(HIWORD(wParam))
            {
            case LBN_SELCHANGE:
                hwndTB = GetDlgItem(hwnd, LOWORD(wParam));
                i = SendMessage(hwndTB, LB_GETCURSEL, 0, 0);
                float frewq = (float) SendMessage(hwndTB, LB_GETITEMDATA, i, 0) / 1000;
                char str[16];
                sprintf(str, "%.3f", frewq);
                SetDlgItemText(hwnd, getInput(LOWORD(wParam)), str);
                break;
            }
            break;
        case ID_PLAY_1:
            currentString = 1;
            break;

        case ID_PLAY_2:
            currentString = 2;
            break;

        case ID_PLAY_3:
            currentString = 3;
            break;

        case ID_PLAY_4:
            currentString = 4;
            break;

        case ID_PLAY_5:
            currentString = 5;
            break;

        case ID_PLAY_6:
            currentString = 6;
            break;

        case ID_CYCLENEXT:
            stopTone();
            cycleString += 1;
            if(cycleString < 1)
            {
                cycleString = 1;
            }
            if(cycleString > 6)
            {
                cycleString = 1;
            }
            currentString = cycleString;
            break;

        case ID_CYCLEPREV:
            stopTone();
            cycleString -= 1;
            if(cycleString < 1)
            {
                cycleString = 6;
            }
            currentString = cycleString;
            break;

        case ID_LOOP_PLAYBACK:
            currentString = -1;
            stopTone();
            if(HIWORD(wParam) == BN_CLICKED)
            {
                if(SendDlgItemMessage(hwnd, ID_LOOP_PLAYBACK, BM_GETCHECK, 0, 0))
                {
                    loopPlayback = TRUE;
                }
                else
                {
                    loopPlayback = FALSE;
                }
            }
            break;

        case ID_STOP:
            currentString = -1;
            loopPlayback = FALSE;
            stopTone();
            break;

        case ID_SET_TUNING:
            currentString = -1;
            loopPlayback = FALSE;
            stopTone();
            tune = getTuning();
            tune.STRING_1_FREQ = getFrequencyByInput(hwnd, ID_FREQ_1);
            tune.STRING_2_FREQ = getFrequencyByInput(hwnd, ID_FREQ_2);
            tune.STRING_3_FREQ = getFrequencyByInput(hwnd, ID_FREQ_3);
            tune.STRING_4_FREQ = getFrequencyByInput(hwnd, ID_FREQ_4);
            tune.STRING_5_FREQ = getFrequencyByInput(hwnd, ID_FREQ_5);
            tune.STRING_6_FREQ = getFrequencyByInput(hwnd, ID_FREQ_6);
            setTuning(tune);
            break;

        case ID_EXIT:
            DialogProc(hwnd, WM_CLOSE, 0, 0);
            break;

        case ID_ABOUT:
            MessageBeep(0);
            MessageBox(hwnd, "Made by David Harris", "Guitar Tuner v 1.0", MB_ICONQUESTION | MB_OK);
            break;
        }

        pthread_t thread1;
        if(pthread_create(&thread1, NULL, processGUI, (void *) NULL))
        {
            fprintf(stderr, "error processing gui");
        }
        break;
    }

    return FALSE;
}
