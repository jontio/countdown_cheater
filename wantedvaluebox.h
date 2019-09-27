#ifndef WANTEDVALUEBOX_H
#define WANTEDVALUEBOX_H

#include <QDialog>
#include <QPointer>

namespace Ui {
class WantedValueBox;
}

class WantedValueBox : public QDialog
{
    Q_OBJECT

public:
    explicit WantedValueBox(double *value,int *number_of_big_numbers,bool *any_numbers,bool *enable_clock,QWidget *parent = 0);
    ~WantedValueBox();
    double *wantedvalue;
    int *wantednumber_of_big_numbers;
    bool *wantednumber_any_numbers;
    bool *wanted_enable_clock;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_randomize_clicked();

    void on_pushButton_cheat_clicked();

    void on_spinBox_number_of_bigns_valueChanged(int arg1);

    void on_checkBox_any_numbers_stateChanged(int arg1);



    void on_checkBox_clock_stateChanged(int arg1);

private:
    Ui::WantedValueBox *ui;
};

#endif // WANTEDVALUEBOX_H
