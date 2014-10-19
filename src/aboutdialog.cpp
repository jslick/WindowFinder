#include "aboutdialog.h"
#include "appcommon.h"

#include <QVBoxLayout>
#include <QLabel>

AboutDialog::AboutDialog(QWidget* parent) :
    QDialog(parent)
{
    this->setWindowTitle(QString("%1 %2").arg(tr("About"), APP_APPNAME));

    QVBoxLayout* mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    QString about;
    about += QString("<h1>%1</h1>").arg(APP_APPNAME);
    about += QString("<p>%1</p>").arg(tr("Copyright 2014 %1").arg("Jason Eslick"));
    about += QString("<p>%1: <a href=\"http://opensource.org/licenses/MIT\">http://opensource.org/licenses/MIT</a></p>")
            .arg(tr("This software is licensed under the MIT license"));
    about += QString("<p>%1</p>").arg(tr("Built with Qt %1").arg(QT_VERSION_STR));
    about += QString("<p>%1</p>").arg(tr("Qt runtime version is %1").arg(qVersion()));
#if defined(__DATE__) && defined(__TIME__)
    about += QString("<p>%1</p>").arg(tr("Built on %1 at %2").arg(__DATE__, __TIME__));
#endif

    QLabel* lblAboutText = new QLabel(this);
    lblAboutText->setText(about);
    mainLayout->addWidget(lblAboutText);
}
