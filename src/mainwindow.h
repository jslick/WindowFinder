#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

#include <memory>

class View;
class Result;
class ResultsFinder;
class ResultsCollection;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

signals:
    void closed(QCloseEvent* event);

public slots:
    void bringToFront();

    void showAbout();

protected:
    void closeEvent(QCloseEvent* event);

private slots:

    void resetResults();

    void activateResult(QSharedPointer<Result> result);

    void maybeHide();

    void handleTray(QSystemTrayIcon::ActivationReason reason);

private:
    std::unique_ptr<ResultsFinder> resultsFinder;
    std::unique_ptr<ResultsCollection> resultsCollection;
    View* view = 0;
};

#endif // MAINWINDOW_H
