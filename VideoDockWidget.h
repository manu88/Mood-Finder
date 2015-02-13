#ifndef VIDEODOCKWIDGET_H
#define VIDEODOCKWIDGET_H

#include <QWidget>

#include "player.h"

namespace Ui {
class VideoDockWidget ;
}

class VideoDockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoDockWidget (QWidget *parent = 0);
    ~VideoDockWidget ();

private slots:

private:
    Player *_player;

    Ui::VideoDockWidget *ui;




};

#endif // VIDEODOCKWIDGET_H
