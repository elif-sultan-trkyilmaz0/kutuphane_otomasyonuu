//23100011055 Elif Sultan Türkyılmaz

#include "uye_islemleri.h"
#include "ui_uye_islemleri.h"

uye_islemleri::uye_islemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::uye_islemleri)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();


}

uye_islemleri::~uye_islemleri()
{
    delete ui;
}

void uye_islemleri::listele()
{
    sorgu->prepare("select * from uye ");
    if(!sorgu->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Uye tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView->setModel(model);
}

void uye_islemleri::on_tableView_clicked(const QModelIndex &index)
{
    ui->ln_uye_no->setText(model->index(index.row(),0).data().toString());
    ui->ln_uye_ad->setText(model->index(index.row(),1).data().toString());
    ui->ln_uye_soyad->setText(model->index(index.row(),2).data().toString());
}

void uye_islemleri::on_btn_kayit_clicked()
{
    QString ad = ui->ln_uye_ad->text();
    QString soyad = ui->ln_uye_soyad->text();

    if (ad.isEmpty() || soyad.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen tüm alanları doldurunuz!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->prepare("insert into uye(uye_ad,uye_soyad) values(?,?)");
    sorgu->addBindValue(ui->ln_uye_ad->text());
    sorgu->addBindValue(ui->ln_uye_soyad->text());
    if(!sorgu->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Uye tablosuna ekleme yaparken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    listele();
    ui->ln_uye_no->clear();
    ui->ln_uye_ad->clear();
    ui->ln_uye_soyad->clear();

}

void uye_islemleri::on_btn_guncelle_clicked()
{
    sorgu->prepare("update uye set uye_ad=?,uye_soyad=? where uye_no=?");
    sorgu->addBindValue(ui->ln_uye_ad->text());
    sorgu->addBindValue(ui->ln_uye_soyad->text());
    sorgu->addBindValue(ui->ln_uye_no->text());
    if(!sorgu->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Uye tablosunda güncelleme yaparken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    listele();
    ui->ln_uye_no->clear();
    ui->ln_uye_ad->clear();
    ui->ln_uye_soyad->clear();

}


void uye_islemleri::on_btn_sil_clicked()
{
    QString uye_no = ui->ln_uye_no->text();

    if (uye_no.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen silinecek bir üye seçiniz!</b></span>");
        mesaj.exec();
        return;
    }

    // Üyenin henüz teslim etmediği kitap var mı
    sorgu->prepare("select count (*) from odunc_alinan where uye_no = ?");
    sorgu->addBindValue(uye_no);

    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Üye kontrolü sırasında bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    sorgu->next();
    int teslim_edilmemis_kitap_sayisi = sorgu->value(0).toInt();

    if (teslim_edilmemis_kitap_sayisi > 0) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Bu üyenin teslim etmediği kitaplar var. Silme işlemi gerçekleştirilemez!</b></span>");
        mesaj.exec();
        return;
    }

    // Üye silme işlemi
    sorgu->prepare("delete from uye where uye_no = ?");
    sorgu->addBindValue(uye_no);

    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Üye silinirken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    QMessageBox mesaj;
    mesaj.setIcon(QMessageBox::Information);
    mesaj.setWindowTitle("Başarılı");
    mesaj.setText("<span style='color:green; font-size:12pt;'><b>Üye başarıyla silindi.</b></span>");
    mesaj.exec();

    listele();

    // LineEdit'leri temizle
    ui->ln_uye_no->clear();
    ui->ln_uye_ad->clear();
    ui->ln_uye_soyad->clear();
}



