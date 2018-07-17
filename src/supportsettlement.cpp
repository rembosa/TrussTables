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

/* supportsettlement.cpp */

#include "supportsettlement.h"

SupportSettlement::SupportSettlement(Support *support,
                                     qreal   settlement,
                                     QObject *parent) : QObject(parent)
{
    mSupport    = support;
    mSettlement = settlement;
}

SupportSettlement::~SupportSettlement()
{

}

Support *SupportSettlement::settlementSupport() const
{
    return mSupport;
}

qreal SupportSettlement::settlement()
{
    return mSettlement;
}

void SupportSettlement::setSettlementSupport(Support *support)
{
    mSupport = support;
}

void SupportSettlement::setSettlement(qreal settlement)
{
    mSettlement = settlement;
}

