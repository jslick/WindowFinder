#ifndef VIEW_H
#define VIEW_H

#include <QSharedPointer>
#include <QWidget>

class Result;

/**
 * @interface View
 *
 * @brief A View is a widget that is the main widget of the window
 */
class View : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Subclass constructors should call this constructor
     * @param parent
     */
    explicit View(QWidget* parent = 0);

    virtual ~View() {}

    /**
     * @brief Asks for permission before hiding the main window
     *
     * @return True if the window should close on escape key, false if the window should stay open
     */
    virtual bool escapeRequested() { return true; }

signals:
    /**
     * @brief Emitted when the user chooses a result
     * @param result
     */
    void resultSelected(QSharedPointer<Result> result);
};

#endif // VIEW_H
