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

/* modelchecker.cpp */

#include "modelchecker.h"

ModelChecker::ModelChecker(const QList<Joint *>   &jointsList,
                           const QList<Bar *>     &barsList,
                           const QList<Support *> &supportsList,
                           const UnitsAndLimits   &unitsAndLimits,
                           QObject                *parent) : QThread(parent)
{
    mJointsList     = jointsList;
    mBarsList       = barsList;
    mSupportsList   = supportsList;
    mUnitsAndLimits = unitsAndLimits;

    mDegreeOfIndeterminacy = 0;

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ModelChecker::~ModelChecker()
{

}

Support *ModelChecker::jointSupport(Joint *joint)
{
    //---------------------------------------------------------------------------------------------------------------

    Support *jointSupport = 0;

    foreach (Support *support, mSupportsList)
    {
        if (support->supportJoint() == joint)
        {
            jointSupport = support;
            break;
        }
    }

    return jointSupport;

    //---------------------------------------------------------------------------------------------------------------
}

void ModelChecker::run()
{
    if (mJointsList.size() < 3)
    {
        QString note("Model has less than three joints.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (mBarsList.size() < 2)
    {
        QString note("Model has less than two bars.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (mSupportsList.size() < 2)
    {
        QString note("Model has less than two supports.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasDisconnectedParts())
    {
        QString note("Model has disconnected joints and bars.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasDanglingBar())
    {
        QString note("Model has joint connected to single bar only.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasFixedJoints())
    {
        QString note("Model has all joints fixed.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasInsufficientSupports())
    {
        QString note("Model unstable (external) due to insufficient supports.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasInsufficientBars())
    {
        QString note("Model unstable (internal) due to insufficient number of bars.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    if (hasFixedBars())
    {
        QString note("Model has (at least) one bar with fixed ends.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    //---------------------------------------------------------------------------------------------------------------

    //Create substitute joints and bars for inclined roller support

    QList<Joint *> substituteJointsList;
    QList<Bar *> substituteBarsList;

    foreach (Support *support, mSupportsList)
    {
        if (support->type() == UnitsAndLimits::ROLLER)
        {
            Joint *firstJoint;
            Joint *secondJoint;
            Bar *bar;
            qreal length       = mUnitsAndLimits.minBarLength();
            qreal supportAngle = qDegreesToRadians(support->angle());

            qreal x = support->supportJoint()->xCoordinate() - length * std::sin(supportAngle);
            qreal y = support->supportJoint()->yCoordinate() - length * std::cos(supportAngle);

            firstJoint  = support->supportJoint();
            secondJoint = new Joint(x, y);
            substituteJointsList.append(secondJoint);
            bar = new Bar(firstJoint, secondJoint);
            substituteBarsList.append(bar);
        }
    }

    //---------------------------------------------------------------------------------------------------------------

    //Determine fixed degrees of freedom

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
                        index = mJointsList.size() + substituteJointsList.indexOf(bar->secondJoint());
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

    //---------------------------------------------------------------------------------------------------------------

    //Assemble partitioned stiffness matrix

    foreach (Joint *joint, substituteJointsList)
    {
        mJointsList.append(joint);
    }

    foreach (Bar *bar, substituteBarsList)
    {
        mBarsList.append(bar);
    }

    int order                              = 2 * mJointsList.size();
    int orderReduced                       = order - fixedDegreesOfFreedom.size();
    gsl_matrix *partitionedStiffnessMatrix = gsl_matrix_calloc(orderReduced, orderReduced);

    qreal epsilonMagnitudeSmall = 1.0e-12;

    QList<int> degreesOfFreedom;

    for (int i = 0; i < order; ++i)
    {
        if (!fixedDegreesOfFreedom.contains(i))
        {
            degreesOfFreedom.append(i);
        }
    }

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
                qreal value = matrix[i][j] / length;

                if (std::fabs(value) > epsilonMagnitudeSmall)
                {
                    int row = indexList[i];
                    int col = indexList[j];

                    if (degreesOfFreedom.contains(row) && degreesOfFreedom.contains(col))
                    {
                        int rowIndex = degreesOfFreedom.indexOf(row);
                        int colIndex = degreesOfFreedom.indexOf(col);

                        if (rowIndex >= colIndex)
                        {
                            qreal currentValue = gsl_matrix_get(partitionedStiffnessMatrix, rowIndex, colIndex);
                            currentValue += value;
                            gsl_matrix_set(partitionedStiffnessMatrix, rowIndex, colIndex, currentValue);
                        }
                    }
                }
            }
        }
    }

    foreach (Joint *joint, substituteJointsList)
    {
        mJointsList.removeOne(joint);
    }

    foreach (Bar *bar, substituteBarsList)
    {
        mBarsList.removeOne(bar);
    }

    //---------------------------------------------------------------------------------------------------------------

    if (hasUnstableConfiguration(partitionedStiffnessMatrix))
    {
        foreach (Joint *joint, substituteJointsList)
        {
            delete joint;
        }

        foreach (Bar *bar, substituteBarsList)
        {
            delete bar;
        }

        gsl_matrix_free(partitionedStiffnessMatrix);

        QString note("Model unstable (internal) due to bars configuration.");
        emit notesSignal(note);
        emit progressSignal(100);
        return;
    }

    emit progressSignal(50);
    emit modelStabilitySignal(true);

    QList<Bar *> redundantBarsList;

    //Check each bar for redundancy by removing the bar and running checks
    int barsCount = mBarsList.size();

    for (int index = 0; index < barsCount; index++)
    {
        Bar* bar      = mBarsList[index];
        Joint *jointA = bar->firstJoint();
        Joint *jointB = bar->secondJoint();

        bool removeFirstJoint  = false;
        bool removeSecondJoint = false;
        Support *support       = 0;
        int removeJointIndex   = 0;
        int removeSupportIndex = 0;

        if (bar->firstJoint()->attachedBars()->size() == 1)
        {
            removeFirstJoint = true;
            support          = jointSupport(jointA);
        }

        if (bar->secondJoint()->attachedBars()->size() == 1)
        {
            removeSecondJoint = true;
            support           = jointSupport(jointB);
        }

        jointA->attachedBars()->removeOne(bar);
        jointB->attachedBars()->removeOne(bar);
        jointA->connectedJoints()->removeOne(jointB);
        jointB->connectedJoints()->removeOne(jointA);
        mBarsList.removeAt(index);

        if (removeFirstJoint)
        {
            removeJointIndex = mJointsList.indexOf(jointA);
            mJointsList.removeAt(removeJointIndex);
            removeSupportIndex = mSupportsList.indexOf(support);
            mSupportsList.removeAt(removeSupportIndex);
        }

        if (removeSecondJoint)
        {
            removeJointIndex = mJointsList.indexOf(jointB);
            mJointsList.removeAt(removeJointIndex);
            removeSupportIndex = mSupportsList.indexOf(support);
            mSupportsList.removeAt(removeSupportIndex);
        }

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
        int jointIndex = mJointsList.indexOf(bar->firstJoint());
        indexList.append(2 * jointIndex);
        indexList.append(2 * jointIndex + 1);

        jointIndex = mJointsList.indexOf(bar->secondJoint());
        indexList.append(2 * jointIndex);
        indexList.append(2 * jointIndex + 1);

        qreal matrix[4][4] = {{ C * C,  C * S, -C * C, -C * S},
                              { C * S,  S * S, -C * S, -S * S},
                              {-C * C, -C * S,  C * C,  C * S},
                              {-C * S, -S * S,  C * S,  S * S}};

        if (removeFirstJoint || removeSecondJoint)
        {
            bool checkA = mJointsList.size() < 3;

            bool checkB = false;

            if (!checkA)
            {
                checkB = mBarsList.size() < 2;
            }

            bool checkC = false;

            if (!checkA && !checkB)
            {
                checkC = mSupportsList.size() < 2;
            }

            bool checkD = false;

            if (!checkA && !checkB && !checkC)
            {
                checkD = hasInsufficientBars();
            }

            bool checkE = false;

            if (!checkA && !checkB && !checkC && !checkD)
            {
                checkE = hasInsufficientSupports();
            }

            if (!checkA && !checkB && !checkC && !checkD && !checkE)
            {
                redundantBarsList.append(bar);
            }
        }
        else
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    qreal value = matrix[i][j] / length;

                    if (std::fabs(value) > epsilonMagnitudeSmall)
                    {
                        int row = indexList[i];
                        int col = indexList[j];

                        if (degreesOfFreedom.contains(row) && degreesOfFreedom.contains(col))
                        {
                            int rowIndex = degreesOfFreedom.indexOf(row);
                            int colIndex = degreesOfFreedom.indexOf(col);

                            if (rowIndex >= colIndex)
                            {
                                qreal currentValue = gsl_matrix_get(partitionedStiffnessMatrix, rowIndex, colIndex);
                                currentValue -= value;
                                gsl_matrix_set(partitionedStiffnessMatrix, rowIndex, colIndex, currentValue);
                            }
                        }
                    }
                }
            }

            bool check = hasUnstableConfiguration(partitionedStiffnessMatrix);

            if (!check)
            {
                redundantBarsList.append(bar);
            }
        }

        jointA->attachedBars()->append(bar);
        jointB->attachedBars()->append(bar);
        jointA->connectedJoints()->append(jointB);
        jointB->connectedJoints()->append(jointA);
        mBarsList.insert(index, bar);

        if (removeFirstJoint)
        {
            mJointsList.insert(removeJointIndex, jointA);
            mSupportsList.insert(removeSupportIndex, support);
        }

        if (removeSecondJoint)
        {
            mJointsList.insert(removeJointIndex, jointB);
            mSupportsList.insert(removeSupportIndex, support);
        }

        if (!removeFirstJoint && !removeSecondJoint)
        {
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    qreal value = matrix[i][j] / length;

                    if (std::fabs(value) > epsilonMagnitudeSmall)
                    {
                        int row = indexList[i];
                        int col = indexList[j];

                        if (degreesOfFreedom.contains(row) && degreesOfFreedom.contains(col))
                        {
                            int rowIndex = degreesOfFreedom.indexOf(row);
                            int colIndex = degreesOfFreedom.indexOf(col);

                            if (rowIndex >= colIndex)
                            {
                                qreal currentValue = gsl_matrix_get(partitionedStiffnessMatrix, rowIndex, colIndex);
                                currentValue += value;
                                gsl_matrix_set(partitionedStiffnessMatrix, rowIndex, colIndex, currentValue);
                            }
                        }
                    }
                }
            }
        }
    }

    emit redundantBarsListSignal(redundantBarsList);

    emit progressSignal(75);

    QList<Support *> redundantSupportsList;

    //Check each support for redundancy by removing the support and running checks
    int supportsCount = mSupportsList.size();

    for (int index = 0; index < supportsCount; ++index)
    {
        Support *support = mSupportsList[index];

        UnitsAndLimits::SupportType type = support->type();

        if (type != UnitsAndLimits::FIXED_LEFT
                && type != UnitsAndLimits::FIXED_TOP
                && type != UnitsAndLimits::FIXED_RIGHT
                && type != UnitsAndLimits::FIXED_BOTTOM
                && type != UnitsAndLimits::ROLLER_BOTTOM)
        {
            continue;
        }

        Joint *joint         = support->supportJoint();
        Bar *bar             = 0;
        int removeBarIndex   = 0;
        int removeJointIndex = 0;
        Joint *jointA        = 0;
        Joint *jointB        = 0;

        if (joint->attachedBars()->size() == 1)
        {
            bar    = joint->attachedBars()->first();
            jointA = bar->firstJoint();
            jointB = bar->secondJoint();

            jointA->attachedBars()->removeOne(bar);
            jointB->attachedBars()->removeOne(bar);
            jointA->connectedJoints()->removeOne(jointB);
            jointB->connectedJoints()->removeOne(jointA);
            removeBarIndex = mBarsList.indexOf(bar);
            mBarsList.removeAt(removeBarIndex);
            removeJointIndex = mJointsList.indexOf(joint);
            mJointsList.removeAt(removeJointIndex);
            mSupportsList.removeOne(support);

            bool checkA = mJointsList.size() < 3;

            bool checkB = false;

            if (!checkA)
            {
                checkB = mBarsList.size() < 2;
            }

            bool checkC = false;

            if (!checkA && !checkB)
            {
                checkC = mSupportsList.size() < 2;
            }

            bool checkD = false;

            if (!checkA && !checkB && !checkC)
            {
                checkD = hasInsufficientBars();
            }

            bool checkE = false;

            if (!checkA && !checkB && !checkC && !checkD)
            {
                checkE = hasInsufficientSupports();
            }

            if (!checkA && !checkB && !checkC && !checkD && !checkE)
            {
                redundantSupportsList.append(support);
            }

            jointA->attachedBars()->append(bar);
            jointB->attachedBars()->append(bar);
            jointA->connectedJoints()->append(jointB);
            jointB->connectedJoints()->append(jointA);
            mBarsList.insert(removeBarIndex, bar);
            mJointsList.insert(removeJointIndex, joint);
            mSupportsList.insert(index, support);
        }
        else
        {
            mSupportsList.removeOne(support);

            bool checkA = mSupportsList.size() < 2;

            bool checkB = false;

            if (!checkA)
            {
                checkB = hasInsufficientSupports();
            }

            bool checkC = false;

            if (!checkA && !checkB)
            {
                checkC = hasUnstableConfiguration(partitionedStiffnessMatrix);
            }

            if (!checkA && !checkB && !checkC)
            {
                redundantSupportsList.append(support);
            }

            mSupportsList.insert(index, support);
        }
    }

    emit redundantSupportsListSignal(redundantSupportsList);

    QString note;
    note.append(tr("Model checks            : Ok\n"));
    note.append(tr("Joints count            : ") + QString::number(mJointsList.size()) + tr("\n"));
    note.append(tr("Bars count              : ") + QString::number(mBarsList.size()) + tr("\n"));
    note.append(tr("Supports count          : ") + QString::number(mSupportsList.size()) + tr("\n"));

    //Determine degree of indeterminacy
    hasInsufficientBars();

    if (mDegreeOfIndeterminacy > 0)
    {
        note.append(tr("Degree of indeterminacy : ") + QString::number(mDegreeOfIndeterminacy) + tr("\n"));
    }

    foreach (Joint *joint, substituteJointsList)
    {
        delete joint;
    }

    foreach (Bar *bar, substituteBarsList)
    {
        delete bar;
    }

    gsl_matrix_free(partitionedStiffnessMatrix);

    emit notesSignal(note);
    emit progressSignal(100);

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasDisconnectedParts()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check if structure has disconnected parts
    int index = 0;
    QList<Joint *> connectedJointsList;
    connectedJointsList.append(mJointsList.at(index));

    forever
    {
        foreach (Joint *joint, *connectedJointsList.at(index)->connectedJoints())
        {
            if (!connectedJointsList.contains(joint))
            {
                connectedJointsList.append(joint);
            }
        }

        if (!(index == (connectedJointsList.size() - 1)))
        {
            ++index;
        }
        else
        {
            if (connectedJointsList.size() == mJointsList.size())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasDanglingBar()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check if structure has dangling bar
    foreach (Joint *joint, mJointsList)
    {
        bool checkA = joint->attachedBars()->size() == 1;
        bool checkB = !joint->isSupported();

        if (checkA && checkB)
        {
            return true;
        }
    }

    return false;

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasFixedJoints()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check if structure has all joints fixed
    foreach (Joint *joint, mJointsList)
    {
        if (joint->isSupported())
        {
            Support *support = jointSupport(joint);

            bool checkA = support->type() == UnitsAndLimits::FIXED_BOTTOM;
            bool checkB = support->type() == UnitsAndLimits::FIXED_LEFT;
            bool checkC = support->type() == UnitsAndLimits::FIXED_TOP;
            bool checkD = support->type() == UnitsAndLimits::FIXED_RIGHT;

            bool checkE = (checkA || checkB || checkC || checkD);

            if (!checkE)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return true;

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasInsufficientSupports()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check for instability due to insufficient supports
    int fixedSupportsCount = 0;

    foreach (Support *support, mSupportsList)
    {
        bool checkA = support->type() == UnitsAndLimits::FIXED_BOTTOM;
        bool checkB = support->type() == UnitsAndLimits::FIXED_LEFT;
        bool checkC = support->type() == UnitsAndLimits::FIXED_TOP;
        bool checkD = support->type() == UnitsAndLimits::FIXED_RIGHT;

        bool checkE = (checkA || checkB || checkC || checkD);

        if (checkE)
        {
            ++fixedSupportsCount;
        }
    }

    if (mBarsList.size() == fixedSupportsCount)
    {
        return false; // Stability guaranteed
    }
    else
    {
        qreal epsilonMagnitudeSmall = 1.0e-12;
        qreal epsilonMagnitudeLarge = 5.73e+31;

        QList<qreal> slopesList;
        QList<QPointF> pointsList;

        foreach (Support *support, mSupportsList)
        {
            qreal slope;
            QPointF point;
            Joint *joint      = support->supportJoint();
            qreal xCoordinate = joint->xCoordinate();
            qreal yCoordinate = joint->yCoordinate();
            point.setX(xCoordinate);
            point.setY(yCoordinate);

            if (support->supportJoint()->attachedBars()->size() == 1)
            {
                Bar *bar = support->supportJoint()->attachedBars()->first();

                qreal x1     = bar->firstJoint()->xCoordinate();
                qreal y1     = bar->firstJoint()->yCoordinate();
                qreal x2     = bar->secondJoint()->xCoordinate();
                qreal y2     = bar->secondJoint()->yCoordinate();
                qreal deltaX = x2 - x1;
                qreal deltaY = y2 - y1;

                if (std::fabs(deltaX) < epsilonMagnitudeSmall)
                {
                    slope = epsilonMagnitudeLarge;
                }
                else
                {
                    slope = deltaY / deltaX;
                }

                slopesList.append(slope);
                pointsList.append(point);
            }
            else
            {
                bool checkA = support->type() == UnitsAndLimits::FIXED_BOTTOM;
                bool checkB = support->type() == UnitsAndLimits::FIXED_LEFT;
                bool checkC = support->type() == UnitsAndLimits::FIXED_TOP;
                bool checkD = support->type() == UnitsAndLimits::FIXED_RIGHT;

                if (checkA || checkB || checkC || checkD)
                {
                    slopesList.append(epsilonMagnitudeLarge);
                    pointsList.append(point);
                    slopesList.append(0.0);
                    pointsList.append(point);
                }

                UnitsAndLimits::SupportType type = support->type();

                switch (type)
                {
                    case UnitsAndLimits::ROLLER_LEFT:
                    case UnitsAndLimits::ROLLER_RIGHT:
                        slopesList.append(0.0);
                        pointsList.append(point);
                        break;
                    case UnitsAndLimits::ROLLER_BOTTOM:
                    case UnitsAndLimits::ROLLER_TOP:
                        slopesList.append(epsilonMagnitudeLarge);
                        pointsList.append(point);
                        break;
                    case UnitsAndLimits::ROLLER:
                    {
                        bool checkE = std::fabs(support->angle()) < epsilonMagnitudeSmall;
                        bool checkF = std::fabs(support->angle() - 180.0) < epsilonMagnitudeSmall;
                        bool checkG = std::fabs(support->angle() - 360.0) < epsilonMagnitudeSmall;

                        if (checkE || checkF || checkG)
                        {
                            slopesList.append(epsilonMagnitudeLarge);
                            pointsList.append(point);
                        }
                        else
                        {
                            slope = std::tan(qDegreesToRadians(90.0 - support->angle()));
                            slopesList.append(slope);
                            pointsList.append(point);
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
        }

        if (slopesList.size() < 3)
        {
            return true;
        }
        else
        {
            int indexA     = 0;
            int indexB     = 0;
            int indexC     = 0;
            qreal slopeA   = slopesList.at(indexA);
            QPointF pointA = pointsList.at(indexA);
            QPointF intersectionPoint;

            for (int i = 0; i < slopesList.size(); ++i)
            {
                if (i != indexA)
                {
                    indexB         = i;
                    qreal slopeB   = slopesList.at(indexB);
                    QPointF pointB = pointsList.at(indexB);

                    if (std::fabs(slopeA - slopeB) > epsilonMagnitudeSmall)
                    {
                        qreal x1 = pointA.x();
                        qreal y1 = pointA.y();
                        qreal x2 = pointB.x();
                        qreal y2 = pointB.y();

                        qreal xCoordinate = ((y2 - y1) + (x1 * slopeA - x2 * slopeB))
                                / (slopeA - slopeB);
                        qreal yCoordinate = ((slopeA * y2 - slopeB * y1)
                                              + slopeA * slopeB * (x1 - x2))
                                / (slopeA - slopeB);

                        intersectionPoint.setX(xCoordinate);
                        intersectionPoint.setY(yCoordinate);

                        break;
                    }
                }

                if (i == (slopesList.size() - 1))
                {
                    return true;
                }
            }

            for (int i = 0; i < slopesList.size(); ++i)
            {
                if ((i != indexA) && (i != indexB))
                {
                    indexC            = i;
                    qreal slope       = slopesList.at(indexC);
                    qreal intercept   = pointsList.at(indexC).y() - slope * pointsList.at(indexC).x();
                    qreal xCoordinate = intersectionPoint.x();
                    qreal yCoordinate = intersectionPoint.y();
                    qreal distance    = std::fabs(-slope * xCoordinate + yCoordinate
                                          - intercept)
                            / std::sqrt(std::pow(-slope, 2.0) + std::pow(-intercept, 2.0));

                    if (distance > epsilonMagnitudeSmall)
                    {
                        return false;
                    }
                }

                if (i == (slopesList.size() - 1))
                {
                    return true;
                }
            }
        }
    }

    return true;

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasInsufficientBars()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check for instability due to insufficient bars
    int jointsCount            = mJointsList.size();
    int barsCount              = mBarsList.size();
    int supportComponentsCount = 0;
    int fixedSupportsCount     = 0;
    mDegreeOfIndeterminacy     = 0;

    foreach (Support *support, mSupportsList)
    {
        bool checkA = support->type() == UnitsAndLimits::FIXED_BOTTOM;
        bool checkB = support->type() == UnitsAndLimits::FIXED_LEFT;
        bool checkC = support->type() == UnitsAndLimits::FIXED_TOP;
        bool checkD = support->type() == UnitsAndLimits::FIXED_RIGHT;

        bool checkE = (checkA || checkB || checkC || checkD);

        if (checkE)
        {
            ++fixedSupportsCount;
        }
    }

    if (barsCount == fixedSupportsCount) //Stability guaranteed
    {
        if ((3 * barsCount) > (2 * jointsCount))
        {
            mDegreeOfIndeterminacy = (3 * barsCount) - (2 * jointsCount);
        }

        return false;
    }
    else
    {
        foreach (Joint *joint, mJointsList)
        {
            if (joint->attachedBars()->size() == 1)
            {
                --jointsCount;
                --barsCount;
                ++supportComponentsCount;
            }
            else
            {
                if (joint->isSupported())
                {
                    Support *support = jointSupport(joint);

                    UnitsAndLimits::SupportType type = support->type();

                    switch (type)
                    {
                        case UnitsAndLimits::FIXED_LEFT:
                        case UnitsAndLimits::FIXED_TOP:
                        case UnitsAndLimits::FIXED_RIGHT:
                        case UnitsAndLimits::FIXED_BOTTOM:
                            supportComponentsCount += 2;
                            break;
                        case UnitsAndLimits::ROLLER_LEFT:
                        case UnitsAndLimits::ROLLER_TOP:
                        case UnitsAndLimits::ROLLER_RIGHT:
                        case UnitsAndLimits::ROLLER_BOTTOM:
                        case UnitsAndLimits::ROLLER:
                            supportComponentsCount += 1;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        if ((barsCount + supportComponentsCount) > 2 * jointsCount)
        {
            mDegreeOfIndeterminacy = (barsCount + supportComponentsCount) - 2 * jointsCount;
        }

        if ((barsCount + supportComponentsCount) < 2 * jointsCount)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasFixedBars()
{
    //---------------------------------------------------------------------------------------------------------------

    //Check for bars with fixed ends
    foreach (Bar *bar, mBarsList)
    {
        Joint *jointA = bar->firstJoint();
        Joint *jointB = bar->secondJoint();

        bool checkA = jointA->isSupported();
        bool checkB = jointB->isSupported();

        if (checkA && checkB)
        {
            Support *supportA = jointSupport(jointA);
            Support *supportB = jointSupport(jointB);

            bool checkC = supportA->type() == UnitsAndLimits::FIXED_BOTTOM
                    || supportA->type() == UnitsAndLimits::FIXED_LEFT
                    || supportA->type() == UnitsAndLimits::FIXED_TOP
                    || supportA->type() == UnitsAndLimits::FIXED_RIGHT;

            bool checkD = supportB->type() == UnitsAndLimits::FIXED_BOTTOM
                    || supportB->type() == UnitsAndLimits::FIXED_LEFT
                    || supportB->type() == UnitsAndLimits::FIXED_TOP
                    || supportB->type() == UnitsAndLimits::FIXED_RIGHT;

            if (checkC && checkD)
            {
                return true;
            }
        }
    }

    return false;

    //---------------------------------------------------------------------------------------------------------------
}

bool ModelChecker::hasUnstableConfiguration(gsl_matrix *matrix)
{
    //---------------------------------------------------------------------------------------------------------------

    //Check for instability due to bars configuration

    qreal epsilonMagnitudeSmall = 1.0e-6;

    int order = matrix->size1;

    gsl_matrix *lower = gsl_matrix_calloc(order, order);
    gsl_matrix_memcpy(lower, matrix);

    //turn-off error handler
    gsl_set_error_handler_off();

    int status = gsl_linalg_cholesky_decomp1(lower);
    //fprintf(stderr, "\nCholesky decomposition: %s\n", gsl_strerror(status));

    //restore error handler
    gsl_set_error_handler(NULL);

    if (status != GSL_SUCCESS)
    {
        gsl_matrix_free(lower);
        return true;
    }

    for (int i = 0; i < order; ++i)
    {
        if (std::fabs(gsl_matrix_get(lower, i, i)) < epsilonMagnitudeSmall)
        {
            gsl_matrix_free(lower);
            return true;
        }
    }

    gsl_matrix_free(lower);
    return false;

    //---------------------------------------------------------------------------------------------------------------
}

