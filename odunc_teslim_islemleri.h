#ifndef ODUNC_TESLIM_ISLEMLERI_H
#define ODUNC_TESLIM_ISLEMLERI_H

//23100011055 Elif Sultan Türkyılmaz

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>



namespace Ui {
class odunc_teslim_islemleri;
}

class odunc_teslim_islemleri : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_teslim_islemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~odunc_teslim_islemleri();
    void odunc_alma_listele();
    void odunc_teslim_listele();

private slots:
    void on_tum_odunc_clicked(const QModelIndex &index);

    void on_btn_teslim_clicked();

private:
    Ui::odunc_teslim_islemleri *ui;
    QSqlQuery *sorgu1;
    QSqlQueryModel *model1;
    QSqlQuery *sorgu2;
    QSqlQueryModel *model2;
    QSqlQuery *sorgu3;
    QSqlQueryModel *model3;
};

#endif // ODUNC_TESLIM_ISLEMLERI_H
