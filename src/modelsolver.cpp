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

/* modelsolver.cpp */

#include "modelsolver.h"

ModelSolver::ModelSolver(const QList<Joint *>             &jointsList,
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
                         QObject                          *parent) : QThread(parent)
{
    mJointsList             = jointsList;
    mBarsList               = barsList;
    mSupportsList           = supportsList;
    mJointLoadsList         = jointLoadsList;
    mIncludeSelfWeight      = includeSelfWeight;
    mAreaModulusOption      = areaModulusOption;
    mSupportSettlementsList = supportSettlementsList;
    mThermalEffectsList     = thermalEffectsList;
    mFabricationErrorsList  = fabricationErrorsList;
    mInfluenceLoadsList     = influenceLoadsList;
    mInfluenceLoadName      = influenceLoadName;
    mInfluenceLoadResult    = influenceLoadResult;
    mUnitsAndLimits         = unitsAndLimits;

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ModelSolver::~ModelSolver()
{

}

//void ModelSolver::printVector(const char *caption, gsl_vector *vector)
//{
//    fprintf(stderr, "\n%s\n", caption);

//    size_t count = vector->size;

//    for (size_t i = 0; i < count; ++i)
//    {
//        fprintf(stderr, "%+-10.5g\n", gsl_vector_get(vector, i));
//    }
//}

void ModelSolver::run()
{
    // -----------------------------------------------------------------------------------------------------------------
    // Determine unit conversion factors
    // -----------------------------------------------------------------------------------------------------------------

    qreal areaConversionFactor              = 1.0;
    qreal modulusConversionFactor           = 1.0;
    qreal lengthConversionFactor            = 1.0;
    qreal unitWeightConversionFactor        = 1.0;
    qreal loadConversionFactor              = 1.0;
    qreal supportSettlementConversionFactor = 1.0;
    qreal lengthErrorConversionFactor       = 1.0;

    if (mUnitsAndLimits.areaUnit() == tr("m%1").arg(QString::fromUtf8("\u00B2")))
    {
        areaConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.areaUnit() == tr("cm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareCentimeterToSquareMeter = 1.0e-4;
        areaConversionFactor                = squareCentimeterToSquareMeter;
    }
    else if (mUnitsAndLimits.areaUnit() == tr("mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal squareMillimeterToSquareMeter = 1.0e-6;
        areaConversionFactor                = squareMillimeterToSquareMeter;
    }
    else if (mUnitsAndLimits.areaUnit() == tr("ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        areaConversionFactor = 1.0;
    }
    else
    {
        qreal squareInchToSquareFoot = 1.0 / 144.0;
        areaConversionFactor         = squareInchToSquareFoot;
    }

    if (mUnitsAndLimits.modulusUnit() == tr("N/m%1").arg(QString::fromUtf8("\u00B2")))
    {
        modulusConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.modulusUnit() == tr("N/mm%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal newtonPerSquareMillimeterToNewtonPerSquareMeter = 1.0e+6;
        modulusConversionFactor                               = newtonPerSquareMillimeterToNewtonPerSquareMeter;
    }
    else if (mUnitsAndLimits.modulusUnit() == tr("GPa"))
    {
        qreal gigaPascalToNewtonPerSquareMeter = 1.0e+9;
        modulusConversionFactor                = gigaPascalToNewtonPerSquareMeter;
    }
    else if (mUnitsAndLimits.modulusUnit() == tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")))
    {
        modulusConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.modulusUnit() == tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")))
    {
        qreal poundPerSquareInchToPoundPerSquareFoot = 144.0;
        modulusConversionFactor                      = poundPerSquareInchToPoundPerSquareFoot;
    }
    else
    {
        qreal kipPerSquareInchToPoundPerSquareFoot = 1000.0 * 144.0;
        modulusConversionFactor                    = kipPerSquareInchToPoundPerSquareFoot;
    }

    if (mUnitsAndLimits.coordinateUnit() == tr("m"))
    {
        lengthConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.coordinateUnit() == tr("cm"))
    {
        qreal centimeterToMeter = 1.0e-2;
        lengthConversionFactor  = centimeterToMeter;
    }
    else if (mUnitsAndLimits.coordinateUnit() == tr("mm"))
    {
        qreal millimeterToMeter = 1.0e-3;
        lengthConversionFactor  = millimeterToMeter;
    }
    else if (mUnitsAndLimits.coordinateUnit() == tr("ft"))
    {
        lengthConversionFactor = 1.0;
    }
    else
    {
        qreal inchToFoot       = 1.0 / 12.0;
        lengthConversionFactor = inchToFoot;
    }

    if (mUnitsAndLimits.unitWeightUnit() == tr("kN/m%1").arg(QString::fromUtf8("\u00B3")))
    {
        qreal kiloNewtonPerCubicMeterToNewtonPerCubicMeter = 1.0e+3;
        unitWeightConversionFactor                         = kiloNewtonPerCubicMeterToNewtonPerCubicMeter;
    }
    else
    {
        unitWeightConversionFactor = 1.0;
    }

    if (mUnitsAndLimits.loadUnit() == tr("N"))
    {
        loadConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.loadUnit() == tr("kN"))
    {
        qreal kiloNewtonToNewton = 1.0e+3;
        loadConversionFactor     = kiloNewtonToNewton;
    }
    else if (mUnitsAndLimits.loadUnit() == tr("lb"))
    {
        loadConversionFactor = 1.0;
    }
    else
    {
        qreal kipToPound     = 1.0e+3;
        loadConversionFactor = kipToPound;
    }

    if (mUnitsAndLimits.supportSettlementUnit() == tr("m"))
    {
        supportSettlementConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.supportSettlementUnit() == tr("mm"))
    {
        qreal millimeterToMeter           = 1.0e-3;
        supportSettlementConversionFactor = millimeterToMeter;
    }
    else if (mUnitsAndLimits.supportSettlementUnit() == tr("ft"))
    {
        supportSettlementConversionFactor = 1.0;
    }
    else
    {
        qreal inchToFoot                  = 1.0 / 12.0;
        supportSettlementConversionFactor = inchToFoot;
    }

    if (mUnitsAndLimits.lengthErrorUnit() == tr("m"))
    {
        lengthErrorConversionFactor = 1.0;
    }
    else if (mUnitsAndLimits.lengthErrorUnit() == tr("mm"))
    {
        qreal millimeterToMeter     = 1.0e-3;
        lengthErrorConversionFactor = millimeterToMeter;
    }
    else if (mUnitsAndLimits.lengthErrorUnit() == tr("ft"))
    {
        lengthErrorConversionFactor = 1.0;
    }
    else
    {
        qreal inchToFoot            = 1.0 / 12.0;
        lengthErrorConversionFactor = inchToFoot;
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Create substitute joints and bars for inclined roller supports
    // -----------------------------------------------------------------------------------------------------------------

    qreal maxAreaModulus = 0.0;
    qreal maxFactor      = 0.0;
    Bar *maxBar          = 0;
    qreal maxBarLength   = 0.0;

    foreach (Bar *bar, mBarsList)
    {
        qreal x1     = bar->firstJoint()->xCoordinate();
        qreal y1     = bar->firstJoint()->yCoordinate();
        qreal x2     = bar->secondJoint()->xCoordinate();
        qreal y2     = bar->secondJoint()->yCoordinate();
        qreal deltaX = x2 - x1;
        qreal deltaY = y2 - y1;
        qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

        if (mAreaModulusOption)
        {
            qreal areaModulus = bar->area() * bar->modulus() / length;

            if (areaModulus > maxAreaModulus)
            {
                maxAreaModulus = areaModulus;
            }

            maxBar       = bar;
            maxBarLength = length;
        }
        else
        {
            qreal factor = bar->factor() / length;

            if (factor > maxFactor)
            {
                maxFactor = factor;
            }

            maxBar       = bar;
            maxBarLength = length;
        }
    }

    QList<Joint *> substituteJointsList;
    QList<Bar *> substituteBarsList;

    foreach (Support *support, mSupportsList)
    {
        if (support->type() == UnitsAndLimits::ROLLER)
        {
            Joint *jointA     = 0;
            Joint *jointB     = 0;
            qreal  area       = 0.0;
            qreal  modulus    = 0.0;
            qreal  factor     = 1.0;
            qreal  unitWeight = 0.0;
            Bar   *bar        = 0;

            qreal length = mUnitsAndLimits.minBarLength();

            if (mAreaModulusOption)
            {
                modulus = mUnitsAndLimits.maxModulus();
                area    = kBigFactor * (maxBar->modulus() / modulus) * (length / maxBarLength) * maxBar->area();
            }
            else
            {
                factor = kBigFactor * (length / maxBarLength) * maxBar->factor();
            }

            qreal supportAngle = qDegreesToRadians(support->angle());
            qreal x            = support->supportJoint()->xCoordinate() - length * std::sin(supportAngle);
            qreal y            = support->supportJoint()->yCoordinate() - length * std::cos(supportAngle);

            jointA = support->supportJoint();
            jointB = new Joint(x, y);
            substituteJointsList.append(jointB);

            bar = new Bar(jointA,
                          jointB,
                          area,
                          modulus,
                          factor,
                          unitWeight);

            substituteBarsList.append(bar);
        }
    }

    foreach (Joint *joint, substituteJointsList)
    {
        mJointsList.append(joint);
    }

    foreach (Bar *bar, substituteBarsList)
    {
        mBarsList.append(bar);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Determine fixed degrees of freedom
    // -----------------------------------------------------------------------------------------------------------------

    QList<int> fixedDegreesOfFreedom;

    foreach (Support *support, mSupportsList)
    {
        UnitsAndLimits::SupportType type = support->type();
        int index                        = mJointsList.indexOf(support->supportJoint());

        switch (type)
        {
            case UnitsAndLimits::FIXED_LEFT:
            case UnitsAndLimits::FIXED_TOP:
            case UnitsAndLimits::FIXED_RIGHT:
            case UnitsAndLimits::FIXED_BOTTOM:
                fixedDegreesOfFreedom.append(2 * index);
                fixedDegreesOfFreedom.append(2 * index + 1);
                break;
            case UnitsAndLimits::ROLLER_TOP:
            case UnitsAndLimits::ROLLER_BOTTOM:
                fixedDegreesOfFreedom.append(2 * index + 1);
                break;
            case UnitsAndLimits::ROLLER_LEFT:
            case UnitsAndLimits::ROLLER_RIGHT:
                fixedDegreesOfFreedom.append(2 * index);
                break;
            case UnitsAndLimits::ROLLER:
                foreach (Bar *bar, substituteBarsList)
                {
                    if (bar->firstJoint() == support->supportJoint())
                    {
                        index = mJointsList.indexOf(bar->secondJoint());
                        break;
                    }
                }
                fixedDegreesOfFreedom.append(2 * index);
                fixedDegreesOfFreedom.append(2 * index + 1);
                break;
            default:
                break;
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Assemble stiffness matrices
    // -----------------------------------------------------------------------------------------------------------------

    QList<int> degreesOfFreedom;
    int order = 2 * mJointsList.size();

    for (int i = 0; i < order; ++i)
    {
        if (!fixedDegreesOfFreedom.contains(i))
        {
            degreesOfFreedom.append(i);
        }
    }

    qreal epsilonMagnitudeSmall = 1.0e-12;

    order = 2 * mJointsList.size() - fixedDegreesOfFreedom.size();

    gsl_spmatrix *k11TripletFormat = gsl_spmatrix_alloc(order, order);
    gsl_spmatrix *k12TripletFormat = gsl_spmatrix_alloc(order, fixedDegreesOfFreedom.size());
    gsl_spmatrix *k21TripletFormat = gsl_spmatrix_alloc(fixedDegreesOfFreedom.size(), order);
    gsl_spmatrix *k22TripletFormat = gsl_spmatrix_alloc(fixedDegreesOfFreedom.size(), fixedDegreesOfFreedom.size());

    foreach (Bar *bar, mBarsList)
    {
        qreal x1     = bar->firstJoint()->xCoordinate();
        qreal y1     = bar->firstJoint()->yCoordinate();
        qreal x2     = bar->secondJoint()->xCoordinate();
        qreal y2     = bar->secondJoint()->yCoordinate();
        qreal deltaX = x2 - x1;
        qreal deltaY = y2 - y1;
        qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
        qreal C      = deltaX / length;
        qreal S      = deltaY / length;

        QList<int> indexList;
        int index = mJointsList.indexOf(bar->firstJoint());
        indexList.append(2 * index);
        indexList.append(2 * index + 1);

        index = mJointsList.indexOf(bar->secondJoint());
        indexList.append(2 * index);
        indexList.append(2 * index + 1);

        qreal matrix[4][4] = {{ C * C,  C * S, -C * C, -C * S},
                              { C * S,  S * S, -C * S, -S * S},
                              {-C * C, -C * S,  C * C,  C * S},
                              {-C * S, -S * S,  C * S,  S * S}};

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                qreal value = 0.0;

                if (mAreaModulusOption)
                {
                    qreal area    = bar->area() * areaConversionFactor;
                    qreal modulus = bar->modulus() * modulusConversionFactor;
                    length *= lengthConversionFactor;
                    value = area * modulus * matrix[i][j] / length;
                }
                else
                {
                    length *= lengthConversionFactor;
                    value = bar->factor() * matrix[i][j] / length;
                }

                if (std::fabs(value) > epsilonMagnitudeSmall)
                {
                    int row = indexList[i];
                    int col = indexList[j];

                    if (degreesOfFreedom.contains(row) && degreesOfFreedom.contains(col))
                    {
                        int rowIndex       = degreesOfFreedom.indexOf(row);
                        int colIndex       = degreesOfFreedom.indexOf(col);
                        qreal currentValue = gsl_spmatrix_get(k11TripletFormat, rowIndex, colIndex);
                        currentValue += value;
                        gsl_spmatrix_set(k11TripletFormat, rowIndex, colIndex, currentValue);
                    }

                    if (degreesOfFreedom.contains(row) && fixedDegreesOfFreedom.contains(col))
                    {
                        int rowIndex       = degreesOfFreedom.indexOf(row);
                        int colIndex       = fixedDegreesOfFreedom.indexOf(col);
                        qreal currentValue = gsl_spmatrix_get(k12TripletFormat, rowIndex, colIndex);
                        currentValue += value;
                        gsl_spmatrix_set(k12TripletFormat, rowIndex, colIndex, currentValue);
                    }

                    if (fixedDegreesOfFreedom.contains(row) && degreesOfFreedom.contains(col))
                    {
                        int rowIndex       = fixedDegreesOfFreedom.indexOf(row);
                        int colIndex       = degreesOfFreedom.indexOf(col);
                        qreal currentValue = gsl_spmatrix_get(k21TripletFormat, rowIndex, colIndex);
                        currentValue += value;
                        gsl_spmatrix_set(k21TripletFormat, rowIndex, colIndex, currentValue);
                    }

                    if (fixedDegreesOfFreedom.contains(row) && fixedDegreesOfFreedom.contains(col))
                    {
                        int rowIndex       = fixedDegreesOfFreedom.indexOf(row);
                        int colIndex       = fixedDegreesOfFreedom.indexOf(col);
                        qreal currentValue = gsl_spmatrix_get(k22TripletFormat, rowIndex, colIndex);
                        currentValue += value;
                        gsl_spmatrix_set(k22TripletFormat, rowIndex, colIndex, currentValue);
                    }
                }
            }
        }
    }

    gsl_spmatrix *k11CompressedColumnFormat = gsl_spmatrix_ccs(k11TripletFormat);
    gsl_spmatrix *k12CompressedColumnFormat = gsl_spmatrix_ccs(k12TripletFormat);
    gsl_spmatrix *k21CompressedColumnFormat = gsl_spmatrix_ccs(k21TripletFormat);
    gsl_spmatrix *k22CompressedColumnFormat = gsl_spmatrix_ccs(k22TripletFormat);

    gsl_spmatrix_free(k12TripletFormat);
    gsl_spmatrix_free(k21TripletFormat);
    gsl_spmatrix_free(k22TripletFormat);

    // -----------------------------------------------------------------------------------------------------------------
    // Self-weight loads
    // -----------------------------------------------------------------------------------------------------------------

    QList<JointLoad *> additionalJointLoadsList;

    if (mIncludeSelfWeight)
    {
        foreach (Bar *bar, mBarsList)
        {
            if (substituteBarsList.contains(bar))
            {
                continue;
            }

            qreal x1     = bar->firstJoint()->xCoordinate();
            qreal y1     = bar->firstJoint()->yCoordinate();
            qreal x2     = bar->secondJoint()->xCoordinate();
            qreal y2     = bar->secondJoint()->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

            qreal area = bar->area() * areaConversionFactor;
            length *= lengthConversionFactor;
            qreal unitWeight = bar->unitWeight() * unitWeightConversionFactor;

            qreal verticalComponent = -area * length * unitWeight / 2.0;

            JointLoad *load = new JointLoad(bar->firstJoint(),
                                            0.0,
                                            verticalComponent / loadConversionFactor,
                                            QString("head"));

            additionalJointLoadsList.append(load);

            load = new JointLoad(bar->secondJoint(),
                                 0.0,
                                 verticalComponent / loadConversionFactor,
                                 QString("head"));

            additionalJointLoadsList.append(load);
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Initialise support reactions
    // -----------------------------------------------------------------------------------------------------------------

    QList<Joint *> supportJointsList;
    QList<qreal> reactionHorizontalComponentsList;
    QList<qreal> reactionVerticalComponentsList;

    foreach (Support *support, mSupportsList)
    {
        if (support->type() == UnitsAndLimits::ROLLER)
        {
            Bar *supportSubstituteBar = 0;

            foreach (Bar *bar, substituteBarsList)
            {
                if (bar->firstJoint() == support->supportJoint())
                {
                    supportSubstituteBar = bar;
                    break;
                }
            }

            Joint *joint = supportSubstituteBar->secondJoint();

            supportJointsList.append(joint);
            reactionHorizontalComponentsList.append(0.0);
            reactionVerticalComponentsList.append(0.0);
        }
        else
        {
            supportJointsList.append(support->supportJoint());
            reactionHorizontalComponentsList.append(0.0);
            reactionVerticalComponentsList.append(0.0);
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Analysis for joint loads
    // -----------------------------------------------------------------------------------------------------------------

    gsl_vector *deflectionsColumnVectorU = gsl_vector_calloc(order);
    gsl_vector *loadsColumnVectorK       = gsl_vector_calloc(order);

    foreach (JointLoad *load, additionalJointLoadsList)
    {
        Joint *joint = load->loadJoint();
        int indexA   = mJointsList.indexOf(joint);
        qreal V      = load->verticalComponent() * loadConversionFactor;

        if (supportJointsList.contains(joint))
        {
            int indexB = supportJointsList.indexOf(joint);

            Support *jointSupport = 0;

            foreach (Support *support, mSupportsList)
            {
                if (joint == support->supportJoint())
                {
                    jointSupport = support;
                    break;
                }
            }

            UnitsAndLimits::SupportType type = jointSupport->type();
            qreal value                      = 0.0;

            switch (type)
            {
                case UnitsAndLimits::FIXED_LEFT:
                case UnitsAndLimits::FIXED_TOP:
                case UnitsAndLimits::FIXED_RIGHT:
                case UnitsAndLimits::FIXED_BOTTOM:
                    reactionVerticalComponentsList[indexB] += -V;
                    break;
                case UnitsAndLimits::ROLLER_TOP:
                case UnitsAndLimits::ROLLER_BOTTOM:
                    reactionVerticalComponentsList[indexB] += -V;
                    break;
                case UnitsAndLimits::ROLLER_LEFT:
                case UnitsAndLimits::ROLLER_RIGHT:
                    value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
                    gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
                    break;
                case UnitsAndLimits::ROLLER:
                    value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
                    gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
                    break;
                default:
                    break;
            }
        }
        else
        {
            qreal value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
            gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
        }
    }

    int status = GSL_FAILURE;

    if (!mJointLoadsList.isEmpty()  || !additionalJointLoadsList.isEmpty())
    {
        foreach (JointLoad *load, mJointLoadsList)
        {
            Joint *joint = load->loadJoint();
            int indexA   = mJointsList.indexOf(joint);
            qreal H      = load->horizontalComponent() * loadConversionFactor;
            qreal V      = load->verticalComponent() * loadConversionFactor;

            if (supportJointsList.contains(joint))
            {
                int indexB = supportJointsList.indexOf(joint);

                Support *jointSupport = 0;

                foreach (Support *support, mSupportsList)
                {
                    if (joint == support->supportJoint())
                    {
                        jointSupport = support;
                        break;
                    }
                }

                UnitsAndLimits::SupportType type = jointSupport->type();
                qreal value                      = 0.0;

                switch (type)
                {
                    case UnitsAndLimits::FIXED_LEFT:
                    case UnitsAndLimits::FIXED_TOP:
                    case UnitsAndLimits::FIXED_RIGHT:
                    case UnitsAndLimits::FIXED_BOTTOM:
                        reactionHorizontalComponentsList[indexB] += -H;
                        reactionVerticalComponentsList[indexB]   += -V;
                        break;
                    case UnitsAndLimits::ROLLER_TOP:
                    case UnitsAndLimits::ROLLER_BOTTOM:
                        value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA)) + H;
                        gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA), value);
                        reactionVerticalComponentsList[indexB] += -V;
                        break;
                    case UnitsAndLimits::ROLLER_LEFT:
                    case UnitsAndLimits::ROLLER_RIGHT:
                        reactionHorizontalComponentsList[indexB] += -H;
                        value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
                        gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
                        break;
                    case UnitsAndLimits::ROLLER:
                        value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA)) + H;
                        gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA), value);
                        value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
                        gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                qreal value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA)) + H;
                gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA), value);
                value = gsl_vector_get(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1)) + V;
                gsl_vector_set(loadsColumnVectorK, degreesOfFreedom.indexOf(2 * indexA + 1), value);
            }
        }

        gsl_vector *loadsColumnVector = gsl_vector_calloc(loadsColumnVectorK->size);
        gsl_vector_memcpy(loadsColumnVector, loadsColumnVectorK);
        gsl_vector *deflectionsColumnVector = gsl_vector_calloc(loadsColumnVector->size);

        if (substituteBarsList.isEmpty())
        {
            size_t nZMax      = gsl_spmatrix_nnz(k11CompressedColumnFormat);
            size_t nRow       = k11CompressedColumnFormat->size1;
            size_t nCol       = k11CompressedColumnFormat->size2;
            gsl_spmatrix *k11 = gsl_spmatrix_alloc_nzmax(nRow,
                                                         nCol,
                                                         nZMax,
                                                         GSL_SPMATRIX_CCS);
            gsl_spmatrix_memcpy(k11, k11CompressedColumnFormat);

            const gsl_splinalg_itersolve_type *T = gsl_splinalg_itersolve_gmres;
            gsl_splinalg_itersolve *work         = gsl_splinalg_itersolve_alloc(T,
                                                                                k11->size1,
                                                                                0);

            size_t count = 0;
            do
            {
                status = gsl_splinalg_itersolve_iterate(k11,
                                                        loadsColumnVector,
                                                        kTolerance,
                                                        deflectionsColumnVector,
                                                        work);
            } while (status == GSL_CONTINUE && ++count < kMaxIterations);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }

            gsl_spmatrix_free(k11);
            gsl_splinalg_itersolve_free(work);
        }

        if (!substituteBarsList.isEmpty() || status != GSL_SUCCESS)
        {
            gsl_matrix *k11Dense = gsl_matrix_calloc(k11TripletFormat->size1, k11TripletFormat->size2);
            gsl_spmatrix_sp2d(k11Dense, k11TripletFormat);
            gsl_permutation *p = gsl_permutation_alloc(k11Dense->size1);
            gsl_linalg_pcholesky_decomp(k11Dense, p);
            status = gsl_linalg_pcholesky_solve(k11Dense,
                                                p,
                                                loadsColumnVector,
                                                deflectionsColumnVector);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                size_t count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }
            else
            {
                mSolutionsCount.append(false);
                //fprintf(stderr, "\nAnalysis for joint loads: failed to converge!\n");
            }

            gsl_matrix_free(k11Dense);
            gsl_permutation_free(p);
        }

        gsl_vector_free(loadsColumnVector);
        gsl_vector_free(deflectionsColumnVector);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Analysis for support settlements
    // -----------------------------------------------------------------------------------------------------------------

    gsl_vector *deflectionsColumnVectorK = gsl_vector_calloc(fixedDegreesOfFreedom.size());

    if (!mSupportSettlementsList.isEmpty())
    {
        foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
        {
            qreal settlement = supportSettlement->settlement() * supportSettlementConversionFactor;
            int index        = mJointsList.indexOf(supportSettlement->settlementSupport()->supportJoint());
            gsl_vector_set(deflectionsColumnVectorK,
                           fixedDegreesOfFreedom.indexOf(2 * index + 1),
                           -settlement);
        }

        size_t nZMax      = gsl_spmatrix_nnz(k12CompressedColumnFormat);
        size_t nRow       = k12CompressedColumnFormat->size1;
        size_t nCol       = k12CompressedColumnFormat->size2;
        gsl_spmatrix *k12 = gsl_spmatrix_alloc_nzmax(nRow,
                                                     nCol,
                                                     nZMax,
                                                     GSL_SPMATRIX_CCS);
        gsl_spmatrix_memcpy(k12, k12CompressedColumnFormat);
        gsl_vector *loadsColumnVector = gsl_vector_calloc(order);

        qreal alpha = -1.0;
        qreal beta  = 0.0;
        gsl_spblas_dgemv(CblasNoTrans,
                         alpha,
                         k12,
                         deflectionsColumnVectorK,
                         beta,
                         loadsColumnVector);

        gsl_vector *deflectionsColumnVector = gsl_vector_calloc(loadsColumnVector->size);

        status = GSL_FAILURE;

        if (substituteBarsList.isEmpty())
        {
            nZMax             = gsl_spmatrix_nnz(k11CompressedColumnFormat);
            nRow              = k11CompressedColumnFormat->size1;
            nCol              = k11CompressedColumnFormat->size2;
            gsl_spmatrix *k11 = gsl_spmatrix_alloc_nzmax(nRow,
                                                         nCol,
                                                         nZMax,
                                                         GSL_SPMATRIX_CCS);
            gsl_spmatrix_memcpy(k11, k11CompressedColumnFormat);

            const gsl_splinalg_itersolve_type *T = gsl_splinalg_itersolve_gmres;
            gsl_splinalg_itersolve *work         = gsl_splinalg_itersolve_alloc(T,
                                                                                k11->size1,
                                                                                0);

            size_t count = 0;
            do
            {
                status = gsl_splinalg_itersolve_iterate(k11,
                                                        loadsColumnVector,
                                                        kTolerance,
                                                        deflectionsColumnVector,
                                                        work);
            } while (status == GSL_CONTINUE && ++count < kMaxIterations);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }

            gsl_spmatrix_free(k11);
            gsl_splinalg_itersolve_free(work);
        }

        if (!substituteBarsList.isEmpty() || status != GSL_SUCCESS)
        {
            gsl_matrix *k11Dense = gsl_matrix_calloc(k11TripletFormat->size1, k11TripletFormat->size2);
            gsl_spmatrix_sp2d(k11Dense, k11TripletFormat);
            gsl_permutation *p = gsl_permutation_alloc(k11Dense->size1);
            gsl_linalg_pcholesky_decomp(k11Dense, p);
            status = gsl_linalg_pcholesky_solve(k11Dense,
                                                p,
                                                loadsColumnVector,
                                                deflectionsColumnVector);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                size_t count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }
            else
            {
                mSolutionsCount.append(false);
                //fprintf(stderr, "\nAnalysis for support settlements: failed to converge!\n");
            }

            gsl_matrix_free(k11Dense);
            gsl_permutation_free(p);
        }

        gsl_spmatrix_free(k12);
        gsl_vector_free(loadsColumnVector);
        gsl_vector_free(deflectionsColumnVector);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Analysis for thermal effects
    // -----------------------------------------------------------------------------------------------------------------

    if (!mThermalEffectsList.isEmpty())
    {
        gsl_vector *deflectionsColumnVector = gsl_vector_calloc(order);
        gsl_vector *loadsColumnVector       = gsl_vector_calloc(order);

        foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
        {
            Bar * bar    = thermalEffect->thermalEffectBar();
            qreal x1     = bar->firstJoint()->xCoordinate();
            qreal y1     = bar->firstJoint()->yCoordinate();
            qreal x2     = bar->secondJoint()->xCoordinate();
            qreal y2     = bar->secondJoint()->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            qreal C      = deltaX / length;
            qreal S      = deltaY / length;

            int firstJointIndex  = mJointsList.indexOf(bar->firstJoint());
            int secondJointIndex = mJointsList.indexOf(bar->secondJoint());

            qreal thermalEffectLoad = bar->area() * areaConversionFactor
                    * bar->modulus() * modulusConversionFactor
                    * thermalEffect->thermalCoefficient()
                    * thermalEffect->temperatureChange();

            int indexA = 2 * firstJointIndex;
            int indexB = indexA + 1;
            int indexC = 2 * secondJointIndex;
            int indexD = indexC + 1;

            if (degreesOfFreedom.contains(indexA))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexA));
                value += -thermalEffectLoad * C;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexA), value);
            }

            if (degreesOfFreedom.contains(indexB))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexB));
                value += -thermalEffectLoad * S;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexB), value);
            }

            if (degreesOfFreedom.contains(indexC))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexC));
                value += thermalEffectLoad * C;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexC), value);
            }

            if (degreesOfFreedom.contains(indexD))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexD));
                value += thermalEffectLoad * S;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexD), value);
            }
        }

        status = GSL_FAILURE;

        if (substituteBarsList.isEmpty())
        {
            size_t nZMax      = gsl_spmatrix_nnz(k11CompressedColumnFormat);
            size_t nRow       = k11CompressedColumnFormat->size1;
            size_t nCol       = k11CompressedColumnFormat->size2;
            gsl_spmatrix *k11 = gsl_spmatrix_alloc_nzmax(nRow,
                                                         nCol,
                                                         nZMax,
                                                         GSL_SPMATRIX_CCS);
            gsl_spmatrix_memcpy(k11, k11CompressedColumnFormat);

            const gsl_splinalg_itersolve_type *T = gsl_splinalg_itersolve_gmres;
            gsl_splinalg_itersolve *work         = gsl_splinalg_itersolve_alloc(T,
                                                                                k11->size1,
                                                                                0);

            size_t count = 0;
            do
            {
                status = gsl_splinalg_itersolve_iterate(k11,
                                                        loadsColumnVector,
                                                        kTolerance,
                                                        deflectionsColumnVector,
                                                        work);
            } while (status == GSL_CONTINUE && ++count < kMaxIterations);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }

            gsl_spmatrix_free(k11);
            gsl_splinalg_itersolve_free(work);
        }

        if (!substituteBarsList.isEmpty() || status != GSL_SUCCESS)
        {
            gsl_matrix *k11Dense = gsl_matrix_calloc(k11TripletFormat->size1, k11TripletFormat->size2);
            gsl_spmatrix_sp2d(k11Dense, k11TripletFormat);
            gsl_permutation *p = gsl_permutation_alloc(k11Dense->size1);
            gsl_linalg_pcholesky_decomp(k11Dense, p);
            status = gsl_linalg_pcholesky_solve(k11Dense,
                                                p,
                                                loadsColumnVector,
                                                deflectionsColumnVector);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                size_t count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }
            else
            {
                mSolutionsCount.append(false);
                //fprintf(stderr, "\nAnalysis for thermal effects: failed to converge!\n");
            }

            gsl_matrix_free(k11Dense);
            gsl_permutation_free(p);
        }

        gsl_vector_free(loadsColumnVector);
        gsl_vector_free(deflectionsColumnVector);

    }

    // -----------------------------------------------------------------------------------------------------------------
    // Analysis for fabrication errors
    // -----------------------------------------------------------------------------------------------------------------

    if (!mFabricationErrorsList.isEmpty())
    {
        gsl_vector *deflectionsColumnVector = gsl_vector_calloc(order);
        gsl_vector *loadsColumnVector       = gsl_vector_calloc(order);

        foreach (FabricationError *fabricationError, mFabricationErrorsList)
        {
            Bar * bar    = fabricationError->fabricationErrorBar();
            qreal x1     = bar->firstJoint()->xCoordinate();
            qreal y1     = bar->firstJoint()->yCoordinate();
            qreal x2     = bar->secondJoint()->xCoordinate();
            qreal y2     = bar->secondJoint()->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            qreal C      = deltaX / length;
            qreal S      = deltaY / length;

            int firstJointIndex  = mJointsList.indexOf(bar->firstJoint());
            int secondJointIndex = mJointsList.indexOf(bar->secondJoint());

            qreal fabricationErrorLoad = bar->area() * areaConversionFactor
                    * bar->modulus() * modulusConversionFactor
                    * fabricationError->lengthError() * lengthErrorConversionFactor
                    / (length * lengthConversionFactor);

            int indexA = 2 * firstJointIndex;
            int indexB = indexA + 1;
            int indexC = 2 * secondJointIndex;
            int indexD = indexC + 1;

            if (degreesOfFreedom.contains(indexA))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexA));
                value += -fabricationErrorLoad * C;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexA), value);
            }

            if (degreesOfFreedom.contains(indexB))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexB));
                value += -fabricationErrorLoad * S;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexB), value);
            }

            if (degreesOfFreedom.contains(indexC))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexC));
                value += fabricationErrorLoad * C;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexC), value);
            }

            if (degreesOfFreedom.contains(indexD))
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(indexD));
                value += fabricationErrorLoad * S;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(indexD), value);
            }
        }

        status = GSL_FAILURE;

        if (substituteBarsList.isEmpty())
        {
            size_t nZMax      = gsl_spmatrix_nnz(k11CompressedColumnFormat);
            size_t nRow       = k11CompressedColumnFormat->size1;
            size_t nCol       = k11CompressedColumnFormat->size2;
            gsl_spmatrix *k11 = gsl_spmatrix_alloc_nzmax(nRow,
                                                         nCol,
                                                         nZMax,
                                                         GSL_SPMATRIX_CCS);
            gsl_spmatrix_memcpy(k11, k11CompressedColumnFormat);

            const gsl_splinalg_itersolve_type *T = gsl_splinalg_itersolve_gmres;
            gsl_splinalg_itersolve *work         = gsl_splinalg_itersolve_alloc(T,
                                                                                k11->size1,
                                                                                0);

            size_t count = 0;
            do
            {
                status = gsl_splinalg_itersolve_iterate(k11,
                                                        loadsColumnVector,
                                                        kTolerance,
                                                        deflectionsColumnVector,
                                                        work);
            } while (status == GSL_CONTINUE && ++count < kMaxIterations);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }

            gsl_spmatrix_free(k11);
            gsl_splinalg_itersolve_free(work);
        }

        if (!substituteBarsList.isEmpty() || status != GSL_SUCCESS)
        {
            gsl_matrix *k11Dense = gsl_matrix_calloc(k11TripletFormat->size1, k11TripletFormat->size2);
            gsl_spmatrix_sp2d(k11Dense, k11TripletFormat);
            gsl_permutation *p = gsl_permutation_alloc(k11Dense->size1);
            gsl_linalg_pcholesky_decomp(k11Dense, p);
            status = gsl_linalg_pcholesky_solve(k11Dense,
                                                p,
                                                loadsColumnVector,
                                                deflectionsColumnVector);

            if (status == GSL_SUCCESS)
            {
                //fprintf(stderr, "\nConverged\n");
                size_t count = deflectionsColumnVector->size;

                for (size_t i = 0; i < count; ++i)
                {
                    qreal value = gsl_vector_get(deflectionsColumnVectorU, i);
                    value += gsl_vector_get(deflectionsColumnVector, i);
                    gsl_vector_set(deflectionsColumnVectorU, i, value);
                }

                mSolutionsCount.append(true);
            }
            else
            {
                mSolutionsCount.append(false);
                //fprintf(stderr, "\nAnalysis for fabrication errors: failed to converge!\n");
            }

            gsl_matrix_free(k11Dense);
            gsl_permutation_free(p);
        }

        gsl_vector_free(loadsColumnVector);
        gsl_vector_free(deflectionsColumnVector);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Analysis for influence loads
    // -----------------------------------------------------------------------------------------------------------------

    qreal loadLimitNewton = 1.0e-2;
    qreal loadLimitPound  = 2.0e-3;

    if (!mInfluenceLoadsList.isEmpty() && !mSolutionsCount.contains(false))
    {
        InfluenceLoad *influenceLoad = 0;

        foreach (InfluenceLoad *load, mInfluenceLoadsList)
        {
            if (load->name() == mInfluenceLoadName)
            {
                influenceLoad = load;
                break;
            }
        }

        mInfluenceLoadResult->setParameters(mBarsList.size(), influenceLoad->path().size());

        gsl_vector *deflectionsColumnVector = gsl_vector_calloc(order);
        gsl_vector *loadsColumnVector       = gsl_vector_calloc(order);

        foreach (int jointNumber, influenceLoad->path())
        {
            int jointIndex = jointNumber - 1;
            Joint *joint   = mJointsList.at(jointIndex);

            if (supportJointsList.contains(joint))
            {
                Support *jointSupport = 0;

                foreach (Support *support, mSupportsList)
                {
                    if (joint == support->supportJoint())
                    {
                        jointSupport = support;
                        break;
                    }
                }

                UnitsAndLimits::SupportType type = jointSupport->type();
                qreal value                      = 0.0;

                switch (type)
                {
                    case UnitsAndLimits::ROLLER_LEFT:
                    case UnitsAndLimits::ROLLER_RIGHT:
                        value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1)) - 1.0;
                        gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1), value);
                        break;
                    case UnitsAndLimits::ROLLER:
                        value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1)) - 1.0;
                        gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1), value);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                qreal value = gsl_vector_get(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1)) - 1.0;
                gsl_vector_set(loadsColumnVector, degreesOfFreedom.indexOf(2 * jointIndex + 1), value);
            }

            status = GSL_FAILURE;

            if (substituteBarsList.isEmpty())
            {
                size_t nZMax      = gsl_spmatrix_nnz(k11CompressedColumnFormat);
                size_t nRow       = k11CompressedColumnFormat->size1;
                size_t nCol       = k11CompressedColumnFormat->size2;
                gsl_spmatrix *k11 = gsl_spmatrix_alloc_nzmax(nRow,
                                                             nCol,
                                                             nZMax,
                                                             GSL_SPMATRIX_CCS);
                gsl_spmatrix_memcpy(k11, k11CompressedColumnFormat);

                const gsl_splinalg_itersolve_type *T = gsl_splinalg_itersolve_gmres;
                gsl_splinalg_itersolve *work         = gsl_splinalg_itersolve_alloc(T,
                                                                                    k11->size1,
                                                                                    0);

                size_t count = 0;
                do
                {
                    status = gsl_splinalg_itersolve_iterate(k11,
                                                            loadsColumnVector,
                                                            kTolerance,
                                                            deflectionsColumnVector,
                                                            work);
                } while (status == GSL_CONTINUE && ++count < kMaxIterations);

                if (status == GSL_SUCCESS)
                {
                    //fprintf(stderr, "\nConverged\n");
                    mSolutionsCount.append(true);
                }

                gsl_spmatrix_free(k11);
                gsl_splinalg_itersolve_free(work);
            }

            if (!substituteBarsList.isEmpty() || status != GSL_SUCCESS)
            {
                gsl_matrix *k11Dense = gsl_matrix_calloc(k11TripletFormat->size1, k11TripletFormat->size2);
                gsl_spmatrix_sp2d(k11Dense, k11TripletFormat);
                gsl_permutation *p = gsl_permutation_alloc(k11Dense->size1);
                gsl_linalg_pcholesky_decomp(k11Dense, p);
                status = gsl_linalg_pcholesky_solve(k11Dense,
                                                    p,
                                                    loadsColumnVector,
                                                    deflectionsColumnVector);

                if (status == GSL_SUCCESS)
                {
                    //fprintf(stderr, "\nConverged\n");
                    mSolutionsCount.append(true);
                }
                else
                {
                    mSolutionsCount.append(false);
                    //fprintf(stderr, "\nAnalysis for influence loads: failed to converge!\n");
                }

                gsl_matrix_free(k11Dense);
                gsl_permutation_free(p);
            }

            if (status != GSL_SUCCESS)
            {
                gsl_vector_set_zero(deflectionsColumnVector);
                gsl_vector_set_zero(loadsColumnVector);
                continue;
            }

            gsl_matrix *rowMatrix    = gsl_matrix_calloc(1, 4);
            gsl_vector *columnVector = gsl_vector_calloc(4);
            gsl_vector *product      = gsl_vector_calloc(1);

            foreach (Bar *bar, mBarsList)
            {
                qreal barLoad = 0.0;

                if (substituteBarsList.contains(bar))
                {
                    continue;
                }

                qreal x1     = bar->firstJoint()->xCoordinate();
                qreal y1     = bar->firstJoint()->yCoordinate();
                qreal x2     = bar->secondJoint()->xCoordinate();
                qreal y2     = bar->secondJoint()->yCoordinate();
                qreal deltaX = x2 - x1;
                qreal deltaY = y2 - y1;
                qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

                qreal C = deltaX / length;
                qreal S = deltaY / length;

                gsl_matrix_set(rowMatrix, 0, 0, -C);
                gsl_matrix_set(rowMatrix, 0, 1, -S);
                gsl_matrix_set(rowMatrix, 0, 2,  C);
                gsl_matrix_set(rowMatrix, 0, 3,  S);

                int firstJointIndex  = mJointsList.indexOf(bar->firstJoint());
                int secondJointIndex = mJointsList.indexOf(bar->secondJoint());
                int indexA           = 2 * firstJointIndex;
                int indexB           = indexA + 1;
                int indexC           = 2 * secondJointIndex;
                int indexD           = indexC + 1;

                if (degreesOfFreedom.contains(indexA))
                {
                    qreal value = gsl_vector_get(deflectionsColumnVector, degreesOfFreedom.indexOf(indexA));
                    gsl_vector_set(columnVector, 0, value);
                }

                if (degreesOfFreedom.contains(indexB))
                {
                    qreal value = gsl_vector_get(deflectionsColumnVector, degreesOfFreedom.indexOf(indexB));
                    gsl_vector_set(columnVector, 1, value);
                }

                if (degreesOfFreedom.contains(indexC))
                {
                    qreal value = gsl_vector_get(deflectionsColumnVector, degreesOfFreedom.indexOf(indexC));
                    gsl_vector_set(columnVector, 2, value);
                }

                if (degreesOfFreedom.contains(indexD))
                {
                    qreal value = gsl_vector_get(deflectionsColumnVector, degreesOfFreedom.indexOf(indexD));
                    gsl_vector_set(columnVector, 3, value);
                }

                qreal alpha = 1.0;
                qreal beta  = 0.0;
                gsl_blas_dgemv(CblasNoTrans,
                               alpha,
                               rowMatrix,
                               columnVector,
                               beta,
                               product);

                qreal sum = gsl_vector_get(product, 0);

                gsl_matrix_set_zero(rowMatrix);
                gsl_vector_set_zero(columnVector);
                gsl_vector_set_zero(product);

                if (mAreaModulusOption)
                {
                    barLoad = bar->area() * areaConversionFactor
                            * bar->modulus() * modulusConversionFactor
                            * sum
                            / (length * lengthConversionFactor);
                }
                else
                {
                    barLoad = sum * bar->factor() / (length * lengthConversionFactor);
                }

                if (std::fabs(barLoad) < epsilonMagnitudeSmall)
                {
                    barLoad = 0.0;
                }

                int barIndex = mBarsList.indexOf(bar);
                mInfluenceLoadResult->appendInfluenceLoadOrdinatesListValue(barIndex, barLoad);
            }

            gsl_matrix_free(rowMatrix);
            gsl_vector_free(columnVector);
            gsl_vector_free(product);

            gsl_vector_set_zero(deflectionsColumnVector);
            gsl_vector_set_zero(loadsColumnVector);
        }

        gsl_vector_free(deflectionsColumnVector);
        gsl_vector_free(loadsColumnVector);

        if (mInfluenceLoadResult->influenceLoadOrdinatesList(0).size() == influenceLoad->path().size())
        {
            foreach (Bar *bar, mBarsList)
            {
                int barIndex               = mBarsList.indexOf(bar);
                int maxOrdinateJointNumber = 1;
                int minOrdinateJointNumber = 1;
                qreal maxOrdinate          = 0.0;
                qreal minOrdinate          = 0.0;

                QList<qreal> ordinatesList = mInfluenceLoadResult->influenceLoadOrdinatesList(barIndex);
                int count                  = ordinatesList.size();

                for (int i = 0; i < count; ++i)
                {
                    if (ordinatesList.at(i) > maxOrdinate)
                    {
                        maxOrdinateJointNumber = i + 1;
                        maxOrdinate            = ordinatesList.at(i);
                    }

                    if (ordinatesList.at(i) < minOrdinate)
                    {
                        minOrdinateJointNumber = i + 1;
                        minOrdinate            = ordinatesList.at(i);
                    }
                }

                gsl_interp_accel *accelerator = gsl_interp_accel_alloc();
                gsl_spline *spline            = gsl_spline_alloc(gsl_interp_linear, count);

                qreal xArray[count];
                qreal yArray[count];

                for (int i = 0; i < count; ++i)
                {
                    Joint *jointA = mJointsList.at(influenceLoad->path().first() - 1);
                    Joint *jointB = mJointsList.at(influenceLoad->path().at(i) - 1);
                    qreal deltaX  = (jointB->xCoordinate() - jointA->xCoordinate()) * lengthConversionFactor;
                    xArray[i]     = deltaX;
                    yArray[i]     = ordinatesList.at(i);
                }

                gsl_spline_init(spline, xArray, yArray, count);

                int pointLoadsCount = influenceLoad->pointLoads().size();

                qreal pMin              = 0.0;
                int pMinPointLoadNumber = 1;

                for (int i = 0; i < pointLoadsCount; ++i)
                {
                    QList<qreal> xCoordinatesList;

                    for (int j = 0; j < pointLoadsCount; ++j)
                    {
                        qreal x1     = influenceLoad->pointLoadPositions().at(i) * lengthConversionFactor;
                        qreal x2     = influenceLoad->pointLoadPositions().at(j) * lengthConversionFactor;
                        qreal deltaX = 0.0;

                        if (influenceLoad->direction() == tr("LR"))
                        {
                            deltaX = x1 - x2;
                        }
                        else
                        {
                            deltaX = x2 - x1;
                        }

                        xCoordinatesList.append(xArray[minOrdinateJointNumber - 1] + deltaX);
                    }

                    qreal pCurrent = 0.0;
                    int index      = 0;

                    foreach (qreal xOrdinate, xCoordinatesList)
                    {
                        qreal yOrdinate = 0.0;
                        gsl_spline_eval_e(spline, xOrdinate, accelerator, &yOrdinate);

                        if (yOrdinate != GSL_NAN)
                        {
                            qreal pointLoad = influenceLoad->pointLoads().at(index) * loadConversionFactor;
                            pCurrent += pointLoad * yOrdinate;
                        }

                        ++index;
                    }

                    if (pCurrent < pMin)
                    {
                        pMin                = pCurrent;
                        pMinPointLoadNumber = i + 1;
                    }
                }

                if (mUnitsAndLimits.system() == tr("metric"))
                {
                    if (std::fabs(pMin) < loadLimitNewton)
                    {
                        pMin = 0.0;
                    }
                }
                else
                {
                    if (std::fabs(pMin) < loadLimitPound)
                    {
                        pMin = 0.0;
                    }
                }

                mInfluenceLoadResult->setMinLoad(barIndex, pMin);
                mInfluenceLoadResult->setMinLoadPosition(barIndex, tr("#%1 @ joint %2")
                                                         .arg(QString::number(pMinPointLoadNumber))
                                                         .arg(QString::number(influenceLoad->path()
                                                                              .at(minOrdinateJointNumber - 1))));

                qreal pMax              = 0.0;
                int pMaxPointLoadNumber = 1;

                for (int i = 0; i < pointLoadsCount; ++i)
                {
                    QList<qreal> xCoordinatesList;

                    for (int j = 0; j < pointLoadsCount; ++j)
                    {
                        qreal x1     = influenceLoad->pointLoadPositions().at(i) * lengthConversionFactor;
                        qreal x2     = influenceLoad->pointLoadPositions().at(j) * lengthConversionFactor;
                        qreal deltaX = 0.0;

                        if (influenceLoad->direction() == tr("LR"))
                        {
                            deltaX = x1 - x2;
                        }
                        else
                        {
                            deltaX = x2 - x1;
                        }

                        xCoordinatesList.append(xArray[maxOrdinateJointNumber - 1] + deltaX);
                    }

                    qreal pCurrent = 0.0;
                    int index      = 0;

                    foreach (qreal xOrdinate, xCoordinatesList)
                    {
                        qreal yOrdinate = 0.0;
                        gsl_spline_eval_e(spline, xOrdinate, accelerator, &yOrdinate);

                        if (yOrdinate != GSL_NAN)
                        {
                            qreal pointLoad = influenceLoad->pointLoads().at(index) * loadConversionFactor;
                            pCurrent += pointLoad * yOrdinate;
                        }

                        ++index;
                    }

                    if (pCurrent > pMax)
                    {
                        pMax                = pCurrent;
                        pMaxPointLoadNumber = i + 1;
                    }
                }

                if (mUnitsAndLimits.system() == tr("metric"))
                {
                    if (std::fabs(pMax) < loadLimitNewton)
                    {
                        pMax = 0.0;
                    }
                }
                else
                {
                    if (std::fabs(pMax) < loadLimitPound)
                    {
                        pMax = 0.0;
                    }
                }

                mInfluenceLoadResult->setMaxLoad(barIndex, pMax);
                mInfluenceLoadResult->setMaxLoadPosition(barIndex, tr("#%1 @ joint %2")
                                                         .arg(QString::number(pMaxPointLoadNumber))
                                                         .arg(QString::number(influenceLoad->path()
                                                                              .at(maxOrdinateJointNumber - 1))));

                gsl_spline_free(spline);
                gsl_interp_accel_free(accelerator);
            }
        }
    }

    gsl_spmatrix_free(k11TripletFormat);

    // -----------------------------------------------------------------------------------------------------------------
    // Deflections
    // -----------------------------------------------------------------------------------------------------------------

    QList<qreal> horizontalDeflectionComponentsList;
    QList<qreal> verticalDeflectionComponentsList;

    foreach (Joint *joint, mJointsList)
    {
        if (substituteJointsList.contains(joint))
        {
            continue;
        }

        int index = mJointsList.indexOf(joint);

        qreal horizontalDeflection = 0.0;
        qreal verticalDeflection   = 0.0;

        if (degreesOfFreedom.contains(2 * index))
        {
            horizontalDeflection = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(2 * index));
        }

        if (degreesOfFreedom.contains(2 * index + 1))
        {
            verticalDeflection = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(2 * index + 1));
        }

        foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
        {
            if (supportSettlement->settlementSupport()->supportJoint() == joint)
            {
                verticalDeflection = -supportSettlement->settlement() * supportSettlementConversionFactor;
                break;
            }
        }

        horizontalDeflectionComponentsList.append(horizontalDeflection);
        verticalDeflectionComponentsList.append(verticalDeflection);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Bar loads
    // -----------------------------------------------------------------------------------------------------------------

    QList<qreal> barLoadsList;

    foreach (Bar *bar, mBarsList)
    {
        if (substituteBarsList.contains(bar))
        {
            continue;
        }

        // -------------------------------------------------------------------------------------------------------------
        // Thermal effects component
        // -------------------------------------------------------------------------------------------------------------

        qreal thermalEffectComponent = 0.0;

        if (!mThermalEffectsList.isEmpty())
        {
            foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
            {
                if (thermalEffect->thermalEffectBar() == bar)
                {
                    thermalEffectComponent = -bar->area() * areaConversionFactor
                            * bar->modulus() * modulusConversionFactor
                            * thermalEffect->thermalCoefficient()
                            * thermalEffect->temperatureChange();

                    break;
                }
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Fabrication errors component
        // -------------------------------------------------------------------------------------------------------------

        qreal fabricationErrorComponent = 0.0;

        qreal x1     = bar->firstJoint()->xCoordinate();
        qreal y1     = bar->firstJoint()->yCoordinate();
        qreal x2     = bar->secondJoint()->xCoordinate();
        qreal y2     = bar->secondJoint()->yCoordinate();
        qreal deltaX = x2 - x1;
        qreal deltaY = y2 - y1;
        qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

        if (!mFabricationErrorsList.isEmpty())
        {
            foreach (FabricationError *fabricationError, mFabricationErrorsList)
            {
                if (fabricationError->fabricationErrorBar() == bar)
                {
                    fabricationErrorComponent = -bar->area() * areaConversionFactor
                            * bar->modulus() * modulusConversionFactor
                            * fabricationError->lengthError() * lengthErrorConversionFactor
                            / (length * lengthConversionFactor);

                    break;
                }
            }
        }

        // -------------------------------------------------------------------------------------------------------------
        // Joint loads component
        // -------------------------------------------------------------------------------------------------------------

        qreal jointLoadComponent = 0.0;

        qreal C = deltaX / length;
        qreal S = deltaY / length;

        gsl_matrix *rowMatrix = gsl_matrix_calloc(1, 4);
        gsl_matrix_set(rowMatrix, 0, 0, -C);
        gsl_matrix_set(rowMatrix, 0, 1, -S);
        gsl_matrix_set(rowMatrix, 0, 2,  C);
        gsl_matrix_set(rowMatrix, 0, 3,  S);

        gsl_vector *columnVector = gsl_vector_calloc(4);
        int firstJointIndex      = mJointsList.indexOf(bar->firstJoint());
        int secondJointIndex     = mJointsList.indexOf(bar->secondJoint());
        int indexA               = 2 * firstJointIndex;
        int indexB               = indexA + 1;
        int indexC               = 2 * secondJointIndex;
        int indexD               = indexC + 1;

        if (degreesOfFreedom.contains(indexA))
        {
            qreal value = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(indexA));
            gsl_vector_set(columnVector, 0, value);
        }

        if (degreesOfFreedom.contains(indexB))
        {
            qreal value = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(indexB));
            gsl_vector_set(columnVector, 1, value);
        }
        else
        {
            foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
            {
                Joint *joint = supportSettlement->settlementSupport()->supportJoint();
                int index    = 2 * mJointsList.indexOf(joint) + 1;

                if (index == indexB)
                {
                    qreal value = -supportSettlement->settlement() * supportSettlementConversionFactor;
                    gsl_vector_set(columnVector, 1, value);
                    break;
                }
            }
        }

        if (degreesOfFreedom.contains(indexC))
        {
            qreal value = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(indexC));
            gsl_vector_set(columnVector, 2, value);
        }

        if (degreesOfFreedom.contains(indexD))
        {
            qreal value = gsl_vector_get(deflectionsColumnVectorU, degreesOfFreedom.indexOf(indexD));
            gsl_vector_set(columnVector, 3, value);
        }
        else
        {
            foreach (SupportSettlement *supportSettlement, mSupportSettlementsList)
            {
                Joint *joint = supportSettlement->settlementSupport()->supportJoint();
                int index    = 2 * mJointsList.indexOf(joint) + 1;

                if (index == indexD)
                {
                    qreal value = -supportSettlement->settlement() * supportSettlementConversionFactor;
                    gsl_vector_set(columnVector, 3, value);
                    break;
                }
            }
        }

        gsl_vector *product = gsl_vector_calloc(1);

        qreal alpha = 1.0;
        qreal beta  = 0.0;
        gsl_blas_dgemv(CblasNoTrans,
                       alpha,
                       rowMatrix,
                       columnVector,
                       beta,
                       product);

        qreal sum = gsl_vector_get(product, 0);

        gsl_matrix_free(rowMatrix);
        gsl_vector_free(columnVector);
        gsl_vector_free(product);

        if (mAreaModulusOption)
        {
            jointLoadComponent = bar->area() * areaConversionFactor
                    * bar->modulus() * modulusConversionFactor
                    * sum
                    / (length * lengthConversionFactor);
        }
        else
        {
            jointLoadComponent = sum * bar->factor() / (length * lengthConversionFactor);
        }

        qreal barLoad = thermalEffectComponent + fabricationErrorComponent + jointLoadComponent;

        if (mUnitsAndLimits.system() == tr("metric"))
        {
            if (std::fabs(barLoad) > loadLimitNewton)
            {
                barLoadsList.append(barLoad);
            }
            else
            {
                barLoadsList.append(0.0);
            }
        }
        else
        {
            if (std::fabs(barLoad) > loadLimitPound)
            {
                barLoadsList.append(barLoad);
            }
            else
            {
                barLoadsList.append(0.0);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Support reactions
    // -----------------------------------------------------------------------------------------------------------------

    gsl_vector *loadsColumnVectorU = gsl_vector_calloc(fixedDegreesOfFreedom.size());

    size_t nZMax      = gsl_spmatrix_nnz(k21CompressedColumnFormat);
    size_t nRow       = k21CompressedColumnFormat->size1;
    size_t nCol       = k21CompressedColumnFormat->size2;
    gsl_spmatrix *k21 = gsl_spmatrix_alloc_nzmax(nRow,
                                                 nCol,
                                                 nZMax,
                                                 GSL_SPMATRIX_CCS);
    gsl_spmatrix_memcpy(k21, k21CompressedColumnFormat);
    gsl_vector *productU = gsl_vector_calloc(fixedDegreesOfFreedom.size());

    qreal alpha = 1.0;
    qreal beta  = 0.0;
    gsl_spblas_dgemv(CblasNoTrans,
                     alpha,
                     k21,
                     deflectionsColumnVectorU,
                     beta,
                     productU);

    size_t count = productU->size;

    for (size_t i = 0; i < count; ++i)
    {
        qreal value = gsl_vector_get(loadsColumnVectorU, i);
        value += gsl_vector_get(productU, i);
        gsl_vector_set(loadsColumnVectorU, i, value);
    }

    nZMax             = gsl_spmatrix_nnz(k22CompressedColumnFormat);
    nRow              = k22CompressedColumnFormat->size1;
    nCol              = k22CompressedColumnFormat->size2;
    gsl_spmatrix *k22 = gsl_spmatrix_alloc_nzmax(nRow,
                                                 nCol,
                                                 nZMax,
                                                 GSL_SPMATRIX_CCS);
    gsl_spmatrix_memcpy(k22, k22CompressedColumnFormat);
    gsl_vector *productK = gsl_vector_calloc(fixedDegreesOfFreedom.size());

    gsl_spblas_dgemv(CblasNoTrans,
                     alpha,
                     k22,
                     deflectionsColumnVectorK,
                     beta,
                     productK);

    count = productK->size;

    for (size_t i = 0; i < count; ++i)
    {
        qreal value = gsl_vector_get(loadsColumnVectorU, i);
        value += gsl_vector_get(productK, i);
        gsl_vector_set(loadsColumnVectorU, i, value);
    }

    gsl_spmatrix_free(k21);
    gsl_vector_free(productU);
    gsl_spmatrix_free(k22);
    gsl_vector_free(productK);

    if (!mThermalEffectsList.isEmpty())
    {
        // -------------------------------------------------------------------------------------------------------------
        // Thermal effects component
        // -------------------------------------------------------------------------------------------------------------

        foreach (ThermalEffect *thermalEffect, mThermalEffectsList)
        {
            Bar * bar    = thermalEffect->thermalEffectBar();
            qreal x1     = bar->firstJoint()->xCoordinate();
            qreal y1     = bar->firstJoint()->yCoordinate();
            qreal x2     = bar->secondJoint()->xCoordinate();
            qreal y2     = bar->secondJoint()->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            qreal C      = deltaX / length;
            qreal S      = deltaY / length;

            int firstJointIndex  = mJointsList.indexOf(bar->firstJoint());
            int secondJointIndex = mJointsList.indexOf(bar->secondJoint());

            qreal thermalEffectLoad = bar->area() * areaConversionFactor
                    * bar->modulus() * modulusConversionFactor
                    * thermalEffect->thermalCoefficient()
                    * thermalEffect->temperatureChange();

            int indexA = 2 * firstJointIndex;
            int indexB = indexA + 1;
            int indexC = 2 * secondJointIndex;
            int indexD = indexC + 1;

            if (fixedDegreesOfFreedom.contains(indexA))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexA));
                value += thermalEffectLoad * C;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexA), value);
            }

            if (fixedDegreesOfFreedom.contains(indexB))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexB));
                value += thermalEffectLoad * S;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexB), value);
            }

            if (fixedDegreesOfFreedom.contains(indexC))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexC));
                value += -thermalEffectLoad * C;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexC), value);
            }

            if (fixedDegreesOfFreedom.contains(indexD))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexD));
                value += -thermalEffectLoad * S;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexD), value);
            }
        }
    }

    if (!mFabricationErrorsList.isEmpty())
    {
        // -------------------------------------------------------------------------------------------------------------
        // Fabrication errors component
        // -------------------------------------------------------------------------------------------------------------

        foreach (FabricationError *fabricationError, mFabricationErrorsList)
        {
            Bar * bar    = fabricationError->fabricationErrorBar();
            qreal x1     = bar->firstJoint()->xCoordinate();
            qreal y1     = bar->firstJoint()->yCoordinate();
            qreal x2     = bar->secondJoint()->xCoordinate();
            qreal y2     = bar->secondJoint()->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            qreal C      = deltaX / length;
            qreal S      = deltaY / length;

            int firstJointIndex  = mJointsList.indexOf(bar->firstJoint());
            int secondJointIndex = mJointsList.indexOf(bar->secondJoint());

            qreal fabricationErrorLoad = bar->area() * areaConversionFactor
                    * bar->modulus() * modulusConversionFactor
                    * fabricationError->lengthError() * lengthErrorConversionFactor
                    / (length * lengthConversionFactor);

            int indexA = 2 * firstJointIndex;
            int indexB = indexA + 1;
            int indexC = 2 * secondJointIndex;
            int indexD = indexC + 1;

            if (fixedDegreesOfFreedom.contains(indexA))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexA));
                value += fabricationErrorLoad * C;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexA), value);
            }

            if (fixedDegreesOfFreedom.contains(indexB))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexB));
                value += fabricationErrorLoad * S;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexB), value);
            }

            if (fixedDegreesOfFreedom.contains(indexC))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexC));
                value += -fabricationErrorLoad * C;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexC), value);
            }

            if (fixedDegreesOfFreedom.contains(indexD))
            {
                qreal value = gsl_vector_get(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexD));
                value += -fabricationErrorLoad * S;
                gsl_vector_set(loadsColumnVectorU, fixedDegreesOfFreedom.indexOf(indexD), value);
            }
        }
    }

    foreach (Support *support, mSupportsList)
    {
        int index  = 0;
        int indexA = 0;
        int indexB = 0;

        if (support->type() == UnitsAndLimits::ROLLER)
        {
            Bar *supportSubstituteBar = 0;

            foreach (Bar *bar, substituteBarsList)
            {
                if (bar->firstJoint() == support->supportJoint())
                {
                    supportSubstituteBar = bar;
                    break;
                }
            }

            Joint *joint = supportSubstituteBar->secondJoint();
            indexA       = mJointsList.indexOf(joint);
            indexB       = supportJointsList.indexOf(joint);

            if (fixedDegreesOfFreedom.contains(2 * indexA))
            {
                index = fixedDegreesOfFreedom.indexOf(2 * indexA);
                reactionHorizontalComponentsList[indexB] += gsl_vector_get(loadsColumnVectorU, index);
            }

            if (fixedDegreesOfFreedom.contains(2 * indexA + 1))
            {
                index = fixedDegreesOfFreedom.indexOf(2 * indexA + 1);
                reactionVerticalComponentsList[indexB] += gsl_vector_get(loadsColumnVectorU, index);
            }
        }
        else
        {
            Joint *joint = support->supportJoint();
            indexA       = mJointsList.indexOf(joint);
            indexB       = supportJointsList.indexOf(joint);

            if (fixedDegreesOfFreedom.contains(2 * indexA))
            {
                index = fixedDegreesOfFreedom.indexOf(2 * indexA);
                reactionHorizontalComponentsList[indexB] += gsl_vector_get(loadsColumnVectorU, index);
            }

            if (fixedDegreesOfFreedom.contains(2 * indexA + 1))
            {
                index = fixedDegreesOfFreedom.indexOf(2 * indexA + 1);
                reactionVerticalComponentsList[indexB] += gsl_vector_get(loadsColumnVectorU, index);
            }
        }
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Statical checks
    // -----------------------------------------------------------------------------------------------------------------

    qreal horizontalComponentsSum = 0.0;
    qreal verticalComponentsSum   = 0.0;
    qreal momentsSum              = 0.0;

    foreach (JointLoad *load, mJointLoadsList)
    {
        qreal H = load->horizontalComponent() * loadConversionFactor;
        qreal V = load->verticalComponent() * loadConversionFactor;
        horizontalComponentsSum += H;
        verticalComponentsSum   += V;

        qreal x = load->loadJoint()->xCoordinate() * lengthConversionFactor;
        qreal y = load->loadJoint()->yCoordinate() * lengthConversionFactor;

        momentsSum += (x * V - y * H);
    }

    foreach (JointLoad *load, additionalJointLoadsList)
    {
        qreal V = load->verticalComponent() * loadConversionFactor;
        verticalComponentsSum += V;

        qreal x = load->loadJoint()->xCoordinate() * lengthConversionFactor;

        momentsSum += x * V;
    }

    count = reactionHorizontalComponentsList.size();

    for (size_t i = 0; i < count; ++i)
    {
        qreal H = reactionHorizontalComponentsList.at(i);
        qreal V = reactionVerticalComponentsList.at(i);

        horizontalComponentsSum += H;
        verticalComponentsSum   += V;

        Support *support = mSupportsList.at(i);
        Joint *joint     = support->supportJoint();
        qreal x          = joint->xCoordinate() * lengthConversionFactor;
        qreal y          = joint->yCoordinate() * lengthConversionFactor;

        momentsSum += (x * V - y * H);
    }

    epsilonMagnitudeSmall = 1.0e-6;

    if (std::fabs(horizontalComponentsSum) < epsilonMagnitudeSmall)
    {
        horizontalComponentsSum = 0.0;
    }

    if (std::fabs(verticalComponentsSum) < epsilonMagnitudeSmall)
    {
        verticalComponentsSum = 0.0;
    }

    if (std::fabs(momentsSum) < epsilonMagnitudeSmall)
    {
        momentsSum = 0.0;
    }

    QString note = QString("Statics checks:\n%1Fx = %2\n%1Fy = %3\n%1Mo = %4")
            .arg(QString::fromUtf8("\u03A3"))
            .arg(QString::number(horizontalComponentsSum, 'g', 6))
            .arg(QString::number(verticalComponentsSum, 'g', 6))
            .arg(QString::number(momentsSum, 'g', 6));

    QList<qreal> horizontalComponentsList;
    QList<qreal> verticalComponentsList;

    count = reactionHorizontalComponentsList.size();

    for (size_t i = 0; i < count; ++i)
    {
        if (mUnitsAndLimits.system() == tr("metric"))
        {
            if (std::fabs(reactionHorizontalComponentsList.at(i)) > loadLimitNewton)
            {
                horizontalComponentsList.append(reactionHorizontalComponentsList.at(i));
            }
            else
            {
                horizontalComponentsList.append(0.0);
            }

            if (std::fabs(reactionVerticalComponentsList.at(i)) > loadLimitNewton)
            {
                verticalComponentsList.append(reactionVerticalComponentsList.at(i));
            }
            else
            {
                verticalComponentsList.append(0.0);
            }
        }
        else
        {
            if (std::fabs(reactionHorizontalComponentsList.at(i)) > loadLimitPound)
            {
                horizontalComponentsList.append(reactionHorizontalComponentsList.at(i));
            }
            else
            {
                horizontalComponentsList.append(0.0);
            }

            if (std::fabs(reactionVerticalComponentsList.at(i)) > loadLimitPound)
            {
                verticalComponentsList.append(reactionVerticalComponentsList.at(i));
            }
            else
            {
                verticalComponentsList.append(0.0);
            }
        }
    }

    reactionHorizontalComponentsList = horizontalComponentsList;
    reactionVerticalComponentsList   = verticalComponentsList;

    bool check = !mJointLoadsList.isEmpty() || !additionalJointLoadsList.isEmpty()
            || !mSupportSettlementsList.isEmpty() || !mThermalEffectsList.isEmpty()
            || !mFabricationErrorsList.isEmpty();

    if (!mSolutionsCount.contains(false))
    {
        if (check)
        {
            emit jointHorizontalDeflectionsSignal(horizontalDeflectionComponentsList);
            emit jointVerticalDeflectionsSignal(verticalDeflectionComponentsList);
            emit barLoadsSignal(barLoadsList);
            emit reactionHorizontalComponentsSignal(reactionHorizontalComponentsList);
            emit reactionVerticalComponentsSignal(reactionVerticalComponentsList);
        }

        emit notesSignal(note);
        emit hasSolution();
    }
    else
    {
        mInfluenceLoadResult->resetParameters();
        note = tr("One or more solutions failed to converge!");
        emit notesSignal(note);
    }

    // -----------------------------------------------------------------------------------------------------------------
    // Free resources
    // -----------------------------------------------------------------------------------------------------------------

    foreach (Joint *joint, substituteJointsList)
    {
        delete joint;
    }

    foreach (Bar *bar, substituteBarsList)
    {
        delete bar;
    }

    foreach (JointLoad *load, additionalJointLoadsList)
    {
        delete load;
    }

    gsl_spmatrix_free(k11CompressedColumnFormat);
    gsl_spmatrix_free(k12CompressedColumnFormat);
    gsl_spmatrix_free(k21CompressedColumnFormat);
    gsl_spmatrix_free(k22CompressedColumnFormat);
    gsl_vector_free(deflectionsColumnVectorU);
    gsl_vector_free(loadsColumnVectorK);
    gsl_vector_free(deflectionsColumnVectorK);
    gsl_vector_free(loadsColumnVectorU);
}

