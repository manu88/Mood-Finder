#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QSettings>

#include "VideoDockWidget.h"

#include "datamodel.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const QString getProjectDirectory() const;



public slots:
    void updateDirClicked();
    void chooseProjectDirClicked();

    void saveClicked();


private:
    void closeEvent( QCloseEvent *event) ;
    void resetSettings();
    void saveSettings();

    void setProjectDirectory( const QString &dir );


    Ui::MainWindow *ui;

    //VideoDockWidget _videoWidget;
    Player          _player;

    DataModel _model;
};

#endif // MAINWINDOW_H
