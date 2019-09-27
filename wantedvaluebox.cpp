#include "wantedvaluebox.h"
#include "ui_wantedvaluebox.h"
#include <QDebug>

WantedValueBox::WantedValueBox(double *value,int *number_of_big_numbers,bool *any_numbers,bool *enable_clock,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WantedValueBox)
{
    ui->setupUi(this);
    wantedvalue=nullptr;
    wantednumber_of_big_numbers=nullptr;
    wantednumber_any_numbers=nullptr;
    wanted_enable_clock=nullptr;
    if(value)
    {
        wantednumber_of_big_numbers=number_of_big_numbers;
        wantedvalue=value;
        wantednumber_any_numbers=any_numbers;
        wanted_enable_clock=enable_clock;
        ui->spinBox->setValue(*wantedvalue);
        ui->spinBox_number_of_bigns->setValue(*wantednumber_of_big_numbers);
        if(*wantednumber_any_numbers)ui->checkBox_any_numbers->setChecked(*wantednumber_any_numbers);
        if(*wanted_enable_clock)ui->checkBox_clock->setChecked(*wanted_enable_clock);
        ui->spinBox_number_of_bigns->setEnabled(!ui->checkBox_any_numbers->isChecked());
    }
}

WantedValueBox::~WantedValueBox()
{
    delete ui;
}

void WantedValueBox::on_spinBox_valueChanged(int arg1)
{
    qDebug()<<(*wantedvalue);
    if(wantedvalue)*wantedvalue=(double)arg1;
}

void WantedValueBox::on_pushButton_randomize_clicked()
{
    done(0);
}

void WantedValueBox::on_pushButton_cheat_clicked()
{
    done(1);
}

void WantedValueBox::on_spinBox_number_of_bigns_valueChanged(int arg1)
{
    if(wantednumber_of_big_numbers)*wantednumber_of_big_numbers=arg1;
}

void WantedValueBox::on_checkBox_any_numbers_stateChanged(int arg1)
{
    if(wantednumber_any_numbers)*wantednumber_any_numbers=arg1;
    ui->spinBox_number_of_bigns->setEnabled(!arg1);
}



void WantedValueBox::on_checkBox_clock_stateChanged(int arg1)
{
    if(wanted_enable_clock)*wanted_enable_clock=arg1;
}
