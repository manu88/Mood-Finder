#include <QDebug>
#include "VideoDockWidget.h"
#include "ui_videodockwidget.h"

VideoDockWidget::VideoDockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoDockWidget)
{
    _player = new Player(this);
    ui->setupUi(this);

    setObjectName("VideoDock");
    ui->mainPlayer = _player;

}

VideoDockWidget::~VideoDockWidget()
{
    delete ui;
}

