#include "resultsfinder.h"
#include "resultscollection.h"

#ifdef Q_OS_WIN
#  include <windows.h>

static BOOL CALLBACK findWindowsProc(HWND hwnd, LPARAM lParam)
{
    if (!lParam)
        return FALSE;

    ResultsCollection& resultsCollection = *reinterpret_cast<ResultsCollection*>(lParam);

    WINDOWINFO pwi;
    if (GetWindowInfo(hwnd, &pwi))
    {
        if (!IsWindowVisible(hwnd))
            return TRUE;
//        if (pwi.dwExStyle & WS_EX_TOOLWINDOW | pwi.dwExStyle & WS_EX_TRANSPARENT)
//            return TRUE;
        // TODO:  class name "Progman"?  "explorer.exe"

        wchar_t text[256];
        GetWindowTextW(hwnd, text, sizeof(text) / sizeof(text[0]));
        if (text[0])
            resultsCollection.addResult((WId)hwnd, QString::fromWCharArray(text));
    }


    return TRUE;
}

#else
#  error "Not implemented"
#endif

ResultsFinder::ResultsFinder(QObject* parent) :
    QObject(parent)
{
}

void ResultsFinder::findWindows(ResultsCollection& resultsCollection)
{
#ifdef Q_OS_WIN
    ::EnumWindows(findWindowsProc, reinterpret_cast<LPARAM>(&resultsCollection));
#else
#  error "Not implemented"
#endif
}
