#include "configwindow.h"
#include "ui_configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    this->setFixedHeight(500);
    this->setFixedWidth(500);

    ui->dblClickDelay->setTickPosition(QSlider::TicksBelow);
    ui->maxDiffPerc->setTickPosition(QSlider::TicksBelow);
    ui->minObjSize->setTickPosition(QSlider::TicksBelow);

    ui->dblClickDelVal->setText(QString::number(ui->dblClickDelay->value()) +"s");
    ui->maxPercDiffVal->setText(QString::number(ui->maxDiffPerc->value()) + "%");
    ui->minObjSizeVal->setText(QString::number(ui->minObjSize->value()));
    ui->mouseSpeedVal->setText(QString::number(ui->mouseSpeed->value()));
    ui->dblClickLabel->setText("Opóźnienie\npodwójnego\nkliknięcia:");
    ui->minObjLabel->setText("Minimalny\nrozmiar\nobiektu:");
    ui->maxPercDiffLabel->setText("Maksymalna\nprocentowa\nróżnica:");
    ui->mouseSpeedLabel->setText("Prędkość\nmyszy:");

    connect(ui->minObjSize,SIGNAL(valueChanged(int)),this,SLOT(emitMinObjSizeChanged(int)));
    connect(ui->maxDiffPerc,SIGNAL(valueChanged(int)),this,SLOT(emitMaxPercDiffChanged(int)));
    connect(ui->dblClickDelay,SIGNAL(valueChanged(int)),this,SLOT(emitDblClickDelayChanged(int)));
    connect(ui->sharpenCheckbox,SIGNAL(clicked(bool)),this,SLOT(emitSharpenCheckboxClicked(bool)));
    connect(ui->mouseSpeed,SIGNAL(valueChanged(int)),this,SLOT(emitMouseSpeedValueChanged(int)));

    connect(ui->patternUpButton,SIGNAL(clicked()),this,SLOT(emitUpPatternSaved()));
    connect(ui->patternDownButton,SIGNAL(clicked()),this,SLOT(emitDownPatternSaved()));
    connect(ui->patternLeftButton,SIGNAL(clicked()),this,SLOT(emitLeftPatternSaved()));
    connect(ui->patternRightButton,SIGNAL(clicked()),this,SLOT(emitRightPatternSaved()));
    connect(ui->patternClickButton,SIGNAL(clicked()),this,SLOT(emitClickPatternSaved()));
    connect(ui->patternNeutralButton,SIGNAL(clicked()),this,SLOT(emitNeutralPatternSaved()));
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

void ConfigWindow::emitSharpenCheckboxClicked(bool checked){
    emit sharpenCheckboxClicked(checked);
}

void ConfigWindow::emitMouseSpeedValueChanged(int value){
    emit mouseSpeedValueChanged(value);
    ui->mouseSpeedVal->setText(QString::number(value));
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}
