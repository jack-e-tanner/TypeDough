#include "pinpoint.h"

Pinpoint::Pinpoint(QColor color, QString name, QPoint pos)
    : m_name(name), m_color(color), m_pos(pos) {
    m_id = ++m_next_id;
}