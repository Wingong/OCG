#include "chsheet.h"
#include <QDebug>
#include <QMessageBox>
#include <QWidget>
#include <QList>
#include <QDataStream>
#include <QStringList>

ChSheet::ChSheet(QObject *parent)
    : QObject(parent)
{
}

int ChSheet::genSheet(const QString &path)
{
    QFile countFile(path);
    if(!countFile.open(QFile::ReadOnly))
    {
        throw countFile.errorString();
    }
    _zhList.clear();
    _strokeVec.clear();
    while(!countFile.atEnd())
    {
        QString ch(countFile.readLine());
        QStringList strs = ch.split(QRegExp("\\s+"));
        if(strs.size() != 5)
            throw QString("汉字库格式错误！");
        Zh zh;
        zh.ch = strs[0];
        zh.pinyin = strs[1];
        zh.stroke = strs[2].toInt();
        //来者可追
        for(int i=zh.stroke;i<_strokeVec.size();i++)
        {
            if(_strokeVec[i].min != -1)
            {
                _strokeVec[i].min ++;
                _strokeVec[i].max ++;
            }
        }
        //本格自增
        if(zh.stroke <= _strokeVec.size() && _strokeVec[zh.stroke-1].min != -1)
        {
            _zhList.insert(_strokeVec[zh.stroke-1].max,zh);
            _strokeVec[zh.stroke-1].max ++;
        }
        //本格启动
        else
        {
            //寻找先行
            int index = -1;
            if(zh.stroke > _strokeVec.size())
                _strokeVec.resize(zh.stroke);
            for(int i=zh.stroke-1;i;i--)
            {
                if(_strokeVec[i-1].min != -1)
                {
                    index = i-1;
                    break;
                }
            }
            if(index == -1)
            {
                _strokeVec[zh.stroke-1] = Minmax(0,0);
            }
            else
            {
                _strokeVec[zh.stroke-1] = Minmax(_strokeVec[index].max+1,_strokeVec[index].max+1);
            }
            _zhList.insert(_strokeVec[zh.stroke-1].min,zh);
        }

        //QByteArray ch(countFile.readLine());
        //QDataStream stm(ch);
        //qDebug() << stm;
        //QList<QByteArray> list = ch.split(' ');
        //while(list.removeOne(""));
        //if(list.size() != 5)
        //    throw QString("汉字库格式错误！");
        //Zh zh;
        //zh.ch = list[0];
        //zh.pinyin = list[1];
        //zh.stroke = list[2].toInt();
        //_zhList.push_back(zh);
        //i++;
    }
    emit readyRead(1,_strokeVec.size());
    return 0;
}

Minmax ChSheet::getMinMaxIndex(int minStroke, int maxStroke)
{
    Minmax minmax;
    minmax.min = _strokeVec[minStroke-1].min;
    minmax.max = _strokeVec[maxStroke-1].max;
    return minmax;
}
