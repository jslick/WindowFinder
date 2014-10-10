#ifndef RESULTSCOLLECTION_H
#define RESULTSCOLLECTION_H

#include "result.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>

class ResultsCollection : public QObject
{
    Q_OBJECT
public:
    explicit ResultsCollection(QObject* parent = 0);

signals:
    void resultsReset();
    void resultAdded(QSharedPointer<Result> result);

public slots:
    void resetResults();

    void addResult(WId winId, const QString& resultTitle);

    inline int getLength() const { return results.length(); }

private:
    QList<QSharedPointer<Result>> results;
};

#endif // RESULTSCOLLECTION_H
