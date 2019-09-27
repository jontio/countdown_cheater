#ifndef NUMBER_CHEATER_H
#define NUMBER_CHEATER_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QDebug>
#include <QString>
#include <math.h>

class Number_Cheater : public QObject
{
    Q_OBJECT
public:
    explicit Number_Cheater(QObject *parent = 0);

    QString cheat(double val_wanted, const QVector<double> &vec);

    void vector_work(const QVector<double> &vec);
    uint64_t counter;
    double val_wanted;
    bool val_is_correct;
    QList<QVector<double>> equations;
    bool use_fractions;
    quint64 time_taken_in_mili_seconds;

signals:

public slots:
};

#endif // NUMBER_CHEATER_H
