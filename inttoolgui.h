#ifndef INTTOOLGUI_H
#define INTTOOLGUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class IntToolGUI;
}
QT_END_NAMESPACE

class IntToolGUI : public QMainWindow
{
    Q_OBJECT

public:
    IntToolGUI(QWidget *parent = nullptr);
    ~IntToolGUI();

private slots:
    void on_ImportButton_clicked();

    void on_ExportButton_clicked();

    void on_HelpButton_clicked();

private:
    Ui::IntToolGUI *ui;
};
#endif // INTTOOLGUI_H
