#ifndef CHSHEET_H
#define CHSHEET_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVector>
#include <QFile>

class Zh
{
public:
    QString     ch;
    QString     pinyin;
    int         stroke;
};

class Minmax
{
public:
    int min;
    int max;
    Minmax():min(-1),max(-1){}
    Minmax(int min,int max):min(min),max(max){}
};

class ChSheet : public QObject
{
    Q_OBJECT
signals:
    void readyRead(int min,int max);
public:
    ChSheet(QObject *parent = nullptr);
    int size(){return _zhList.size();}
    int getMaxStroke(){return _strokeVec.size();}
    Minmax getMinMaxIndex(int minStroke,int maxStroke);
    Zh &operator[](int i){return _zhList[i];}
    int genSheet(const QString &path = "123");
private:
    QVector<Zh>     _zhList;
    QVector<Minmax> _strokeVec;
};

#endif // CHSHEET_H
