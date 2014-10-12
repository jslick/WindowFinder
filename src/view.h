#ifndef VIEW_H
#define VIEW_H

#include <QSharedPointer>
#include <QWidget>

class Result;

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget* parent = 0);

    virtual ~View() {}

    virtual bool escapeRequested() { return true; }

signals:
    void resultSelected(QSharedPointer<Result> result);

public slots:

};

#endif // VIEW_H
