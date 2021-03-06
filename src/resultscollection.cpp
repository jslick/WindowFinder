#include "resultscollection.h"
#include "result.h"

#include <QDebug>

ResultsCollection::ResultsCollection(QObject* parent) :
    QObject(parent)
{
}

void ResultsCollection::resetResults()
{
    this->results.clear();
    emit resultsReset();
}

void ResultsCollection::addResult(WId winId, const QString& resultTitle, QPixmap icon)
{
    QSharedPointer<Result> p(new Result(winId, resultTitle, icon));
    this->results << p;

    emit resultAdded(p);
}

const QSharedPointer<Result>& ResultsCollection::getResult(int index)
{
    return this->results[index];
}
