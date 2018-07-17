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

/* thermaleffect.cpp */

#include "thermaleffect.h"

ThermalEffect::ThermalEffect(Bar     *bar,
                             qreal   temperatureChange,
                             qreal   thermalCoefficient,
                             QObject *parent) : QObject(parent)
{
    mBar                = bar;
    mTemperatureChange  = temperatureChange;
    mThermalCoefficient = thermalCoefficient;
}

ThermalEffect::~ThermalEffect()
{

}

Bar *ThermalEffect::thermalEffectBar() const
{
    return mBar;
}

qreal ThermalEffect::temperatureChange()
{
    return mTemperatureChange;
}

qreal ThermalEffect::thermalCoefficient()
{
    return mThermalCoefficient;
}

void ThermalEffect::setThermalEffectBar(Bar *bar)
{
    mBar = bar;
}

void ThermalEffect::setTemperatureChange(qreal temperatureChange)
{
    mTemperatureChange = temperatureChange;
}

void ThermalEffect::setThermalCoefficient(qreal thermalCoefficient)
{
    mThermalCoefficient = thermalCoefficient;
}

