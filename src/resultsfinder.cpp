#include "resultsfinder.h"
#include "resultscollection.h"

#include <QCoreApplication>

#ifdef Q_OS_WIN
#  include <windows.h>
#  include <QtWin>

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
        if (pwi.dwExStyle & WS_EX_TOOLWINDOW)
            return TRUE;

        // Exclude progman and start button
        WCHAR className[255];
        GetClassNameW(hwnd, className, sizeof(className)/sizeof(className[0]));
        if (wcscmp(className, L"Progman") == 0 || wcscmp(className, L"Button") == 0)
            return TRUE;

        // Exclude self
        qint64 selfPid = QCoreApplication::applicationPid();
        DWORD processId = 0;
        GetWindowThreadProcessId(hwnd, &processId);
        if (processId == selfPid)
            return TRUE;

        wchar_t text[256];
        text[0] = 0;
        GetWindowTextW(hwnd, text, sizeof(text) / sizeof(text[0]));
        if (text[0])
        {
            QPixmap icon;
            LRESULT lr = SendMessage(hwnd, WM_GETICON, ICON_SMALL2, 0);
            if (!lr)
                lr = SendMessage(hwnd, WM_GETICON, ICON_BIG, 0);
            if (!lr)
                lr = GetClassLongPtrW(hwnd, GCL_HICONSM);
            if (!lr)
                lr = GetClassLongPtrW(hwnd, GCL_HICON);
            if (lr)
                icon = QtWin::fromHICON((HICON)lr);

            resultsCollection.addResult((WId)hwnd, QString::fromWCharArray(text), icon);
        }
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
