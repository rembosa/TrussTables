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

/* modelchecker.h */

#ifndef MODELCHECKER_H
#define MODELCHECKER_H

#include <cmath>

#include <QPointF>
#include <QThread>
#include <QtMath>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>

#include "bar.h"
#include "joint.h"
#include "support.h"
#include "unitsandlimits.h"

class ModelChecker : public QThread
{
        Q_OBJECT

    public:
        ModelChecker(const QList<Joint *>   &jointsList,
                     const QList<Bar *>     &barsList,
                     const QList<Support *> &supportsList,
                     const UnitsAndLimits   &unitsAndLimits,
                     QObject                *parent = 0);

        ~ModelChecker();

        Support *jointSupport(Joint *joint);

    protected:
        void run();

    signals:
        void notesSignal(QString note);
        void redundantBarsListSignal(QList<Bar* > barsList);
        void redundantSupportsListSignal(QList<Support* > supportsList);
        void modelStabilitySignal(bool stability);
        void progressSignal(int progress);

    private:
        QList<Joint *>   mJointsList;
        QList<Bar *>     mBarsList;
        QList<Support *> mSupportsList;
        UnitsAndLimits   mUnitsAndLimits;
        int              mDegreeOfIndeterminacy;

        //Check if structure has disconnected parts
        bool hasDisconnectedParts();

        //Check if structure has dangling bar
        bool hasDanglingBar();

        //Check if structure has all joints fixed
        bool hasFixedJoints();

        //Check for instability due to insufficient supports
        bool hasInsufficientSupports();

        //Check for instability due to insufficient bars
        bool hasInsufficientBars();

        //Check for bars with fixed ends
        bool hasFixedBars();

        //Check for instability due to bars configuration
        bool hasUnstableConfiguration(gsl_matrix *matrix = 0);
};

#endif // MODELCHECKER_H
