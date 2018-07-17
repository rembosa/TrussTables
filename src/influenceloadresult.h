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

/* influenceloadresult.h */

#ifndef INFLUENCELOADRESULT_H
#define INFLUENCELOADRESULT_H

#include <QObject>
#include <QStringList>

class InfluenceLoadResult : public QObject
{
        Q_OBJECT

    public:
        explicit InfluenceLoadResult(int barsCount, int ordinatesCount, QObject *parent = 0);
        ~InfluenceLoadResult();

    public slots:
        int barsCount() const;
        int ordinatesCount() const;
        void setParameters(int barsCount, int ordinatesCount);
        const QList<qreal> &influenceLoadOrdinatesList(int index) const;
        void setInfluenceLoadOrdinatesList(int index, const QList<qreal> &list);
        void appendInfluenceLoadOrdinatesListValue(int listIndex, qreal value);
        qreal minLoad(int index) const;
        void setMinLoad(int index, qreal minLoad);
        const QString &minloadPosition(int index) const;
        void setMinLoadPosition(int index, const QString &minLoadPosition);
        qreal maxLoad(int index) const;
        void setMaxLoad(int index, qreal maxLoad);
        const QString &maxloadPosition(int index) const;
        void setMaxLoadPosition(int index, const QString &maxLoadPosition);
        void resetParameters();

    private:
        int                  mBarsCount;
        int                  mOrdinatesCount;
        QList<QList<qreal>*> mInfluenceLoadOrdinatesLists;
        QList<qreal>         mMinLoadList;
        QStringList          mMinLoadPositionList;
        QList<qreal>         mMaxLoadList;
        QStringList          mMaxLoadPositionList;
};

#endif // INFLUENCELOADRESULT_H
