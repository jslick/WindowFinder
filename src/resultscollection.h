#ifndef RESULTSCOLLECTION_H
#define RESULTSCOLLECTION_H

#include "result.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QPixmap>

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

    void addResult(WId winId, const QString& resultTitle, QPixmap icon = QPixmap());

    inline int getLength() const { return results.length(); }

    const QSharedPointer<Result>& getResult(int index);

private:
    QList<QSharedPointer<Result>> results;
};

#endif // RESULTSCOLLECTION_H
