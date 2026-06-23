//23100011055 Elif Sultan Türkyılmaz

#include "kitap_islemleri.h"
#include "ui_kitap_islemleri.h"

kitap_islemleri::kitap_islemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::kitap_islemleri)
{
    ui->setupUi(this);
    sorgu=new QSqlQuery(db);
    listele();

}

kitap_islemleri::~kitap_islemleri()
{
    delete ui;
}

void kitap_islemleri::listele()
{   sorgu->prepare("select * from kitap ");
    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap tablosunu çekerken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    model=new QSqlQueryModel();
    model->setQuery(*sorgu);
    ui->tableView->setModel(model);
}

void kitap_islemleri::on_btn_kayit_clicked()
{
    QString kitap_ad = ui->ln_kitap_ad->text();
    QString kitap_sayisi = ui->ln_kitap_sayisi->text();

    if (kitap_ad.isEmpty() || kitap_sayisi.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen tüm alanları doldurunuz!</b></span>");
        mesaj.exec();
        return;
    }
    sorgu->prepare("insert into kitap(kitap_ad,kitap_sayisi) values(?,?)");
    sorgu->addBindValue(ui->ln_kitap_ad->text());
    sorgu->addBindValue(ui->ln_kitap_sayisi->text());
    if(!sorgu->exec()){
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black ; font-size:12pt;'><b>Kitap tablosuna ekleme yaparken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }
    listele();
    ui->ln_kitap_no->clear();
    ui->ln_kitap_ad->clear();
    ui->ln_kitap_sayisi->clear();
}


void kitap_islemleri::on_btn_guncelle_clicked()
{
    sorgu->prepare("update kitap set kitap_ad=?,kitap_sayisi=? where kitap_no=?");
    sorgu->addBindValue(ui->ln_kitap_ad->text());
    sorgu->addBindValue(ui->ln_kitap_sayisi->text());
    sorgu->addBindValue(ui->ln_kitap_no->text());
    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap tablosunda güncelleme yaparken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    listele();
    ui->ln_kitap_no->clear();
    ui->ln_kitap_ad->clear();
    ui->ln_kitap_sayisi->clear();

}


void kitap_islemleri::on_tableView_clicked(const QModelIndex &index)
{
    QString kitap_no = model->index(index.row(), 0).data().toString();
    ui->ln_kitap_no->setText(kitap_no);
    ui->ln_kitap_ad->setText(model->index(index.row(),1).data().toString());
    ui->ln_kitap_sayisi->setText(model->index(index.row(),2).data().toString());
    odunc_alma_durum_listele(kitap_no);
    daha_once_odunc_alma_listele(kitap_no);
}
void kitap_islemleri::odunc_alma_durum_listele(const QString &kitap_no)
{
    QSqlQuery *sorgu_odunc = new QSqlQuery();
    sorgu_odunc->prepare("select uye_no, odunc_alma_tarihi from odunc_alinan where kitap_no = ?");
    sorgu_odunc->addBindValue(kitap_no);

    if (!sorgu_odunc->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Şu an ödünç alınan kitaplar listelenirken hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    QSqlQueryModel *model_odunc = new QSqlQueryModel();
    model_odunc->setQuery(*sorgu_odunc);
    model_odunc->setHeaderData(0, Qt::Horizontal, "uye_no");
    model_odunc->setHeaderData(1, Qt::Horizontal, "odunc_alma_tarihi");

    ui->odunc_alma_durum->setModel(model_odunc);
}

void kitap_islemleri::daha_once_odunc_alma_listele(const QString &kitap_no)
{
    QSqlQuery *sorgu_once_odunc = new QSqlQuery();
    sorgu_once_odunc->prepare("select uye_no, alma_tarihi, verme_tarihi, borc from odunc_teslim_edilen where kitap_no = ?");
    sorgu_once_odunc->addBindValue(kitap_no);

    if (!sorgu_once_odunc->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Daha önce ödünç alınan kitaplar listelenirken hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    QSqlQueryModel *model_once_odunc = new QSqlQueryModel();
    model_once_odunc->setQuery(*sorgu_once_odunc);
    model_once_odunc->setHeaderData(0, Qt::Horizontal, "uye_no");
    model_once_odunc->setHeaderData(1, Qt::Horizontal, "alma_arihi");
    model_once_odunc->setHeaderData(2, Qt::Horizontal, "verme_tarihi");
    model_once_odunc->setHeaderData(3, Qt::Horizontal, "borç (TL)");

    ui->daha_once_odunc_alma->setModel(model_once_odunc);

}

void kitap_islemleri::on_btn_sil_clicked()
{
    QString kitap_no = ui->ln_kitap_no->text();

    // Boş alan kontrolü
    if (kitap_no.isEmpty()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Lütfen silinecek bir kitap seçiniz!</b></span>");
        mesaj.exec();
        return;
    }

    // Kitabın ödünçte olup olmadığı kontrol ediliyor
    sorgu->prepare("select count (*) from odunc_alinan where kitap_no = ?");
    sorgu->addBindValue(kitap_no);

    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap kontrolü yapılırken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    sorgu->next();
    int odunc_sayisi = sorgu->value(0).toInt();

    if (odunc_sayisi > 0) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Warning);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Bu kitap hala ödünçte, teslim edilmeden silinemez!</b></span>");
        mesaj.exec();
        return;
    }

    // Kitap silme işlemi
    sorgu->prepare("delete from kitap where kitap_no = ?");
    sorgu->addBindValue(kitap_no);

    if (!sorgu->exec()) {
        QMessageBox mesaj;
        mesaj.setIcon(QMessageBox::Critical);
        mesaj.setWindowTitle("Hata");
        mesaj.setText("<span style='color:black; font-size:12pt;'><b>Kitap silinirken bir hata oluştu!</b></span>");
        mesaj.exec();
        return;
    }

    QMessageBox mesaj;
    mesaj.setIcon(QMessageBox::Information);
    mesaj.setWindowTitle("Başarılı");
    mesaj.setText("<span style='color:green; font-size:12pt;'><b>Kitap başarıyla silindi.</b></span>");
    mesaj.exec();


    // Tabloyu güncelle
    listele();

    // LineEdit'leri temizle
    ui->ln_kitap_no->clear();
    ui->ln_kitap_ad->clear();
    ui->ln_kitap_sayisi->clear();
}



