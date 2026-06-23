//23100011055 Elif Sultan Türkyılmaz
#include "odunc_alma_islemleri.h"
#include "ui_odunc_alma_islemleri.h"

odunc_alma_islemleri::odunc_alma_islemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::odunc_alma_islemleri)
{
    ui->setupUi(this);
    sorgu = new QSqlQuery(db);
    sorgu1 = new QSqlQuery(db);
    sorgu2 = new QSqlQuery(db);
    sorgu3 = new QSqlQuery(db);
    uye_listele();
    kitap_listele();
    odunc_listele();
}

odunc_alma_islemleri::~odunc_alma_islemleri()
{
    delete ui;
}

void odunc_alma_islemleri::uye_listele()
{
    sorgu1->prepare("select * from uye ");
    if(!sorgu1->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Uye tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model1=new QSqlQueryModel();
    model1->setQuery(*sorgu1);
    ui->tum_uyeler->setModel(model1);
}

void odunc_alma_islemleri::kitap_listele()
{
    sorgu2->prepare("select * from kitap ");
    if(!sorgu2->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model2=new QSqlQueryModel();
    model2->setQuery(*sorgu2);
    ui->tum_kitaplar->setModel(model2);
}

void odunc_alma_islemleri::odunc_listele()
{
    sorgu3->prepare("select * from odunc_alinan");
    if(!sorgu3->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Ödünç alma tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    model3=new QSqlQueryModel();
    model3->setQuery(*sorgu3);
    ui->tum_odunc->setModel(model3);

}




void odunc_alma_islemleri::on_tum_uyeler_clicked(const QModelIndex &index)
{
    ui->ln_uye_no->setText(model1->index(index.row(),0).data().toString());
}


void odunc_alma_islemleri::on_tum_kitaplar_clicked(const QModelIndex &index)
{
    ui->ln_kitap_no->setText(model2->index(index.row(),0).data().toString());

}


void odunc_alma_islemleri::on_odunc_Al_clicked()
{
    QString uye_no = ui->ln_uye_no->text();
    QString kitap_no = ui->ln_kitap_no->text();
    QString alma_tarihi = ui->dateEdit->text();

    // line_edit dolu mu  kontrolü
    if (uye_no.isEmpty() || kitap_no.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen bir üye ve kitap seçiniz!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->prepare("select count (*) from odunc_alinan where uye_no = ? and kitap_no = ?");
    sorgu->addBindValue(uye_no);
    sorgu->addBindValue(kitap_no);
    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Ödünç alma kontrolü sırasında bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->next();
    int odunc_sayisi = sorgu->value(0).toInt();
    if (odunc_sayisi > 0) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Bu kitabı zaten ödünç aldınız ve henüz teslim etmediniz!</b></span>");
        mesaj.exec();
        return;
    }


    // Kitap sayisi kontrolü
    sorgu->prepare("select kitap_sayisi from kitap where kitap_no = ?");
    sorgu->addBindValue(kitap_no);
    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap sayısı kontrolü sırasında bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->next();
    int stok = sorgu->value(0).toInt();
    if (stok <= 0) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Bu kitaptan kalmamıştır, ödünç verilemez!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->prepare("insert into odunc_alinan(uye_no,kitap_no,odunc_alma_tarihi) values(?,?,?)");
    sorgu->addBindValue(uye_no);
    sorgu->addBindValue(kitap_no);
    sorgu->addBindValue(alma_tarihi);
    if(!sorgu->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Ödünç alma tablosuna ekleme yaparken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    // Kitap sayısını azalt
    sorgu->prepare("update kitap set kitap_sayisi = kitap_sayisi - 1 where kitap_no = ?");
    sorgu->addBindValue(kitap_no);
    sorgu->exec();

    QMessageBox mesaj;
    mesaj.setIcon(QMessageBox::Information);
    mesaj.setWindowTitle("Başarılı");
    mesaj.setText("<span style='color:green; font-size:12pt;'><b>Kitap  ödünç alındı.</b></span>");
    mesaj.exec();
    kitap_listele();
    odunc_listele(); // Ödünç alınan kitaplar listesini güncelle
    ui->ln_kitap_no->clear();
    ui->ln_uye_no->clear();

}

