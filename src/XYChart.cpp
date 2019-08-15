/*
 * This file is part of Quick Charts.
 * Copyright 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "XYChart.h"

#include "RangeGroup.h"
#include "datasource/ChartDataSource.h"

void calculateStackedYRange(XYChart *chart, ComputedRange &range, float &minY, float &maxY)
{
    auto sources = chart->valueSources();

    minY = 0.0;
    for (auto source : sources) {
        minY = std::min(minY, source->minimum().toFloat());
    }

    for (int i = range.startX; i < range.endX; ++i) {
        float yDistance = 0.0;
        for (auto source : sources) {
            yDistance += source->item(i).toFloat();
        }
        maxY = std::max(maxY, std::max(0.0f, minY) + yDistance);
    }
}

bool operator==(const ComputedRange &first, const ComputedRange &second)
{
    return first.startX == second.startX && first.endX == second.endX && qFuzzyCompare(first.startY, second.startY)
        && qFuzzyCompare(first.endY, second.endY);
}

XYChart::XYChart(QQuickItem *parent)
    : Chart(parent)
{
    m_xRange = new RangeGroup{this};
    connect(m_xRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
    m_yRange = new RangeGroup{this};
    connect(m_yRange, &RangeGroup::rangeChanged, this, &XYChart::updateComputedRange);
}

RangeGroup *XYChart::xRange() const
{
    return m_xRange;
}

RangeGroup *XYChart::yRange() const
{
    return m_yRange;
}

XYChart::Direction XYChart::direction() const
{
    return m_direction;
}

void XYChart::setDirection(XYChart::Direction newDirection)
{
    if (newDirection == m_direction) {
        return;
    }

    m_direction = newDirection;
    onDataChanged();
    Q_EMIT directionChanged();
}

bool XYChart::stacked() const
{
    return m_stacked;
}

void XYChart::setStacked(bool newStacked)
{
    if (newStacked == m_stacked) {
        return;
    }

    m_stacked = newStacked;
    onDataChanged();
    Q_EMIT stackedChanged();
}

ComputedRange XYChart::computedRange() const
{
    return m_computedRange;
}

void XYChart::updateComputedRange()
{
    ComputedRange result;

    if (m_xRange->automatic()) {
        updateAutomaticXRange(result);
    } else {
        result.startX = m_xRange->from();
        result.endX = m_xRange->to();
    }
    result.distanceX = result.endX - result.startX;

    if (m_yRange->automatic()) {
        updateAutomaticYRange(result);
    } else {
        result.startY = m_yRange->from();
        result.endY = m_yRange->to();
    }
    result.distanceY = result.endY - result.startY;

    if (result == m_computedRange) {
        return;
    }

    m_computedRange = result;
    Q_EMIT computedRangeChanged();
}

void XYChart::updateAutomaticXRange(ComputedRange &range)
{
    range.startX = 0;
    int maxX = -1;
    const auto sources = valueSources();
    for (auto valueSource : sources) {
        maxX = std::max(maxX, valueSource->itemCount());
    }
    range.endX = maxX;
}

void XYChart::updateAutomaticYRange(ComputedRange &range)
{
    auto minY = std::numeric_limits<float>::max();
    auto maxY = std::numeric_limits<float>::min();

    const auto sources = valueSources();
    if (!m_stacked) {
        for (auto valueSource : sources) {
            minY = std::min(minY, valueSource->minimum().toFloat());
            maxY = std::max(maxY, valueSource->maximum().toFloat());
        }
    } else {
        calculateStackedYRange(this, range, minY, maxY);
    }
    range.startY = std::min(0.0f, minY);
    range.endY = std::max(0.0f, maxY);
}

QDebug operator<<(QDebug debug, const ComputedRange &range)
{
    debug << "Range: startX" << range.startX << "endX" << range.endX << "distance" << range.distanceX << "startY" << range.startY << "endY"
          << range.endY << "distance" << range.distanceY;
    return debug;
}
