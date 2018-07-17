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

/* unitsandlimits.cpp */

#include "unitsandlimits.h"

UnitsAndLimits::UnitsAndLimits(QObject *parent) : QObject(parent)
{
    mSystem                 = tr("metric");
    mCoordinateUnit         = tr("m");
    mAreaUnit               = tr("mm%1").arg(QString::fromUtf8("\u00B2"));
    mModulusUnit            = tr("GPa");
    mUnitWeightUnit         = tr("kN/m%1").arg(QString::fromUtf8("\u00B3"));
    mLoadUnit               = tr("kN");
    mSupportSettlementUnit  = tr("mm");
    mTemperatureChangeUnit  = tr("%2C").arg(QString::fromUtf8("\u00B0"));
    mThermalCoefficientUnit = tr("/%2C").arg(QString::fromUtf8("\u00B0"));
    mLengthErrorUnit        = tr("mm");
}

UnitsAndLimits::UnitsAndLimits(const UnitsAndLimits &unitsAndLimits, QObject *parent) : QObject(parent)
{
    mSystem                 = unitsAndLimits.system();
    mCoordinateUnit         = unitsAndLimits.coordinateUnit();
    mAreaUnit               = unitsAndLimits.areaUnit();
    mModulusUnit            = unitsAndLimits.modulusUnit();
    mUnitWeightUnit         = unitsAndLimits.unitWeightUnit();
    mLoadUnit               = unitsAndLimits.loadUnit();
    mSupportSettlementUnit  = unitsAndLimits.supportSettlementUnit();
    mTemperatureChangeUnit  = unitsAndLimits.temperatureChangeUnit();
    mThermalCoefficientUnit = unitsAndLimits.thermalCoefficientUnit();
    mLengthErrorUnit        = unitsAndLimits.lengthErrorUnit();
}

UnitsAndLimits& UnitsAndLimits::operator=(const UnitsAndLimits &unitsAndLimits)
{
    if (this != &unitsAndLimits)
    {
        mSystem                 = unitsAndLimits.system();
        mCoordinateUnit         = unitsAndLimits.coordinateUnit();
        mAreaUnit               = unitsAndLimits.areaUnit();
        mModulusUnit            = unitsAndLimits.modulusUnit();
        mUnitWeightUnit         = unitsAndLimits.unitWeightUnit();
        mLoadUnit               = unitsAndLimits.loadUnit();
        mSupportSettlementUnit  = unitsAndLimits.supportSettlementUnit();
        mTemperatureChangeUnit  = unitsAndLimits.temperatureChangeUnit();
        mThermalCoefficientUnit = unitsAndLimits.thermalCoefficientUnit();
        mLengthErrorUnit        = unitsAndLimits.lengthErrorUnit();
    }

    return *this;
}

UnitsAndLimits::~UnitsAndLimits()
{

}

qreal UnitsAndLimits::maxXCoordinate()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMaxXCoordinateMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMaxXCoordinateMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMaxXCoordinateMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMaxXCoordinateFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMaxXCoordinateFoot * footToInch;
    }
}

qreal UnitsAndLimits::maxYCoordinate()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMaxYCoordinateMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMaxYCoordinateMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMaxYCoordinateMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMaxYCoordinateFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMaxYCoordinateFoot * footToInch;
    }
}

qreal UnitsAndLimits::minBarLength()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMinBarLengthMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMinBarLengthMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMinBarLengthMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMinBarLengthFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMinBarLengthFoot * footToInch;
    }
}

qreal UnitsAndLimits::maxBarLength()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMaxBarLengthMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMaxBarLengthMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMaxBarLengthMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMaxBarLengthFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMaxBarLengthFoot * footToInch;
    }
}

qreal UnitsAndLimits::minArea()
{
    if (mAreaUnit == tr("m%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareMillimeterToSquareMeter = 1.0e-6;
        return kMinAreaSquareMillimeter * squareMillimeterToSquareMeter;
    }
    else if (mAreaUnit == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareMillimeterToSquareCentimeter = 1.0e-2;
        return kMinAreaSquareMillimeter * squareMillimeterToSquareCentimeter;
    }
    else if (mAreaUnit == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kMinAreaSquareMillimeter;
    }
    else if (mAreaUnit == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareInchToSquareFoot = 1 / 144.0;
        return kMinAreaSquareInch * squareInchToSquareFoot;
    }
    else
    {
        return kMinAreaSquareInch;
    }
}

qreal UnitsAndLimits::maxArea()
{
    if (mAreaUnit == tr("m%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareMillimeterToSquareMeter = 1.0e-6;
        return kMaxAreaSquareMillimeter * squareMillimeterToSquareMeter;
    }
    else if (mAreaUnit == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareMillimeterToSquareCentimeter = 1.0e-2;
        return kMaxAreaSquareMillimeter * squareMillimeterToSquareCentimeter;
    }
    else if (mAreaUnit == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kMaxAreaSquareMillimeter;
    }
    else if (mAreaUnit == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareInchToSquareFoot = 1 / 144.0;
        return kMaxAreaSquareInch * squareInchToSquareFoot;
    }
    else
    {
        return kMaxAreaSquareInch;
    }
}

qreal UnitsAndLimits::minModulus()
{
    if (mModulusUnit == tr("N/m%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal gigaPascalToNewtonPerSquareMeter = 1.0e9;
        return kMinModulusGigaPascal * gigaPascalToNewtonPerSquareMeter;
    }
    else if (mModulusUnit == tr("N/mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal gigaPascalToNewtonPerSquareMillimeter = 1.0e15;
        return kMinModulusGigaPascal * gigaPascalToNewtonPerSquareMillimeter;
    }
    else if (mModulusUnit == tr("GPa"))
    {
        return kMinModulusGigaPascal;
    }
    else if (mModulusUnit == tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal poundPerSquareInchToPoundPerSquareFoot = 144.0;
        return kMinModulusPsi * poundPerSquareInchToPoundPerSquareFoot;
    }
    else if (mModulusUnit == tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kMinModulusPsi;
    }
    else
    {
        qreal poundPerSquareInchToKipPerSquareInch = 1.0e-3;
        return kMinModulusPsi * poundPerSquareInchToKipPerSquareInch;
    }
}

qreal UnitsAndLimits::maxModulus()
{
    if (mModulusUnit == tr("N/m%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal gigaPascalToNewtonPerSquareMeter = 1.0e9;
        return kMaxModulusGigaPascal * gigaPascalToNewtonPerSquareMeter;
    }
    else if (mModulusUnit == tr("N/mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal gigaPascalToNewtonPerSquareMillimeter = 1.0e15;
        return kMaxModulusGigaPascal * gigaPascalToNewtonPerSquareMillimeter;
    }
    else if (mModulusUnit == tr("GPa"))
    {
        return kMaxModulusGigaPascal;
    }
    else if (mModulusUnit == tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal poundPerSquareInchToPoundPerSquareFoot = 144.0;
        return kMaxModulusPsi * poundPerSquareInchToPoundPerSquareFoot;
    }
    else if (mModulusUnit == tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kMaxModulusPsi;
    }
    else
    {
        qreal poundPerSquareInchToKipPerSquareInch = 1.0e-3;
        return kMaxModulusPsi * poundPerSquareInchToKipPerSquareInch;
    }
}

qreal UnitsAndLimits::minFactor()
{
    return kMinFactor;
}

qreal UnitsAndLimits::maxFactor()
{
    return kMaxFactor;
}

qreal UnitsAndLimits::minUnitWeight()
{
    if (mUnitWeightUnit == tr("kN/m%1").arg(QString::fromUtf8("\u00B3")))
    {
        return kMinUnitWeightKiloNewtonPerCubicMeter;
    }
    else
    {
        return kMinUnitWeightPoundPerCubicFoot;
    }
}

qreal UnitsAndLimits::maxUnitWeight()
{
    if (mUnitWeightUnit == tr("kN/m%1").arg(QString::fromUtf8("\u00B3")))
    {
        return kMaxUnitWeightKiloNewtonPerCubicMeter;
    }
    else
    {
        return kMaxUnitWeightPoundPerCubicFoot;
    }
}

qreal UnitsAndLimits::minJointLoadResultant()
{
    if (mLoadUnit == tr("N"))
    {
        qreal kiloNewtonToNewton = 1.0e3;
        return kMinJointLoadResultantMagnitudeKiloNewton * kiloNewtonToNewton;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kMinJointLoadResultantMagnitudeKiloNewton;
    }
    else if (mLoadUnit == tr("lb"))
    {
        qreal kipToPound = 1.0e3;
        return kMinJointLoadResultantMagnitudeKip * kipToPound;
    }
    else
    {
        return kMinJointLoadResultantMagnitudeKip;
    }
}

qreal UnitsAndLimits::maxJointLoadComponent()
{
    if (mLoadUnit == tr("N"))
    {
        qreal kiloNewtonToNewton = 1.0e3;
        return kMaxJointLoadComponentMagnitudeKiloNewton * kiloNewtonToNewton;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kMaxJointLoadComponentMagnitudeKiloNewton;
    }
    else if (mLoadUnit == tr("lb"))
    {
        qreal kipToPound = 1.0e3;
        return kMaxJointLoadComponentMagnitudeKip * kipToPound;
    }
    else
    {
        return kMaxJointLoadComponentMagnitudeKip;
    }
}

qreal UnitsAndLimits::minSupportSettlement()
{
    if (mSupportSettlementUnit == tr("m"))
    {
        qreal millimeterToMeter = 1.0e-3;
        return kMinSettlementMillimeter * millimeterToMeter;
    }
    else if (mSupportSettlementUnit == tr("mm"))
    {
        return kMinSettlementMillimeter;
    }
    else if (mSupportSettlementUnit == tr("ft"))
    {
        qreal inchToFoot = 1.0 / 12.0;
        return kMinSettlementInch * inchToFoot;
    }
    else
    {
        return kMinSettlementInch;
    }
}

qreal UnitsAndLimits::maxSupportSettlement()
{
    if (mSupportSettlementUnit == tr("m"))
    {
        qreal millimeterToMeter = 1.0e-3;
        return kMaxSettlementMillimeter * millimeterToMeter;
    }
    else if (mSupportSettlementUnit == tr("mm"))
    {
        return kMaxSettlementMillimeter;
    }
    else if (mSupportSettlementUnit == tr("ft"))
    {
        qreal inchToFoot = 1.0 / 12.0;
        return kMaxSettlementInch * inchToFoot;
    }
    else
    {
        return kMaxSettlementInch;
    }
}

qreal UnitsAndLimits::minTemperatureChange()
{
    if (mTemperatureChangeUnit == tr("%1C").arg(QString::fromUtf8("\u00B0")))
    {
        return kMinTemperatureChangeCentigrade;
    }
    else
    {
        return kMinTemperatureChangeFahrenheit;
    }
}

qreal UnitsAndLimits::maxTemperatureChange()
{
    if (mTemperatureChangeUnit == tr("%1C").arg(QString::fromUtf8("\u00B0")))
    {
        return kMaxTemperatureChangeCentigrade;
    }
    else
    {
        return kMaxTemperatureChangeFahrenheit;
    }
}

qreal UnitsAndLimits::minThermalCoefficient()
{
    if (mThermalCoefficientUnit == tr("/%1C").arg(QString::fromUtf8("\u00B0")))
    {
        return kMinThermalCoefficientCentigrade;
    }
    else
    {
        return kMinThermalCoefficientFahrenheit;
    }
}

qreal UnitsAndLimits::maxThermalCoefficient()
{
    if (mThermalCoefficientUnit == tr("/%1C").arg(QString::fromUtf8("\u00B0")))
    {
        return kMaxThermalCoefficientCentigrade;
    }
    else
    {
        return kMaxThermalCoefficientFahrenheit;
    }
}

qreal UnitsAndLimits::minLengthError()
{
    if (mLengthErrorUnit == tr("m"))
    {
        qreal millimeterToMeter = 1.0e-3;
        return kMinLengthErrorMagnitudeMillimeter * millimeterToMeter;
    }
    else if (mLengthErrorUnit == tr("mm"))
    {
        return kMinLengthErrorMagnitudeMillimeter;
    }
    else if (mLengthErrorUnit == tr("ft"))
    {
        qreal inchToFoot = 1.0 / 12.0;
        return kMinLengthErrorMagnitudeInch * inchToFoot;
    }
    else
    {
        return kMinLengthErrorMagnitudeInch;
    }
}

qreal UnitsAndLimits::maxLengthError()
{
    if (mLengthErrorUnit == tr("m"))
    {
        qreal millimeterToMeter = 1.0e-3;
        return kMaxLengthErrorMagnitudeMillimeter * millimeterToMeter;
    }
    else if (mLengthErrorUnit == tr("mm"))
    {
        return kMaxLengthErrorMagnitudeMillimeter;
    }
    else if (mLengthErrorUnit == tr("ft"))
    {
        qreal inchToFoot = 1.0 / 12.0;
        return kMaxLengthErrorMagnitudeInch * inchToFoot;
    }
    else
    {
        return kMaxLengthErrorMagnitudeInch;
    }
}

qreal UnitsAndLimits::minPointLoad()
{
    if (mLoadUnit == tr("N"))
    {
        qreal kiloNewtonToNewton = 1.0e3;
        return kMinPointLoadKiloNewton * kiloNewtonToNewton;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kMinPointLoadKiloNewton;
    }
    else if (mLoadUnit == tr("lb"))
    {
        qreal kipToPound = 1.0e3;
        return kMinPointLoadKip * kipToPound;
    }
    else
    {
        return kMinPointLoadKip;
    }
}

qreal UnitsAndLimits::maxPointLoad()
{
    if (mLoadUnit == tr("N"))
    {
        qreal kiloNewtonToNewton = 1.0e3;
        return kMaxPointLoadKiloNewton * kiloNewtonToNewton;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kMaxPointLoadKiloNewton;
    }
    else if (mLoadUnit == tr("lb"))
    {
        qreal kipToPound = 1.0e3;
        return kMaxPointLoadKip * kipToPound;
    }
    else
    {
        return kMaxPointLoadKip;
    }
}

qreal UnitsAndLimits::minOffset()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMinOffsetMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMinOffsetMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMinOffsetMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMinOffsetFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMinOffsetFoot * footToInch;
    }
}

qreal UnitsAndLimits::maxOffset()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kMaxOffsetMeter;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        qreal meterToCentimeter = 100.0;
        return kMaxOffsetMeter * meterToCentimeter;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        qreal meterToMillimeter = 1000.0;
        return kMaxOffsetMeter * meterToMillimeter;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kMaxOffsetFoot;
    }
    else
    {
        qreal footToInch = 12.0;
        return kMaxOffsetFoot * footToInch;
    }
}

int UnitsAndLimits::deltaXDecimals()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kDeltaXMeterDecimals;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        return kDeltaXCentimeterDecimals;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        return kDeltaXMillimeterDecimals;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kDeltaXFootDecimals;
    }
    else
    {
        return kDeltaXInchDecimals;
    }
}

int UnitsAndLimits::deltaYDecimals()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kDeltaYMeterDecimals;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        return kDeltaYCentimeterDecimals;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        return kDeltaYMillimeterDecimals;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kDeltaYFootDecimals;
    }
    else
    {
        return kDeltaYInchDecimals;
    }
}

int UnitsAndLimits::jointLoadDecimals()
{
    if (mLoadUnit == tr("N"))
    {
        return kJointLoadNewtonDecimals;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kJointLoadKiloNewtonDecimals;
    }
    else if (mLoadUnit == tr("lb"))
    {
        return kJointLoadPoundDecimals;
    }
    else
    {
        return kJointLoadKipDecimals;
    }
}

int UnitsAndLimits::areaDecimals()
{
    if (mAreaUnit == tr("m%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kSquareMeterDecimals;
    }
    else if (mAreaUnit == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kSquareCentimeterDecimals;
    }
    else if (mAreaUnit == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kSquareMillimeterDecimals;
    }
    else if (mAreaUnit == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        return kSquareFootDecimals;
    }
    else
    {
        return kSquareInchDecimals;
    }
}

int UnitsAndLimits::modulusDecimals()
{
    return kModulusDecimals;
}

int UnitsAndLimits::factorDecimals()
{
    return kFactorDecimals;
}

int UnitsAndLimits::unitWeightDecimals()
{
    return kUnitWeightDecimals;
}

int UnitsAndLimits::supportAngleDecimals()
{
    return kSupportAngleDecimals;
}

int UnitsAndLimits::supportSettlementDecimals()
{
    if (mSupportSettlementUnit == tr("m"))
    {
        return kSettlementMeterDecimals;
    }
    else if (mSupportSettlementUnit == tr("mm"))
    {
        return kSettlementMillimeterDecimals;
    }
    else if (mSupportSettlementUnit == tr("ft"))
    {
        return kSettlementFootDecimals;
    }
    else
    {
        return kSettlementInchDecimals;
    }
}

int UnitsAndLimits::temperatureChangeDecimals()
{
    return kTemperatureChangeDecimals;
}

int UnitsAndLimits::thermalCoefficientDecimals()
{
    return kThermalCoefficientDecimals;
}

int UnitsAndLimits::lengthErrorDecimals()
{
    if (mLengthErrorUnit == tr("m"))
    {
        return kLengthErrorMeterDecimals;
    }
    else if (mLengthErrorUnit == tr("mm"))
    {
        return kLengthErrorMillimeterDecimals;
    }
    else if (mLengthErrorUnit == tr("ft"))
    {
        return kLengthErrorFootDecimals;
    }
    else
    {
        return kLengthErrorInchDecimals;
    }
}

int UnitsAndLimits::pointLoadDecimals()
{
    if (mLoadUnit == tr("N"))
    {
        return kPointLoadNewtonDecimals;
    }
    else if (mLoadUnit == tr("kN"))
    {
        return kPointLoadKiloNewtonDecimals;
    }
    else if (mLoadUnit == tr("lb"))
    {
        return kPointLoadPoundDecimals;
    }
    else
    {
        return kPointLoadKipDecimals;
    }
}

int UnitsAndLimits::offsetDecimals()
{
    if (mCoordinateUnit == tr("m"))
    {
        return kOffsetMeterDecimals;
    }
    else if (mCoordinateUnit == tr("cm"))
    {
        return kOffsetCentimeterDecimals;
    }
    else if (mCoordinateUnit == tr("mm"))
    {
        return kOffsetMillimeterDecimals;
    }
    else if (mCoordinateUnit == tr("ft"))
    {
        return kOffsetFootDecimals;
    }
    else
    {
        return kOffsetInchDecimals;
    }
}

int UnitsAndLimits::maxPointLoads()
{
    return kMaxPointLoads;
}

const QString &UnitsAndLimits::system() const
{
    return mSystem;
}

const QString &UnitsAndLimits::coordinateUnit() const
{
    return mCoordinateUnit;
}

const QString &UnitsAndLimits::areaUnit() const
{
    return mAreaUnit;
}

const QString &UnitsAndLimits::modulusUnit() const
{
    return mModulusUnit;
}

const QString &UnitsAndLimits::unitWeightUnit() const
{
    return mUnitWeightUnit;
}

const QString &UnitsAndLimits::loadUnit() const
{
    return mLoadUnit;
}

const QString &UnitsAndLimits::supportSettlementUnit() const
{
    return mSupportSettlementUnit;
}

const QString &UnitsAndLimits::temperatureChangeUnit() const
{
    return mTemperatureChangeUnit;
}

const QString &UnitsAndLimits::thermalCoefficientUnit() const
{
    return mThermalCoefficientUnit;
}

const QString &UnitsAndLimits::lengthErrorUnit() const
{
    return mLengthErrorUnit;
}

void UnitsAndLimits::setSystem(const QString &system)
{
    mSystem = system;
}

void UnitsAndLimits::setCoordinateUnit(const QString &coordinateUnit)
{
    mCoordinateUnit = coordinateUnit;
}

void UnitsAndLimits::setAreaUnit(const QString &areaUnit)
{
    mAreaUnit = areaUnit;
}

void UnitsAndLimits::setModulusUnit(const QString &modulusUnit)
{
    mModulusUnit = modulusUnit;
}

void UnitsAndLimits::setUnitWeightUnit(const QString &unitWeightUnit)
{
    mUnitWeightUnit = unitWeightUnit;
}

void UnitsAndLimits::setLoadUnit(const QString &loadUnit)
{
    mLoadUnit = loadUnit;
}

void UnitsAndLimits::setSupportSettlementUnit(const QString &supportSettlementUnit)
{
    mSupportSettlementUnit = supportSettlementUnit;
}

void UnitsAndLimits::setTemperatureChangeUnit(const QString &temperatureChangeUnit)
{
    mTemperatureChangeUnit = temperatureChangeUnit;
}

void UnitsAndLimits::setThermalCoefficientUnit(const QString &thermalCoefficientUnit)
{
    mThermalCoefficientUnit = thermalCoefficientUnit;
}

void UnitsAndLimits::setLengthErrorUnit(const QString &lengthErrorUnit)
{
    mLengthErrorUnit = lengthErrorUnit;
}
