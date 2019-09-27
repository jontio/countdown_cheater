#include "number_cheater.h"
#include <QElapsedTimer>

Number_Cheater::Number_Cheater(QObject *parent) : QObject(parent)
{
    //find wanted number using numbers
    use_fractions=false;
    val_is_correct=false;
    counter=0;
    equations.clear();
    time_taken_in_mili_seconds=-1;
}

QString Number_Cheater::cheat(double tmp_val_wanted, const QVector<double> &vec)
{

    QElapsedTimer elapsedtimer;
    elapsedtimer.start();
    time_taken_in_mili_seconds=-1;

    //find wanted number using numbers
    val_is_correct=false;
    val_wanted=tmp_val_wanted;
    qDebug()<<vec;
    counter=0;
    equations.clear();
    vector_work(vec);
    qDebug()<<"number of trys ="<<counter;

    //have we done it???
    if(equations.size()==0)
    {
        time_taken_in_mili_seconds=elapsedtimer.elapsed();
        qDebug()<<"It can't be done";
        return "It can't be done";
    }

    //start factoring numbers with operations
    qDebug()<<"now working on factoring";
    QVector<double> vec_tmp=vec;
    QVector<QString> vec_strings;
    vec_strings.resize(vec_tmp.size());
    for(int i=0;i<vec_tmp.size();i++)
    {
        vec_strings[i]=((QString)"%0").arg(vec_tmp[i]);
    }
//    qDebug()<<vec_strings;
//    qDebug()<<equations;
    for(int k=0;k<equations.size();k++)
    {

//        qDebug()<<vec_strings;
//        qDebug()<<vec_tmp;

        //remove number pair we are using
        assert(equations[k].size()==3);
        int i=equations[k][1];
        double val_a=equations[k][0];
        double val_b=equations[k][2];
        int val_a_idx=-1;
        int val_b_idx=-1;
        for(int j=0;j<vec_tmp.size();j++)
        {
            if(vec_tmp[j]==val_a)
            {
                val_a_idx=j;
                break;
            }
        }
        assert(val_a_idx>=0);
        QString val_a_text=vec_strings[val_a_idx];
        vec_tmp.remove(val_a_idx);
        vec_strings.remove(val_a_idx);
        for(int j=0;j<vec_tmp.size();j++)
        {
            if(vec_tmp[j]==val_b)
            {
                val_b_idx=j;
                break;
            }
        }
        assert(val_b_idx>=0);
        QString val_b_text=vec_strings[val_b_idx];
        vec_tmp.remove(val_b_idx);
        vec_strings.remove(val_b_idx);

        //add the result of using these two numbers
        switch (i)
        {
        case 0://+
            qDebug()<<val_a<<"+"<<val_b<<"="<<(val_a+val_b);
            vec_tmp.push_back((val_a+val_b));
            vec_strings.push_back("("+val_a_text+"+"+val_b_text+")");
            break;
        case 1://*
            qDebug()<<val_a<<"*"<<val_b<<"="<<(val_a*val_b);
            vec_tmp.push_back((val_a*val_b));
            vec_strings.push_back("("+val_a_text+"*"+val_b_text+")");
            break;
        case 2://-
            qDebug()<<val_a<<"-"<<val_b<<"="<<(val_a-val_b);
            vec_tmp.push_back((val_a-val_b));
            vec_strings.push_back("("+val_a_text+"-"+val_b_text+")");
            break;
        case 3://-
            qDebug()<<val_b<<"-"<<val_a<<"="<<(val_b-val_a);
            vec_tmp.push_back((val_b-val_a));
            vec_strings.push_back("("+val_b_text+"-"+val_a_text+")");
            break;
        case 4:// /
            qDebug()<<val_a<<"/"<<val_b<<"="<<(val_a/val_b);
            vec_tmp.push_back((val_a/val_b));
            vec_strings.push_back("("+val_a_text+"/"+val_b_text+")");
            break;
        case 5:// /
            qDebug()<<val_b<<"/"<<val_a<<"="<<(val_b/val_a);
            vec_tmp.push_back((val_b/val_a));
            vec_strings.push_back("("+val_b_text+"/"+val_a_text+")");
            break;
        default:
            break;
        }
    }
//    qDebug()<<vec_strings;
//    qDebug()<<vec_tmp;

    //find sol in list
    int val_wanted_loc=-1;
    for(int k=0;k<vec_tmp.size();k++)
    {
        if(vec_tmp[k]==val_wanted)
        {
            val_wanted_loc=k;
            break;
        }
    }
    assert(val_wanted_loc>=0);
    QString solution_text=vec_strings[val_wanted_loc];
    if(solution_text[0]=="(")
    {
        solution_text.remove(0,1);
        solution_text.chop(1);
    }
    solution_text+=" = "+((QString)"%0").arg(val_wanted);

    qDebug()<<"Solution is:"<<solution_text;
    time_taken_in_mili_seconds=elapsedtimer.elapsed();
    return solution_text;

}


void Number_Cheater::vector_work(const QVector<double> &vec)
{
    //qDebug()<<"void MainWindow::vector_work(const QVector<double> &vec)";
    QVector<double> new_vec;
    new_vec.resize(vec.size()-1);
    for(int a=0;a<vec.size();a++)
    {
        for(int b=a+1;b<vec.size();b++)
        {
            //qDebug()<<a<<b; using these two numbers

            //create a vector without these two numbers and leave the last entry spare
            int new_vec_pos=0;
            for(int j=0;j<vec.size();j++)
            {
                if((j==a)||(j==b))continue;
                new_vec[new_vec_pos]=vec[j];
                new_vec_pos++;
            }

            //run through all ops with a and b for the last space of the new vector
            for(int i=-2;i<6;i++)//dropping allowed
            //for(int i=0;i<6;i++)//must used every number
            {
                switch (i)
                {
                case 0://+
                    new_vec[new_vec_pos]=vec[a]+vec[b];
                    break;
                case 1://*
                    new_vec[new_vec_pos]=vec[a]*vec[b];
                    break;
                case 2://-
                    new_vec[new_vec_pos]=vec[a]-vec[b];
                    if(new_vec[new_vec_pos]<0.0)continue;//pass on negative intermediate numbers
                    break;
                case 3://-
                    new_vec[new_vec_pos]=vec[b]-vec[a];
                    if(new_vec[new_vec_pos]<0.0)continue;//pass on negative intermediate numbers
                    break;
                case 4:// /
                    new_vec[new_vec_pos]=vec[a]/vec[b];
                    if((!use_fractions)&&((new_vec[new_vec_pos]-floor(new_vec[new_vec_pos]))!=0))continue;//pass on fractions
                    break;
                case 5:// /
                    new_vec[new_vec_pos]=vec[b]/vec[a];
                    if((!use_fractions)&&((new_vec[new_vec_pos]-floor(new_vec[new_vec_pos]))!=0))continue;//pass on fractions
                    break;
                case -2:// drop b
                    new_vec[new_vec_pos]=vec[a];
                    break;
                case -1:// drop a
                    new_vec[new_vec_pos]=vec[b];
                    break;
                default:
                    assert(1==0);
                    break;
                }

                //qDebug()<<new_vec;

                if(new_vec.size()>1)
                {
                    vector_work(new_vec);
                }
                 else
                 {
                    //qDebug()<<"sol="<<new_vec[0];
                    counter++;
                    if(val_wanted==new_vec[0])
                    {
                        val_is_correct=true;
                        qDebug()<<"calulated ="<<new_vec[0];
                        qDebug()<<"got val";
                    }
                 }
                if(val_is_correct)
                {
                    if(i>=0)//ignore dropped numbers
                    {
                        QVector<double> equation_single={vec[a],(double)i,vec[b]};
                        equations.push_front(equation_single);
                    }
                    return;
                }

            }

        }
    }
}
