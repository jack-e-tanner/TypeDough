#ifndef PINPOINT_H
#define PINPOINT_H

#include <QString>
#include <QColor>
#include <QPoint>

class Pinpoint
{
public:
    Pinpoint(QColor color, QString name, QPoint pos);

private:
    int m_id;
    QString m_name;
    QColor m_color;
    QPoint m_pos;

    int m_next_id;
};

#endif // PINPOINT_H
