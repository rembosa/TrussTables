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

/* joint.cpp */

#include "joint.h"

Joint::Joint(qreal   xCoordinate,
             qreal   yCoordinate,
             QObject *parent) : QObject(parent)
{
    mXCoordinate = xCoordinate;
    mYCoordinate = yCoordinate;
    mIsSupported = false;
}

Joint::~Joint()
{

}

qreal Joint::xCoordinate() const
{
    return mXCoordinate;
}

qreal Joint::yCoordinate() const
{
    return mYCoordinate;
}

bool Joint::isSupported() const
{
    return mIsSupported;
}

void Joint::setXCoordinate(qreal xCoordinate)
{
    mXCoordinate = xCoordinate;
}

void Joint::setYCoordinate(qreal yCoordinate)
{
    mYCoordinate = yCoordinate;
}

void Joint::setSupported(bool isSupported)
{
    mIsSupported = isSupported;
}

QList<Joint *> *Joint::connectedJoints()
{
    return &mConnectedJoints;
}

QList<Bar *> *Joint::attachedBars()
{
    return &mAttachedBars;
}
