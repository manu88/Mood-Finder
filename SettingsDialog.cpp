#include <QFileDialog>
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(SettingsDialogDelegate *delegate) :
    QDialog   (),
    ui        (new Ui::SettingsDialog),
    _delegate ( delegate )
{
    ui->setupUi(this);

    ui->rootDirLineEdit->setText(       _delegate->getProjectDirectory()  );
    ui->nameXmlEdit->setText(           _delegate->getXMLName() );
    ui->xmlLocationLocalEdit->setText(  _delegate->getLocalXMLLocation()  );
    ui->xmlLocationServerEdit->setText( _delegate->getServerXMLLocation() );
    ui->backupCount->setValue(          _delegate->getNumBackup()         );

    connect( ui->browseRootDirButton , SIGNAL(clicked()) , this , SLOT(setRootDirClicked()) );
    connect( ui->browseXMLLocal, SIGNAL(clicked()) , this , SLOT(setLocalXMLLocationClicked()) );
    connect( ui->browseXMLServer, SIGNAL(clicked()) , this , SLOT(setServerXMLLocationClicked()) );
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void SettingsDialog::setRootDirClicked()
{
    QFileDialog dialog(this , tr("Choose Root projects Directory") );
     dialog.setFileMode(QFileDialog::DirectoryOnly );

     if (dialog.exec())
      {
          const QString newDir = dialog.selectedFiles().at(0);
          //qDebug("new Dir is : %s",newDir.toStdString().c_str() );


     }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void SettingsDialog::setLocalXMLLocationClicked()
{

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void SettingsDialog::setServerXMLLocationClicked()
{

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

QString SettingsDialog::getRootDir() const
{
    return ui->rootDirLineEdit->text();
}

QString SettingsDialog::getXmlName() const
{
    return ui->nameXmlEdit->text();
}

QString SettingsDialog::getLocalXmlLocationDir() const
{
    return ui->xmlLocationLocalEdit->text();
}

QString SettingsDialog::getServerXmlLocationDir() const
{
    return ui->xmlLocationServerEdit->text();
}

int SettingsDialog::getNumBackup() const
{
    return ui->backupCount->value();

}
