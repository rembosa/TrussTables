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

/* htmlreportexporter.h */

#ifndef HTMLREPORTEXPORTER_H
#define HTMLREPORTEXPORTER_H

#include <QCheckBox>
#include <QDate>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QObject>
#include <QStandardItemModel>
#include <QTextStream>
#include <QTime>
#include <QUrl>

#include "bar.h"
#include "fabricationerror.h"
#include "influenceload.h"
#include "influenceloadresult.h"
#include "joint.h"
#include "jointload.h"
#include "modelviewer.h"
#include "notesdialog.h"
#include "support.h"
#include "supportsettlement.h"
#include "thermaleffect.h"
#include "unitsandlimits.h"

class HtmlReportExporter : public QObject
{
        Q_OBJECT

    public:
        explicit HtmlReportExporter(const QString              &title                              = QString(),
                                    QString                    *descriptionText                    = 0,
                                    const QString              &exportDirectory                    = QString(),
                                    QList<Joint *>             *jointsList                         = 0,
                                    QList<Bar *>               *barsList                           = 0,
                                    QStandardItemModel         *barsStandardItemModel              = 0,
                                    QList<Support *>           *supportsList                       = 0,
                                    ModelViewer                *modelVIewer                        = 0,
                                    QCheckBox                  *jointLoadsCheckBox                 = 0,
                                    QCheckBox                  *selfWeightCheckBox                 = 0,
                                    QCheckBox                  *supportSettlementsCheckBox         = 0,
                                    QCheckBox                  *thermalEffectsCheckBox             = 0,
                                    QCheckBox                  *fabricationErrorsCheckBox          = 0,
                                    QCheckBox                  *influenceLoadsCheckBox             = 0,
                                    QList<JointLoad *>         *jointLoadsList                     = 0,
                                    QList<SupportSettlement *> *supportSettlementsList             = 0,
                                    QList<ThermalEffect *>     *thermalEffectsList                 = 0,
                                    QList<FabricationError *>  *fabricationErrorsList              = 0,
                                    QList<InfluenceLoad *>     *influenceLoadsList                 = 0,
                                    QList<qreal>               *horizontalDeflectionComponentsList = 0,
                                    QList<qreal>               *verticalDeflectionComponentsList   = 0,
                                    QList<qreal>               *barLoadsList                       = 0,
                                    QList<qreal>               *reactionHorizontalComponentsList   = 0,
                                    QList<qreal>               *reactionVerticalComponentsList     = 0,
                                    QString                    *solutionInfluenceLoadName          = 0,
                                    InfluenceLoadResult        *influenceLoadResult                = 0,
                                    UnitsAndLimits             *unitsAndLimits                     = 0,
                                    QObject                    *parent                             = 0);

        ~HtmlReportExporter();

    public slots:
        void exportHtmlReport();

    private:
        QString                    mTitle;
        QString                    *mDescriptionText;
        QString                    mExportDirectory;
        QList<Joint *>             *mJointsList;
        QList<Bar *>               *mBarsList;
        QStandardItemModel         *mBarsStandardItemModel;
        QList<Support *>           *mSupportsList;
        ModelViewer                *mModelViewer;
        QCheckBox                  *mJointLoadsCheckBox;
        QCheckBox                  *mSelfWeightCheckBox;
        QCheckBox                  *mSupportSettlementsCheckBox;
        QCheckBox                  *mThermalEffectsCheckBox;
        QCheckBox                  *mFabricationErrorsCheckBox;
        QCheckBox                  *mInfluenceLoadsCheckBox;
        QList<JointLoad *>         *mJointLoadsList;
        QList<SupportSettlement *> *mSupportSettlementsList;
        QList<ThermalEffect *>     *mThermalEffectsList;
        QList<FabricationError *>  *mFabricationErrorsList;
        QList<InfluenceLoad *>     *mInfluenceLoadsList;
        QList<qreal>               *mHorizontalDeflectionComponentsList;
        QList<qreal>               *mVerticalDeflectionComponentsList;
        QList<qreal>               *mBarLoadsList;
        QList<qreal>               *mReactionHorizontalComponentsList;
        QList<qreal>               *mReactionVerticalComponentsList;
        QString                    *mSolutionInfluenceLoadName;
        InfluenceLoadResult        *mInfluenceLoadResult;
        UnitsAndLimits             *mUnitsAndLimits;
};

#endif // HTMLREPORTEXPORTER_H
