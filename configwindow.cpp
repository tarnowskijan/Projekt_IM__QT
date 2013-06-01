#include "configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    this->setFixedHeight(550);
    this->setFixedWidth(500);

    ui->maxHUE->setTickPosition(QSlider::TicksBelow);
    ui->minHUE->setTickPosition(QSlider::TicksBelow);
    ui->maxVAL->setTickPosition(QSlider::TicksBelow);
    ui->minVAL->setTickPosition(QSlider::TicksBelow);
    ui->maxSAT->setTickPosition(QSlider::TicksBelow);
    ui->minSAT->setTickPosition(QSlider::TicksBelow);
    ui->dblClickDelay->setTickPosition(QSlider::TicksBelow);
    ui->maxDiffPerc->setTickPosition(QSlider::TicksBelow);
    ui->minObjSize->setTickPosition(QSlider::TicksBelow);

    ui->dblClickDelVal->setText(QString::number(ui->dblClickDelay->value()) +"s");
    ui->maxPercDiffVal->setText(QString::number(ui->maxDiffPerc->value()) + "%");
    ui->minObjSizeVal->setText(QString::number(ui->minObjSize->value()));
    ui->minSatVal->setText(QString::number(ui->minSAT->value()));
    ui->maxSatVal->setText(QString::number(ui->maxSAT->value()));
    ui->minHueVal->setText(QString::number(ui->minHUE->value()));
    ui->maxHueVal->setText(QString::number(ui->maxHUE->value()));
    ui->minValVal->setText(QString::number(ui->minVAL->value()));
    ui->maxValVal->setText(QString::number(ui->maxVAL->value()));

    connect(ui->minHUE,SIGNAL(valueChanged(int)),this,SLOT(emitMinHueChanged(int)));
    connect(ui->maxHUE,SIGNAL(valueChanged(int)),this,SLOT(emitMaxHueChanged(int)));
    connect(ui->minVAL,SIGNAL(valueChanged(int)),this,SLOT(emitMinValChanged(int)));
    connect(ui->maxVAL,SIGNAL(valueChanged(int)),this,SLOT(emitMaxValChanged(int)));
    connect(ui->minSAT,SIGNAL(valueChanged(int)),this,SLOT(emitMinSatChanged(int)));
    connect(ui->maxSAT,SIGNAL(valueChanged(int)),this,SLOT(emitMaxSatChanged(int)));
    connect(ui->minObjSize,SIGNAL(valueChanged(int)),this,SLOT(emitMinObjSizeChanged(int)));
    connect(ui->maxDiffPerc,SIGNAL(valueChanged(int)),this,SLOT(emitMaxPercDiffChanged(int)));
    connect(ui->dblClickDelay,SIGNAL(valueChanged(int)),this,SLOT(emitDblClickDelayChanged(int)));

    connect(ui->patternUpButton,SIGNAL(clicked()),this,SLOT(emitUpPatternSaved()));
    connect(ui->patternDownButton,SIGNAL(clicked()),this,SLOT(emitDownPatternSaved()));
    connect(ui->patternLeftButton,SIGNAL(clicked()),this,SLOT(emitLeftPatternSaved()));
    connect(ui->patternRightButton,SIGNAL(clicked()),this,SLOT(emitRightPatternSaved()));
    connect(ui->patternClickButton,SIGNAL(clicked()),this,SLOT(emitClickPatternSaved()));
    connect(ui->patternNeutralButton,SIGNAL(clicked()),this,SLOT(emitNeutralPatternSaved()));
}

void ConfigWindow::emitMinHueChanged(int val){
    emit minHueChanged(val);
    ui->minHueVal->setText(QString::number(val));
}
void ConfigWindow::emitMaxHueChanged(int val){
    emit maxHueChanged(val);
    ui->maxHueVal->setText(QString::number(val));
}
void ConfigWindow::emitMinSatChanged(int val){
    emit minSatChanged(val);
    ui->minSatVal->setText(QString::number(val));
}
void ConfigWindow::emitMaxSatChanged(int val){
    emit maxSatChanged(val);
    ui->maxSatVal->setText(QString::number(val));
}
void ConfigWindow::emitMinValChanged(int val){
    emit minValChanged(val);
    ui->minValVal->setText(QString::number(val));
}
void ConfigWindow::emitMaxValChanged(int val){
    emit maxValChanged(val);
    ui->maxValVal->setText(QString::number(val));
}
void ConfigWindow::emitMinObjSizeChanged(int val){
    emit minObjSizeChanged(val);
    ui->minObjSizeVal->setText(QString::number(val));
}
void ConfigWindow::emitMaxPercDiffChanged(int val){
    emit maxPercDiffChanged(val);
    ui->maxPercDiffVal->setText(QString::number(val)+ "%");
}
void ConfigWindow::emitDblClickDelayChanged(int val){
    emit dblClickDelayChanged(val);
    ui->dblClickDelVal->setText(QString::number(val) +"s");
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}
