//23100011055 Elif Sultan Türkyılmaz
#ifndef KITAP_ISLEMLERI_H
#define KITAP_ISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class kitap_islemleri;
}

class kitap_islemleri : public QDialog
{
    Q_OBJECT

public:
    explicit kitap_islemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~kitap_islemleri();
    void listele();
    void odunc_alma_durum_listele(const QString &kitapNo);
    void daha_once_odunc_alma_listele(const QString &kitapNo);


private slots:
    void on_btn_kayit_clicked();

    void on_btn_guncelle_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_btn_sil_clicked();

private:
    Ui::kitap_islemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
};

#endif // KITAP_ISLEMLERI_H
