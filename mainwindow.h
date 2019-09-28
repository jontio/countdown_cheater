#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <number_cheater.h>
#include <wantedvaluebox.h>
#include <QMediaPlayer>
#include <QVideoWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Number_Cheater *number_cheat;
    WantedValueBox *wantedvaluebox;

    QPointer<QMediaPlayer> player;
    QPointer<QVideoWidget> videowidget;
protected:
    void resizeEvent(QResizeEvent *ev);

private slots:

    void on_pushButton_clicked();
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void startclock();
    void solve();

    void probtest();

private:
    Ui::MainWindow *ui;
    double wanted_number;
};

#endif // MAINWINDOW_H
