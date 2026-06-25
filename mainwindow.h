#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backendmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_askbutton_2_clicked();    // "Ask" Button
    void on_uploadbutton_clicked();    // "Upload" Button
    void on_savebutton_clicked();      // "Save Chat" Button

    void displayAnswer(QString answer);
    void displayError(QString error);
    void displayStatus(QString status);

private:
    Ui::MainWindow *ui;
};
#endif