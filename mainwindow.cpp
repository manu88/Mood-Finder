
#include <QDebug>
#include <QFileDialog>

#include <QMessageBox>



#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "commons.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _model(0)
{
    QSettings settings;

    ui->setupUi(this);
    ui->dockWidget->setWidget( &_player);

    ui->tableView->setModel( &_model);

    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());

    connect(ui->actionMettre_jour, SIGNAL(triggered()) , this , SLOT(updateDirClicked()) );
    connect(ui->actionSave_XML, SIGNAL(triggered()) , this , SLOT(saveClicked()) );

    connect(ui->actionPr_f_rences , SIGNAL(triggered()) , this, SLOT(PreferencesClicked()) );

    setWindowTitle(APP_NAME);

    /* restore settings */
    restoreState(settings.value("mainWindowState").toByteArray());
    restoreDockWidget(ui->dockWidget);

    if (!ui->dockWidget->isVisible() )
        ui->dockWidget->setVisible(true);

    ui->tableView->horizontalHeader()->restoreState( settings.value("HoriTab").toByteArray() );

    updateDirClicked();

}

MainWindow::~MainWindow()
{
    delete ui;


}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Settings
*/
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::setProjectDirectory( const QString &dir )
{
    QSettings settings;
    settings.setValue(DIR_PROJECT_TOK , dir);

    settings.sync();
}

const QString MainWindow::getProjectDirectory() const
{
    QSettings settings;
    return settings.value(DIR_PROJECT_TOK).toString();
}

void MainWindow::resetSettings()
{
    QSettings settings;

    settings.remove(DIR_PROJECT_TOK);

    settings.sync();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::saveSettings()
{
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    settings.setValue("HoriTab",ui->tableView->horizontalHeader()->saveState());


    settings.sync();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
       UI Slots
*/
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
void MainWindow::closeEvent( QCloseEvent *event)
{
    saveSettings();
}

void MainWindow::updateDirClicked()
{
    QSettings settings;

    const QString dir = settings.value( DIR_PROJECT_TOK ).toString();

    if (dir.isEmpty())
    {
        qDebug("No directory provided for Projects -> should ask to provide one");
        chooseProjectDirClicked();
    }

    if( !_model.loadXML( tr(DIR_XML) + tr(NAME_XML)) )
    {
        QMessageBox msgBox;
        msgBox.setText("No xml file found.");
        msgBox.setInformativeText("Would you like to scan projects' directory?");
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);

        const int ret = msgBox.exec();

         if (ret == QMessageBox::Yes )
            _model.parseProjectDirectory( getProjectDirectory() );
    }

    //

    //_model.compareAndSaveXML( tr(DIR_XML) + tr(NAME_XML));

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::chooseProjectDirClicked()
{
    QFileDialog dialog(this , tr("Choose Root projects Directory") );
     dialog.setFileMode(QFileDialog::DirectoryOnly );

     if (dialog.exec())
      {
          const QString newDir = dialog.selectedFiles().at(0);
          //qDebug("new Dir is : %s",newDir.toStdString().c_str() );

          setProjectDirectory( newDir );
     }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::saveClicked()
{
    _model.compareAndSaveXML( tr(DIR_XML) + tr(NAME_XML));
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::PreferencesClicked()
{
    SettingsDialog *dial = new SettingsDialog(this);

    int ret = dial->exec();

}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
