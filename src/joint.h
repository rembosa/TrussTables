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

/* joint.h */

#ifndef JOINT_H
#define JOINT_H

#include <QObject>

class Bar;

class Joint : public QObject
{
        Q_OBJECT

    public:
        explicit Joint(qreal   xCoordinate = 0.0,
                       qreal   yCoordinate = 0.0,
                       QObject *parent     = 0);

        ~Joint();

        qreal xCoordinate() const;

        qreal yCoordinate() const;

        bool isSupported() const;

        void setXCoordinate(qreal xCoordinate);

        void setYCoordinate(qreal yCoordinate);

        void setSupported(bool isSupported);

        QList<Joint *> *connectedJoints();

        QList<Bar *> *attachedBars();

    private:
        qreal          mXCoordinate;
        qreal          mYCoordinate;
        bool           mIsSupported;
        QList<Joint *> mConnectedJoints;
        QList<Bar *>   mAttachedBars;
};

#endif // JOINT_H
