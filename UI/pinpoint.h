#ifndef PINPOINT_H
#define PINPOINT_H

#include <QString>
#include <QColor>
#include <QPointF>

class Pinpoint
{
public:
    Pinpoint(int id, const QString& name, const QColor& color, const QPointF& pos);

    int id() const { return m_id; }
    QString name() const {return m_name; }
    QColor color() const { return m_color; }
    QPointF pos() const { return m_pos; }

    inline void set_name(const QString& name)  {
        m_name = name;
    }

    inline void set_color(const QColor& color) {
        m_color = color;
    }

private:
    int m_id;
    QString m_name;
    QColor m_color;
    QPointF m_pos;
};

#endif // PINPOINT_H
