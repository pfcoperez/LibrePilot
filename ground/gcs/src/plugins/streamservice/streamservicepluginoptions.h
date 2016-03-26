#ifndef STREAMSERVICEPLUGINOPTIONS_H
#define STREAMSERVICEPLUGINOPTIONS_H

#include <QWidget>

namespace Ui {
class StreamServicePluginOptions;
}

class StreamServicePluginOptions : public QWidget
{
    Q_OBJECT

public:
    explicit StreamServicePluginOptions(QWidget *parent = 0);
    ~StreamServicePluginOptions();

private:
    Ui::StreamServicePluginOptions *ui;
};

#endif // STREAMSERVICEPLUGINOPTIONS_H
