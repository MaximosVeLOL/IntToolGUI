#ifndef HELPDIALOG_H
#define HELPDIALOG_H
#endif // HELPDIALOG_H

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

private:
    Ui::IntToolGUI *ui;
};
