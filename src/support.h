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

/* support.h */

#ifndef SUPPORT_H
#define SUPPORT_H

#include <QObject>

#include "joint.h"
#include "unitsandlimits.h"

class Support : public QObject
{
        Q_OBJECT
    public:
        explicit Support(UnitsAndLimits::SupportType type    = UnitsAndLimits::FIXED_BOTTOM,
                         qreal                       angle   = 0.0,
                         Joint                       *joint  = 0,
                         QObject                     *parent = 0);

        ~Support();

        UnitsAndLimits::SupportType type() const;

        qreal angle() const;

        Joint *supportJoint() const;

        void setSupportType(UnitsAndLimits::SupportType type);

        void setSupportJoint(Joint *joint);

        void setSupportAngle(qreal angle);

    private:
        UnitsAndLimits::SupportType mType;
        qreal                       mAngle;
        Joint                       *mSupportJoint;
};

#endif // SUPPORT_H
