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

void ResultsCollection::addResult(WId winId, const QString& resultTitle)
{
    QSharedPointer<Result> p(new Result(winId, resultTitle));
    this->results << p;

    emit resultAdded(p);
}
