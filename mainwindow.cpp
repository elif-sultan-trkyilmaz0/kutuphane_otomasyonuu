//23100011055 Elif Sultan Türkyılmaz
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* Uzun yol hali
    db.setDatabaseName("C:/Users/elift/OneDrive/Masaüstü/Bilgisayar mühendisliği 2.sınıf/bahar dönemi/görsel programlama/23100011055/build/23100011055_veri_tabani.db");
    if(!db.open()){
        ui->statusbar->showMessage("Veri tabanına bağlanılamadı.");
    }
    else{
        ui->statusbar->showMessage("Veri tabanına bağlanıldı.");

    }*/
    QString dbyolu =QCoreApplication ::applicationDirPath() +"/23100011055_veri_tabani.db";
    db.setDatabaseName(dbyolu);
    if(!db.open()){
        ui->statusbar->showMessage("Veri tabanına bağlanılamadı.");
    }
    else{
        ui->statusbar->showMessage("Veri tabanına bağlanıldı.");

    }



    model1=new QSqlTableModel();
    model1->setTable("uye");
    model1->select();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_uye_islemleri_clicked()
{
    uye_islemleri *uyeler= new uye_islemleri(db,this);
    uyeler->show();

}

void MainWindow::on_btn_kitap_islemleri_clicked()
{
    kitap_islemleri *kitaplar= new kitap_islemleri(db,this);
    kitaplar->show();

}


void MainWindow::on_btn_odunc_alma_clicked()
{
    odunc_alma_islemleri *odunc= new odunc_alma_islemleri(db,this);
    odunc->show();
}



void MainWindow::on_btn_odunc_teslim_clicked()
{
    odunc_teslim_islemleri *teslim= new odunc_teslim_islemleri(db,this);
    teslim->show();
}


void MainWindow::on_pushButton_clicked()
{
     this->close();
}

