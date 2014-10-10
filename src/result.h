#ifndef RESULT_H
#define RESULT_H

#include <QWidget>

class Result
{
public:
    Result();

    explicit Result(WId winId, const QString& resultTitle);

    WId getWinId() const { return this->winId; }

    const QString& getResultTitle() const { return this->resultTitle; }

private:
    WId     winId = 0;
    QString resultTitle;
};

#endif // RESULT_H
