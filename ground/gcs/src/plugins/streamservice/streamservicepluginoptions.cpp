#include "streamservicepluginoptions.h"
#include "ui_streamservicepluginoptions.h"

StreamServicePluginOptions::StreamServicePluginOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamServicePluginOptions)
{
    ui->setupUi(this);
}

StreamServicePluginOptions::~StreamServicePluginOptions()
{
    delete ui;
}
