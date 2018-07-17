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

/* jointload.h */

#ifndef JOINTLOAD_H
#define JOINTLOAD_H

#include <QObject>

#include "joint.h"

class JointLoad : public QObject
{
        Q_OBJECT

    public:
        explicit JointLoad(Joint         *loadJoint          = 0,
                           qreal         horizontalComponent = 0.0,
                           qreal         verticalComponent   = 0.0,
                           const QString &position           = QString("tail"),
                           QObject       *parent             = 0);

        ~JointLoad();

        Joint *loadJoint() const;

        qreal horizontalComponent();

        qreal verticalComponent();

        const QString &position();

        void setLoadJoint(Joint *loadJoint);

        void setHorizontalComponent(qreal horizontalComponent);

        void setVerticalComponent(qreal verticalComponent);

        void setPosition(const QString &position);

    private:
        Joint   *mLoadJoint;
        qreal   mHorizontalComponent;
        qreal   mVerticalComponent;
        QString mPosition;
};

#endif // JOINTLOAD_H
