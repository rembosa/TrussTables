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

/* influenceload.cpp */

#include "influenceload.h"

InfluenceLoad::InfluenceLoad(const QString      &name,
                             const QString      &direction,
                             const QList<int>   &path,
                             const QList<qreal> &pointLoads,
                             const QList<qreal> &pointLoadPositions,
                             QObject            *parent) : QObject(parent)
{
    mName               = name;
    mDirection          = direction;
    mPath               = path;
    mPointLoads         = pointLoads;
    mPointLoadPositions = pointLoadPositions;
}

InfluenceLoad::~InfluenceLoad()
{

}

const QString &InfluenceLoad::name()
{
    return mName;
}

const QString &InfluenceLoad::direction()
{
    return mDirection;
}

const QList<int> &InfluenceLoad::path()
{
    return mPath;
}

const QList<qreal> &InfluenceLoad::pointLoads()
{
    return mPointLoads;
}

const QList<qreal> &InfluenceLoad::pointLoadPositions()
{
    return mPointLoadPositions;
}

void InfluenceLoad::setName(const QString &name)
{
    mName = name;
}

void InfluenceLoad::setDirection(const QString &direction)
{
    mDirection = direction;
}

void InfluenceLoad::setPath(const QList<int> &path)
{
    mPath = path;
}

void InfluenceLoad::setPointLoads(const QList<qreal> &pointLoads)
{
    mPointLoads = pointLoads;
}

void InfluenceLoad::setPointLoadPositions(const QList<qreal> &pointLoadPositions)
{
    mPointLoadPositions = pointLoadPositions;
}
