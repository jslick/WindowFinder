#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private slots:
    void resetResults();

    void activateResult(QSharedPointer<Result> result);

private:
    std::unique_ptr<ResultsFinder> resultsFinder;
    std::unique_ptr<ResultsCollection> resultsCollection;
    View* view = 0;
};

#endif // MAINWINDOW_H
