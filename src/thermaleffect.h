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

/* thermaleffect.h */

#ifndef THERMALEFFECT_H
#define THERMALEFFECT_H

#include <QObject>

#include "bar.h"

class ThermalEffect : public QObject
{
        Q_OBJECT

    public:
    explicit ThermalEffect(Bar     *bar               = 0,
                           qreal   temperatureChange  = 0.0,
                           qreal   thermalCoefficient = 0.0,
                           QObject *parent            = 0);

        ~ThermalEffect();

        Bar *thermalEffectBar() const;

        qreal temperatureChange();

        qreal thermalCoefficient();

        void setThermalEffectBar(Bar *bar);

        void setTemperatureChange(qreal temperatureChange);

        void setThermalCoefficient(qreal thermalCoefficient);

    private:
        Bar   *mBar;
        qreal mTemperatureChange;
        qreal mThermalCoefficient;
};

#endif // THERMALEFFECT_H
