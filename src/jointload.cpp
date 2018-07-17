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

/* jointload.cpp */

#include "jointload.h"

JointLoad::JointLoad(Joint         *loadJoint,
                     qreal         horizontalComponent,
                     qreal         verticalComponent,
                     const QString &position,
                     QObject       *parent) : QObject(parent)
{
    mLoadJoint           = loadJoint;
    mHorizontalComponent = horizontalComponent;
    mVerticalComponent   = verticalComponent;
    mPosition            = position;
}

JointLoad::~JointLoad()
{

}

Joint *JointLoad::loadJoint() const
{
    return mLoadJoint;
}

qreal JointLoad::horizontalComponent()
{
    return mHorizontalComponent;
}

qreal JointLoad::verticalComponent()
{
    return mVerticalComponent;
}

const QString &JointLoad::position()
{
    return mPosition;
}

void JointLoad::setLoadJoint(Joint *loadJoint)
{
    mLoadJoint = loadJoint;
}

void JointLoad::setHorizontalComponent(qreal horizontalComponent)
{
    mHorizontalComponent = horizontalComponent;
}

void JointLoad::setVerticalComponent(qreal verticalComponent)
{
    mVerticalComponent = verticalComponent;
}

void JointLoad::setPosition(const QString &position)
{
    mPosition = position;
}
