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

/* htmlreportexporter.cpp */

#include "htmlreportexporter.h"

HtmlReportExporter::HtmlReportExporter(const QString              &title,
                                       QString                    *descriptionText,
                                       const QString              &exportDirectory,
                                       QList<Joint *>             *jointsList,
                                       QList<Bar *>               *barsList,
                                       QStandardItemModel         *barsStandardItemModel,
                                       QList<Support *>           *supportsList,
                                       ModelViewer                *modelVIewer,
                                       QCheckBox                  *jointLoadsCheckBox,
                                       QCheckBox                  *selfWeightCheckBox,
                                       QCheckBox                  *supportSettlementsCheckBox,
                                       QCheckBox                  *thermalEffectsCheckBox,
                                       QCheckBox                  *fabricationErrorsCheckBox,
                                       QCheckBox                  *influenceLoadsCheckBox,
                                       QList<JointLoad *>         *jointLoadsList,
                                       QList<SupportSettlement *> *supportSettlementsList,
                                       QList<ThermalEffect *>     *thermalEffectsList,
                                       QList<FabricationError *>  *fabricationErrorsList,
                                       QList<InfluenceLoad *>     *influenceLoadsList,
                                       QList<qreal>               *horizontalDeflectionComponentsList,
                                       QList<qreal>               *verticalDeflectionComponentsList,
                                       QList<qreal>               *barLoadsList,
                                       QList<qreal>               *reactionHorizontalComponentsList,
                                       QList<qreal>               *reactionVerticalComponentsList,
                                       QString                    *solutionInfluenceLoadName,
                                       InfluenceLoadResult        *influenceLoadResult,
                                       UnitsAndLimits             *unitsAndLimits,
                                       QObject                    *parent) : QObject(parent)
{
    mTitle                              = title;
    mDescriptionText                    = descriptionText;
    mExportDirectory                    = exportDirectory;
    mJointsList                         = jointsList;
    mBarsList                           = barsList;
    mBarsStandardItemModel              = barsStandardItemModel;
    mSupportsList                       = supportsList;
    mModelViewer                        = modelVIewer;
    mJointLoadsCheckBox                 = jointLoadsCheckBox;
    mSelfWeightCheckBox                 = selfWeightCheckBox;
    mSupportSettlementsCheckBox         = supportSettlementsCheckBox;
    mThermalEffectsCheckBox             = thermalEffectsCheckBox;
    mFabricationErrorsCheckBox          = fabricationErrorsCheckBox;
    mInfluenceLoadsCheckBox             = influenceLoadsCheckBox;
    mJointLoadsList                     = jointLoadsList;
    mSupportSettlementsList             = supportSettlementsList;
    mThermalEffectsList                 = thermalEffectsList;
    mFabricationErrorsList              = fabricationErrorsList;
    mInfluenceLoadsList                 = influenceLoadsList;
    mHorizontalDeflectionComponentsList = horizontalDeflectionComponentsList;
    mVerticalDeflectionComponentsList   = verticalDeflectionComponentsList;
    mBarLoadsList                       = barLoadsList;
    mReactionHorizontalComponentsList   = reactionHorizontalComponentsList;
    mReactionVerticalComponentsList     = reactionVerticalComponentsList;
    mSolutionInfluenceLoadName          = solutionInfluenceLoadName;
    mInfluenceLoadResult                = influenceLoadResult;
    mUnitsAndLimits                     = unitsAndLimits;
}

HtmlReportExporter::~HtmlReportExporter()
{

}

void HtmlReportExporter::exportHtmlReport()
{
    QString fileName = mExportDirectory + tr("/%1_report.html").arg(mTitle);
    QFile dataFile(fileName);

    if (dataFile.open(QFile::WriteOnly | QFile::Text | QFile::Truncate))
    {
        QTextStream dataTextStream(&dataFile);

        QString htmlText;
        htmlText.append("<!DOCTYPE HTML>\n");
        htmlText.append("<html lang=\"en_US\">\n");
        htmlText.append("<head>\n");
        htmlText.append("<meta charset=\"UTF-8\">\n");
        htmlText.append("<title>TrussTables Analysis Report</title>\n");
        htmlText.append("<style>\n");
        htmlText.append("table, th, td {\n");
        htmlText.append("border: 2px solid black;\n");
        htmlText.append("border-collapse: collapse;\n");
        htmlText.append("padding: 5px;\n");
        htmlText.append("font-family: \"arial\", Georgia, Serif;\n");
        htmlText.append("}\n");
        htmlText.append("th, caption {\n");
        htmlText.append("text-align: left;\n");
        htmlText.append("}\n");
        htmlText.append("p, pre, h1 {\n");
        htmlText.append("font-family: \"arial\", Georgia, Serif;\n");
        htmlText.append("}\n");
        htmlText.append("</style>\n");
        htmlText.append("</head>\n");
        htmlText.append("<body>\n");
        htmlText.append("<h1>ANALYSIS REPORT</h1>\n");
        htmlText.append(tr("<p>Title: %1</p>\n").arg(mTitle));

        {//Project Details

            if (!mDescriptionText->isEmpty())
            {
                htmlText.append("<p>Description:</p>\n");
                htmlText.append("<pre>\n");
                htmlText.append(tr("%1\n").arg(*mDescriptionText));
                htmlText.append("</pre>\n");
            }

            htmlText.append(tr("<p>Date: %1</p>\n").arg(QDate::currentDate().toString(tr("dd/MMM/yy"))));
            htmlText.append(tr("<p>Time: %1</p>\n").arg(QTime::currentTime().toString(tr("hh:mm ap"))));
        }

        {//Model Details

            htmlText.append("<br><table>\n");
            htmlText.append("<caption><b>Model&nbsp;Details</b></caption>\n");
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Joints</td>\n"));
            htmlText.append(tr("<td>%1</td>\n").arg(mJointsList->size()));
            htmlText.append("</tr>\n");
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Axial Rigidity Option</td>\n"));

            if (mBarsStandardItemModel->columnCount() == 5)
            {
                htmlText.append(tr("<td>%1</td>\n").arg(tr("value")));
            }
            else
            {
                htmlText.append(tr("<td>%1</td>\n").arg(tr("factor")));
            }

            htmlText.append("</tr>\n");
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Bars</td>\n"));
            htmlText.append(tr("<td>%1</td>\n").arg(mBarsList->size()));
            htmlText.append("</tr>\n");
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Supports</td>\n"));
            htmlText.append(tr("<td>%1</td>\n").arg(mSupportsList->size()));
            htmlText.append("</tr>\n");
            htmlText.append("</table>\n");
        }

        {//Units

            htmlText.append("<br><br><table>\n");
            htmlText.append("<caption><b>Units</b></caption>\n");

            //System
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>System</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                htmlText.append(tr("<td>%1</td>\n").arg(tr("metric")));
            }
            else
            {
                htmlText.append(tr("<td>%1</td>\n").arg(tr("imperial")));
            }

            htmlText.append("</tr>\n");

            //Coordinates
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Coordinates</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->coordinateUnit() == tr("m"))
                {
                    htmlText.append("<td>m</td>\n");
                }
                else if (mUnitsAndLimits->coordinateUnit() == tr("cm"))
                {
                    htmlText.append("<td>cm</td>\n");
                }
                else
                {
                    htmlText.append("<td>mm</td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->coordinateUnit() == tr("ft"))
                {
                    htmlText.append("<td>ft</td>\n");
                }
                else
                {
                    htmlText.append("<td>in.</td>\n");
                }
            }

            htmlText.append("</tr>\n");

            //Area of Section
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Area of Section</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->areaUnit() == tr("m%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>m<sup>2</sup></td>\n");
                }
                else if (mUnitsAndLimits->areaUnit() == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>cm<sup>2</sup></td>\n");
                }
                else
                {
                    htmlText.append("<td>mm<sup>2</sup></td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->areaUnit() == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>ft<sup>2</sup></td>\n");
                }
                else
                {
                    htmlText.append("<td>in.<sup>2</sup></td>\n");
                }
            }

            htmlText.append("</tr>\n");

            //Modulus of Elasticity
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Modulus of Elasticity</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->modulusUnit() == tr("N/m%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>N/m<sup>2</sup></td>\n");
                }
                else if (mUnitsAndLimits->modulusUnit() == tr("N/mm%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>N/mm<sup>2</sup></td>\n");
                }
                else
                {
                    htmlText.append("<td>GPa</td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->modulusUnit() == tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>lb/ft<sup>2</sup></td>\n");
                }
                else if (mUnitsAndLimits->modulusUnit() == tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")))
                {
                    htmlText.append("<td>lb/in.<sup>2</sup></td>\n");
                }
                else
                {
                    htmlText.append("<td>kip/in.<sup>2</sup></td>\n");
                }
            }

            htmlText.append("</tr>\n");

            //Unit Weight
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Unit Weight</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                htmlText.append("<td>kN/m<sup>3</sup></td>\n");
            }
            else
            {
                htmlText.append("<td>lb/ft<sup>3</sup></td>\n");
            }

            htmlText.append("</tr>\n");

            //Load
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Load</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->loadUnit() == tr("N"))
                {
                    htmlText.append("<td>N</td>\n");
                }
                else
                {
                    htmlText.append("<td>kN</td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->loadUnit() == tr("lb"))
                {
                    htmlText.append("<td>lb</td>\n");
                }
                else
                {
                    htmlText.append("<td>kip</td>\n");
                }
            }

            htmlText.append("</tr>\n");

            //Support Settlement
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Support Settlement</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->supportSettlementUnit() == tr("m"))
                {
                    htmlText.append("<td>m</td>\n");
                }
                else
                {
                    htmlText.append("<td>mm</td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->supportSettlementUnit() == tr("ft"))
                {
                    htmlText.append("<td>ft</td>\n");
                }
                else
                {
                    htmlText.append("<td>in.</td>\n");
                }
            }

            htmlText.append("</tr>\n");

            //Temperature Change
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Temperature Change</td>\n"));

            if (mUnitsAndLimits->temperatureChangeUnit() == tr("%1C").arg(QString::fromUtf8("\u00B0")))
            {
                htmlText.append("<td><sup>o</sup>C</td>\n");
            }
            else
            {
                htmlText.append("<td><sup>o</sup>F</td>\n");
            }

            htmlText.append("</tr>\n");

            //Thermal Coefficient
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Thermal Coefficient</td>\n"));

            if (mUnitsAndLimits->thermalCoefficientUnit() == tr("/%1C").arg(QString::fromUtf8("\u00B0")))
            {
                htmlText.append("<td>/<sup>o</sup>C</td>\n");
            }
            else
            {
                htmlText.append("<td>/<sup>o</sup>F</td>\n");
            }

            htmlText.append("</tr>\n");

            //Length Error
            htmlText.append("<tr>\n");
            htmlText.append(tr("<td>Length Error</td>\n"));

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->lengthErrorUnit() == tr("m"))
                {
                    htmlText.append("<td>m</td>\n");
                }
                else
                {
                    htmlText.append("<td>mm</td>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->lengthErrorUnit() == tr("ft"))
                {
                    htmlText.append("<td>ft</td>\n");
                }
                else
                {
                    htmlText.append("<td>in.</td>\n");
                }
            }

            htmlText.append("</tr>\n");

            htmlText.append("</table>\n");
        }

        int count = 0;

        {//Joints

            htmlText.append("<br><br><table>\n");
            htmlText.append("<caption><b>Joints</b></caption>\n");

            htmlText.append("<tr>\n");
            htmlText.append("<th>#</th>\n");

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                if (mUnitsAndLimits->coordinateUnit() == tr("m"))
                {
                    htmlText.append("<th>x (m)</th>\n");
                    htmlText.append("<th>y (m)</th>\n");
                }
                else if (mUnitsAndLimits->coordinateUnit() == tr("cm"))
                {
                    htmlText.append("<th>x (cm)</th>\n");
                    htmlText.append("<th>y (cm)</th>\n");
                }
                else
                {
                    htmlText.append("<th>x (mm)</th>\n");
                    htmlText.append("<th>y (mm)</th>\n");
                }
            }
            else
            {
                if (mUnitsAndLimits->coordinateUnit() == tr("ft"))
                {
                    htmlText.append("<th>x (ft)</th>\n");
                    htmlText.append("<th>y (ft)</th>\n");
                }
                else
                {
                    htmlText.append("<th>x (in.)</th>\n");
                    htmlText.append("<th>y (in.)</th>\n");
                }
            }

            htmlText.append("</tr>\n");

            count = mJointsList->size();

            for (int i = 0; i < count; ++i)
            {
                qreal x = mJointsList->at(i)->xCoordinate();
                qreal y = mJointsList->at(i)->yCoordinate();
                htmlText.append("<tr>\n");
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(x, 'f', mUnitsAndLimits->deltaXDecimals())));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(y, 'f', mUnitsAndLimits->deltaYDecimals())));
                htmlText.append("</tr>\n");
            }

            htmlText.append("</table>\n");
        }

        {//Bars

            htmlText.append("<br><br><table>\n");
            htmlText.append("<caption><b>Bars</b></caption>\n");

            htmlText.append("<tr>\n");
            htmlText.append("<th>#</th>\n");
            htmlText.append("<th>First Joint</th>\n");
            htmlText.append("<th>Second Joint</th>\n");

            if (mBarsStandardItemModel->columnCount() == 5)
            {
                //Area of Section

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    if (mUnitsAndLimits->areaUnit() == tr("m%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>A (m<sup>2</sup>)</th>\n");
                    }
                    else if (mUnitsAndLimits->areaUnit() == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>A (cm<sup>2</sup>)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>A (mm<sup>2</sup>)</th>\n");
                    }
                }
                else
                {
                    if (mUnitsAndLimits->areaUnit() == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>A (ft<sup>2</sup>)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>A (in.<sup>2</sup>)</th>\n");
                    }
                }

                //Modulus of Elasticity

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    if (mUnitsAndLimits->modulusUnit() == tr("N/m%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>E (N/m<sup>2</sup>)</th>\n");
                    }
                    else if (mUnitsAndLimits->modulusUnit() == tr("N/mm%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>E (N/mm<sup>2</sup>)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>E (GPa)</th>\n");
                    }
                }
                else
                {
                    if (mUnitsAndLimits->modulusUnit() == tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>E (lb/ft<sup>2</sup>)</th>\n");
                    }
                    else if (mUnitsAndLimits->modulusUnit() == tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")))
                    {
                        htmlText.append("<th>E (lb/in.<sup>2</sup>)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>E (kip/in.<sup>2</sup>)</th>\n");
                    }
                }

                //Unit Weight

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    htmlText.append("<th>W (kN/m<sup>3</sup>)</th>\n");
                }
                else
                {
                    htmlText.append("<th>W (lb/ft<sup>3</sup>)</th>\n");
                }
            }
            else
            {
                htmlText.append("<th>Factor</th>\n");
            }

            htmlText.append("</tr>\n");

            count = mBarsList->size();

            for (int i = 0; i < count; ++i)
            {
                Joint *jointA    = mBarsList->at(i)->firstJoint();
                Joint *jointB    = mBarsList->at(i)->secondJoint();
                int jointNumberA = mJointsList->indexOf(jointA) + 1;
                int jointNumberB = mJointsList->indexOf(jointB) + 1;
                qreal factor     = mBarsList->at(i)->factor();
                qreal area       = mBarsList->at(i)->area();
                qreal modulus    = mBarsList->at(i)->modulus();
                qreal unitWeight = mBarsList->at(i)->unitWeight();

                htmlText.append("<tr>\n");
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(jointNumberA)));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(jointNumberB)));

                if (mBarsStandardItemModel->columnCount() == 5)
                {
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(area, 'f',
                                                                            mUnitsAndLimits->areaDecimals())));
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(modulus, 'f',
                                                                            mUnitsAndLimits->modulusDecimals())));
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(unitWeight, 'f',
                                                                            mUnitsAndLimits->unitWeightDecimals())));
                }
                else
                {
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(factor, 'f',
                                                                            mUnitsAndLimits->factorDecimals())));
                }

                htmlText.append("</tr>\n");
            }

            htmlText.append("</table>\n");
        }

        {//Supports

            htmlText.append("<br><br><table>\n");
            htmlText.append("<caption><b>Supports</b></caption>\n");

            htmlText.append("<tr>\n");
            htmlText.append("<th>#</th>\n");
            htmlText.append("<th>Joint</th>\n");
            htmlText.append("<th>Type</th>\n");
            htmlText.append("<th>Angle(<sup>o</sup>)</th>\n");
            htmlText.append("</tr>\n");

            count = mSupportsList->size();

            for (int i = 0; i < count; ++i)
            {
                Joint *joint                     = mSupportsList->at(i)->supportJoint();
                int jointNumber                  = mJointsList->indexOf(joint) + 1;
                UnitsAndLimits::SupportType type = mSupportsList->at(i)->type();
                QString supportTypeName;
                qreal angle                      = mSupportsList->at(i)->angle();

                switch (type)
                {
                    case UnitsAndLimits::FIXED_LEFT:
                    case UnitsAndLimits::FIXED_TOP:
                    case UnitsAndLimits::FIXED_RIGHT:
                    case UnitsAndLimits::FIXED_BOTTOM:
                        supportTypeName = QString(tr("fixed"));
                        break;
                    case UnitsAndLimits::ROLLER:
                        supportTypeName = QString(tr("inclined roller"));
                        break;
                    case UnitsAndLimits::ROLLER_LEFT:
                    case UnitsAndLimits::ROLLER_TOP:
                    case UnitsAndLimits::ROLLER_RIGHT:
                    case UnitsAndLimits::ROLLER_BOTTOM:
                        supportTypeName = QString(tr("roller"));
                        break;
                    default:
                        break;
                }

                htmlText.append("<tr>\n");
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(jointNumber)));
                htmlText.append(tr("<td>%1</td>\n").arg(supportTypeName));
                htmlText.append(tr("<td>%1</td>\n").arg(QString::number(angle, 'f',
                                                                        mUnitsAndLimits->supportAngleDecimals())));
                htmlText.append("</tr>\n");
            }

            htmlText.append("</table>\n");
        }

        {//Model Image

            QImage image = mModelViewer->modelImage(ModelViewer::NO_LOAD,
                                                    ModelViewer::NO_SOLUTION,
                                                    0);
            int w = image.width();
            int h = image.height();

            QDir exportDirectory(mExportDirectory);
            exportDirectory.mkdir(tr("images"));
            image.save(mExportDirectory + tr("/images/model_image.png"), "PNG", 50);
            htmlText.append(tr("<img src=\"images/model_image.png\" "
                               "alt=\"model image\" style=\"width:%1px;height:%2px;\">\n")
                            .arg(QString::number(w))
                            .arg(QString::number(h)));
        }

        {//Joint Loads

            if (!mJointLoadsList->isEmpty())
            {
                if (mJointLoadsCheckBox->isEnabled())
                {
                    if (mJointLoadsCheckBox->isChecked())
                    {
                        htmlText.append("<br><br><table>\n");
                        htmlText.append("<caption><b>Joint Loads</b></caption>\n");

                        htmlText.append("<tr>\n");
                        htmlText.append("<th>#</th>\n");
                        htmlText.append("<th>Joint</th>\n");

                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            if (mUnitsAndLimits->loadUnit() == tr("N"))
                            {
                                htmlText.append(tr("<th>H (%1)</th>\n").arg(tr("N")));
                                htmlText.append(tr("<th>V (%1)</th>\n").arg(tr("N")));
                            }
                            else
                            {
                                htmlText.append(tr("<th>H (%1)</th>\n").arg(tr("kN")));
                                htmlText.append(tr("<th>V (%1)</th>\n").arg(tr("kN")));
                            }
                        }
                        else
                        {
                            if (mUnitsAndLimits->loadUnit() == tr("lb"))
                            {
                                htmlText.append(tr("<th>H (%1)</th>\n").arg(tr("lb")));
                                htmlText.append(tr("<th>V (%1)</th>\n").arg(tr("lb")));
                            }
                            else
                            {
                                htmlText.append(tr("<th>H (%1)</th>\n").arg(tr("kip")));
                                htmlText.append(tr("<th>V (%1)</th>\n").arg(tr("kip")));
                            }
                        }

                        htmlText.append("</tr>\n");

                        count = mJointLoadsList->size();

                        for (int i = 0; i < count; ++i)
                        {
                            Joint *joint    = mJointLoadsList->at(i)->loadJoint();
                            int jointNumber = mJointsList->indexOf(joint) + 1;
                            qreal H         = mJointLoadsList->at(i)->horizontalComponent();
                            qreal V         = mJointLoadsList->at(i)->verticalComponent();

                            htmlText.append("<tr>\n");
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(jointNumber)));
                            QString text;
                            text = QString::number(H, 'f', mUnitsAndLimits->jointLoadDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            text = QString::number(V, 'f', mUnitsAndLimits->jointLoadDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            htmlText.append("</tr>\n");
                        }

                        htmlText.append("</table>\n");

                        QImage image;

                        if (mSelfWeightCheckBox->isEnabled())
                        {
                            if (mSelfWeightCheckBox->isChecked())
                            {
                                image = mModelViewer->modelImage(ModelViewer::JOINT_LOAD_WITH_SELF_WEIGHT,
                                                                 ModelViewer::NO_SOLUTION,
                                                                 0);

                                htmlText.append("<p><emp>Analysis includes bar self-weight</emp></p>\n");
                            }
                            else
                            {
                                image = mModelViewer->modelImage(ModelViewer::JOINT_LOAD,
                                                                 ModelViewer::NO_SOLUTION,
                                                                 0);
                            }
                        }
                        else
                        {
                            image = mModelViewer->modelImage(ModelViewer::JOINT_LOAD,
                                                             ModelViewer::NO_SOLUTION,
                                                             0);
                        }

                        int w = image.width();
                        int h = image.height();

                        image.save(mExportDirectory + tr("/images/model_joint_loads_image.png"), "PNG", 50);
                        htmlText.append(tr("<img src=\"images/model_joint_loads_image.png\" "
                                           "alt=\"model joint loads image\" "
                                           "style=\"width:%1px;height:%2px;\">\n")
                                        .arg(QString::number(w))
                                        .arg(QString::number(h)));
                    }
                    else
                    {
                        if (mSelfWeightCheckBox->isEnabled())
                        {
                            if (mSelfWeightCheckBox->isChecked())
                            {
                                QImage image = mModelViewer->modelImage(ModelViewer::SELF_WEIGHT,
                                                                        ModelViewer::NO_SOLUTION,
                                                                        0);
                                htmlText.append("<p><emp>Analysis includes bar self-weight</emp></p>\n");

                                int w = image.width();
                                int h = image.height();

                                image.save(mExportDirectory + tr("/images/model_joint_loads_image.png"), "PNG", 50);
                                htmlText.append(tr("<img src=\"images/model_joint_loads_image.png\" "
                                                   "alt=\"model joint loads image\" "
                                                   "style=\"width:%1px;height:%2px;\">\n")
                                                .arg(QString::number(w))
                                                .arg(QString::number(h)));
                            }
                        }
                    }
                }
            }
            else
            {
                if (mSelfWeightCheckBox->isEnabled())
                {
                    if (mSelfWeightCheckBox->isChecked())
                    {
                        QImage image = mModelViewer->modelImage(ModelViewer::SELF_WEIGHT,
                                                                ModelViewer::NO_SOLUTION,
                                                                0);
                        htmlText.append("<p><emp>Analysis includes bar self-weight</emp></p>\n");

                        int w = image.width();
                        int h = image.height();

                        image.save(mExportDirectory + tr("/images/model_joint_loads_image.png"), "PNG", 50);
                        htmlText.append(tr("<img src=\"images/model_joint_loads_image.png\" "
                                           "alt=\"model joint loads image\" "
                                           "style=\"width:%1px;height:%2px;\">\n")
                                        .arg(QString::number(w))
                                        .arg(QString::number(h)));
                    }
                }
            }
        }

        {//Support Settlements

            if (!mSupportSettlementsList->isEmpty())
            {
                if (mSupportSettlementsCheckBox->isEnabled())
                {
                    if (mSupportSettlementsCheckBox->isChecked())
                    {
                        htmlText.append("<br><br><table>\n");
                        htmlText.append("<caption><b>Support Settlements</b></caption>\n");

                        htmlText.append("<tr>\n");
                        htmlText.append("<th>#</th>\n");
                        htmlText.append("<th>Support</th>\n");

                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            if (mUnitsAndLimits->supportSettlementUnit() == tr("m"))
                            {
                                htmlText.append("<th>&#8710; (m)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>&#8710; (mm)</th>\n");
                            }
                        }
                        else
                        {
                            if (mUnitsAndLimits->supportSettlementUnit() == tr("ft"))
                            {
                                htmlText.append("<th>&#8710; (ft)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>&#8710; (in.)</th>\n");
                            }
                        }

                        htmlText.append("</tr>\n");

                        count = mSupportSettlementsList->size();

                        for (int i = 0; i < count; ++i)
                        {
                            Support *support  = mSupportSettlementsList->at(i)->settlementSupport();
                            Joint *joint      = support->supportJoint();
                            int supportNumber = mSupportsList->indexOf(support) + 1;
                            int jointNumber   = mJointsList->indexOf(joint) + 1;
                            qreal settlement  = mSupportSettlementsList->at(i)->settlement();

                            htmlText.append("<tr>\n");
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                            QString text;
                            text = tr("# %1 @ joint %2")
                                    .arg(QString::number(supportNumber))
                                    .arg(QString::number(jointNumber));
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            text = QString::number(settlement, 'f', mUnitsAndLimits->supportSettlementDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            htmlText.append("</tr>\n");
                        }

                        htmlText.append("</table>\n");

                        {//Model Support Settlements Image

                            QImage image = mModelViewer->modelImage(ModelViewer::SUPPORT_SETTLEMENT,
                                                                    ModelViewer::NO_SOLUTION,
                                                                    0);
                            int w = image.width();
                            int h = image.height();

                            image.save(mExportDirectory + tr("/images/model_support_settlements_image.png"), "PNG", 50);
                            htmlText.append(tr("<img src=\"images/model_support_settlements_image.png\" "
                                               "alt=\"model support settlements image\" "
                                               "style=\"width:%1px;height:%2px;\">\n")
                                            .arg(QString::number(w))
                                            .arg(QString::number(h)));
                        }
                    }
                }
            }
        }

        {//Thermal Effects

            if (!mThermalEffectsList->isEmpty())
            {
                if (mThermalEffectsCheckBox->isEnabled())
                {
                    if (mThermalEffectsCheckBox->isChecked())
                    {
                        htmlText.append("<br><br><table>\n");
                        htmlText.append("<caption><b>Thermal Effects</b></caption>\n");

                        htmlText.append("<tr>\n");
                        htmlText.append("<th>#</th>\n");
                        htmlText.append("<th>Bar</th>\n");

                        if (mUnitsAndLimits->temperatureChangeUnit() == tr("%1C").arg(QString::fromUtf8("\u00B0")))
                        {
                            htmlText.append("<th>&#8710;t (<sup>o</sup>C)</th>\n");
                        }
                        else
                        {
                            htmlText.append("<th>&#8710;t (<sup>o</sup>F)</th>\n");
                        }

                        if (mUnitsAndLimits->thermalCoefficientUnit() == tr("/%1C").arg(QString::fromUtf8("\u00B0")))
                        {
                            htmlText.append("<th>&#945; (/<sup>o</sup>C)</th>\n");
                        }
                        else
                        {
                            htmlText.append("<th>&#945; (/<sup>o</sup>F)</th>\n");
                        }

                        htmlText.append("</tr>\n");

                        count = mThermalEffectsList->size();

                        for (int i = 0; i < count; ++i)
                        {
                            Bar *bar                 = mThermalEffectsList->at(i)->thermalEffectBar();
                            int barNumber            = mBarsList->indexOf(bar) + 1;
                            qreal temperatureChange  = mThermalEffectsList->at(i)->temperatureChange();
                            qreal thermalCoefficient = mThermalEffectsList->at(i)->thermalCoefficient();

                            htmlText.append("<tr>\n");
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(barNumber)));
                            QString text;
                            text = QString::number(temperatureChange, 'f',
                                                   mUnitsAndLimits->temperatureChangeDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            text = QString::number(thermalCoefficient, 'g',
                                                   mUnitsAndLimits->thermalCoefficientDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            htmlText.append("</tr>\n");
                        }

                        htmlText.append("</table>\n");

                        {//Model Thermal Effects Image

                            QImage image = mModelViewer->modelImage(ModelViewer::THERMAL_EFFECT,
                                                                    ModelViewer::NO_SOLUTION,
                                                                    0);
                            int w = image.width();
                            int h = image.height();

                            image.save(mExportDirectory + tr("/images/model_thermal_effects_image.png"), "PNG", 50);
                            htmlText.append(tr("<img src=\"images/model_thermal_effects_image.png\" "
                                               "alt=\"model thermal effects image\" "
                                               "style=\"width:%1px;height:%2px;\">\n")
                                            .arg(QString::number(w))
                                            .arg(QString::number(h)));
                        }
                    }
                }
            }
        }

        {//Assembly/Fabrication Errors

            if (!mFabricationErrorsList->isEmpty())
            {
                if (mFabricationErrorsCheckBox->isEnabled())
                {
                    if (mFabricationErrorsCheckBox->isChecked())
                    {
                        htmlText.append("<br><br><table>\n");
                        htmlText.append("<caption><b>Assembly/Fabrication Errors</b></caption>\n");

                        htmlText.append("<tr>\n");
                        htmlText.append("<th>#</th>\n");
                        htmlText.append("<th>Bar</th>\n");

                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            if (mUnitsAndLimits->lengthErrorUnit() == tr("m"))
                            {
                                htmlText.append("<th>&#8710;L (m)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>&#8710;L (mm)</th>\n");
                            }
                        }
                        else
                        {
                            if (mUnitsAndLimits->lengthErrorUnit() == tr("ft"))
                            {
                                htmlText.append("<th>&#8710;L (ft)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>&#8710;L (in.)</th>\n");
                            }
                        }

                        htmlText.append("</tr>\n");

                        count = mFabricationErrorsList->size();

                        for (int i = 0; i < count; ++i)
                        {
                            Bar *bar          = mFabricationErrorsList->at(i)->fabricationErrorBar();
                            int barNumber     = mBarsList->indexOf(bar) + 1;
                            qreal lengthError = mFabricationErrorsList->at(i)->lengthError();

                            htmlText.append("<tr>\n");
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(barNumber)));
                            QString text;
                            text = QString::number(lengthError, 'f', mUnitsAndLimits->lengthErrorDecimals());
                            htmlText.append(tr("<td>%1</td>\n").arg(text));
                            htmlText.append("</tr>\n");
                        }

                        htmlText.append("</table>\n");

                        {//Model Assembly/Fabrication Errors Image

                            QImage image = mModelViewer->modelImage(ModelViewer::ASSEMBLY_FABRICATION_ERROR,
                                                                    ModelViewer::NO_SOLUTION,
                                                                    0);
                            int w = image.width();
                            int h = image.height();

                            image.save(mExportDirectory + tr("/images/model_assembly_fabrication_errors_image.png"),
                                       "PNG",
                                       50);
                            htmlText.append(tr("<img src=\"images/model_assembly_fabrication_errors_image.png\" "
                                               "alt=\"model assembly/fabrication errors image\" "
                                               "style=\"width:%1px;height:%2px;\">\n")
                                            .arg(QString::number(w))
                                            .arg(QString::number(h)));
                        }
                    }
                }
            }
        }

        {//Influence Loads

            if (!mInfluenceLoadsList->isEmpty())
            {
                if (mInfluenceLoadsCheckBox->isEnabled())
                {
                    if (mInfluenceLoadsCheckBox->isChecked())
                    {
                        htmlText.append("<br><br><table>\n");
                        htmlText.append("<caption><b>Influence Loads</b></caption>\n");

                        htmlText.append("<tr>\n");
                        htmlText.append("<th>#</th>\n");
                        htmlText.append("<th>Name</th>\n");
                        htmlText.append("<th>Direction</th>\n");
                        htmlText.append("<th>Path</th>\n");

                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            if (mUnitsAndLimits->loadUnit() == tr("N"))
                            {
                                htmlText.append("<th>Point Loads (N)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>Point Loads (kN)</th>\n");
                            }
                        }
                        else
                        {
                            if (mUnitsAndLimits->loadUnit() == tr("lb"))
                            {
                                htmlText.append("<th>Point Loads (lb)</th>\n");
                            }
                            else
                            {
                                htmlText.append("<th>Point Loads (kip)</th>\n");
                            }
                        }

                        htmlText.append("</tr>\n");

                        count = mInfluenceLoadsList->size();

                        for (int i = 0; i < count; ++i)
                        {
                            InfluenceLoad *load   = mInfluenceLoadsList->at(i);
                            QString nameText      = load->name();
                            QString directionText = load->direction();

                            QString pathText;
                            int pathJointsCount = load->path().size();

                            for (int j = 0; j < pathJointsCount; ++j)
                            {
                                pathText.append(QString::number(load->path().at(j)));

                                if (j < (pathJointsCount - 1))
                                {
                                    pathText.append("\n");
                                }
                            }

                            QString loadsText;
                            int pointLoadsCount = load->pointLoads().size();

                            for (int j = 0; j < pointLoadsCount; ++j)
                            {
                                QString s1, s2, s3, s4;
                                int decimals = mUnitsAndLimits->pointLoadDecimals();
                                s1           = QString::number(load->pointLoads().at(j), 'f', decimals);
                                s2           = mUnitsAndLimits->loadUnit();
                                decimals     = mUnitsAndLimits->offsetDecimals();
                                s3           = QString::number(load->pointLoadPositions().at(j), 'f', decimals);
                                s4           = mUnitsAndLimits->coordinateUnit();
                                loadsText.append(tr("%1 %2 @ %3 %4").arg(s1).arg(s2).arg(s3).arg(s4));

                                if (j < (pointLoadsCount - 1))
                                {
                                    loadsText.append("\n");
                                }
                            }

                            htmlText.append("<tr>\n");
                            htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                            htmlText.append(tr("<td>%1</td>\n").arg(nameText));
                            htmlText.append(tr("<td>%1</td>\n").arg(directionText));
                            htmlText.append(tr("<td>\n<pre>\n%1\n</pre>\n</td>\n").arg(pathText));
                            htmlText.append(tr("<td>\n<pre>\n%1\n</pre>\n</td>\n").arg(loadsText));
                            htmlText.append("</tr>\n");
                        }

                        htmlText.append("</table>\n");

                        {//Influence Loads Image

                            QImage image = mModelViewer->modelImage(ModelViewer::INFLUENCE_LOAD,
                                                                    ModelViewer::NO_SOLUTION,
                                                                    0);
                            int w = image.width();
                            int h = image.height();

                            image.save(mExportDirectory + tr("/images/model_influence_loads_image.png"), "PNG", 50);
                            htmlText.append(tr("<img src=\"images/model_influence_loads_image.png\" "
                                               "alt=\"model influence loads image\" "
                                               "style=\"width:%1px;height:%2px;\">\n")
                                            .arg(QString::number(w))
                                            .arg(QString::number(h)));
                        }
                    }
                }
            }
        }

        {//Joint Deflections

            if (!mHorizontalDeflectionComponentsList->isEmpty())
            {
                htmlText.append("<br><br><table>\n");

                if (mBarsStandardItemModel->columnCount() == 5)
                {
                    htmlText.append("<caption><b>Joint Deflections</b></caption>\n");

                    htmlText.append("<tr>\n");
                    htmlText.append("<th>Joint</th>\n");

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        htmlText.append("<th>&#8710;x (m)</th>\n");
                        htmlText.append("<th>&#8710;y (m)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>&#8710;x (ft)</th>\n");
                        htmlText.append("<th>&#8710;y (ft)</th>\n");
                    }

                    htmlText.append("</tr>\n");
                }
                else
                {
                    QString s;

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        s = tr("(A - m<sup>2</sup> &#38; E - N/m<sup>2</sup>)");
                    }
                    else
                    {
                        s = tr("(A - ft<sup>2</sup> &#38; E - lb/ft<sup>2</sup>)");
                    }

                    htmlText.append(tr("<caption><b>Joint Deflections <br>%1</b></caption>\n").arg(s));

                    htmlText.append("<tr>\n");
                    htmlText.append("<th>Joint</th>\n");

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        htmlText.append("<th>&#8710;x &#215; AE (m)</th>\n");
                        htmlText.append("<th>&#8710;y &#215; AE (m)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>&#8710;x &#215; AE (ft)</th>\n");
                        htmlText.append("<th>&#8710;y &#215; AE (ft)</th>\n");
                    }

                    htmlText.append("</tr>\n");
                }

                count = mHorizontalDeflectionComponentsList->size();

                for (int i = 0; i < count; ++i)
                {
                    qreal horizontalComponent = mHorizontalDeflectionComponentsList->at(i);
                    qreal verticalComponent   = mVerticalDeflectionComponentsList->at(i);

                    htmlText.append("<tr>\n");
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(horizontalComponent, 'g', 6)));
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(verticalComponent, 'g', 6)));
                    htmlText.append("</tr>\n");
                }

                htmlText.append("</table>\n");

                if (mBarsStandardItemModel->columnCount() == 5)
                {
                    {//Joint Deflections Image

                        QImage image = mModelViewer->modelImage(ModelViewer::NO_LOAD,
                                                                ModelViewer::JOINT_DEFLECTION,
                                                                0);
                        int w = image.width();
                        int h = image.height();

                        image.save(mExportDirectory + tr("/images/joint_deflections_image.png"), "PNG", 50);
                        htmlText.append(tr("<img src=\"images/joint_deflections_image.png\" "
                                           "alt=\"joint deflections image\" "
                                           "style=\"width:%1px;height:%2px;\">\n")
                                        .arg(QString::number(w))
                                        .arg(QString::number(h)));
                    }
                }
            }
        }

        {//Bar Loads

            if (!mBarLoadsList->isEmpty())
            {
                htmlText.append("<br><br><table>\n");

                htmlText.append("<caption><b>Bar Loads</b></caption>\n");

                htmlText.append("<tr>\n");
                htmlText.append("<th>Bar</th>\n");

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    htmlText.append("<th>Load (N)</th>\n");
                }
                else
                {
                    htmlText.append("<th>Load (lb)</th>\n");
                }

                htmlText.append("</tr>\n");

                count = mBarLoadsList->size();

                for (int i = 0; i < count; ++i)
                {
                    qreal barLoad = mBarLoadsList->at(i);

                    htmlText.append("<tr>\n");
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(i + 1)));
                    htmlText.append(tr("<td>%1</td>\n").arg(QString::number(barLoad, 'g', 6)));
                    htmlText.append("</tr>\n");
                }

                htmlText.append("</table>\n");

                {//Bar Loads Image

                    QImage image = mModelViewer->modelImage(ModelViewer::NO_LOAD,
                                                            ModelViewer::BAR_LOAD,
                                                            0);
                    int w = image.width();
                    int h = image.height();

                    image.save(mExportDirectory + tr("/images/bar_loads_image.png"), "PNG", 50);
                    htmlText.append(tr("<img src=\"images/bar_loads_image.png\" "
                                       "alt=\"bar loads image\" "
                                       "style=\"width:%1px;height:%2px;\">\n")
                                    .arg(QString::number(w))
                                    .arg(QString::number(h)));
                }
            }
        }

        {//Support Reactions

            if (!mReactionHorizontalComponentsList->isEmpty())
            {
                htmlText.append("<br><br><table>\n");

                htmlText.append("<caption><b>Support Reactions</b></caption>\n");

                htmlText.append("<tr>\n");
                htmlText.append("<th>Support</th>\n");

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    htmlText.append("<th>H (N)</th>\n");
                    htmlText.append("<th>V (N)</th>\n");
                }
                else
                {
                    htmlText.append("<th>H (lb)</th>\n");
                    htmlText.append("<th>V (lb)</th>\n");
                }

                htmlText.append("</tr>\n");

                count = mReactionHorizontalComponentsList->size();

                for (int i = 0; i < count; ++i)
                {
                    Support *support = mSupportsList->at(i);
                    Joint *joint     = support->supportJoint();
                    int jointNumber  = mJointsList->indexOf(joint) + 1;
                    qreal h          = mReactionHorizontalComponentsList->at(i);
                    qreal v          = mReactionVerticalComponentsList->at(i);

                    htmlText.append("<tr>\n");
                    QString s1(QString::number(i + 1));
                    QString s2(QString::number(jointNumber));
                    htmlText.append(tr("<td>%1 @ joint %2</td>\n").arg(s1).arg(s2));
                    s1 = QString::number(h, 'g', 6);
                    s2 = QString::number(v, 'g', 6);
                    htmlText.append(tr("<td>%1</td>\n").arg(s1));
                    htmlText.append(tr("<td>%1</td>\n").arg(s2));
                    htmlText.append("</tr>\n");
                }

                htmlText.append("</table>\n");

                {//Support Reactions Image

                    QImage image = mModelViewer->modelImage(ModelViewer::NO_LOAD,
                                                            ModelViewer::SUPPORT_REACTION,
                                                            0);
                    int w = image.width();
                    int h = image.height();

                    image.save(mExportDirectory + tr("/images/support_reactions_image.png"), "PNG", 50);
                    htmlText.append(tr("<img src=\"images/support_reactions_image.png\" "
                                       "alt=\"support reactions image\" "
                                       "style=\"width:%1px;height:%2px;\">\n")
                                    .arg(QString::number(w))
                                    .arg(QString::number(h)));
                }
            }
        }

        {//Influence Load Result

            if (!mSolutionInfluenceLoadName->isEmpty())
            {
                InfluenceLoad *influenceLoad = 0;

                foreach (InfluenceLoad *load, *mInfluenceLoadsList)
                {
                    if (load->name() == *mSolutionInfluenceLoadName)
                    {
                        influenceLoad = load;
                        break;
                    }
                }

                count = influenceLoad->path().size();

                if (mInfluenceLoadResult->influenceLoadOrdinatesList(0).size() == count)
                {
                    htmlText.append(tr("<p>Influence Load Analysed:&nbsp;%1</p>\n").arg(*mSolutionInfluenceLoadName));

                    htmlText.append("<br><br><table>\n");

                    htmlText.append("<caption><b>Influence Load Result</b></caption>\n");

                    htmlText.append("<tr>\n");

                    htmlText.append("<th>#</th>\n");

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        htmlText.append("<th>Pmin (N)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>Pmin (lb)</th>\n");
                    }

                    htmlText.append("<th>Position</th>\n");

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        htmlText.append("<th>Pmax (N)</th>\n");
                    }
                    else
                    {
                        htmlText.append("<th>Pmax (lb)</th>\n");
                    }

                    htmlText.append("<th>Position</th>\n");

                    htmlText.append("</tr>\n");

                    count = mInfluenceLoadResult->barsCount();

                    for (int i = 0; i < count; ++i)
                    {
                        htmlText.append("<tr>\n");
                        QString s;
                        s = QString::number(i + 1);
                        htmlText.append(tr("<td>%1</td>\n").arg(s));
                        s = QString::number(mInfluenceLoadResult->minLoad(i), 'g', 6);
                        htmlText.append(tr("<td>%1</td>\n").arg(s));
                        s = mInfluenceLoadResult->minloadPosition(i);
                        htmlText.append(tr("<td>%1</td>\n").arg(s));
                        s = QString::number(mInfluenceLoadResult->maxLoad(i), 'g', 6);
                        htmlText.append(tr("<td>%1</td>\n").arg(s));
                        s = mInfluenceLoadResult->maxloadPosition(i);
                        htmlText.append(tr("<td>%1</td>\n").arg(s));
                        htmlText.append("</tr>\n");
                    }

                    htmlText.append("</table>\n");

                    {//Influence Line Images

                        for (int i = 0; i < count; ++i)
                        {
                            QImage image = mModelViewer->modelImage(ModelViewer::NO_LOAD,
                                                                    ModelViewer::INFLUENCE_LOAD_SOLUTION,
                                                                    i);
                            int w = image.width();
                            int h = image.height();

                            QString s;
                            s = QString::number(i + 1);
                            image.save(mExportDirectory + tr("/images/influence_line%1_image.png").arg(s), "PNG", 50);
                            htmlText.append(tr("<img src=\"images/influence_line%1_image.png\" "
                                               "alt=\"influence line%1 image\" "
                                               "style=\"width:%2px;height:%3px;\">\n")
                                            .arg(s)
                                            .arg(QString::number(w))
                                            .arg(QString::number(h)));
                        }
                    }
                }
            }
        }

        htmlText.append("</body>\n");
        htmlText.append("</html>\n");

        dataTextStream << htmlText;
        dataFile.close();
        QDesktopServices::openUrl(QUrl(tr("file:///") + fileName, QUrl::TolerantMode));
    }
}

