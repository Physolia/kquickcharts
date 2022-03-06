/*
 * This file is part of KQuickCharts
 * SPDX-FileCopyrightText: 2019 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "ChartDataSource.h"

#include <QVariant>

ChartDataSource::ChartDataSource(QObject *parent)
    : QObject(parent)
{
}

QVariant ChartDataSource::first() const
{
    return item(0);
}

// QVariant ChartDataSource::minimumVariant(const QVariantList &input) const
// {
//     auto itr = std::min_element(input.cbegin(), input.cend(), [](const QVariant &first, const QVariant &second) {
//         return QVariant::compare(first, second) == QPartialOrdering::Less;
//     });
//     if (itr != input.cend()) {
//         return *itr;
//     }
//     return QVariant{};
// }
//
// QVariant ChartDataSource::maximumVariant(const QVariantList &input) const
// {
//     auto itr = std::max_element(input.cbegin(), input.cend(), [](const QVariant &first, const QVariant &second) {
//         return QVariant::compare(first, second) == QPartialOrdering::Less;
//     });
//     if (itr != input.cend()) {
//         return *itr;
//     }
//     return QVariant{};
// }
