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

/* influenceload.h */

#ifndef INFLUENCELOAD_H
#define INFLUENCELOAD_H

#include <QObject>

class InfluenceLoad : public QObject
{
        Q_OBJECT

    public:
        explicit InfluenceLoad(const QString      &name               = QString(""),
                               const QString      &direction          = QString("LR"),
                               const QList<int>   &path               = QList<int>(),
                               const QList<qreal> &pointLoads         = QList<qreal>(),
                               const QList<qreal> &pointLoadPositions = QList<qreal>(),
                               QObject            *parent             = 0);

        ~InfluenceLoad();

        const QString &name();

        const QString &direction();

        const QList<int> &path();

        const QList<qreal> &pointLoads();

        const QList<qreal> &pointLoadPositions();

        void setName(const QString &name);

        void setDirection(const QString &direction);

        void setPath(const QList<int> &path);

        void setPointLoads(const QList<qreal> &pointLoads);

        void setPointLoadPositions(const QList<qreal> &pointLoadPositions);

    private:
        QString      mName;
        QString      mDirection;
        QList<int>   mPath;
        QList<qreal> mPointLoads;
        QList<qreal> mPointLoadPositions;
};

#endif // INFLUENCELOAD_H
