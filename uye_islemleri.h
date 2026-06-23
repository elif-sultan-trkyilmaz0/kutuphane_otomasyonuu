//23100011055 Elif Sultan Türkyılmaz

#ifndef UYE_ISLEMLERI_H
#define UYE_ISLEMLERI_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>

namespace Ui {
class uye_islemleri;
}

class uye_islemleri : public QDialog
{
    Q_OBJECT

public:
    explicit uye_islemleri(QSqlDatabase, QWidget *parent = nullptr);
    ~uye_islemleri();
    void listele();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_btn_kayit_clicked();


    void on_btn_guncelle_clicked();

    void on_btn_sil_clicked();

private:
    Ui::uye_islemleri *ui;
    QSqlQuery *sorgu;
    QSqlQueryModel *model;
};

#endif // UYE_ISLEMLERI_H
