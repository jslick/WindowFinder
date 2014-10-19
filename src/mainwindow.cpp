#include "mainwindow.h"
#include "appcommon.h"

#include "aboutdialog.h"
#include "resultsfinder.h"
#include "resultscollection.h"
#include "simpleview.h"

#include <UGlobalHotkey/uglobalhotkeys.h>
#include <QPointer>
#include <QApplication>
#include <QAction>
#include <QTimer>
#include <QDesktopWidget>
#include <QMenu>
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
    this->setWindowTitle(APP_APPNAME);
    this->setWindowIcon(QIcon(":/app/app64.png"));

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

    connect(hotkeyManager, SIGNAL(activated(size_t)), SLOT(bringToFront()));

    QAction* hideAction = new QAction(tr("Hide"), this);
    hideAction->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(hideAction, SIGNAL(triggered()), SLOT(maybeHide()));
    this->addAction(hideAction);

    QMetaObject::connectSlotsByName(this);

    this->resize(640, 640);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());

    QMenu* trayMenu = new QMenu(this);
    trayMenu->addAction(tr("Show"), this, SLOT(bringToFront()));
    trayMenu->addSeparator();
    trayMenu->addAction(tr("About"), this, SLOT(showAbout()));
    trayMenu->addAction(tr("Exit"), this, SLOT(close()));
    trayMenu->setDefaultAction(trayMenu->actions().first());
    QSystemTrayIcon* trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->windowIcon());
    trayIcon->setContextMenu(trayMenu);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            SLOT(handleTray(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    QTimer::singleShot(0, this, SLOT(resetResults()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::bringToFront()
{
    this->show();
    this->raise();
    this->activateWindow();

    this->resetResults();
}

void MainWindow::showAbout()
{
    static QPointer<AboutDialog> dlg;
    if (!dlg)
        dlg = new AboutDialog(this);

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
    dlg->raise();
    dlg->activateWindow();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit closed(event);
}

void MainWindow::resetResults()
{
    this->resultsCollection->resetResults();
    this->resultsFinder->findWindows(*this->resultsCollection);
}

void MainWindow::activateResult(QSharedPointer<Result> result)
{
    this->hide();

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

        // Put in timeout so that PostMessage (if iconic) will happen first
        // (NOTE:  This probably still races, but to a lesser degree)
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, [hwnd,timer]()
        {
            BringWindowToTop(hwnd);
            SetForegroundWindow(hwnd);
            timer->deleteLater();
        });
        timer->start(260);
    }
    else
        qDebug() << "Handle is not a window: " << hwnd;
#else
#  error "Not implemented"
#endif
}

void MainWindow::maybeHide()
{
    if (this->view->escapeRequested())
        this->hide();
}

void MainWindow::handleTray(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        this->bringToFront();
        break;

    default:
        break;
    }
}
