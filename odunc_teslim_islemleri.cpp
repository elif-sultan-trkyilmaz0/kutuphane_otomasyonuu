//23100011055 Elif Sultan Türkyılmaz

#include "odunc_teslim_islemleri.h"
#include "ui_odunc_teslim_islemleri.h"

odunc_teslim_islemleri::odunc_teslim_islemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::odunc_teslim_islemleri)
{
    ui->setupUi(this);
    sorgu1=new QSqlQuery(db);
    sorgu2=new QSqlQuery(db);
    sorgu3=new QSqlQuery(db);
    odunc_alma_listele();
    odunc_teslim_listele();

}

odunc_teslim_islemleri::~odunc_teslim_islemleri()
{
    delete ui;
}

void odunc_teslim_islemleri::odunc_alma_listele()
{
    sorgu1->prepare("select * from odunc_alinan");
    if(!sorgu1->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Ödünç alma tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model1=new QSqlQueryModel();
    model1->setQuery(*sorgu1);
    ui->tum_odunc->setModel(model1);
}

void odunc_teslim_islemleri::odunc_teslim_listele()
{
    sorgu2->prepare("select * from odunc_teslim_edilen");
    if(!sorgu2->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Ödünç teslim edilen  tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model2=new QSqlQueryModel();
    model2->setQuery(*sorgu2);
    ui->tum_teslim->setModel(model2);
}

void odunc_teslim_islemleri::on_tum_odunc_clicked(const QModelIndex &index)
{
     ui->ln_uye_no->setText(model1->index(index.row(),0).data().toString());
     ui->ln_kitap_no->setText(model1->index(index.row(),1).data().toString());

     QString tarih_str = model1->index(index.row(), 2).data().toString();
     QDate tarih = QDate::fromString(tarih_str, "d.MM.yyyy");

     if (tarih.isValid()) {
         ui->teslim_tarih->setDate(tarih);
     } else {
         QMessageBox mesaj;
         mesaj.setIcon(QMessageBox::Warning);
         mesaj.setWindowTitle("Hata");
         mesaj.setTextFormat(Qt::RichText);
         mesaj.setText(QString("<span style='color:black; font-size:12pt;'><b>Tarih formatı hatalı: %1</b></span>").arg(tarih_str));
         mesaj.exec();
     }

}


void odunc_teslim_islemleri::on_btn_teslim_clicked()
{
    QString uye_no = ui->ln_uye_no->text();
    QString kitap_no = ui->ln_kitap_no->text();
    QDate teslim_tarihi = ui->teslim_tarih->date();

    // Boş alan kontrolü
    if (uye_no.isEmpty() || kitap_no.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen bir üye ve kitap seçiniz!</b></span>");
        mesaj.exec();
        return;
    }

    // Alış tarihini veritabanından çekme
    sorgu3->prepare("select odunc_alma_tarihi from odunc_alinan where uye_no = ? and kitap_no = ?");
    sorgu3->addBindValue(uye_no);
    sorgu3->addBindValue(kitap_no);

    if (!sorgu3->exec() || !sorgu3->next()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Alış tarihi bulunamadı!</b></span>");
        mesaj.exec();
        return;
    }

    QString alma_tarihi_str = sorgu3->value(0).toString();
    QDate alma_tarihi = QDate::fromString(alma_tarihi_str, "d.MM.yyyy");

    if (!alma_tarihi.isValid()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Alış tarihi geçersiz!</b></span>");
        mesaj.exec();
        return;
    }

    // Gün farkı hesaplama
    int gun_farki = alma_tarihi.daysTo(teslim_tarihi);
    int borc = 0;

    if (gun_farki > 15) {
        borc = (gun_farki - 15) * 4;
    }

    // Teslim edilen kitap kaydını ekle
    sorgu2->prepare("insert into odunc_teslim_edilen (uye_no, kitap_no, alma_tarihi, verme_tarihi, borc) values(?, ?, ?, ?, ?)");
    sorgu2->addBindValue(uye_no);
    sorgu2->addBindValue(kitap_no);
    sorgu2->addBindValue(alma_tarihi_str);
    sorgu2->addBindValue(teslim_tarihi.toString("d.MM.yyyy"));
    sorgu2->addBindValue(borc);

    if (!sorgu2->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Tarih formatı hatalı!</b></span>");

        return;
    }

    // Kitap sayısını arttır
    QSqlQuery stokQuery;
    stokQuery.prepare("update kitap set kitap_sayisi = kitap_sayisi + 1 where kitap_no = ?");
    stokQuery.addBindValue(kitap_no);
    stokQuery.exec();

    // Ödünç alınan kaydı sil
    QSqlQuery silQuery;
    silQuery.prepare("delete from odunc_alinan where uye_no = ? and kitap_no = ?");
    silQuery.addBindValue(uye_no);
    silQuery.addBindValue(kitap_no);
    silQuery.exec();

    QMessageBox mesaj;
    mesaj.setIcon(QMessageBox::Information);
    mesaj.setWindowTitle("Başarılı");
    mesaj.setTextFormat(Qt::RichText); // HTML biçimlendirmesini etkinleştir
    mesaj.setText(QString("<span style='color:green; font-size:12pt;'><b>Kitap başarıyla teslim edildi.</b></span><br><span style='color:black;'>Borç: %1 TL</span>").arg(borc));
    mesaj.exec();


    // Listeleri güncelle
    odunc_alma_listele();
    odunc_teslim_listele();
    ui->ln_kitap_no->clear();
    ui->ln_uye_no->clear();
}




