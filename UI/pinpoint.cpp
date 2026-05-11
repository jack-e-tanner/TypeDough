#include "pinpoint.h"

Pinpoint::Pinpoint(int id, const QString& name, const QColor& color, const QPointF& pos)
    : m_id(id), m_name(name), m_color(color), m_pos(pos) {}