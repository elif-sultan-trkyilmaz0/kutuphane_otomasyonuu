//23100011055 Elif Sultan Türkyılmaz

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSqlDatabase>
#include<QSqlTableModel>
#include<uye_islemleri.h>
#include<kitap_islemleri.h>
#include<odunc_alma_islemleri.h>
#include<odunc_teslim_islemleri.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_uye_islemleri_clicked();
    void on_btn_kitap_islemleri_clicked();
    void on_btn_odunc_alma_clicked();

    void on_btn_odunc_teslim_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    QSqlTableModel * model1;
};
#endif // MAINWINDOW_H
