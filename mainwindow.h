#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSettings>

//#include "VideoDockWidget.h"
#include "SettingsDialog.h"
#include "datamodel.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public SettingsDialogDelegate
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString getProjectDirectory() const;
    const QString getXMLName() const;
    const QString getLocalXMLLocation() const;
    const QString getServerXMLLocation() const;
    int           getNumBackup() const;

    /* Settings delegate*/
    QString getRootDir() const
    {
        return getProjectDirectory();
    }


public slots:
    void updateDirClicked();
    void chooseProjectDirClicked();

    void saveClicked();

    void PreferencesClicked();

    void verticalHeaderDoubleClicked( int index );


private:
    void closeEvent( QCloseEvent *event) ;
    void resetSettings();
    void saveSettings();

    void setProjectDirectory(  const QString &dir );
    void setXMLName(  const QString &name );
    void setLocalXMLLocation(  const QString &dir );
    void setServerXMLLocation( const QString &dir );
    void setNumBackup( int num);

    Ui::MainWindow *ui;

    //VideoDockWidget _videoWidget;
    Player          _player;

    DataModel _model;
};

#endif // MAINWINDOW_H
