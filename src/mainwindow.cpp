#include "mainwindow.h"

#include "resultsfinder.h"
#include "resultscollection.h"
#include "simpleview.h"

#include <UGlobalHotkey/uglobalhotkeys.h>
#include <QAction>
#include <QTimer>
#include <QDebug>
#ifdef Q_OS_WIN
#  include <windows.h>
#else
#  error "Not implemented"
#endif

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      resultsFinder(new ResultsFinder(this)),
      resultsCollection(new ResultsCollection(nullptr)),
      view(new SimpleView(*this->resultsCollection, this))
{
    this->setWindowTitle("Window Finder");

    {
        Qt::WindowFlags flags = this->windowFlags();
        flags &= ~Qt::WindowMaximizeButtonHint;
        flags |= Qt::WindowStaysOnTopHint;
        this->setWindowFlags(flags);
    }

    this->setCentralWidget(this->view);
    connect(this->view, SIGNAL(resultSelected(QSharedPointer<Result>)), SLOT(activateResult(QSharedPointer<Result>)));

    this->setWindowFlags(this->windowFlags() & ~Qt::TaskButton & ~Qt::MaxMouseButton);

    UGlobalHotkeys* hotkeyManager = new UGlobalHotkeys(this);
    try
    {
        hotkeyManager->registerHotkey("Win+H");
    }
    catch (UException& e)
    {
        qDebug() << "Unable to register hotkey: " << e.what();
    }

    connect(hotkeyManager, &UGlobalHotkeys::activated, [this](size_t /*id*/)
    {
        this->show();
        this->raise();
        this->activateWindow();

        this->resetResults();
    });

    QAction* hideAction = new QAction(tr("Hide"), this);
    hideAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(hideAction, SIGNAL(triggered()), SLOT(hide()));
    this->addAction(hideAction);

    QMetaObject::connectSlotsByName(this);

    this->resize(640, 400);

    QTimer::singleShot(0, this, SLOT(resetResults()));  // TODO:  remove
}

MainWindow::~MainWindow()
{

}

void MainWindow::resetResults()
{
    this->resultsCollection->resetResults();
    this->resultsFinder->findWindows(*this->resultsCollection);
}

void MainWindow::activateResult(QSharedPointer<Result> result)
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)result->getWinId();
    if (IsWindow(hwnd))
    {
        if (IsIconic(hwnd))
        {
            WINDOWPLACEMENT wndpl;
            if (GetWindowPlacement(hwnd, &wndpl))
            {
                if (wndpl.showCmd == SW_MAXIMIZE || wndpl.showCmd == SW_SHOWMAXIMIZED)
                    PostMessage(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                else
                    PostMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            }
        }
        else
        {
            BringWindowToTop(hwnd);
            SetForegroundWindow(hwnd);
        }
    }
    else
        qDebug() << "Handle is not a window: " << hwnd;
#else
#  error "Not implemented"
#endif

    this->hide();
}
