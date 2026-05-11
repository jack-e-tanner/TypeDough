#ifndef PINPOINT_H
#define PINPOINT_H

#include <QString>
#include <QColor>
#include <QPoint>

struct pinpoint
{
    int id;
    QString name;
    QColor color;
    QPoint pos;
};

#endif // PINPOINT_H
