//23100011055 Elif Sultan Türkyılmaz

#ifndef ODUNC_ALMA_ISLEMLERI_H
#define ODUNC_ALMA_ISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class odunc_alma_islemleri;
}

class odunc_alma_islemleri : public QDialog
{
    Q_OBJECT

public:
    explicit odunc_alma_islemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~odunc_alma_islemleri();
    void uye_listele();
    void kitap_listele();
    void odunc_listele();



private slots:
    void on_tum_uyeler_clicked(const QModelIndex &index);

    void on_tum_kitaplar_clicked(const QModelIndex &index);

    void on_odunc_Al_clicked();

private:
    Ui::odunc_alma_islemleri *ui;
    QSqlQuery *sorgu;
    QSqlQuery *sorgu1;
    QSqlQueryModel *model1;
    QSqlQuery *sorgu2;
    QSqlQueryModel *model2;
    QSqlQuery *sorgu3;
    QSqlQueryModel *model3;

};

#endif // ODUNC_ALMA_ISLEMLERI_H
