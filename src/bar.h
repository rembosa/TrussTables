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

/* bar.h */

#ifndef BAR_H
#define BAR_H

#include <QObject>

class Joint;

class Bar : public QObject
{
        Q_OBJECT

    public:
        explicit Bar(Joint   *firstJoint  = 0,
                     Joint   *secondJoint = 0,
                     qreal   area         = 0.0,
                     qreal   modulus      = 0.0,
                     qreal   factor       = 0.0,
                     qreal   unitWeight   = 0.0,
                     QObject *parent      = 0);

        ~Bar();

        Joint *firstJoint() const;

        Joint *secondJoint() const;

        qreal area() const;

        qreal modulus() const;

        qreal factor() const;

        qreal unitWeight() const;

        void setFirstJoint(Joint *joint);

        void setSecondJoint(Joint *joint);

        void setArea(qreal area);

        void setModulus(qreal modulus);

        void setFactor(qreal factor);

        void setUnitWeight(qreal unitWeight);

    private:
        Joint *mFirstJoint;
        Joint *mSecondJoint;
        qreal mArea;
        qreal mModulus;
        qreal mFactor;
        qreal mUnitWeight;
};

#endif // BAR_H
