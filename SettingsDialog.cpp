#include <QFileDialog>
#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(SettingsDialogDelegate *delegate) :
    QDialog   (),
    ui        (new Ui::SettingsDialog),
    _delegate ( delegate )
{
    ui->setupUi(this);

    ui->rootDirLineEdit->setText( _delegate->getRootDir() );

    connect( ui->browseRootDirButton , SIGNAL(clicked()) , this , SLOT(setRootDirClicked()) );


}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::setRootDirClicked()
{
    QFileDialog dialog(this , tr("Choose Root projects Directory") );
     dialog.setFileMode(QFileDialog::DirectoryOnly );

     if (dialog.exec())
      {
          const QString newDir = dialog.selectedFiles().at(0);
          //qDebug("new Dir is : %s",newDir.toStdString().c_str() );

          _delegate->setRootDir( newDir );
     }
}
