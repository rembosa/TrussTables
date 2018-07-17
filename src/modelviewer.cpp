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

/* modelviewer.cpp */

#include "modelviewer.h"

ModelViewer::ModelViewer(UnitsAndLimits             *unitsAndLimits,
                         QList<Joint *>             *jointsList,
                         QList<Bar *>               *barsList,
                         QList<Support *>           *supportsList,
                         QList<JointLoad *>         *jointLoadsList,
                         QList<SupportSettlement *> *supportSettlementsList,
                         QList<ThermalEffect *>     *thermalEffectsList,
                         QList<FabricationError *>  *fabricationErrorsList,
                         QList<InfluenceLoad *>     *influenceLoadsList,
                         QStandardItemModel         *barsStandardItemModel,
                         QList<qreal>               *horizontalDeflectionComponentsList,
                         QList<qreal>               *verticalDeflectionComponentsList,
                         QList<qreal>               *barLoadsList,
                         QList<qreal>               *reactionHorizontalComponentsList,
                         QList<qreal>               *reactionVerticalComponentsList,
                         QString                    *solutionInfluenceLoadName,
                         InfluenceLoadResult        *influenceLoadResult,
                         QWidget                    *parent) : QWidget(parent)
{
    mUnitsAndLimits                     = unitsAndLimits;
    mJointsList                         = jointsList;
    mBarsList                           = barsList;
    mSupportsList                       = supportsList;
    mJointLoadsList                     = jointLoadsList;
    mSupportSettlementsList             = supportSettlementsList;
    mThermalEffectsList                 = thermalEffectsList;
    mFabricationErrorsList              = fabricationErrorsList;
    mInfluenceLoadsList                 = influenceLoadsList;
    mBarsStandardItemModel              = barsStandardItemModel;
    mHorizontalDeflectionComponentsList = horizontalDeflectionComponentsList;
    mVerticalDeflectionComponentsList   = verticalDeflectionComponentsList;
    mBarLoadsList                       = barLoadsList;
    mReactionHorizontalComponentsList   = reactionHorizontalComponentsList;
    mReactionVerticalComponentsList     = reactionVerticalComponentsList;
    mSolutionInfluenceLoadName          = solutionInfluenceLoadName;
    mInfluenceLoadResult                = influenceLoadResult;
    mDrawCoordinateAxes                 = true;
    mDrawJointNumber                    = true;
    mDrawBarNumber                      = true;

    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    resize(kDefaultWidth, kDefaultHeight);

    mCoordinateScaleFactor = 1.0;
    mMarginOffset          = 75;
    mLoadDisplayOption     = NO_LOAD;
    mSolutionDisplayOption = NO_SOLUTION;
    mInfluenceLoadBarIndex = 0;
    mForceScaleFactor      = 0.2;
    mDeflectionScaleFactor = 10000.0;
    mModelPixmap           = new QPixmap(width(), height());

    updateModelPixmap();
}

ModelViewer::~ModelViewer()
{
    delete mModelPixmap;
}

QSize ModelViewer::sizeHint() const
{
    return QSize(kDefaultWidth, kDefaultHeight);
}

QPoint ModelViewer::point(int x, int y) const
{
    return QPoint(x, -y);
}

QPointF ModelViewer::point(qreal x, qreal y) const
{
    return QPointF(x, -y);
}

void ModelViewer::updateModelPixmap()
{
    mModelPixmap->fill();
    QPainter painter(mModelPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    QFont painterFont(painter.font());
    painterFont.setPointSize(10);
    painter.setFont(painterFont);

    QPen linePen(QColor(Qt::black));
    linePen.setWidth(1);
    painter.setPen(linePen);

    painter.drawRect(kBorderOffset,
                     kBorderOffset,
                     width() - 2 * kBorderOffset,
                     height() - 2 * kBorderOffset);

    painter.setClipRegion(QRegion(kBorderOffset,
                                  kBorderOffset,
                                  width() - 2 * kBorderOffset,
                                  height() - 2 * kBorderOffset));

    painter.translate(mMarginOffset, height() - mMarginOffset);

    linePen.setColor(QColor(Qt::blue));
    linePen.setWidth(2);
    painter.setPen(linePen);

    qreal w, h;
    QFontMetricsF fontMetrics(painter.font());

    if (mDrawCoordinateAxes)
    {
        //Draw coordinate axes

        painter.drawRect(QRectF(point(-5.0, 5.0), point(5.0, -5.0)));
        painter.drawLine(point( 0.0,  0.0), point( 0.0, 50.0));
        painter.drawLine(point( 0.0, 50.0), point(-5.0, 45.0));
        painter.drawLine(point( 0.0, 50.0), point( 5.0, 45.0));
        painter.drawLine(point( 0.0,  0.0), point(50.0,  0.0));
        painter.drawLine(point(50.0,  0.0), point(45.0,  5.0));
        painter.drawLine(point(50.0,  0.0), point(45.0, -5.0));
    }

    //Draw Joints

    qreal xMax = 0.0;
    qreal yMax = 0.0;

    foreach (Joint *joint, *mJointsList)
    {
        qreal xCoordinate = joint->xCoordinate();
        qreal yCoordinate = joint->yCoordinate();

        if (xCoordinate > xMax)
        {
            xMax = xCoordinate;
        }

        if (yCoordinate > yMax)
        {
            yMax = yCoordinate;
        }
    }

    qreal effectiveWidth  = width() - 2.0 * mMarginOffset;
    qreal effectiveHeight = height() - 2.0 * mMarginOffset;

    if (mJointsList->size() == 1)
    {
        mCoordinateScaleFactor = 1.0;
    }
    else
    {
        qreal widthScaleFactor  = effectiveWidth / xMax;
        qreal heightScaleFactor = effectiveHeight / yMax;

        bool checkWidthScaleFactor  = widthScaleFactor * yMax < effectiveHeight;
        bool checkHeightScaleFactor = heightScaleFactor * xMax < effectiveWidth;

        if (checkWidthScaleFactor)
        {
            mCoordinateScaleFactor = widthScaleFactor;
        }

        if (checkHeightScaleFactor)
        {
            mCoordinateScaleFactor = heightScaleFactor;
        }

        if (!checkWidthScaleFactor && !checkHeightScaleFactor)
        {
            mCoordinateScaleFactor = widthScaleFactor;
        }
    }

    foreach (Joint *joint, *mJointsList)
    {
        qreal x = mCoordinateScaleFactor * joint->xCoordinate();
        qreal y = mCoordinateScaleFactor * joint->yCoordinate();

        painter.setPen(QColor(Qt::magenta));

        if (joint->connectedJoints()->isEmpty())
        {
            painter.setBrush(QBrush(QColor(Qt::magenta)));
        }
        else
        {
            painter.setBrush(QBrush(QColor(Qt::white)));
        }

        painter.drawEllipse(point(x, y),
                            kRadius,
                            kRadius);

        if (mDrawJointNumber)
        {
            int number = 1 + mJointsList->indexOf(joint);
            w          = 1.1 * fontMetrics.boundingRect(QString::number(number)).width();
            h          = fontMetrics.boundingRect(QString::number(number)).height();

            painter.setPen(QColor(Qt::black));

            painter.drawText(QRectF(point(x - w - 5.0, y + h + 5.0), point(x - 5.0, y + 5.0)),
                             Qt::AlignTop | Qt::AlignLeft,
                             QString::number(number));
        }
    }

    //Draw Bars

    qreal radius = 0.0;

    painter.setPen(QColor(Qt::black));
    painter.setBrush(QBrush(QColor(Qt::white)));

    foreach (Bar *bar, *mBarsList)
    {
        qreal x1 = mCoordinateScaleFactor * bar->firstJoint()->xCoordinate();
        qreal y1 = mCoordinateScaleFactor * bar->firstJoint()->yCoordinate();
        qreal x2 = mCoordinateScaleFactor * bar->secondJoint()->xCoordinate();
        qreal y2 = mCoordinateScaleFactor * bar->secondJoint()->yCoordinate();

        painter.drawLine(point(x1, y1), point(x2, y2));

        if (mDrawBarNumber)
        {
            if (!(mSolutionDisplayOption == BAR_LOAD && !mBarLoadsList->isEmpty()))
            {
                int number = 1 + mBarsList->indexOf(bar);
                w          = fontMetrics.boundingRect(QString::number(number)).width();
                h          = fontMetrics.boundingRect(QString::number(number)).height();
                radius     = qMax(w, h) / 1.5;

                qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
                qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

                painter.drawEllipse(point(xp, yp), radius, radius);

                painter.drawText(QRectF(point(xp - w / 2.0, yp + h / 2.0), point(xp + w / 2.0, yp - h / 2.0)),
                                 Qt::AlignHCenter | Qt::AlignVCenter,
                                 QString::number(number));
            }
        }
    }

    if (!(mSolutionDisplayOption == SUPPORT_REACTION && !mReactionHorizontalComponentsList->isEmpty()))
    {
        //Draw Supports

        foreach (Support *support, *mSupportsList)
        {
            UnitsAndLimits::SupportType type = support->type();
            qreal angle                      = support->angle();
            Joint *joint                     = support->supportJoint();

            painter.save();
            painter.translate(point(mCoordinateScaleFactor * joint->xCoordinate(),
                                    mCoordinateScaleFactor * joint->yCoordinate()));

            switch (type)
            {
                case UnitsAndLimits::FIXED_LEFT:
                case UnitsAndLimits::FIXED_TOP:
                case UnitsAndLimits::FIXED_RIGHT:
                case UnitsAndLimits::FIXED_BOTTOM:
                    painter.scale(0.75, 0.75);
                    painter.rotate(angle);
                    painter.drawLine(0.0, 0.0, 18.75, 37.5);
                    painter.drawLine(22.5, 37.5, -22.5, 37.5);
                    painter.drawLine(-18.75, 37.5, 0.0, 0.0);
                    painter.drawLine(-22.5, 45.0, -15.0, 37.5);
                    painter.drawLine(-15.0, 45.0, -7.5, 37.5);
                    painter.drawLine(-7.5, 45.0, 0.0, 37.5);
                    painter.drawLine(0.0, 45.0, 7.5, 37.5);
                    painter.drawLine(7.5, 45.0, 15.0, 37.5);
                    painter.drawLine(15.0, 45.0, 22.5, 37.5);
                    break;
                case UnitsAndLimits::ROLLER:
                case UnitsAndLimits::ROLLER_LEFT:
                case UnitsAndLimits::ROLLER_TOP:
                case UnitsAndLimits::ROLLER_RIGHT:
                case UnitsAndLimits::ROLLER_BOTTOM:
                    painter.scale(0.75, 0.75);
                    painter.rotate(angle);
                    painter.drawLine(0.0, 0.0, 15.0, 30.0);
                    painter.drawLine(22.5, 30.0, -22.5, 30.0);
                    painter.drawLine(-15.0, 30.0, 0.0, 0.0);
                    painter.drawEllipse(-18.75, 30.0, 7.5, 7.5);
                    painter.drawEllipse(-3.75, 30.0, 7.5, 7.5);
                    painter.drawEllipse(11.25, 30.0, 7.5, 7.5);
                    painter.drawLine(-22.5, 37.5, 22.5, 37.5);
                    painter.drawLine(-22.5, 45.0, -15.0, 37.5);
                    painter.drawLine(-15.0, 45.0, -7.5, 37.5);
                    painter.drawLine(-7.5, 45.0, 0.0, 37.5);
                    painter.drawLine(0.0, 45.0, 7.5, 37.5);
                    painter.drawLine(7.5, 45.0, 15.0, 37.5);
                    painter.drawLine(15.0, 45.0, 22.5, 37.5);
                    break;
                default:
                    break;
            }

            painter.restore();
        }
    }

    linePen.setColor(QColor(Qt::magenta));
    linePen.setWidth(3);
    QPen textPen(QColor(Qt::black));
    textPen.setWidth(1);

    if (mLoadDisplayOption != NO_LOAD)
    {
        //Draw joint Loads
        if ((mLoadDisplayOption == JOINT_LOAD) || (mLoadDisplayOption == JOINT_LOAD_WITH_SELF_WEIGHT))
        {
            qreal loadConversionFactor       = 1.0;
            qreal areaConversionFactor       = 1.0;
            qreal lengthConversionFactor     = 1.0;
            qreal unitWeightConversionFactor = 1.0;

            if (mUnitsAndLimits->loadUnit() == tr("N"))
            {
                loadConversionFactor = 1.0;
            }
            else if (mUnitsAndLimits->loadUnit() == tr("kN"))
            {
                qreal kiloNewtonToNewton = 1.0e+3;
                loadConversionFactor     = kiloNewtonToNewton;
            }
            else if (mUnitsAndLimits->loadUnit() == tr("lb"))
            {
                loadConversionFactor = 1.0;
            }
            else
            {
                qreal kipToPound     = 1.0e+3;
                loadConversionFactor = kipToPound;
            }

            if (mUnitsAndLimits->areaUnit() == tr("m%1").arg(QString::fromUtf8("\u00B2")))
            {
                areaConversionFactor = 1.0;
            }
            else if (mUnitsAndLimits->areaUnit() == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
            {
                qreal squareCentimeterToSquareMeter = 1.0e-4;
                areaConversionFactor                = squareCentimeterToSquareMeter;
            }
            else if (mUnitsAndLimits->areaUnit() == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
            {
                qreal squareMillimeterToSquareMeter = 1.0e-6;
                areaConversionFactor                = squareMillimeterToSquareMeter;
            }
            else if (mUnitsAndLimits->areaUnit() == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
            {
                areaConversionFactor = 1.0;
            }
            else
            {
                qreal squareInchToSquareFoot = 1.0 / 144.0;
                areaConversionFactor         = squareInchToSquareFoot;
            }

            if (mUnitsAndLimits->coordinateUnit() == tr("m"))
            {
                lengthConversionFactor = 1.0;
            }
            else if (mUnitsAndLimits->coordinateUnit() == tr("cm"))
            {
                qreal centimeterToMeter = 1.0e-2;
                lengthConversionFactor  = centimeterToMeter;
            }
            else if (mUnitsAndLimits->coordinateUnit() == tr("mm"))
            {
                qreal millimeterToMeter = 1.0e-3;
                lengthConversionFactor  = millimeterToMeter;
            }
            else if (mUnitsAndLimits->coordinateUnit() == tr("ft"))
            {
                lengthConversionFactor = 1.0;
            }
            else
            {
                qreal inchToFoot       = 1.0 / 12.0;
                lengthConversionFactor = inchToFoot;
            }

            if (mUnitsAndLimits->unitWeightUnit() == tr("kN/m%1").arg(QString::fromUtf8("\u00B3")))
            {
                qreal kiloNewtonPerCubicMeterToNewtonPerCubicMeter = 1.0e+3;
                unitWeightConversionFactor                         = kiloNewtonPerCubicMeterToNewtonPerCubicMeter;
            }
            else
            {
                unitWeightConversionFactor = 1.0;
            }

            bool checkA = false;

            if (mLoadDisplayOption == JOINT_LOAD_WITH_SELF_WEIGHT)
            {
                if (!mBarsList->isEmpty())
                {
                    if (mBarsStandardItemModel->columnCount() == 5)
                    {
                        checkA = true;
                    }
                }
            }

            foreach (Joint *joint, *mJointsList)
            {
                qreal h = 0.0;
                qreal v = 0.0;
                QChar position('h');

                //Applied joint loads

                bool checkB = false;

                foreach (JointLoad *load, *mJointLoadsList)
                {
                    if (load->loadJoint() == joint)
                    {

                        h += load->horizontalComponent() * loadConversionFactor;
                        v += load->verticalComponent() * loadConversionFactor;

                        if (load->position() == QString("head"))
                        {
                            position = QChar('h');
                        }
                        else
                        {
                            position = QChar('t');
                        }

                        checkB = true;
                        break;
                    }
                }

                //Joint loads from self-weight

                if (checkA)
                {
                    foreach (Bar *bar, *joint->attachedBars())
                    {
                        qreal x1     = bar->firstJoint()->xCoordinate();
                        qreal y1     = bar->firstJoint()->yCoordinate();
                        qreal x2     = bar->secondJoint()->xCoordinate();
                        qreal y2     = bar->secondJoint()->yCoordinate();
                        qreal deltaX = x2 - x1;
                        qreal deltaY = y2 - y1;
                        qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                        length *= lengthConversionFactor;
                        qreal area       = bar->area() * areaConversionFactor;
                        qreal unitWeight = bar->unitWeight() * unitWeightConversionFactor;

                        v -= area * length * unitWeight / 2.0;
                    }
                }

                if (!checkA && !checkB)
                {
                    continue;
                }

                qreal magnitude             = std::sqrt(std::pow(h, 2.0) + std::pow(v, 2.0));
                qreal angle                 = 0.0;
                qreal epsilonMagnitudeSmall = 1.0e-12;

                if (std::fabs(h) < epsilonMagnitudeSmall)
                {
                    angle = qDegreesToRadians(90.0);
                }
                else
                {
                    angle = std::atan(v / h);
                }

                angle = qDegreesToRadians(90.0) - angle;

                epsilonMagnitudeSmall = 1.0e-3;

                while (true)
                {
                    bool checkC = std::fabs(h - magnitude * std::sin(angle)) < epsilonMagnitudeSmall;
                    bool checkD = std::fabs(v - magnitude * std::cos(angle)) < epsilonMagnitudeSmall;

                    if (checkC && checkD)
                    {
                        break;
                    }

                    angle += qDegreesToRadians(45.0);

                    if (angle > qDegreesToRadians(360.0))
                    {
                        break;
                    }
                }

                painter.save();

                painter.translate(point(mCoordinateScaleFactor * joint->xCoordinate(),
                                        mCoordinateScaleFactor * joint->yCoordinate()));

                painter.save();

                painter.rotate(qRadiansToDegrees(angle));

                qreal scaledMagnitude = 0.0;
                magnitude /= loadConversionFactor;

                if (mUnitsAndLimits->loadUnit() == tr("N"))
                {
                    qreal newtonToKiloNewton = 1.0e-3;
                    scaledMagnitude          = magnitude * newtonToKiloNewton * mForceScaleFactor;
                }
                else if (mUnitsAndLimits->loadUnit() == tr("kN"))
                {
                    scaledMagnitude = magnitude * mForceScaleFactor;
                }
                else if (mUnitsAndLimits->loadUnit() == tr("lb"))
                {
                    qreal poundToNewton      = 4.448;
                    qreal newtonToKiloNewton = 1.0e-3;
                    scaledMagnitude          = magnitude * poundToNewton * newtonToKiloNewton * mForceScaleFactor;
                }
                else
                {
                    qreal kipToKiloNewton = 4.448;
                    scaledMagnitude       = magnitude * kipToKiloNewton * mForceScaleFactor;
                }

                QString string(tr("%1 %2")
                               .arg(QString::number(magnitude, 'f', mUnitsAndLimits->jointLoadDecimals()))
                               .arg(mUnitsAndLimits->loadUnit()));

                if (position == 't')
                {
                    painter.setPen(linePen);
                    painter.drawLine(point(0.0, 0.0), point(0.0, 5.0 + scaledMagnitude));
                    painter.drawLine(point(0.0, 5.0 + scaledMagnitude), point(-5.0, scaledMagnitude));
                    painter.drawLine(point(0.0, 5.0 + scaledMagnitude), point(+5.0, scaledMagnitude));

                    painter.setPen(textPen);

                    painter.restore();

                    w = 1.1 * fontMetrics.boundingRect(string).width();
                    h = fontMetrics.boundingRect(string).height();

                    painter.drawText(QRectF(point(5.0, -5.0), point(w + 5.0, -h - 5.0)),
                                     Qt::AlignLeft | Qt::AlignTop,
                                     string);
                }
                else
                {
                    painter.setPen(linePen);
                    painter.drawLine(point(0.0, -5.0 - scaledMagnitude), point(0.0, 0.0));
                    painter.drawLine(point(0.0, 0.0), point(-5.0, -5.0));
                    painter.drawLine(point(0.0, 0.0), point(+5.0, -5.0));

                    painter.setPen(textPen);

                    painter.restore();

                    w = 1.1 * fontMetrics.boundingRect(string).width();
                    h = fontMetrics.boundingRect(string).height();

                    painter.drawText(QRectF(point(5.0, h + 5.0), point(w + 5.0, 5.0)),
                                     Qt::AlignLeft | Qt::AlignBottom,
                                     string);
                }

                painter.restore();
            }
        }
        else if (mLoadDisplayOption == SUPPORT_SETTLEMENT)
        {
            //Draw Support Settlements

            linePen.setColor(QColor(Qt::green));
            linePen.setWidth(2.0);

            foreach (SupportSettlement *supportSettlement, *mSupportSettlementsList)
            {
                qreal x = mCoordinateScaleFactor * supportSettlement->settlementSupport()->supportJoint()
                        ->xCoordinate();
                qreal y = mCoordinateScaleFactor * supportSettlement->settlementSupport()->supportJoint()
                        ->yCoordinate();

                painter.save();

                painter.translate(point(x, y));
                painter.setPen(linePen);

                painter.drawLine(0,   0, 20,  0);
                painter.drawLine(0,  20, 20, 20);
                painter.drawLine(15,  0, 15, 20);
                painter.drawLine(15, 20, 20, 15);
                painter.drawLine(15, 20, 10, 15);

                QString string = tr("%1S").arg(QString::fromUtf8("\u0394"));

                w = fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.setPen(textPen);

                painter.drawText(QRectF(point(1.5 * w, 0.0), point(2.5 * w + w, -h)),
                                 Qt::AlignTop | Qt::AlignLeft,
                                 string);

                painter.restore();
            }
        }
        else if (mLoadDisplayOption == THERMAL_EFFECT)
        {
            //Draw Thermal Effects

            painter.setPen(textPen);

            qreal epsilonMagnitudeSmall = 1.0e-12;

            foreach (ThermalEffect *thermalEffect, *mThermalEffectsList)
            {
                Bar *bar = thermalEffect->thermalEffectBar();
                qreal xA = bar->firstJoint()->xCoordinate();
                qreal yA = bar->firstJoint()->yCoordinate();
                qreal xB = bar->secondJoint()->xCoordinate();
                qreal yB = bar->secondJoint()->yCoordinate();

                qreal angle = 0.0;

                if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
                {
                    angle = 0.0;
                }
                else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
                {
                    angle = qDegreesToRadians(90.0);
                }
                else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = std::acos(deltaY / length);
                }
                else
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
                }

                qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
                qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

                qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
                qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

                painter.save();

                painter.translate(point(xp, yp));
                painter.rotate(-90.0 + qRadiansToDegrees(angle));

                QString string = tr("%1T").arg(QString::fromUtf8("\u0394"));

                w = fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(radius + 5.0, h + 5.0), point(radius + w + 10.0, 5.0)),
                                 Qt::AlignTop | Qt::AlignRight,
                                 string);

                painter.restore();
            }
        }
        else if (mLoadDisplayOption == ASSEMBLY_FABRICATION_ERROR)
        {
            //Draw Fabrication Errors

            painter.setPen(textPen);

            qreal epsilonMagnitudeSmall = 1.0e-12;

            foreach (FabricationError *fabricationError, *mFabricationErrorsList)
            {
                Bar *bar = fabricationError->fabricationErrorBar();
                qreal xA = bar->firstJoint()->xCoordinate();
                qreal yA = bar->firstJoint()->yCoordinate();
                qreal xB = bar->secondJoint()->xCoordinate();
                qreal yB = bar->secondJoint()->yCoordinate();

                qreal angle = 0.0;

                if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
                {
                    angle = 0.0;
                }
                else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
                {
                    angle = qDegreesToRadians(90.0);
                }
                else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = std::acos(deltaY / length);
                }
                else
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
                }

                qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
                qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

                qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
                qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

                painter.save();

                painter.translate(point(xp, yp));
                painter.rotate(-90.0 + qRadiansToDegrees(angle));

                QString string = tr("%1L").arg(QString::fromUtf8("\u0394"));

                w = fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(radius + 5.0, h + 5.0), point(radius + w + 10.0, 5.0)),
                                 Qt::AlignTop | Qt::AlignRight,
                                 string);

                painter.restore();
            }
        }
        else
        {
            //Draw influence load paths

            linePen.setColor(QColor(Qt::darkMagenta));
            linePen.setWidth(5);
            painter.setPen(linePen);

            foreach (InfluenceLoad *influenceLoad, *mInfluenceLoadsList)
            {
                Joint *jointA = mJointsList->at(influenceLoad->path().first() - 1);
                Joint *jointB = mJointsList->at(influenceLoad->path().last() - 1);

                qreal x1 = mCoordinateScaleFactor * jointA->xCoordinate();
                qreal y1 = mCoordinateScaleFactor * jointA->yCoordinate();
                qreal x2 = mCoordinateScaleFactor * jointB->xCoordinate();
                qreal y2 = mCoordinateScaleFactor * jointB->yCoordinate();

                painter.drawLine(point(x1, y1), point(x2, y2));
            }
        }
    }

    if (mSolutionDisplayOption != NO_SOLUTION)
    {
        if (mSolutionDisplayOption == JOINT_DEFLECTION)
        {
            if (mBarsStandardItemModel->columnCount() == 5 && !mHorizontalDeflectionComponentsList->isEmpty())
            {
                //Draw joint deflections

                linePen.setColor(QColor(Qt::blue));
                linePen.setWidth(1);
                painter.setPen(linePen);

                foreach (Bar *bar, *mBarsList)
                {
                    Joint *jointA = bar->firstJoint();
                    Joint *jointB = bar->secondJoint();

                    int jointIndexA = mJointsList->indexOf(jointA);
                    int jointIndexB = mJointsList->indexOf(jointB);

                    qreal deflectionX1 = mHorizontalDeflectionComponentsList->at(jointIndexA);
                    qreal deflectionY1 = mVerticalDeflectionComponentsList->at(jointIndexA);
                    qreal deflectionX2 = mHorizontalDeflectionComponentsList->at(jointIndexB);
                    qreal deflectionY2 = mVerticalDeflectionComponentsList->at(jointIndexB);

                    qreal factor = 1.0;

                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        factor = mDeflectionScaleFactor;
                    }
                    else
                    {
                        qreal footToMeter = 0.3048;
                        factor            = mDeflectionScaleFactor * footToMeter;
                    }

                    qreal x1 = mCoordinateScaleFactor * jointA->xCoordinate() + factor * deflectionX1;
                    qreal y1 = mCoordinateScaleFactor * jointA->yCoordinate() + factor * deflectionY1;
                    qreal x2 = mCoordinateScaleFactor * jointB->xCoordinate() + factor * deflectionX2;
                    qreal y2 = mCoordinateScaleFactor * jointB->yCoordinate() + factor * deflectionY2;

                    painter.drawLine(point(x1, y1), point(x2, y2));
                }
            }
        }

        if (mSolutionDisplayOption == BAR_LOAD && !mBarLoadsList->isEmpty())
        {
            //Draw Bar Loads

            painter.setPen(textPen);

            qreal epsilonMagnitudeSmall = 1.0e-12;

            foreach (Bar *bar, *mBarsList)
            {
                qreal xA = bar->firstJoint()->xCoordinate();
                qreal yA = bar->firstJoint()->yCoordinate();
                qreal xB = bar->secondJoint()->xCoordinate();
                qreal yB = bar->secondJoint()->yCoordinate();

                qreal angle = 0.0;

                if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
                {
                    angle = 0.0;
                }
                else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
                {
                    angle = qDegreesToRadians(90.0);
                }
                else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = std::acos(deltaY / length);
                }
                else
                {
                    qreal deltaX = std::fabs(xB - xA);
                    qreal deltaY = std::fabs(yB - yA);
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
                }

                qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
                qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
                qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

                qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
                qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

                painter.save();

                painter.translate(point(xp, yp));
                painter.rotate(-90.0 + qRadiansToDegrees(angle));

                QString unitString;

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    unitString = tr("N");
                }
                else
                {
                    unitString = tr("lb");
                }

                int index     = mBarsList->indexOf(bar);
                qreal barLoad = mBarLoadsList->at(index);

                QString string(tr("%1 %2")
                               .arg(QString::number(barLoad, 'f', 1))
                               .arg(unitString));

                w = fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(radius - 1.125 * w, 0.0 + 1.125 * h),
                                        point(radius + 1.125 * w, 0.0 - 1.125 * h)),
                                 Qt::AlignTop | Qt::AlignRight,
                                 string);

                painter.restore();

                if (barLoad < 0.0)
                {
                    painter.setBrush(QBrush(QColor(255, 0, 0, 60)));
                }
                else if (barLoad > 0.0)
                {
                    painter.setBrush(QBrush(QColor(0, 255, 0, 60)));
                }
                else
                {
                    continue;
                }

                xp = (x1 + x2) / 2.0;
                yp = (y1 + y2) / 2.0;

                painter.save();

                painter.translate(point(xp, yp));
                painter.rotate(-90.0 + qRadiansToDegrees(angle));

                qreal deltaX = x2 - x1;
                qreal deltaY = y2 - y1;
                qreal width  = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                qreal height = 0.0;

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    qreal newtonToKiloNewton = 1.0e-3;
                    height                   = newtonToKiloNewton * barLoad * mForceScaleFactor;
                }
                else
                {
                    qreal poundToNewton      = 4.448;
                    qreal newtonToKiloNewton = 1.0e-3;
                    height                   = newtonToKiloNewton * poundToNewton * barLoad * mForceScaleFactor;
                }

                painter.drawRect(QRectF(QPointF(-width / 2.0, height / 2.0), QPointF(width / 2.0, -height / 2.0)));

                painter.restore();
            }
        }

        if (mSolutionDisplayOption == SUPPORT_REACTION && !mReactionHorizontalComponentsList->isEmpty())
        {
            //Draw Support Reactions

            foreach (Support *support, *mSupportsList)
            {
                Joint *joint = support->supportJoint();

                painter.save();
                qreal x = mCoordinateScaleFactor * joint->xCoordinate();
                qreal y = mCoordinateScaleFactor * joint->yCoordinate();
                painter.translate(point(x, y));

                int index = mSupportsList->indexOf(support);

                qreal horizontalComponent = mReactionHorizontalComponentsList->at(index);
                qreal verticalComponent   = mReactionVerticalComponentsList->at(index);

                qreal scaledMagnitude       = 0.0;
                qreal epsilonMagnitudeSmall = 1.0e-12;

                if (std::fabs(horizontalComponent) > epsilonMagnitudeSmall)
                {
                    if (horizontalComponent > 0.0)
                    {
                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = horizontalComponent * newtonToKiloNewton * mForceScaleFactor;
                        }
                        else
                        {
                            qreal poundToNewton      = 4.448;
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = horizontalComponent * poundToNewton * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }

                        painter.setPen(linePen);
                        painter.drawLine(point(-5.0, 0.0), point(-scaledMagnitude - 10.0, 0.0));
                        painter.drawLine(point(-5.0, 0.0), point(-10.0,  5.0));
                        painter.drawLine(point(-5.0, 0.0), point(-10.0, -5.0));
                    }
                    else
                    {
                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = std::fabs(horizontalComponent) * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }
                        else
                        {
                            qreal poundToNewton      = 4.448;
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = std::fabs(horizontalComponent) * poundToNewton
                                    * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }

                        painter.setPen(linePen);
                        painter.drawLine(point(-5.0, 0.0), point(-scaledMagnitude - 10.0, 0.0));
                        painter.drawLine(point(-scaledMagnitude - 10.0, 0.0), point(-scaledMagnitude - 5.0,  5.0));
                        painter.drawLine(point(-scaledMagnitude - 10.0, 0.0), point(-scaledMagnitude - 5.0, -5.0));
                    }
                }

                if (std::fabs(verticalComponent) > epsilonMagnitudeSmall)
                {
                    if (verticalComponent > 0.0)
                    {
                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = verticalComponent * newtonToKiloNewton * mForceScaleFactor;
                        }
                        else
                        {
                            qreal poundToNewton      = 4.448;
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = verticalComponent * poundToNewton * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }

                        painter.setPen(linePen);
                        painter.drawLine(point(0.0, -5.0), point(0.0 ,-scaledMagnitude - 10.0));
                        painter.drawLine(point(0.0, -5.0), point( 5.0, -10.0));
                        painter.drawLine(point(0.0, -5.0), point(-5.0, -10.0));
                    }
                    else
                    {
                        if (mUnitsAndLimits->system() == tr("metric"))
                        {
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = std::fabs(verticalComponent) * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }
                        else
                        {
                            qreal poundToNewton      = 4.448;
                            qreal newtonToKiloNewton = 1.0e-3;
                            scaledMagnitude          = std::fabs(verticalComponent) * poundToNewton * newtonToKiloNewton
                                    * mForceScaleFactor;
                        }

                        painter.setPen(linePen);
                        painter.drawLine(point(0.0, -5.0), point(0.0 ,-scaledMagnitude - 10.0));
                        painter.drawLine(point(0.0, -scaledMagnitude - 10.0), point( 5.0, -scaledMagnitude - 5.0));
                        painter.drawLine(point(0.0, -scaledMagnitude - 10.0), point(-5.0, -scaledMagnitude - 5.0));
                    }
                }

                painter.setPen(textPen);
                QString unitString;

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    unitString = tr("N");
                }
                else
                {
                    unitString = tr("lb");
                }

                QString string = tr("Rx : %1 %3\nRy : %2 %3")
                        .arg(QString::number(horizontalComponent, 'f', 1))
                        .arg(QString::number(verticalComponent, 'f', 1))
                        .arg(unitString);

                w = fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(5.0, -5.0), point(w + 5.0, -2.0 * h - 5.0)),
                                 Qt::AlignLeft | Qt::AlignTop,
                                 string);

                painter.restore();
            }
        }

        if (mSolutionDisplayOption == INFLUENCE_LOAD_SOLUTION)
        {
            //Draw influence lines

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

                int count = influenceLoad->path().size();

                if (mInfluenceLoadResult->influenceLoadOrdinatesList(0).size() == count)
                {
                    linePen.setColor(QColor(Qt::blue));
                    linePen.setWidth(1);

                    painter.save();

                    QPointF firstPoint;

                    QList<qreal> ordinatesList;
                    ordinatesList = mInfluenceLoadResult->influenceLoadOrdinatesList(mInfluenceLoadBarIndex);
                    qreal maxOrdinateMagnitude = 1.0;

                    foreach (qreal ordinate, ordinatesList)
                    {
                        if (std::fabs(ordinate) > maxOrdinateMagnitude)
                        {
                            maxOrdinateMagnitude = std::fabs(ordinate);
                        }
                    }

                    qreal ordinateFactor = 0.75 * ((height() - 2.0 * mMarginOffset) / (2.0)) / maxOrdinateMagnitude;

                    for (int i = 0; i < count - 1; ++i)
                    {
                        int jointAIndex = influenceLoad->path().at(i) - 1;
                        int jointBIndex = influenceLoad->path().at(i + 1) - 1;
                        Joint *jointA   = mJointsList->at(jointAIndex);
                        Joint *jointB   = mJointsList->at(jointBIndex);
                        qreal xA = jointA->xCoordinate() * mCoordinateScaleFactor;
                        qreal xB = jointB->xCoordinate() * mCoordinateScaleFactor;
                        qreal yo = (height() - 2.0 * mMarginOffset) / 2.0;
                        qreal iA = mInfluenceLoadResult->influenceLoadOrdinatesList(mInfluenceLoadBarIndex).at(i);
                        qreal iB = mInfluenceLoadResult->influenceLoadOrdinatesList(mInfluenceLoadBarIndex).at(i + 1);
                        qreal yA = yo + iA * ordinateFactor;
                        qreal yB = yo + iB * ordinateFactor;

                        painter.setPen(linePen);
                        painter.drawLine(point(xA, yA), point(xB, yB));

                        if (i == 0)
                        {
                            firstPoint = point(xA, yA);
                        }

                        if (i == count - 2)
                        {
                            qreal x1 = firstPoint.x();
                            qreal y1 = (height() - 2.0 * mMarginOffset) / 2.0;
                            qreal x2 = xB;
                            qreal y2 = y1;

                            painter.drawLine(point(x1, y1), point(x2, y2));

                            painter.setPen(textPen);

                            QString text(QString::number(iB, 'g', 3));
                            w = fontMetrics.boundingRect(text).width();
                            h = fontMetrics.boundingRect(text).height();

                            painter.drawText(QRectF(point(xB, yB + h), point(xB + w, yB)),
                                             Qt::AlignLeft | Qt::AlignBottom,
                                             text);
                        }

                        painter.setPen(textPen);

                        QString text(QString::number(iA, 'g', 3));
                        w = fontMetrics.boundingRect(text).width();
                        h = fontMetrics.boundingRect(text).height();

                        painter.drawText(QRectF(point(xA, yA + h), point(xA + w, yA)),
                                         Qt::AlignLeft | Qt::AlignBottom,
                                         text);
                    }

                    painter.restore();
                }
            }
        }
    }

    update();
}

QPixmap *ModelViewer::modelPixmap()
{
    return mModelPixmap;
}

void ModelViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0,
                       0,
                       width(),
                       height(),
                       *mModelPixmap);
}

void ModelViewer::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x() - mMarginOffset;
    int y = -event->pos().y() + height() - mMarginOffset;

    QString position(tr("X: ")
                     + QString::number((qreal)x / mCoordinateScaleFactor, 'f', mUnitsAndLimits->deltaXDecimals())
                     + tr(" %1 ").arg(mUnitsAndLimits->coordinateUnit())
                     + tr("Y: ")
                     + QString::number((qreal)y / mCoordinateScaleFactor, 'f', mUnitsAndLimits->deltaYDecimals())
                     + tr(" %1").arg(mUnitsAndLimits->coordinateUnit()));

    emit positionSignal(position);

    event->ignore();
}

void ModelViewer::updateSize(int width,
                             int height,
                             int marginOffset)
{
    resize(width, height);
    mMarginOffset = marginOffset;
    delete mModelPixmap;
    mModelPixmap = new QPixmap(width, height);
    updateModelPixmap();
}

void ModelViewer::updateModel()
{
    updateModelPixmap();
}

void ModelViewer::showHideAxes(bool show)
{
    mDrawCoordinateAxes = show;
    updateModelPixmap();
}

void ModelViewer::showHideJointNumber(bool show)
{
    mDrawJointNumber = show;
    updateModelPixmap();
}

void ModelViewer::showHideBarNumber(bool show)
{
    mDrawBarNumber = show;
    updateModelPixmap();
}

void ModelViewer::updateLoadDisplayOption(ModelViewer::LoadDisplayOption option)
{
    mLoadDisplayOption = option;
    updateModelPixmap();
}

void ModelViewer::updateForceScaleFactor(qreal factor)
{
    mForceScaleFactor = factor;
    updateModelPixmap();
}

void ModelViewer::updateSolutionDisplayOption(ModelViewer::SolutionDisplayOption option, int influenceLoadBarIndex)
{
    mSolutionDisplayOption = option;
    mInfluenceLoadBarIndex = influenceLoadBarIndex;
    updateModelPixmap();
}

void ModelViewer::updateDeflectionScaleFactor(qreal factor)
{
    mDeflectionScaleFactor = factor;
    updateModelPixmap();
}

QImage ModelViewer::modelImage(LoadDisplayOption loadDisplayOption,
                               SolutionDisplayOption solutionDisplayOption,
                               int influenceLoadBarIndex)
{
    QImage image(width(), height(), QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    QFont painterFont(painter.font());
    painterFont.setPointSize(10);
    painter.setFont(painterFont);

    QPen linePen(QColor(Qt::black));
    linePen.setWidth(1);
    painter.setPen(linePen);

    painter.setClipRegion(QRegion(kBorderOffset,
                                  kBorderOffset,
                                  width() - 2 * kBorderOffset,
                                  height() - 2 * kBorderOffset));

    painter.translate(mMarginOffset, height() - mMarginOffset);

    qreal w, h;
    QFontMetricsF fontMetrics(painterFont);
    qreal radius = 0.0;

    if (solutionDisplayOption != INFLUENCE_LOAD_SOLUTION)
    {
        linePen.setColor(QColor(Qt::blue));
        linePen.setWidth(2);
        painter.setPen(linePen);

        if (mDrawCoordinateAxes)
        {
            //Draw coordinate axes

            painter.drawRect(QRectF(point(-5.0, 5.0), point(5.0, -5.0)));
            painter.drawLine(point( 0.0,  0.0), point( 0.0, 50.0));
            painter.drawLine(point( 0.0, 50.0), point(-5.0, 45.0));
            painter.drawLine(point( 0.0, 50.0), point( 5.0, 45.0));
            painter.drawLine(point( 0.0,  0.0), point(50.0,  0.0));
            painter.drawLine(point(50.0,  0.0), point(45.0,  5.0));
            painter.drawLine(point(50.0,  0.0), point(45.0, -5.0));
        }

        //Draw Joints

        qreal xMax = 0.0;
        qreal yMax = 0.0;

        foreach (Joint *joint, *mJointsList)
        {
            qreal xCoordinate = joint->xCoordinate();
            qreal yCoordinate = joint->yCoordinate();

            if (xCoordinate > xMax)
            {
                xMax = xCoordinate;
            }

            if (yCoordinate > yMax)
            {
                yMax = yCoordinate;
            }
        }

        qreal effectiveWidth  = width() - 2.0 * mMarginOffset;
        qreal effectiveHeight = height() - 2.0 * mMarginOffset;

        qreal widthScaleFactor  = effectiveWidth / xMax;
        qreal heightScaleFactor = effectiveHeight / yMax;

        bool checkWidthScaleFactor  = widthScaleFactor * yMax < effectiveHeight;
        bool checkHeightScaleFactor = heightScaleFactor * xMax < effectiveWidth;

        if (checkWidthScaleFactor)
        {
            mCoordinateScaleFactor = widthScaleFactor;
        }

        if (checkHeightScaleFactor)
        {
            mCoordinateScaleFactor = heightScaleFactor;
        }

        if (!checkWidthScaleFactor && !checkHeightScaleFactor)
        {
            mCoordinateScaleFactor = widthScaleFactor;
        }

        painter.setBrush(QBrush(QColor(Qt::white)));

        foreach (Joint *joint, *mJointsList)
        {
            qreal x = mCoordinateScaleFactor * joint->xCoordinate();
            qreal y = mCoordinateScaleFactor * joint->yCoordinate();

            painter.setPen(QColor(Qt::magenta));

            painter.drawEllipse(point(x, y),
                                kRadius,
                                kRadius);

            if (mDrawJointNumber)
            {
                int number = 1 + mJointsList->indexOf(joint);
                w          = fontMetrics.boundingRect(QString::number(number)).width();
                h          = fontMetrics.boundingRect(QString::number(number)).height();

                painter.setPen(QColor(Qt::black));

                painter.drawText(QRectF(point(x - w - 5.0, y + h + 5.0), point(x - 5.0, y + 5.0)),
                                 Qt::AlignTop | Qt::AlignLeft,
                                 QString::number(number));
            }
        }

        //Draw Bars

        painter.setPen(QColor(Qt::black));

        foreach (Bar *bar, *mBarsList)
        {
            qreal x1 = mCoordinateScaleFactor * bar->firstJoint()->xCoordinate();
            qreal y1 = mCoordinateScaleFactor * bar->firstJoint()->yCoordinate();
            qreal x2 = mCoordinateScaleFactor * bar->secondJoint()->xCoordinate();
            qreal y2 = mCoordinateScaleFactor * bar->secondJoint()->yCoordinate();

            painter.drawLine(point(x1, y1), point(x2, y2));

            if (mDrawBarNumber)
            {
                if (solutionDisplayOption != BAR_LOAD)
                {
                    int number = 1 + mBarsList->indexOf(bar);
                    w          = fontMetrics.boundingRect(QString::number(number)).width();
                    h          = fontMetrics.boundingRect(QString::number(number)).height();
                    radius     = qMax(w, h) / 1.5;

                    qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
                    qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

                    painter.drawEllipse(point(xp, yp), radius, radius);

                    painter.drawText(QRectF(point(xp - w / 2.0, yp + h / 2.0), point(xp + w / 2.0, yp - h / 2.0)),
                                     Qt::AlignHCenter | Qt::AlignVCenter,
                                     QString::number(number));
                }
            }
        }

        if (solutionDisplayOption != SUPPORT_REACTION)
        {
            //Draw Supports

            foreach (Support *support, *mSupportsList)
            {
                UnitsAndLimits::SupportType type = support->type();
                qreal angle                      = support->angle();
                Joint *joint                     = support->supportJoint();

                painter.save();
                painter.translate(point(mCoordinateScaleFactor * joint->xCoordinate(),
                                        mCoordinateScaleFactor * joint->yCoordinate()));

                switch (type)
                {
                    case UnitsAndLimits::FIXED_LEFT:
                    case UnitsAndLimits::FIXED_TOP:
                    case UnitsAndLimits::FIXED_RIGHT:
                    case UnitsAndLimits::FIXED_BOTTOM:
                        painter.scale(0.75, 0.75);
                        painter.rotate(angle);
                        painter.drawLine(0.0, 0.0, 18.75, 37.5);
                        painter.drawLine(22.5, 37.5, -22.5, 37.5);
                        painter.drawLine(-18.75, 37.5, 0.0, 0.0);
                        painter.drawLine(-22.5, 45.0, -15.0, 37.5);
                        painter.drawLine(-15.0, 45.0, -7.5, 37.5);
                        painter.drawLine(-7.5, 45.0, 0.0, 37.5);
                        painter.drawLine(0.0, 45.0, 7.5, 37.5);
                        painter.drawLine(7.5, 45.0, 15.0, 37.5);
                        painter.drawLine(15.0, 45.0, 22.5, 37.5);
                        break;
                    case UnitsAndLimits::ROLLER:
                    case UnitsAndLimits::ROLLER_LEFT:
                    case UnitsAndLimits::ROLLER_TOP:
                    case UnitsAndLimits::ROLLER_RIGHT:
                    case UnitsAndLimits::ROLLER_BOTTOM:
                        painter.scale(0.75, 0.75);
                        painter.rotate(angle);
                        painter.drawLine(0.0, 0.0, 15.0, 30.0);
                        painter.drawLine(22.5, 30.0, -22.5, 30.0);
                        painter.drawLine(-15.0, 30.0, 0.0, 0.0);
                        painter.drawEllipse(-18.75, 30.0, 7.5, 7.5);
                        painter.drawEllipse(-3.75, 30.0, 7.5, 7.5);
                        painter.drawEllipse(11.25, 30.0, 7.5, 7.5);
                        painter.drawLine(-22.5, 37.5, 22.5, 37.5);
                        painter.drawLine(-22.5, 45.0, -15.0, 37.5);
                        painter.drawLine(-15.0, 45.0, -7.5, 37.5);
                        painter.drawLine(-7.5, 45.0, 0.0, 37.5);
                        painter.drawLine(0.0, 45.0, 7.5, 37.5);
                        painter.drawLine(7.5, 45.0, 15.0, 37.5);
                        painter.drawLine(15.0, 45.0, 22.5, 37.5);
                        break;
                    default:
                        break;
                }

                painter.restore();
            }
        }
    }

    QPen textPen(QColor(Qt::black));

    linePen.setColor(QColor(Qt::magenta));
    linePen.setWidth(3);
    textPen.setWidth(1);

    //Draw joint Loads
    if ((loadDisplayOption == JOINT_LOAD) || (loadDisplayOption == JOINT_LOAD_WITH_SELF_WEIGHT)
            || (loadDisplayOption == SELF_WEIGHT))
    {
        qreal loadConversionFactor       = 1.0;
        qreal areaConversionFactor       = 1.0;
        qreal lengthConversionFactor     = 1.0;
        qreal unitWeightConversionFactor = 1.0;

        if (mUnitsAndLimits->loadUnit() == tr("N"))
        {
            loadConversionFactor = 1.0;
        }
        else if (mUnitsAndLimits->loadUnit() == tr("kN"))
        {
            qreal kiloNewtonToNewton = 1.0e+3;
            loadConversionFactor     = kiloNewtonToNewton;
        }
        else if (mUnitsAndLimits->loadUnit() == tr("lb"))
        {
            loadConversionFactor = 1.0;
        }
        else
        {
            qreal kipToPound     = 1.0e+3;
            loadConversionFactor = kipToPound;
        }

        if (mUnitsAndLimits->areaUnit() == tr("m%1").arg(QString::fromUtf8("\u00B2")))
        {
            areaConversionFactor = 1.0;
        }
        else if (mUnitsAndLimits->areaUnit() == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
        {
            qreal squareCentimeterToSquareMeter = 1.0e-4;
            areaConversionFactor                = squareCentimeterToSquareMeter;
        }
        else if (mUnitsAndLimits->areaUnit() == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
        {
            qreal squareMillimeterToSquareMeter = 1.0e-6;
            areaConversionFactor                = squareMillimeterToSquareMeter;
        }
        else if (mUnitsAndLimits->areaUnit() == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
        {
            areaConversionFactor = 1.0;
        }
        else
        {
            qreal squareInchToSquareFoot = 1.0 / 144.0;
            areaConversionFactor         = squareInchToSquareFoot;
        }

        if (mUnitsAndLimits->coordinateUnit() == tr("m"))
        {
            lengthConversionFactor = 1.0;
        }
        else if (mUnitsAndLimits->coordinateUnit() == tr("cm"))
        {
            qreal centimeterToMeter = 1.0e-2;
            lengthConversionFactor  = centimeterToMeter;
        }
        else if (mUnitsAndLimits->coordinateUnit() == tr("mm"))
        {
            qreal millimeterToMeter = 1.0e-3;
            lengthConversionFactor  = millimeterToMeter;
        }
        else if (mUnitsAndLimits->coordinateUnit() == tr("ft"))
        {
            lengthConversionFactor = 1.0;
        }
        else
        {
            qreal inchToFoot       = 1.0 / 12.0;
            lengthConversionFactor = inchToFoot;
        }

        if (mUnitsAndLimits->unitWeightUnit() == tr("kN/m%1").arg(QString::fromUtf8("\u00B3")))
        {
            qreal kiloNewtonPerCubicMeterToNewtonPerCubicMeter = 1.0e+3;
            unitWeightConversionFactor                         = kiloNewtonPerCubicMeterToNewtonPerCubicMeter;
        }
        else
        {
            unitWeightConversionFactor = 1.0;
        }

        bool checkA = false;

        if ((loadDisplayOption == JOINT_LOAD_WITH_SELF_WEIGHT) || (loadDisplayOption == SELF_WEIGHT))
        {
            if (mBarsStandardItemModel->columnCount() == 5)
            {
                checkA = true;
            }
        }

        foreach (Joint *joint, *mJointsList)
        {
            qreal h = 0.0;
            qreal v = 0.0;
            QChar position('h');

            //Applied joint loads

            bool checkB = false;

            if ((loadDisplayOption == JOINT_LOAD) || (loadDisplayOption == JOINT_LOAD_WITH_SELF_WEIGHT))
            {
                foreach (JointLoad *load, *mJointLoadsList)
                {
                    if (load->loadJoint() == joint)
                    {

                        h += load->horizontalComponent() * loadConversionFactor;
                        v += load->verticalComponent() * loadConversionFactor;

                        if (load->position() == QString("head"))
                        {
                            position = QChar('h');
                        }
                        else
                        {
                            position = QChar('t');
                        }

                        checkB = true;
                        break;
                    }
                }
            }

            //Joint loads from self-weight

            if (checkA)
            {
                foreach (Bar *bar, *joint->attachedBars())
                {
                    qreal x1     = bar->firstJoint()->xCoordinate();
                    qreal y1     = bar->firstJoint()->yCoordinate();
                    qreal x2     = bar->secondJoint()->xCoordinate();
                    qreal y2     = bar->secondJoint()->yCoordinate();
                    qreal deltaX = x2 - x1;
                    qreal deltaY = y2 - y1;
                    qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                    length *= lengthConversionFactor;
                    qreal area       = bar->area() * areaConversionFactor;
                    qreal unitWeight = bar->unitWeight() * unitWeightConversionFactor;

                    v -= area * length * unitWeight / 2.0;
                }
            }

            if (!checkA && !checkB)
            {
                continue;
            }

            qreal magnitude             = std::sqrt(std::pow(h, 2.0) + std::pow(v, 2.0));
            qreal angle                 = 0.0;
            qreal epsilonMagnitudeSmall = 1.0e-12;

            if (std::fabs(h) < epsilonMagnitudeSmall)
            {
                angle = qDegreesToRadians(90.0);
            }
            else
            {
                angle = std::atan(v / h);
            }

            angle = qDegreesToRadians(90.0) - angle;

            epsilonMagnitudeSmall = 1.0e-3;

            while (true)
            {
                bool checkC = std::fabs(h - magnitude * std::sin(angle)) < epsilonMagnitudeSmall;
                bool checkD = std::fabs(v - magnitude * std::cos(angle)) < epsilonMagnitudeSmall;

                if (checkC && checkD)
                {
                    break;
                }

                angle += qDegreesToRadians(45.0);

                if (angle > qDegreesToRadians(360.0))
                {
                    break;
                }
            }

            painter.save();

            painter.translate(point(mCoordinateScaleFactor * joint->xCoordinate(),
                                    mCoordinateScaleFactor * joint->yCoordinate()));

            painter.save();

            painter.rotate(qRadiansToDegrees(angle));

            qreal scaledMagnitude = 0.0;
            magnitude /= loadConversionFactor;

            if (mUnitsAndLimits->loadUnit() == tr("N"))
            {
                qreal newtonToKiloNewton = 1.0e-3;
                scaledMagnitude          = magnitude * newtonToKiloNewton * mForceScaleFactor;
            }
            else if (mUnitsAndLimits->loadUnit() == tr("kN"))
            {
                scaledMagnitude = magnitude * mForceScaleFactor;
            }
            else if (mUnitsAndLimits->loadUnit() == tr("lb"))
            {
                qreal poundToNewton      = 4.448;
                qreal newtonToKiloNewton = 1.0e-3;
                scaledMagnitude          = magnitude * poundToNewton * newtonToKiloNewton * mForceScaleFactor;
            }
            else
            {
                qreal kipToKiloNewton = 4.448;
                scaledMagnitude       = magnitude * kipToKiloNewton * mForceScaleFactor;
            }

            QString string(tr("%1 %2")
                           .arg(QString::number(magnitude, 'f', mUnitsAndLimits->jointLoadDecimals()))
                           .arg(mUnitsAndLimits->loadUnit()));

            if (position == 't')
            {
                painter.setPen(linePen);
                painter.drawLine(point(0.0, 0.0), point(0.0, 5.0 + scaledMagnitude));
                painter.drawLine(point(0.0, 5.0 + scaledMagnitude), point(-5.0, scaledMagnitude));
                painter.drawLine(point(0.0, 5.0 + scaledMagnitude), point(+5.0, scaledMagnitude));

                painter.setPen(textPen);

                painter.restore();

                w = 1.1 * fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(5.0, -5.0), point(w + 5.0, -h - 5.0)),
                                 Qt::AlignLeft | Qt::AlignTop,
                                 string);
            }
            else
            {
                painter.setPen(linePen);
                painter.drawLine(point(0.0, -5.0 - scaledMagnitude), point(0.0, 0.0));
                painter.drawLine(point(0.0, 0.0), point(-5.0, -5.0));
                painter.drawLine(point(0.0, 0.0), point(+5.0, -5.0));

                painter.setPen(textPen);

                painter.restore();

                w = 1.1 * fontMetrics.boundingRect(string).width();
                h = fontMetrics.boundingRect(string).height();

                painter.drawText(QRectF(point(5.0, h + 5.0), point(w + 5.0, 5.0)),
                                 Qt::AlignLeft | Qt::AlignBottom,
                                 string);
            }

            painter.restore();
        }
    }

    if (loadDisplayOption == SUPPORT_SETTLEMENT)
    {
        //Draw Support Settlements

        linePen.setColor(QColor(Qt::green));
        linePen.setWidth(2.0);

        foreach (SupportSettlement *supportSettlement, *mSupportSettlementsList)
        {
            qreal x = mCoordinateScaleFactor * supportSettlement->settlementSupport()->supportJoint()
                    ->xCoordinate();
            qreal y = mCoordinateScaleFactor * supportSettlement->settlementSupport()->supportJoint()
                    ->yCoordinate();

            painter.save();

            painter.translate(point(x, y));
            painter.setPen(linePen);

            painter.drawLine(0,   0, 20,  0);
            painter.drawLine(0,  20, 20, 20);
            painter.drawLine(15,  0, 15, 20);
            painter.drawLine(15, 20, 20, 15);
            painter.drawLine(15, 20, 10, 15);

            QString string = tr("%1S").arg(QString::fromUtf8("\u0394"));

            w = fontMetrics.boundingRect(string).width();
            h = fontMetrics.boundingRect(string).height();

            painter.setPen(textPen);

            painter.drawText(QRectF(point(1.5 * w, 0.0), point(2.5 * w + w, -h)),
                             Qt::AlignTop | Qt::AlignLeft,
                             string);

            painter.restore();
        }
    }

    if (loadDisplayOption == THERMAL_EFFECT)
    {
        //Draw Thermal Effects

        painter.setPen(textPen);

        qreal epsilonMagnitudeSmall = 1.0e-12;

        foreach (ThermalEffect *thermalEffect, *mThermalEffectsList)
        {
            Bar *bar = thermalEffect->thermalEffectBar();
            qreal xA = bar->firstJoint()->xCoordinate();
            qreal yA = bar->firstJoint()->yCoordinate();
            qreal xB = bar->secondJoint()->xCoordinate();
            qreal yB = bar->secondJoint()->yCoordinate();

            qreal angle = 0.0;

            if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
            {
                angle = 0.0;
            }
            else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
            {
                angle = qDegreesToRadians(90.0);
            }
            else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = std::acos(deltaY / length);
            }
            else
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
            }

            qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
            qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

            qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
            qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

            painter.save();

            painter.translate(point(xp, yp));
            painter.rotate(-90.0 + qRadiansToDegrees(angle));

            QString string = tr("%1T").arg(QString::fromUtf8("\u0394"));

            w = fontMetrics.boundingRect(string).width();
            h = fontMetrics.boundingRect(string).height();

            painter.drawText(QRectF(point(radius + 5.0, h + 5.0), point(radius + w + 10.0, 5.0)),
                             Qt::AlignTop | Qt::AlignRight,
                             string);

            painter.restore();
        }
    }

    if (loadDisplayOption == ASSEMBLY_FABRICATION_ERROR)
    {
        //Draw Fabrication Errors

        painter.setPen(textPen);

        qreal epsilonMagnitudeSmall = 1.0e-12;

        foreach (FabricationError *fabricationError, *mFabricationErrorsList)
        {
            Bar *bar = fabricationError->fabricationErrorBar();
            qreal xA = bar->firstJoint()->xCoordinate();
            qreal yA = bar->firstJoint()->yCoordinate();
            qreal xB = bar->secondJoint()->xCoordinate();
            qreal yB = bar->secondJoint()->yCoordinate();

            qreal angle = 0.0;

            if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
            {
                angle = 0.0;
            }
            else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
            {
                angle = qDegreesToRadians(90.0);
            }
            else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = std::acos(deltaY / length);
            }
            else
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
            }

            qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
            qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

            qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
            qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

            painter.save();

            painter.translate(point(xp, yp));
            painter.rotate(-90.0 + qRadiansToDegrees(angle));

            QString string = tr("%1L").arg(QString::fromUtf8("\u0394"));

            w = fontMetrics.boundingRect(string).width();
            h = fontMetrics.boundingRect(string).height();

            painter.drawText(QRectF(point(radius + 5.0, h + 5.0), point(radius + w + 10.0, 5.0)),
                             Qt::AlignTop | Qt::AlignRight,
                             string);

            painter.restore();
        }
    }

    if (loadDisplayOption == INFLUENCE_LOAD)
    {
        //Draw influence load paths

        linePen.setColor(QColor(Qt::darkMagenta));
        linePen.setWidth(5);
        painter.setPen(linePen);

        foreach (InfluenceLoad *influenceLoad, *mInfluenceLoadsList)
        {
            Joint *jointA = mJointsList->at(influenceLoad->path().first() - 1);
            Joint *jointB = mJointsList->at(influenceLoad->path().last() - 1);

            qreal x1 = mCoordinateScaleFactor * jointA->xCoordinate();
            qreal y1 = mCoordinateScaleFactor * jointA->yCoordinate();
            qreal x2 = mCoordinateScaleFactor * jointB->xCoordinate();
            qreal y2 = mCoordinateScaleFactor * jointB->yCoordinate();

            painter.drawLine(point(x1, y1), point(x2, y2));
        }
    }

    if (solutionDisplayOption == JOINT_DEFLECTION)
    {
        if (mBarsStandardItemModel->columnCount() == 5)
        {
            //Draw joint deflections

            linePen.setColor(QColor(Qt::blue));
            linePen.setWidth(1);
            painter.setPen(linePen);

            foreach (Bar *bar, *mBarsList)
            {
                Joint *jointA = bar->firstJoint();
                Joint *jointB = bar->secondJoint();

                int jointIndexA = mJointsList->indexOf(jointA);
                int jointIndexB = mJointsList->indexOf(jointB);

                qreal deflectionX1 = mHorizontalDeflectionComponentsList->at(jointIndexA);
                qreal deflectionY1 = mVerticalDeflectionComponentsList->at(jointIndexA);
                qreal deflectionX2 = mHorizontalDeflectionComponentsList->at(jointIndexB);
                qreal deflectionY2 = mVerticalDeflectionComponentsList->at(jointIndexB);

                qreal factor = 1.0;

                if (mUnitsAndLimits->system() == tr("metric"))
                {
                    factor = mDeflectionScaleFactor;
                }
                else
                {
                    qreal footToMeter = 0.3048;
                    factor            = mDeflectionScaleFactor * footToMeter;
                }

                qreal x1 = mCoordinateScaleFactor * jointA->xCoordinate() + factor * deflectionX1;
                qreal y1 = mCoordinateScaleFactor * jointA->yCoordinate() + factor * deflectionY1;
                qreal x2 = mCoordinateScaleFactor * jointB->xCoordinate() + factor * deflectionX2;
                qreal y2 = mCoordinateScaleFactor * jointB->yCoordinate() + factor * deflectionY2;

                painter.drawLine(point(x1, y1), point(x2, y2));
            }
        }
    }

    if (solutionDisplayOption == BAR_LOAD)
    {
        //Draw Bar Loads

        painter.setPen(textPen);

        qreal epsilonMagnitudeSmall = 1.0e-12;

        foreach (Bar *bar, *mBarsList)
        {
            qreal xA = bar->firstJoint()->xCoordinate();
            qreal yA = bar->firstJoint()->yCoordinate();
            qreal xB = bar->secondJoint()->xCoordinate();
            qreal yB = bar->secondJoint()->yCoordinate();

            qreal angle = 0.0;

            if (std::fabs(xA - xB) < epsilonMagnitudeSmall)
            {
                angle = 0.0;
            }
            else if (std::fabs(yA - yB) < epsilonMagnitudeSmall)
            {
                angle = qDegreesToRadians(90.0);
            }
            else if (((xA < xB) && (yA < yB)) || ((xB < xA) && (yB < yA)))
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = std::acos(deltaY / length);
            }
            else
            {
                qreal deltaX = std::fabs(xB - xA);
                qreal deltaY = std::fabs(yB - yA);
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
                angle        = qDegreesToRadians(90.0) + std::acos(deltaX / length);
            }

            qreal x1 = bar->firstJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y1 = bar->firstJoint()->yCoordinate() * mCoordinateScaleFactor;
            qreal x2 = bar->secondJoint()->xCoordinate() * mCoordinateScaleFactor;
            qreal y2 = bar->secondJoint()->yCoordinate() * mCoordinateScaleFactor;

            qreal xp = x1 / 3.0 + 2.0 * x2 / 3.0;
            qreal yp = y1 / 3.0 + 2.0 * y2 / 3.0;

            painter.save();

            painter.translate(point(xp, yp));
            painter.rotate(-90.0 + qRadiansToDegrees(angle));

            QString unitString;

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                unitString = tr("N");
            }
            else
            {
                unitString = tr("lb");
            }

            int index     = mBarsList->indexOf(bar);
            qreal barLoad = mBarLoadsList->at(index);

            QString string(tr("%1 %2")
                           .arg(QString::number(barLoad, 'f', 1))
                           .arg(unitString));

            w = fontMetrics.boundingRect(string).width();
            h = fontMetrics.boundingRect(string).height();

            painter.drawText(QRectF(point(radius - 1.125 * w, 0.0 + 1.125 * h),
                                    point(radius + 1.125 * w, 0.0 - 1.125 * h)),
                             Qt::AlignTop | Qt::AlignRight,
                             string);

            painter.restore();

            if (barLoad < 0.0)
            {
                painter.setBrush(QBrush(QColor(255, 0, 0, 60)));
            }
            else if (barLoad > 0.0)
            {
                painter.setBrush(QBrush(QColor(0, 255, 0, 60)));
            }
            else
            {
                continue;
            }

            xp = (x1 + x2) / 2.0;
            yp = (y1 + y2) / 2.0;

            painter.save();

            painter.translate(point(xp, yp));
            painter.rotate(-90.0 + qRadiansToDegrees(angle));

            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal width  = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            qreal height = 0.0;

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                qreal newtonToKiloNewton = 1.0e-3;
                height                   = newtonToKiloNewton * barLoad * mForceScaleFactor;
            }
            else
            {
                qreal poundToNewton      = 4.448;
                qreal newtonToKiloNewton = 1.0e-3;
                height                   = newtonToKiloNewton * poundToNewton * barLoad * mForceScaleFactor;
            }

            painter.drawRect(QRectF(QPointF(-width / 2.0, height / 2.0), QPointF(width / 2.0, -height / 2.0)));

            painter.restore();
        }
    }

    if (solutionDisplayOption == SUPPORT_REACTION)
    {
        //Draw Support Reactions

        foreach (Support *support, *mSupportsList)
        {
            Joint *joint = support->supportJoint();

            painter.save();
            qreal x = mCoordinateScaleFactor * joint->xCoordinate();
            qreal y = mCoordinateScaleFactor * joint->yCoordinate();
            painter.translate(point(x, y));

            int index = mSupportsList->indexOf(support);

            qreal horizontalComponent = mReactionHorizontalComponentsList->at(index);
            qreal verticalComponent   = mReactionVerticalComponentsList->at(index);

            qreal scaledMagnitude       = 0.0;
            qreal epsilonMagnitudeSmall = 1.0e-12;

            if (std::fabs(horizontalComponent) > epsilonMagnitudeSmall)
            {
                if (horizontalComponent > 0.0)
                {
                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = horizontalComponent * newtonToKiloNewton * mForceScaleFactor;
                    }
                    else
                    {
                        qreal poundToNewton      = 4.448;
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = horizontalComponent * poundToNewton * newtonToKiloNewton
                                * mForceScaleFactor;
                    }

                    painter.setPen(linePen);
                    painter.drawLine(point(-5.0, 0.0), point(-scaledMagnitude - 10.0, 0.0));
                    painter.drawLine(point(-5.0, 0.0), point(-10.0,  5.0));
                    painter.drawLine(point(-5.0, 0.0), point(-10.0, -5.0));
                }
                else
                {
                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = std::fabs(horizontalComponent) * newtonToKiloNewton
                                * mForceScaleFactor;
                    }
                    else
                    {
                        qreal poundToNewton      = 4.448;
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = std::fabs(horizontalComponent) * poundToNewton * newtonToKiloNewton
                                * mForceScaleFactor;
                    }

                    painter.setPen(linePen);
                    painter.drawLine(point(-5.0, 0.0), point(-scaledMagnitude - 10.0, 0.0));
                    painter.drawLine(point(-scaledMagnitude - 10.0, 0.0), point(-scaledMagnitude - 5.0,  5.0));
                    painter.drawLine(point(-scaledMagnitude - 10.0, 0.0), point(-scaledMagnitude - 5.0, -5.0));
                }
            }

            if (std::fabs(verticalComponent) > epsilonMagnitudeSmall)
            {
                if (verticalComponent > 0.0)
                {
                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = verticalComponent * newtonToKiloNewton * mForceScaleFactor;
                    }
                    else
                    {
                        qreal poundToNewton      = 4.448;
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = verticalComponent * poundToNewton * newtonToKiloNewton
                                * mForceScaleFactor;
                    }

                    painter.setPen(linePen);
                    painter.drawLine(point(0.0, -5.0), point(0.0 ,-scaledMagnitude - 10.0));
                    painter.drawLine(point(0.0, -5.0), point( 5.0, -10.0));
                    painter.drawLine(point(0.0, -5.0), point(-5.0, -10.0));
                }
                else
                {
                    if (mUnitsAndLimits->system() == tr("metric"))
                    {
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = std::fabs(verticalComponent) * newtonToKiloNewton
                                * mForceScaleFactor;
                    }
                    else
                    {
                        qreal poundToNewton      = 4.448;
                        qreal newtonToKiloNewton = 1.0e-3;
                        scaledMagnitude          = std::fabs(verticalComponent) * poundToNewton * newtonToKiloNewton
                                * mForceScaleFactor;
                    }

                    painter.setPen(linePen);
                    painter.drawLine(point(0.0, -5.0), point(0.0 ,-scaledMagnitude - 10.0));
                    painter.drawLine(point(0.0, -scaledMagnitude - 10.0), point( 5.0, -scaledMagnitude - 5.0));
                    painter.drawLine(point(0.0, -scaledMagnitude - 10.0), point(-5.0, -scaledMagnitude - 5.0));
                }
            }

            painter.setPen(textPen);
            QString unitString;

            if (mUnitsAndLimits->system() == tr("metric"))
            {
                unitString = tr("N");
            }
            else
            {
                unitString = tr("lb");
            }

            QString string = tr("Rx : %1 %3\nRy : %2 %3")
                    .arg(QString::number(horizontalComponent, 'f', 1))
                    .arg(QString::number(verticalComponent, 'f', 1))
                    .arg(unitString);

            w = fontMetrics.boundingRect(string).width();
            h = fontMetrics.boundingRect(string).height();

            painter.drawText(QRectF(point(5.0, -5.0), point(w + 5.0, -2.0 * h - 5.0)),
                             Qt::AlignLeft | Qt::AlignTop,
                             string);

            painter.restore();
        }
    }

    if (solutionDisplayOption == INFLUENCE_LOAD_SOLUTION)
    {
        //Draw influence lines

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

            int count = influenceLoad->path().size();

            if (mInfluenceLoadResult->influenceLoadOrdinatesList(0).size() == count)
            {
                QString s = tr("Bar # %1 influence line").arg(QString::number(influenceLoadBarIndex + 1));
                w         = fontMetrics.boundingRect(s).width();
                h         = fontMetrics.boundingRect(s).height();

                qreal x1 = 0.0;
                qreal y1 = height() - 2.0 * mMarginOffset;
                qreal x2 = x1 + w;
                qreal y2 = y1 - h;
                painter.drawText(QRectF(point(x1, y1), point(x2, y2)), Qt::AlignLeft | Qt::AlignTop, s);

                linePen.setColor(QColor(Qt::blue));
                linePen.setWidth(1);

                painter.save();

                QPointF firstPoint;

                QList<qreal> ordinatesList;
                ordinatesList              = mInfluenceLoadResult->influenceLoadOrdinatesList(influenceLoadBarIndex);
                qreal maxOrdinateMagnitude = 1.0;

                foreach (qreal ordinate, ordinatesList)
                {
                    if (std::fabs(ordinate) > maxOrdinateMagnitude)
                    {
                        maxOrdinateMagnitude = std::fabs(ordinate);
                    }
                }

                qreal ordinateFactor = 0.75 * ((height() - 2.0 * mMarginOffset) / (2.0)) / maxOrdinateMagnitude;

                for (int i = 0; i < count - 1; ++i)
                {
                    int jointAIndex = influenceLoad->path().at(i) - 1;
                    int jointBIndex = influenceLoad->path().at(i + 1) - 1;
                    Joint *jointA   = mJointsList->at(jointAIndex);
                    Joint *jointB   = mJointsList->at(jointBIndex);
                    qreal xA        = jointA->xCoordinate() * mCoordinateScaleFactor;
                    qreal xB        = jointB->xCoordinate() * mCoordinateScaleFactor;
                    qreal yo        = (height() - 2.0 * mMarginOffset) / 2.0;
                    qreal iA        = mInfluenceLoadResult->influenceLoadOrdinatesList(influenceLoadBarIndex).at(i);
                    qreal iB        = mInfluenceLoadResult->influenceLoadOrdinatesList(influenceLoadBarIndex).at(i + 1);
                    qreal yA        = yo + iA * ordinateFactor;
                    qreal yB        = yo + iB * ordinateFactor;

                    painter.setPen(linePen);
                    painter.drawLine(point(xA, yA), point(xB, yB));

                    if (i == 0)
                    {
                        firstPoint = point(xA, yA);
                    }

                    if (i == count - 2)
                    {
                        qreal x1 = firstPoint.x();
                        qreal y1 = (height() - 2.0 * mMarginOffset) / 2.0;
                        qreal x2 = xB;
                        qreal y2 = y1;

                        painter.drawLine(point(x1, y1), point(x2, y2));

                        painter.setPen(textPen);

                        QString text(QString::number(iB, 'g', 3));
                        w = fontMetrics.boundingRect(text).width();
                        h = fontMetrics.boundingRect(text).height();

                        painter.drawText(QRectF(point(xB, yB + h), point(xB + w, yB)),
                                         Qt::AlignLeft | Qt::AlignBottom,
                                         text);
                    }

                    painter.setPen(textPen);

                    QString text(QString::number(iA, 'g', 3));
                    w = fontMetrics.boundingRect(text).width();
                    h = fontMetrics.boundingRect(text).height();

                    painter.drawText(QRectF(point(xA, yA + h), point(xA + w, yA)),
                                     Qt::AlignLeft | Qt::AlignBottom,
                                     text);
                }

                painter.restore();
            }
        }
    }

    return image;
}

