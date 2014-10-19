#include "simpleview.h"
#include "resultscollection.h"

#include <QAction>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

static const int LABEL_COL = 0;
static const int WINDOW_ICON_COL = 1;
static const int WINDOW_TITLE_COL = 2;

class TableItem : public QTableWidgetItem
{
public:
    TableItem(QSharedPointer<Result>& result)
        : QTableWidgetItem(result->getResultTitle()),
          result(result)
    {}

    QSharedPointer<Result> result;
};

SimpleView::SimpleView(ResultsCollection& resultsCollection, QWidget* parent) :
    View(parent),
    resultsCollection(resultsCollection),
    mainLayout(new QVBoxLayout),
    searchEdit(new QLineEdit(this)),
    resultsTable(new QTableWidget(this))
{
    this->setLayout(this->mainLayout);

    connect(this->searchEdit, SIGNAL(textChanged(QString)), SLOT(filterResults(QString)));
    connect(this->searchEdit, SIGNAL(returnPressed()), SLOT(selectCurrentResult()));
    this->mainLayout->addWidget(this->searchEdit);

    this->resultsTable->setWordWrap(false);
    this->resultsTable->setShowGrid(false);
    this->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->resultsTable->setColumnCount(3);
    this->resultsTable->setColumnWidth(LABEL_COL, 20);
    this->resultsTable->setColumnWidth(WINDOW_ICON_COL, 20);
    this->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->resultsTable->horizontalHeader()->hide();
    this->resultsTable->horizontalHeader()->setStretchLastSection(true);
    this->resultsTable->verticalHeader()->hide();
    connect(this->resultsTable, SIGNAL(activated(QModelIndex)), SLOT(selectCurrentResult()));
    this->mainLayout->addWidget(this->resultsTable);

    QSignalMapper* numMapper = new QSignalMapper(this);
    connect(numMapper, SIGNAL(mapped(int)), SLOT(selectResult(int)));
    for (int i = 0; i <= 9; i++)
    {
        QAction* numAction = new QAction(this);
        numAction->setShortcut(QKeySequence(QString("Alt+%1").arg(i)));
        numMapper->setMapping(numAction, i);
        connect(numAction, SIGNAL(triggered()), numMapper, SLOT(map()));
        this->addAction(numAction);
    }

    connect(&this->resultsCollection, SIGNAL(resultsReset()), SLOT(clearList()));
    connect(&this->resultsCollection, SIGNAL(resultAdded(QSharedPointer<Result>)), SLOT(addResult(QSharedPointer<Result>)));
}

bool SimpleView::escapeRequested()
{
    if (this->searchEdit->text().isEmpty())
        return true;
    else
    {
        this->searchEdit->setText("");
        return false;
    }
}

void SimpleView::clearList()
{
    this->searchEdit->clear();
    this->resultsTable->clear();
    this->resultsTable->setRowCount(0);
}

void SimpleView::addResult(QSharedPointer<Result> result)
{
    int row = this->resultsTable->rowCount();
    this->resultsTable->setRowCount(row + 1);
    if (row < 10)
    {
        QTableWidgetItem* numItem = new QTableWidgetItem(QString::number(row < 9 ? row + 1 : 0));
        this->resultsTable->setItem(row, LABEL_COL, numItem);
    }
    else
    {
        this->resultsTable->setItem(row, LABEL_COL, new QTableWidgetItem);
    }

    this->resultsTable->setItem(row, WINDOW_ICON_COL, new QTableWidgetItem(result->getIcon(), ""));

    TableItem* item = new TableItem(result);
    this->resultsTable->setItem(row, WINDOW_TITLE_COL, item);

    this->resultsTable->setRowHeight(row, 18);
    if (row == 0)
        this->resultsTable->selectRow(row);
}

void SimpleView::filterResults(QString text)
{
    if (text.isEmpty())
    {
        for (int i = 0; i < this->resultsTable->rowCount(); i++)
        {
            this->resultsTable->showRow(i);
            this->resultsTable->item(i, LABEL_COL)->setText(i <= 9 ? QString::number(i + 1) : QString());
        }
        if (this->resultsTable->rowCount())
            this->resultsTable->selectRow(0);
        return;
    }

    std::vector<bool> matches(this->resultsCollection.getLength(), false);

    QStringList words = text.split(QRegularExpression(R"(\s+)"), QString::SkipEmptyParts);

    for (int i = 0; i < this->resultsCollection.getLength(); i++)
    {
        const QSharedPointer<Result>& result = this->resultsCollection.getResult(i);
        for (const QString& word : words)
        {
            if (result->getResultTitle().contains(word, Qt::CaseInsensitive))
            {
                matches[i] = true;
            }
            else
            {
                matches[i] = false;
                break;
            }
        }
    }

    int rownum = 1;
    for (size_t i = 0; i < matches.size(); i++)
    {
        if (matches[i])
        {
            this->resultsTable->showRow(i);
            if (rownum < 10)
                this->resultsTable->item(i, LABEL_COL)->setText(QString::number(rownum));
            if (rownum == 1)
                this->resultsTable->selectRow(i);

            rownum++;
        }
        else
            this->resultsTable->hideRow(i);
    }
}

void SimpleView::selectCurrentResult()
{
    QList<QTableWidgetItem*> items = this->resultsTable->selectedItems();
    for (auto item : items)
    {
        if (auto resultItem = dynamic_cast<TableItem*>(item))
        {
            emit resultSelected(resultItem->result);
            break;
        }
    }
}

void SimpleView::selectResult(int num)
{
    num--;
    if (num < 0)
        num = 9;

    QString numstr = QString::number(num + 1);
    for (int i = num; i < this->resultsTable->rowCount(); i++)
    {
        if (!this->resultsTable->isRowHidden(i) && this->resultsTable->item(i, LABEL_COL)->text() == numstr)
        {
            TableItem* resultItem = dynamic_cast<TableItem*>(this->resultsTable->item(i, WINDOW_TITLE_COL));
            if (resultItem)
                emit resultSelected(resultItem->result);
        }
    }
}
