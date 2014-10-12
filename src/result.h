#ifndef RESULT_H
#define RESULT_H

#include <QWidget>
#include <QPixmap>

class Result
{
public:
    Result();

    explicit Result(WId winId, const QString& resultTitle, QPixmap icon = QPixmap());

    WId getWinId() const { return this->winId; }

    const QString& getResultTitle() const { return this->resultTitle; }

    const QPixmap& getIcon() const { return this->icon; }

private:
    WId     winId = 0;
    QString resultTitle;
    QPixmap icon;
};

#endif // RESULT_H
