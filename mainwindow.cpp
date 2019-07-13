#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    try
    {
        sheet.genSheet(tr("zh_lib.txt"));
    }
    catch(QString errorString)
    {
        ui->txtCount->setText("");
        QMessageBox::critical(this,QString("错误"),errorString,QMessageBox::Ok);
    }
    ui->txtCount->setText(QString::number(sheet.size()));
    ui->txtPath->setText("zh_lib.txt");
    ui->txtMin->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]?")));
    ui->txtMax->setValidator(new QRegExpValidator(QRegExp("[1-9][0-9]?")));

    int max = sheet.getMaxStroke();
    Minmax minmax = sheet.getMinMaxIndex(1,max);
    ui->txtMin->setText("1");
    ui->txtMax->setText(QString::number(max));
    minStroke = 1;
    maxStroke = max;
    minIndex = minmax.min;
    maxIndex = minmax.max;

    QTime t = QTime::currentTime();
    srand(t.msec() + t.second()*1000);

    connect(&sheet,&ChSheet::readyRead,this,[=](int min,int max){
        ui->txtMin->setText(QString::number(min));
        ui->txtMax->setText(QString::number(max));
        Minmax minmax = sheet.getMinMaxIndex(min,max);
        minStroke = min;
        maxStroke = max;
        minIndex = minmax.min;
        maxIndex = minmax.max;
    });
    connect(ui->txtMin,&QLineEdit::textChanged,this,[=](const QString &str){
        int min = str.toInt();
        if(min > maxStroke || min > ui->txtMax->text().toInt())
        {
            ui->txtMin->setText(str.left(str.size()-1));
        }
        else
        {
            return;
        }
        if(str == "")
        {
            ui->txtMin->setText("1");
        }
        minStroke = min;
        minIndex = sheet.getMinMaxIndex(minStroke,maxStroke).min;
    });
    connect(ui->txtMax,&QLineEdit::textChanged,this,[=](const QString &str){
        int max = str.toInt();
        if(max < minStroke || max < ui->txtMin->text().toInt())
        {
            ui->txtMax->setText(str.left(str.size()-1));
        }
        if(str == "")
        {
            ui->txtMax->setText(ui->txtMin->text());
        }
        maxStroke = max;
        maxIndex = sheet.getMinMaxIndex(minStroke,maxStroke).max;
    });
    connect(ui->txtPath,&QLineEdit::textChanged,this,[=](const QString &str){
        if(ui->btnRead->isEnabled() && str.size() == 0)
        {
            ui->btnRead->setEnabled(false);
        }
        else if(!ui->btnRead->isEnabled() && str.size())
        {
            ui->btnRead->setEnabled(true);
        }
    });
    connect(ui->btnExplore,&QPushButton::clicked,this,[=](){
        QString path = QFileDialog::getOpenFileName(this,"选择库...",".","文本文件 (*.txt)");
        if(path != "")
        {
            ui->txtPath->setText(path);
        }
    });
    connect(ui->btnRead,&QPushButton::clicked,this,[=](){
        try
        {
            sheet.genSheet(ui->txtPath->text());
        }
        catch(QString errorString)
        {
            ui->txtCount->setText("");
            QMessageBox::critical(this,QString("错误"),errorString,QMessageBox::Ok);
        }
        ui->txtCount->setText(QString::number(sheet.size()));

    });
    connect(ui->btnTransfer,&QPushButton::clicked,this,[=](){
        QString src = ui->txtSrc->toPlainText();
        QString dest;
        QRegExp rePunc("[，。？！]");
        for(auto &i:src)
        {
            if(rePunc.indexIn(i) != -1)
                dest.append(i);
            else
            {
                int rnd = rand()*(maxIndex-minIndex+1)/RAND_MAX + minIndex;
                dest.append(sheet[rnd].ch);
            }
        }
        ui->txtDest->setPlainText(dest);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
