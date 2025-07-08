#include "inttoolgui.h"
#include "./ui_inttoolgui.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include "IntTool.h"
//Credits to CuckyDev, I only made this so beginners don't have to use the command terminal. This is also my first Qt program, so don't expect it to suck!!!!!!11

IntToolGUI::IntToolGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::IntToolGUI)
{
    ui->setupUi(this);
}

IntToolGUI::~IntToolGUI()
{
    delete ui;
}
IntToolUtil util;
void IntToolGUI::on_ImportButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a INT file!", QDir::homePath(), "Parappa 1 Int Files (*.int *.INT)");
    QString outputFolder = QFileDialog::getExistingDirectory(this, "Output Folder", QDir::homePath(), QFileDialog::ShowDirsOnly);
    util.ImportFile(fileName.toStdString(), outputFolder.toStdString());
    QDir prev(outputFolder);
    prev.cdUp();
    QDesktopServices::openUrl(QUrl::fromLocalFile(prev.absolutePath()));
}


void IntToolGUI::on_ExportButton_clicked()
{
    QString txtFile = QFileDialog::getOpenFileName(this, "Open a text file!", QDir::homePath(), "IntTool txt Files (*.txt)");
    QFileInfo info(txtFile);
    QDir dir(txtFile);
    info.fileName();
    dir.cdUp();
    QString intFile = QFileDialog::getSaveFileName(this, "Output file of unpacked folder", QDir::homePath(), "Parappa 1 Int Files (*.int *.INT)");
    util.ExportFile(intFile.toStdString(), dir.absolutePath().toStdString(), info.baseName().toStdString());
    QDesktopServices::openUrl(QUrl::fromLocalFile(dir.absolutePath()));
}

