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

/* bar.cpp */

#include "bar.h"

Bar::Bar(Joint   *firstJoint,
         Joint   *secondJoint,
         qreal   area,
         qreal   modulus,
         qreal   factor,
         qreal   unitWeight,
         QObject *parent) : QObject(parent)
{
    mFirstJoint  = firstJoint;
    mSecondJoint = secondJoint;
    mArea        = area;
    mModulus     = modulus;
    mFactor      = factor;
    mUnitWeight  = unitWeight;
}

Bar::~Bar()
{

}

Joint *Bar::firstJoint() const
{
    return mFirstJoint;
}

Joint *Bar::secondJoint() const
{
    return mSecondJoint;
}

qreal Bar::area() const
{
    return mArea;
}

qreal Bar::modulus() const
{
    return mModulus;
}

qreal Bar::factor() const
{
    return mFactor;
}

qreal Bar::unitWeight() const
{
    return mUnitWeight;
}

void Bar::setFirstJoint(Joint *joint)
{
    mFirstJoint = joint;
}

void Bar::setSecondJoint(Joint *joint)
{
    mSecondJoint = joint;
}

void Bar::setArea(qreal area)
{
    mArea = area;
}

void Bar::setModulus(qreal modulus)
{
    mModulus = modulus;
}

void Bar::setFactor(qreal factor)
{
    mFactor = factor;
}

void Bar::setUnitWeight(qreal unitWeight)
{
    mUnitWeight = unitWeight;
}
