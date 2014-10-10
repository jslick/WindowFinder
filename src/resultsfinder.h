#ifndef RESULTSFINDER_H
#define RESULTSFINDER_H

#include <QObject>

class ResultsCollection;

class ResultsFinder : public QObject
{
    Q_OBJECT
public:
    explicit ResultsFinder(QObject* parent = 0);
    void findWindows(ResultsCollection& resultsCollection);

signals:

};

#endif // RESULTSFINDER_H
