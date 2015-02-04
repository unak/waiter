#define UNICODE
#include <windows.h>

static WCHAR const* const MYWNDCLASS = L"waiter";

static HWND
CreateDummyWindow(HINSTANCE inst, const WCHAR* title)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = inst;
    wc.lpszClassName = MYWNDCLASS;
    if (!RegisterClass(&wc)) {
        MessageBox(0, L"cannot register window class", MYWNDCLASS, MB_OK | MB_ICONERROR);
        return 0;
    }

    HWND hWnd = CreateWindow(MYWNDCLASS, title, 0, 0, 0, 0, 0, 0, 0, inst, 0);
    if (!hWnd) {
        MessageBox(0, L"cannot create window", MYWNDCLASS, MB_OK | MB_ICONERROR);
        return 0;
    }

    return hWnd;
}

static HANDLE
SpawnProcess(WCHAR* cmd, int show)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = show;
    if (!CreateProcess(0, cmd, 0, 0, FALSE, CREATE_UNICODE_ENVIRONMENT, 0, 0, &si, &pi)) {
        MessageBox(0, L"cannot spawn process", MYWNDCLASS, MB_OK | MB_ICONERROR);
        return INVALID_HANDLE_VALUE;
    }

    CloseHandle(pi.hThread);
    return pi.hProcess;
}

int
WinMain(HINSTANCE inst, HINSTANCE, LPSTR, int show)
{
    WCHAR*cmd = GetCommandLine();
    bool quote = false;
    while (*cmd) {
        if (!quote && *cmd == L' ') {
            break;
        }
        if (*cmd == L'"') {
            quote = !quote;
        }

        cmd++;
    }
    while (*cmd && *cmd == L' ') {
        cmd++;
    }
    if (!*cmd) {
        MessageBox(0, L"no command specified", MYWNDCLASS, MB_OK | MB_ICONERROR);
        return 1;
    }

    HWND hWnd = CreateDummyWindow(inst, cmd);
    if (!hWnd) {
        return 1;
    }

    HANDLE process = SpawnProcess(cmd, show);
    if (process != INVALID_HANDLE_VALUE) {
        WaitForSingleObject(process, INFINITE);
        CloseHandle(process);
    }

    DestroyWindow(hWnd);

    return process == INVALID_HANDLE_VALUE;
}
