#ifndef SIMPLEVIEW_H
#define SIMPLEVIEW_H

#include "view.h"

class ResultsCollection;
class QVBoxLayout;
class QLineEdit;
class QTableWidget;

class SimpleView : public View
{
    Q_OBJECT
public:
    explicit SimpleView(ResultsCollection& resultsCollection, QWidget* parent = 0);

signals:

public slots:

private slots:
    void clearList();

    void addResult(QSharedPointer<Result> result);

    void filterResults(QString text);

    void selectCurrentResult();

    void selectResult(int num);

private:
    ResultsCollection& resultsCollection;
    QVBoxLayout*    mainLayout = 0;
    QLineEdit*      searchEdit = 0;
    QTableWidget*   resultsTable = 0;
};

#endif // SIMPLEVIEW_H
