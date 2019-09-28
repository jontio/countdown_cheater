#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>
#include <stdlib.h>
#include <time.h>
#include <QMainWindow>
#include <QTimer>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(static_cast<uint>(time(NULL)));
    ui->pushButton->setText("Click Here");
    number_cheat=new Number_Cheater(this);  
#ifdef __linux__
    videowidget = new QVideoWidget(this);
#else
    videowidget = new QVideoWidget(ui->textEdit);
#endif

    //this is how to use
    number_cheat->cheat(666,QVector<double>{75,8,7,4,1,5});

#ifdef __linux__
    ui->textEdit->setFontFamily("Purisa");
    ui->textEdit->setFontPointSize(25);
#else
    ui->textEdit->setFontFamily("Curlz MT");
    ui->textEdit->setFontPointSize(25);
#endif
    ui->textEdit->setPlainText("1+1=2");

//    probtest();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug()<<status;
    switch(status)
    {
    case QMediaPlayer::InvalidMedia:
        if(player)player->deleteLater();
        videowidget->setVisible(false);
        ui->textEdit->setText("Can't find clock.mp4 to play");
        QTimer::singleShot(5000,this,SLOT(solve()));
        break;
    case QMediaPlayer::EndOfMedia:
        if(player)player->deleteLater();
        videowidget->setVisible(false);
        solve();
        break;
    case QMediaPlayer::BufferedMedia:
        videowidget->show();
        videowidget->setFocus();
        break;
    default:
        break;
    }
}


void MainWindow::on_pushButton_clicked()
{

    ui->pushButton->setEnabled(false);

    wanted_number=ui->lcdNumber->value();
    static int number_of_big_numbers=2;
    static bool any_numbers=false;
    static bool want_clock=true;
    wantedvaluebox=new WantedValueBox(&wanted_number,&number_of_big_numbers,&any_numbers,&want_clock,this);

    //if user wants to play a game
    if(!wantedvaluebox->exec())
    {
        wanted_number=(rand()%999)+1;
        ui->lcdNumber->display(wanted_number);

        //select numbers based on game rules
        QVector<double> small_set={1 , 1 , 2 , 2 , 3 , 3 , 4 , 4 , 5 , 5 , 6 , 6 , 7 , 7 , 8 , 8 , 9 , 9 , 10 , 10 };
        QVector<double> large_set={25 , 50 , 75 , 100 };
        if(number_of_big_numbers>=1)ui->spinBox_1->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
         else ui->spinBox_1->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=2)ui->spinBox_2->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
         else ui->spinBox_2->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=3)ui->spinBox_3->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
         else ui->spinBox_3->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=4)ui->spinBox_4->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
         else ui->spinBox_4->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        ui->spinBox_5->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        ui->spinBox_6->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));

        //non game rules any number between 1 and 100
        if(any_numbers)
        {
            ui->spinBox_1->setValue((rand()%100)+1);
            ui->spinBox_2->setValue((rand()%100)+1);
            ui->spinBox_3->setValue((rand()%100)+1);
            ui->spinBox_4->setValue((rand()%100)+1);
            ui->spinBox_5->setValue((rand()%100)+1);
            ui->spinBox_6->setValue((rand()%100)+1);
        }

        ui->textEdit->clear();

        ui->pushButton->setText("...");
        if(want_clock)
        {
            QTimer::singleShot(100,this,SLOT(startclock()));
            return;
        }

    }

    solve();
}

void MainWindow::startclock()
{
    //play clock
    if(!player)player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/media/media/clock.mp4"));
    player->setVideoOutput(videowidget);
    videowidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
#ifdef __linux__
    videowidget->setGeometry(ui->textEdit->geometry().x()+11,ui->textEdit->geometry().y()+11,ui->textEdit->geometry().width(),ui->textEdit->geometry().height());
#else
    videowidget->setGeometry(0,0,ui->textEdit->geometry().width(),ui->textEdit->geometry().height());
#endif
    connect(player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
    QTimer::singleShot(250,player,SLOT(play()));//some reasion without this on linux i would get a gray background for awhile
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
#ifdef __linux__
    videowidget->setGeometry(ui->textEdit->geometry().x()+11,ui->textEdit->geometry().y()+11,ui->textEdit->geometry().width(),ui->textEdit->geometry().height());
#else
    videowidget->setGeometry(0,0,ui->textEdit->geometry().width(),ui->textEdit->geometry().height());
#endif
}

void MainWindow::solve()
{
    ui->pushButton->setText("...");
    ui->lcdNumber->display(wanted_number);
    QVector<double> vec;
    vec.resize(6);
    vec[0]=ui->spinBox_1->value();
    vec[1]=ui->spinBox_2->value();
    vec[2]=ui->spinBox_3->value();
    vec[3]=ui->spinBox_4->value();
    vec[4]=ui->spinBox_5->value();
    vec[5]=ui->spinBox_6->value();
    wanted_number=ui->lcdNumber->value();
    QString result=number_cheat->cheat(wanted_number,vec);
    ui->textEdit->setPlainText(result+"\nThis took me "+QString::number(number_cheat->time_taken_in_mili_seconds)+"ms\nI tried "+QString::number(number_cheat->counter)+" different things.");
    qDebug()<<"number_cheat->time_taken_in_mili_seconds=="<<number_cheat->time_taken_in_mili_seconds;
    qDebug()<<"number_cheat->counter=="<<number_cheat->counter;
    ui->pushButton->setText("Click Here");
    ui->pushButton->setEnabled(true);
}

//just for Monte Carlo simulation
void MainWindow::probtest()
{
    int number_of_big_numbers=4;

    int ntrys=0;
    int nfails=0;

    for(int i=0;i<30000;i++)
    {

        wanted_number=(rand()%999)+1;
        ui->lcdNumber->display(wanted_number);

        //select numbers based on game rules
        QVector<double> small_set={1 , 1 , 2 , 2 , 3 , 3 , 4 , 4 , 5 , 5 , 6 , 6 , 7 , 7 , 8 , 8 , 9 , 9 , 10 , 10 };
        QVector<double> large_set={25 , 50 , 75 , 100 };
        if(number_of_big_numbers>=1)ui->spinBox_1->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
        else ui->spinBox_1->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=2)ui->spinBox_2->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
        else ui->spinBox_2->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=3)ui->spinBox_3->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
        else ui->spinBox_3->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        if(number_of_big_numbers>=4)ui->spinBox_4->setValue(static_cast<int>(large_set.takeAt((rand()%large_set.size()))));
        else ui->spinBox_4->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        ui->spinBox_5->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));
        ui->spinBox_6->setValue(static_cast<int>(small_set.takeAt((rand()%small_set.size()))));

//        ui->spinBox_1->setValue((rand()%100)+1);
//        ui->spinBox_2->setValue((rand()%100)+1);
//        ui->spinBox_3->setValue((rand()%100)+1);
//        ui->spinBox_4->setValue((rand()%100)+1);
//        ui->spinBox_5->setValue((rand()%100)+1);
//        ui->spinBox_6->setValue((rand()%100)+1);

        QVector<double> vec;
        vec.resize(6);
        vec[0]=ui->spinBox_1->value();
        vec[1]=ui->spinBox_2->value();
        vec[2]=ui->spinBox_3->value();
        vec[3]=ui->spinBox_4->value();
        vec[4]=ui->spinBox_5->value();
        vec[5]=ui->spinBox_6->value();
        wanted_number=ui->lcdNumber->value();
        qDebug()<<vec<<wanted_number;
        QString result=number_cheat->cheat(wanted_number,vec);
        ntrys++;
        if("It can't be done"==result)
        {
            qDebug()<<"Fail";
            nfails++;
        }

    }

    double failprob=((double)nfails)/((double)ntrys);
    qDebug()<<"failprob="<<failprob;


}
