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

/* unitsandlimits.h */

#ifndef UNITSANDLIMITS_H
#define UNITSANDLIMITS_H

#include <QObject>

class UnitsAndLimits : public QObject
{
        Q_OBJECT

    public:
        explicit UnitsAndLimits(QObject *parent = 0);

        explicit UnitsAndLimits(const UnitsAndLimits &unitsAndLimits, QObject *parent = 0);

        ~UnitsAndLimits();

        UnitsAndLimits& operator=(const UnitsAndLimits &unitsAndLimits);

        enum SupportType
        {
            FIXED_LEFT,
            FIXED_TOP,
            FIXED_RIGHT,
            FIXED_BOTTOM,
            ROLLER,
            ROLLER_LEFT,
            ROLLER_TOP,
            ROLLER_RIGHT,
            ROLLER_BOTTOM
        };

        // Coordinates
        static const qreal kMaxXCoordinateMeter      = 1000.0;
        static const qreal kMaxYCoordinateMeter      = 1000.0;
        static const qreal kMaxXCoordinateFoot       = 3000.0;
        static const qreal kMaxYCoordinateFoot       = 3000.0;
        static const int   kDeltaXMeterDecimals      = 3;
        static const int   kDeltaXCentimeterDecimals = 1;
        static const int   kDeltaXMillimeterDecimals = 0;
        static const int   kDeltaXFootDecimals       = 3;
        static const int   kDeltaXInchDecimals       = 2;
        static const int   kDeltaYMeterDecimals      = 3;
        static const int   kDeltaYCentimeterDecimals = 1;
        static const int   kDeltaYMillimeterDecimals = 0;
        static const int   kDeltaYFootDecimals       = 3;
        static const int   kDeltaYInchDecimals       = 2;

        // Bar Length
        static const qreal kMinBarLengthMeter = 0.3;
        static const qreal kMaxBarLengthMeter = 30.0;
        static const qreal kMinBarLengthFoot  = 1.0;
        static const qreal kMaxBarLengthFoot  = 100.0;

        // Bar Area
        static const qreal kMinAreaSquareMillimeter  = 10.0;
        static const qreal kMaxAreaSquareMillimeter  = 800000.0;
        static const qreal kMinAreaSquareInch        = 0.02;
        static const qreal kMaxAreaSquareInch        = 1000.0;
        static const int   kSquareMeterDecimals      = 6;
        static const int   kSquareCentimeterDecimals = 2;
        static const int   kSquareMillimeterDecimals = 0;
        static const int   kSquareFootDecimals       = 5;
        static const int   kSquareInchDecimals       = 3;

        // Bar Modulus
        static const qreal kMinModulusGigaPascal = 8.0;
        static const qreal kMaxModulusGigaPascal = 500.0;
        static const qreal kMinModulusPsi        = 1000000.0;
        static const qreal kMaxModulusPsi        = 70000000.0;
        static const int   kModulusDecimals      = 2;

        // Factor
        static const qreal kMinFactor      = 1.0;
        static const qreal kMaxFactor      = 10.0;
        static const int   kFactorDecimals = 2;

        // Bar Unit weight
        static const qreal kMinUnitWeightKiloNewtonPerCubicMeter = 5.0;
        static const qreal kMaxUnitWeightKiloNewtonPerCubicMeter = 100.0;
        static const qreal kMinUnitWeightPoundPerCubicFoot       = 30.0;
        static const qreal kMaxUnitWeightPoundPerCubicFoot       = 600.0;
        static const int   kUnitWeightDecimals                   = 2;

        // Support Angle
        static const int kSupportAngleDecimals = 1;

        // Joint Load
        static const qreal kMinJointLoadResultantMagnitudeKiloNewton = 1.0;
        static const qreal kMaxJointLoadComponentMagnitudeKiloNewton = 1000.0;
        static const qreal kMinJointLoadResultantMagnitudeKip        = 0.2;
        static const qreal kMaxJointLoadComponentMagnitudeKip        = 200.0;
        static const int   kJointLoadNewtonDecimals                  = 0;
        static const int   kJointLoadKiloNewtonDecimals              = 3;
        static const int   kJointLoadPoundDecimals                   = 1;
        static const int   kJointLoadKipDecimals                     = 4;

        // Settlement
        static const qreal kMinSettlementMillimeter      = 1.0;
        static const qreal kMaxSettlementMillimeter      = 50;
        static const qreal kMinSettlementInch            = 0.04;
        static const qreal kMaxSettlementInch            = 2.0;
        static const int   kSettlementMeterDecimals      = 3;
        static const int   kSettlementMillimeterDecimals = 0;
        static const int   kSettlementFootDecimals       = 3;
        static const int   kSettlementInchDecimals       = 2;

        // Temperature change
        static const qreal kMinTemperatureChangeCentigrade = 5.0;
        static const qreal kMaxTemperatureChangeCentigrade = 200.0;
        static const qreal kMinTemperatureChangeFahrenheit = 9.0;
        static const qreal kMaxTemperatureChangeFahrenheit = 400.0;
        static const int   kTemperatureChangeDecimals      = 2;

        // Thermal coefficient
        static const qreal kMinThermalCoefficientCentigrade = 8.0e-6;
        static const qreal kMaxThermalCoefficientCentigrade = 30.0e-6;
        static const qreal kMinThermalCoefficientFahrenheit = 4.0e-6;
        static const qreal kMaxThermalCoefficientFahrenheit = 14.0e-6;
        static const int   kThermalCoefficientDecimals      = 7;

        // Length Error
        static const qreal kMinLengthErrorMagnitudeMillimeter = 1.0;
        static const qreal kMaxLengthErrorMagnitudeMillimeter = 100.0;
        static const qreal kMinLengthErrorMagnitudeInch       = 0.04;
        static const qreal kMaxLengthErrorMagnitudeInch       = 4.0;
        static const int   kLengthErrorMeterDecimals          = 3;
        static const int   kLengthErrorMillimeterDecimals     = 0;
        static const int   kLengthErrorFootDecimals           = 3;
        static const int   kLengthErrorInchDecimals           = 2;

        // Point load
        static const qreal kMinPointLoadKiloNewton      = 1.0;
        static const qreal kMaxPointLoadKiloNewton      = 1000.0;
        static const qreal kMinPointLoadKip             = 0.2;
        static const qreal kMaxPointLoadKip             = 200.0;
        static const int   kPointLoadNewtonDecimals     = 0;
        static const int   kPointLoadKiloNewtonDecimals = 3;
        static const int   kPointLoadPoundDecimals      = 1;
        static const int   kPointLoadKipDecimals        = 4;

        // Offset
        static const qreal kMinOffsetMeter           = 1.0;
        static const qreal kMaxOffsetMeter           = 20.0;
        static const qreal kMinOffsetFoot            = 3.0;
        static const qreal kMaxOffsetFoot            = 60.0;
        static const int   kOffsetMeterDecimals      = 3;
        static const int   kOffsetCentimeterDecimals = 1;
        static const int   kOffsetMillimeterDecimals = 0;
        static const int   kOffsetFootDecimals       = 3;
        static const int   kOffsetInchDecimals       = 2;

        // Number of point loads
        static const int kMaxPointLoads = 20;

        qreal maxXCoordinate();
        qreal maxYCoordinate();
        qreal minBarLength();
        qreal maxBarLength();
        qreal minArea();
        qreal maxArea();
        qreal minModulus();
        qreal maxModulus();
        qreal minFactor();
        qreal maxFactor();
        qreal minUnitWeight();
        qreal maxUnitWeight();
        qreal minJointLoadResultant();
        qreal maxJointLoadComponent();
        qreal minSupportSettlement();
        qreal maxSupportSettlement();
        qreal minTemperatureChange();
        qreal maxTemperatureChange();
        qreal minThermalCoefficient();
        qreal maxThermalCoefficient();
        qreal minLengthError();
        qreal maxLengthError();
        qreal minPointLoad();
        qreal maxPointLoad();
        qreal minOffset();
        qreal maxOffset();

        int deltaXDecimals();
        int deltaYDecimals();
        int jointLoadDecimals();
        int areaDecimals();
        int modulusDecimals();
        int factorDecimals();
        int unitWeightDecimals();
        int supportAngleDecimals();
        int supportSettlementDecimals();
        int temperatureChangeDecimals();
        int thermalCoefficientDecimals();
        int lengthErrorDecimals();
        int pointLoadDecimals();
        int offsetDecimals();
        int maxPointLoads();

        const QString &system() const;
        const QString &coordinateUnit() const;
        const QString &areaUnit() const;
        const QString &modulusUnit() const;
        const QString &unitWeightUnit() const;
        const QString &loadUnit() const;
        const QString &supportSettlementUnit() const;
        const QString &temperatureChangeUnit() const;
        const QString &thermalCoefficientUnit() const;
        const QString &lengthErrorUnit() const;

        void setSystem(const QString &system);
        void setCoordinateUnit(const QString &coordinateUnit);
        void setAreaUnit(const QString &areaUnit);
        void setModulusUnit(const QString &modulusUnit);
        void setUnitWeightUnit(const QString &unitWeightUnit);
        void setLoadUnit(const QString &loadUnit);
        void setSupportSettlementUnit(const QString &supportSettlementUnit);
        void setTemperatureChangeUnit(const QString &temperatureChangeUnit);
        void setThermalCoefficientUnit(const QString &thermalCoefficientUnit);
        void setLengthErrorUnit(const QString &lengthErrorUnit);

    private:
        QString mSystem;
        QString mCoordinateUnit;
        QString mAreaUnit;
        QString mModulusUnit;
        QString mUnitWeightUnit;
        QString mLoadUnit;
        QString mSupportSettlementUnit;
        QString mTemperatureChangeUnit;
        QString mThermalCoefficientUnit;
        QString mLengthErrorUnit;
};

#endif // UNITSANDLIMITS_H
