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

/* modelviewer.h */

#ifndef MODELVIEWER_H
#define MODELVIEWER_H

#include <cmath>

#include <QApplication>
#include <QFontMetricsF>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QStandardItemModel>
#include <QtMath>
#include <QWidget>

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

class ModelViewer : public QWidget
{
        Q_OBJECT

    public:
        ModelViewer(UnitsAndLimits             *unitsAndLimits                     = 0,
                    QList<Joint *>             *jointsList                         = 0,
                    QList<Bar *>               *barsList                           = 0,
                    QList<Support *>           *supportsList                       = 0,
                    QList<JointLoad *>         *jointLoadsList                     = 0,
                    QList<SupportSettlement *> *supportSettlementsList             = 0,
                    QList<ThermalEffect *>     *thermalEffectsList                 = 0,
                    QList<FabricationError *>  *fabricationErrorsList              = 0,
                    QList<InfluenceLoad *>     *influenceLoadsList                 = 0,
                    QStandardItemModel         *barsStandardItemModel              = 0,
                    QList<qreal>               *horizontalDeflectionComponentsList = 0,
                    QList<qreal>               *verticalDeflectionComponentsList   = 0,
                    QList<qreal>               *barLoadsList                       = 0,
                    QList<qreal>               *reactionHorizontalComponentsList   = 0,
                    QList<qreal>               *reactionVerticalComponentsList     = 0,
                    QString                    *solutionInfluenceLoadName          = 0,
                    InfluenceLoadResult        *influenceLoadResult                = 0,
                    QWidget                    *parent                             = 0);

        ~ModelViewer();

        enum LoadDisplayOption
        {
            JOINT_LOAD,
            JOINT_LOAD_WITH_SELF_WEIGHT,
            SELF_WEIGHT,
            SUPPORT_SETTLEMENT,
            THERMAL_EFFECT,
            ASSEMBLY_FABRICATION_ERROR,
            INFLUENCE_LOAD,
            NO_LOAD
        };

        enum SolutionDisplayOption
        {
            JOINT_DEFLECTION,
            BAR_LOAD,
            SUPPORT_REACTION,
            INFLUENCE_LOAD_SOLUTION,
            NO_SOLUTION
        };

        QSize sizeHint() const;

        static const int   kBorderOffset  = 10;
        static const int   kDefaultWidth  = 400;
        static const int   kDefaultHeight = 400;
        static const qreal kRadius        = 2.5;

        QPoint point(int x, int y) const;

        QPointF point(qreal x, qreal y) const;

        void updateModelPixmap();
        QPixmap *modelPixmap();
        QImage modelImage(LoadDisplayOption     loadDisplayOption,
                          SolutionDisplayOption solutionDisplayOption,
                          int                   influenceLoadBarIndex);

    protected:
        void paintEvent(QPaintEvent *);
        void mouseMoveEvent(QMouseEvent *event);

    signals:
        void positionSignal(QString &position);

    public slots:
        void updateSize(int width,
                        int height,
                        int marginOffset);
        void updateModel();
        void showHideAxes(bool show);
        void showHideJointNumber(bool show);
        void showHideBarNumber(bool show);
        void updateLoadDisplayOption(ModelViewer::LoadDisplayOption option);
        void updateForceScaleFactor(qreal factor);
        void updateSolutionDisplayOption(ModelViewer::SolutionDisplayOption option, int influenceLoadBarIndex);
        void updateDeflectionScaleFactor(qreal factor);

    private:
        QPixmap                    *mModelPixmap;
        QList<Joint *>             *mJointsList;
        QList<Bar *>               *mBarsList;
        QList<Support *>           *mSupportsList;
        QList<JointLoad *>         *mJointLoadsList;
        QList<SupportSettlement *> *mSupportSettlementsList;
        QList<ThermalEffect *>     *mThermalEffectsList;
        QList<FabricationError *>  *mFabricationErrorsList;
        QList<InfluenceLoad *>     *mInfluenceLoadsList;
        QStandardItemModel         *mBarsStandardItemModel;
        QList<qreal>               *mHorizontalDeflectionComponentsList;
        QList<qreal>               *mVerticalDeflectionComponentsList;
        QList<qreal>               *mBarLoadsList;
        QList<qreal>               *mReactionHorizontalComponentsList;
        QList<qreal>               *mReactionVerticalComponentsList;
        QString                    *mSolutionInfluenceLoadName;
        InfluenceLoadResult        *mInfluenceLoadResult;
        qreal                      mCoordinateScaleFactor;
        UnitsAndLimits             *mUnitsAndLimits;
        bool                       mDrawCoordinateAxes;
        bool                       mDrawJointNumber;
        bool                       mDrawBarNumber;
        int                        mMarginOffset;
        LoadDisplayOption          mLoadDisplayOption;
        SolutionDisplayOption      mSolutionDisplayOption;
        int                        mInfluenceLoadBarIndex;
        qreal                      mForceScaleFactor;
        qreal                      mDeflectionScaleFactor;
};

#endif // MODELVIEWER_H
