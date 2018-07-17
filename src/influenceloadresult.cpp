/********************************************************************************************
 * This file is part of TrussTables
 * Copyright 2018, Ambrose Louis Okune <sambero.osilu@gmail.com>
 *
 * TrussTables is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Public License as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * TrussTables is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with TrussTables.
 * If not, see <http://www.gnu.org/licenses/>.
 ********************************************************************************************/

/* influenceloadresult.cpp */

#include "influenceloadresult.h"

InfluenceLoadResult::InfluenceLoadResult(int barsCount, int ordinatesCount, QObject *parent) : QObject(parent)
{
    mBarsCount      = barsCount;
    mOrdinatesCount = ordinatesCount;

    for (int i = 0; i < mBarsCount; ++i)
    {
        mInfluenceLoadOrdinatesLists.append(new QList<qreal>);
        mMinLoadList.append(0.0);
        mMinLoadPositionList.append(QString());
        mMaxLoadList.append(0.0);
        mMaxLoadPositionList.append(QString());
    }
}

InfluenceLoadResult::~InfluenceLoadResult()
{
    foreach (QList<qreal> *list, mInfluenceLoadOrdinatesLists)
    {
        delete list;
    }
}

int InfluenceLoadResult::barsCount() const
{
    return mBarsCount;
}

int InfluenceLoadResult::ordinatesCount() const
{
    return mOrdinatesCount;
}

void InfluenceLoadResult::setParameters(int barsCount, int ordinatesCount)
{
    foreach (QList<qreal> *list, mInfluenceLoadOrdinatesLists)
    {
        delete list;
    }

    mInfluenceLoadOrdinatesLists.clear();
    mMinLoadList.clear();
    mMinLoadPositionList.clear();
    mMaxLoadList.clear();
    mMaxLoadPositionList.clear();

    mBarsCount      = barsCount;
    mOrdinatesCount = ordinatesCount;

    for (int i = 0; i < mBarsCount; ++i)
    {
        mInfluenceLoadOrdinatesLists.append(new QList<qreal>);
        mMinLoadList.append(0.0);
        mMinLoadPositionList.append(QString());
        mMaxLoadList.append(0.0);
        mMaxLoadPositionList.append(QString());
    }
}

const QList<qreal> &InfluenceLoadResult::influenceLoadOrdinatesList(int index) const
{
    return *mInfluenceLoadOrdinatesLists.at(index);
}

void InfluenceLoadResult::setInfluenceLoadOrdinatesList(int index, const QList<qreal> &list)
{
    *mInfluenceLoadOrdinatesLists[index] = list;
}

void InfluenceLoadResult::appendInfluenceLoadOrdinatesListValue(int listIndex, qreal value)
{
    mInfluenceLoadOrdinatesLists[listIndex]->append(value);
}

qreal InfluenceLoadResult::minLoad(int index) const
{
    return mMinLoadList.at(index);
}

void InfluenceLoadResult::setMinLoad(int index, qreal minLoad)
{
    mMinLoadList[index] = minLoad;
}

const QString &InfluenceLoadResult::minloadPosition(int index) const
{
    return mMinLoadPositionList.at(index);
}

void InfluenceLoadResult::setMinLoadPosition(int index, const QString &minLoadPosition)
{
    mMinLoadPositionList[index] = minLoadPosition;
}

qreal InfluenceLoadResult::maxLoad(int index) const
{
    return mMaxLoadList.at(index);
}

void InfluenceLoadResult::setMaxLoad(int index, qreal maxLoad)
{
    mMaxLoadList[index] = maxLoad;
}

const QString &InfluenceLoadResult::maxloadPosition(int index) const
{
    return mMaxLoadPositionList.at(index);
}

void InfluenceLoadResult::setMaxLoadPosition(int index, const QString &maxLoadPosition)
{
    mMaxLoadPositionList[index] = maxLoadPosition;
}

void InfluenceLoadResult::resetParameters()
{
    mBarsCount      = 0;
    mOrdinatesCount = 0;

    foreach (QList<qreal> *list, mInfluenceLoadOrdinatesLists)
    {
        delete list;
    }

    mInfluenceLoadOrdinatesLists.clear();
    mMinLoadList.clear();
    mMinLoadPositionList.clear();
    mMaxLoadList.clear();
    mMaxLoadPositionList.clear();
}
