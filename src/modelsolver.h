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

/* modelsolver.h */

#ifndef MODELSOLVER_H
#define MODELSOLVER_H

#include <cmath>

#include <QMessageBox>
#include <QThread>
#include <QtMath>

#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_spblas.h>
#include <gsl/gsl_splinalg.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_vector.h>

#include "bar.h"
#include "fabricationerror.h"
#include "influenceload.h"
#include "influenceloadresult.h"
#include "joint.h"
#include "jointload.h"
#include "support.h"
#include "supportsettlement.h"
#include "thermaleffect.h"
#include "unitsandlimits.h"

class ModelSolver : public QThread
{
        Q_OBJECT

    public:
        ModelSolver(const QList<Joint *>             &jointsList,
                    const QList<Bar *>               &barsList,
                    const QList<Support *>           &supportsList,
                    const QList<JointLoad *>         &jointLoadsList,
                    bool                             includeSelfWeight,
                    bool                             areaModulusOption,
                    const QList<SupportSettlement *> &supportSettlementsList,
                    const QList<ThermalEffect *>     &thermalEffectsList,
                    const QList<FabricationError *>  &fabricationErrorsList,
                    const QList<InfluenceLoad *>     &influenceLoadsList,
                    const QString                    &influenceLoadName,
                    InfluenceLoadResult              *influenceLoadResult,
                    const UnitsAndLimits             &unitsAndLimits,
                    QObject                          *parent = 0);

        ~ModelSolver();

        static const qreal kBigFactor      = 1.0e+5;
        static const qreal kTolerance      = 1.0e-14;
        static const size_t kMaxIterations = 1000;

//        void printVector(const char *caption, gsl_vector *vector);

    protected:
        void run();

    signals:
        void hasSolution();
        void jointHorizontalDeflectionsSignal(QList<qreal> horizontalDeflectionComponentsList);
        void jointVerticalDeflectionsSignal(QList<qreal> verticalDeflectionComponentsList);
        void barLoadsSignal(QList<qreal> barLoadsList);
        void reactionHorizontalComponentsSignal(QList<qreal> reactionHorizontalComponentsList);
        void reactionVerticalComponentsSignal(QList<qreal> reactionVerticalComponentsList);
        void notesSignal(QString note);

    private:
        QList<Joint *>             mJointsList;
        QList<Bar *>               mBarsList;
        QList<Support *>           mSupportsList;
        QList<JointLoad *>         mJointLoadsList;
        bool                       mIncludeSelfWeight;
        bool                       mAreaModulusOption;
        QList<SupportSettlement *> mSupportSettlementsList;
        QList<ThermalEffect *>     mThermalEffectsList;
        QList<FabricationError *>  mFabricationErrorsList;
        QList<InfluenceLoad *>     mInfluenceLoadsList;
        QString                    mInfluenceLoadName;
        InfluenceLoadResult        *mInfluenceLoadResult;
        UnitsAndLimits             mUnitsAndLimits;
        QList<bool>                mSolutionsCount;
};

#endif // MODELSOLVER_H
