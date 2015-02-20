
#include <QDebug>
#include <QFileDialog>

#include <QMessageBox>


#include <QKeyEvent>
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

    connect(ui->tableView->verticalHeader(), SIGNAL(sectionDoubleClicked(int)) ,
                this ,SLOT(verticalHeaderDoubleClicked(int)));

    ui->tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect( ui->tableView->verticalHeader(), SIGNAL(customContextMenuRequested(QPoint)),
                SLOT(customMenuRequested(QPoint)));

    connect(ui->actionPousser , SIGNAL(triggered()) , this , SLOT(PushServerClicked()) );
    connect(ui->actionTirer, SIGNAL(triggered()) , this , SLOT(PullServerClicked()) );

    connect(ui->actionDelete    , SIGNAL( triggered() ) , this , SLOT( deleteLineClicked() ) );
    connect(ui->actionDuplicate , SIGNAL( triggered() ) , this , SLOT( duplicateLineClicked() ) );

    connect(ui->searchButton, SIGNAL(clicked()) , this , SLOT(searchClicked()) );
    connect(ui->clrSearchButton, SIGNAL(clicked()) , this , SLOT(clearSearchClicked()) );
    setWindowTitle(APP_NAME);

    /* restore settings */
    restoreState(settings.value("mainWindowState").toByteArray());
    restoreDockWidget(ui->dockWidget);

    if (!ui->dockWidget->isVisible() )
        ui->dockWidget->setVisible(true);

    ui->tableView->horizontalHeader()->restoreState( settings.value("HoriTab").toByteArray() );

    ui->searchFiled->installEventFilter( this);
    updateDirClicked();



}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::logStatus( const QString &msg)
{
    statusBar()->showMessage( msg );
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

void MainWindow::setXMLName(  const QString &name )
{
    QSettings settings;
    settings.setValue( NAME_XML_TOK , name);

    settings.sync();
}

void MainWindow::setLocalXMLLocation(  const QString &dir )
{
    QSettings settings;
    settings.setValue(DIR_LOCAL_XML_TOK , dir);

    settings.sync();
}

void MainWindow::setServerXMLLocation( const QString &dir )
{
    QSettings settings;
    settings.setValue(DIR_SERVER_XML_TOK , dir);

    settings.sync();
}

void MainWindow::setNumBackup( int num)
{
    QSettings settings;
    settings.setValue(NUM_BACKUPS_TOK , num);

    settings.sync();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

const QString MainWindow::getProjectDirectory() const
{
    QSettings settings;
    return settings.value(DIR_PROJECT_TOK).toString();
}

const QString MainWindow::getXMLName() const
{
    QSettings settings;
    return settings.value(NAME_XML_TOK).toString();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

const QString MainWindow::getLocalXMLLocation() const
{
    QSettings settings;
    return settings.value(DIR_LOCAL_XML_TOK).toString();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
const QString MainWindow::getServerXMLLocation() const
{
    QSettings settings;
    return settings.value(DIR_SERVER_XML_TOK).toString();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
int MainWindow::getNumBackup() const
{
    QSettings settings;
    return settings.value(NUM_BACKUPS_TOK).toInt();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

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

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::searchClicked()
{
    if (_model.getCurrentMode() == DataSearchMode)
        _model.clearSearch();

    if ( ui->searchFiled->text().isEmpty() )
        return;

    const QString str = ui->searchFiled->text();
    qDebug() << "search for " << str;

    const int size = _model.search( str );

    QString retSearch;
    if (size == 0)
        retSearch = "No entries found";

    else if ( size == 1)
        retSearch = "1 entry found";

    else
        retSearch = QString("%1 entries found").arg(size);

    ui->searchLabel->setText( retSearch );

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::clearSearchClicked()
{
    _model.clearSearch();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::updateDirClicked()
{
    QSettings settings;

    const QString dir = settings.value( DIR_PROJECT_TOK ).toString();

    if (dir.isEmpty())
    {
        qDebug("No directory provided for Projects -> should ask to provide one");
        chooseProjectDirClicked();
    }

    if( !_model.loadXML( getLocalXMLLocation() + getXMLName() + ".xml" ) )
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

    logStatus( _model.getDataBaseDate().toString() );


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
    _model.compareAndSaveXML( getLocalXMLLocation() + getXMLName() + ".xml" );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::PreferencesClicked()
{
    SettingsDialog *dial = new SettingsDialog(this);

    if (dial->exec())
    {
        setProjectDirectory( dial->getRootDir() );
        setXMLName( dial->getXmlName() );
        setLocalXMLLocation( dial->getLocalXmlLocationDir() );
        setServerXMLLocation( dial->getServerXmlLocationDir() );
        setNumBackup( dial->getNumBackup() );
    }


    delete dial;


}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::verticalHeaderDoubleClicked( int index )
{

    const DataEntry entry = _model.getEntryForIndex( index);

    qDebug() << "Should play " << entry.filePath << "at TC " << Timecode::tcToQString(entry.timecode) ;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::duplicateLineClicked()
{
    if (ui->tableView->selectionModel()->selection().indexes().empty())
        return;

    const int row =  ui->tableView->selectionModel()->selection().indexes().at(0).row();

    qDebug() << "add line below " <<row;

    _model.duplicateEntryNum( row );

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::deleteLineClicked()
{
    if (ui->tableView->selectionModel()->selection().indexes().empty())
        return;

    const int row =  ui->tableView->selectionModel()->selection().indexes().at(0).row();

    qDebug() << "delete line" <<row;

    _model.deleteEntryNum( row );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::customMenuRequested(QPoint pos)
{
    QModelIndex index=ui->tableView->indexAt(pos);

    ui->tableView->selectRow(index.row());

    QMenu *menu=new QMenu(this);

    menu->addAction( ui->actionDuplicate );
    menu->addAction( ui->actionDelete );

    menu->popup(ui->tableView->verticalHeader()->viewport()->mapToGlobal(pos));

}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->searchFiled && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_Return)
        {
            searchClicked();
            return true;
        }
    }

    return false;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::PullServerClicked()
{
    qDebug() << "Pull file to server";
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainWindow::PushServerClicked()
{
    qDebug() << "Push file to server";
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

