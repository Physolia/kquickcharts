#include "LineChartNode.h"

#include <QColor>
#include <QtMath>

#include "LineChartMaterial.h"
#include "LineSegmentNode.h"

static const int MaxPointsInSegment = 100;

LineChartNode::LineChartNode()
{
}

LineChartNode::~LineChartNode()
{
}

void LineChartNode::setRect(const QRectF &rect)
{
    if (rect == m_rect)
        return;

    m_rect = rect;
    updatePoints();
}

void LineChartNode::setLineWidth(float width)
{
    if(qFuzzyCompare(width, m_lineWidth))
        return;

    m_lineWidth = width;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [this](LineSegmentNode* node) { node->setLineWidth(m_lineWidth / m_rect.width()); });
}

void LineChartNode::setLineColor(const QColor& color)
{
    if(m_lineColor == color)
        return;

    m_lineColor = color;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [color](LineSegmentNode* node) { node->setLineColor(color); });
}

void LineChartNode::setFillColor(const QColor& color)
{
    if(m_fillColor == color)
        return;

    m_fillColor = color;
    std::for_each(m_segments.cbegin(), m_segments.cend(), [color](LineSegmentNode* node) { node->setFillColor(color); });
}

void LineChartNode::setValues(const QVector<qreal>& values)
{
    m_values = values;
    updatePoints();
}

void LineChartNode::updatePoints()
{
    if(m_values.isEmpty())
        return;

    auto segmentCount = qCeil(qreal(m_values.count()) / MaxPointsInSegment);
    if(segmentCount != m_segments.count()) {
        removeAllChildNodes();
        m_segments.clear();

        for(int i = 0; i < segmentCount; ++i) {
            auto node = new LineSegmentNode{};
            m_segments << node;
            appendChildNode(node);
        }
    }

    auto segmentWidth = m_rect.width() / segmentCount;
    auto currentX = m_rect.left();
    auto pointStart = 0;
    auto pointsPerSegment = m_values.count() / m_segments.count();

    for(auto segment : qAsConst(m_segments)) {
        auto rect = QRectF(currentX, m_rect.top(), segmentWidth, m_rect.height());
        currentX += segmentWidth;
        segment->setRect(rect);
        segment->setLineWidth(m_lineWidth / m_rect.width());
        segment->setLineColor(m_lineColor);
        segment->setFillColor(m_fillColor);
        segment->setValues(m_values.mid(pointStart, pointsPerSegment));
        pointStart += pointsPerSegment;
    }
}
