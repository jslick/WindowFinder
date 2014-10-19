#ifndef RESULTSCOLLECTION_H
#define RESULTSCOLLECTION_H

#include "result.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>
#include <QPixmap>

/**
 * @brief A model class for discovered results
 */
class ResultsCollection : public QObject
{
    Q_OBJECT
public:
    explicit ResultsCollection(QObject* parent = 0);

signals:
    /**
     * @brief Emitted when the collection is emptied
     */
    void resultsReset();

    /**
     * @param result
     */
    void resultAdded(QSharedPointer<Result> result);

public slots:
    /**
     * @brief Empties the collection and emits resultsReset()
     */
    void resetResults();

    /**
     * @brief Creates a Result and emits resultAdded
     * @param winId         Native handle to the window
     * @param resultTitle   Window title
     * @param icon          Window icon
     */
    void addResult(WId winId, const QString& resultTitle, QPixmap icon = QPixmap());

    /**
     * @return Number of results in the collection
     */
    inline int getLength() const { return results.length(); }

    /**
     * @param index
     * @return result
     */
    const QSharedPointer<Result>& getResult(int index);

private:
    QList<QSharedPointer<Result>> results;
};

#endif // RESULTSCOLLECTION_H
