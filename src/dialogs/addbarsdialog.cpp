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

/* addbarsdialog.cpp */

#include "addbarsdialog.h"
#include "ui_addbarsdialog.h"

AddBarsDialog::AddBarsDialog(UnitsAndLimits *unitsAndLimits,
                             QList<Joint *> *jointsList,
                             QList<Bar *>   *barsList,
                             QTableView     *jointsTableView,
                             QTableView     *barsTableView,
                             QWidget        *parent) : QDialog(parent), ui(new Ui::AddBarsDialog)
{
    ui->setupUi(this);

    ui->mirrorAxisComboBox->addItem(tr("Horizontal"));
    ui->mirrorAxisComboBox->addItem(tr("Vertical"));
    ui->mirrorAxisComboBox->setCurrentIndex(-1);

    mUnitsAndLimits  = unitsAndLimits;
    mJointsList      = jointsList;
    mBarsList        = barsList;
    mJointsTableView = jointsTableView;
    mBarsTableView   = barsTableView;
    mPoint           = QPoint(0, 0);

    updateUnits();
    updateAreaModulusFactorOption();

    connect(ui->fromJointComboBox, SIGNAL(activated(int)), this, SLOT(setSecondJointOptions(int)));
    connect(ui->addBarsPushButton, SIGNAL(clicked()), this, SLOT(addBars()));
}

AddBarsDialog::~AddBarsDialog()
{
    delete ui;
}

QPoint AddBarsDialog::position() const
{
    return mPoint;
}

void AddBarsDialog::updateMirrorJointOptions()
{
    ui->mirrorJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->mirrorJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->mirrorJointComboBox->setCurrentIndex(-1);
    ui->mirrorAxisComboBox->setCurrentIndex(-1);
}

void AddBarsDialog::updateUnits()
{
    ui->areaLabel->setText(tr("Area of section (%1)").arg(mUnitsAndLimits->areaUnit()));
    ui->modulusLabel->setText(tr("Modulus of elasticity (%1)").arg(mUnitsAndLimits->modulusUnit()));
    ui->unitWeightLabel->setText(tr("Unit weight (%1)").arg(mUnitsAndLimits->unitWeightUnit()));
    ui->addBarsDeltaXLabel->setText(tr("%1x (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));
    ui->addBarsDeltaYLabel->setText(tr("%1y (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));

    ui->addBarsDeltaXDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                  "Min : %1 (%3)\n"
                                                  "Max :  %2 (%3)")
                                               .arg(QString::number(-mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                               .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                               .arg(mUnitsAndLimits->coordinateUnit()));

    ui->addBarsDeltaYDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                  "Min : %1 (%3)\n"
                                                  "Max :  %2 (%3)")
                                               .arg(QString::number(-mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                               .arg(QString::number(mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                               .arg(mUnitsAndLimits->coordinateUnit()));

    ui->areaDoubleSpinBox->setToolTip(tr("Valid range\n"
                                         "Min : %1 (%3)\n"
                                         "Max : %2 (%3)")
                                      .arg(QString::number(mUnitsAndLimits->minArea(), 'g', 3))
                                      .arg(QString::number(mUnitsAndLimits->maxArea(), 'g', 3))
                                      .arg(mUnitsAndLimits->areaUnit()));

    ui->modulusDoubleSpinBox->setToolTip(tr("Valid range\n"
                                            "Min : %1 (%3)\n"
                                            "Max : %2 (%3)")
                                         .arg(QString::number(mUnitsAndLimits->minModulus(), 'g', 3))
                                         .arg(QString::number(mUnitsAndLimits->maxModulus(), 'g', 3))
                                         .arg(mUnitsAndLimits->modulusUnit()));

    ui->factorDoubleSpinBox->setToolTip(tr("Valid range\n"
                                           "Min : %1\n"
                                           "Max : %2")
                                        .arg(QString::number(mUnitsAndLimits->minFactor(), 'g', 3))
                                        .arg(QString::number(mUnitsAndLimits->maxFactor(), 'g', 3)));

    ui->unitWeightDoubleSpinBox->setToolTip(tr("Valid range\n"
                                               "Min : %1 (%3)\n"
                                               "Max : %2 (%3)")
                                            .arg(QString::number(mUnitsAndLimits->minUnitWeight(), 'g', 3))
                                            .arg(QString::number(mUnitsAndLimits->maxUnitWeight(), 'g', 3))
                                            .arg(mUnitsAndLimits->unitWeightUnit()));

    ui->repeatSpinBox->clear();
    ui->repeatSpinBox->setValue(0);

    ui->addBarsDeltaXDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxXCoordinate());
    ui->addBarsDeltaXDoubleSpinBox->setMaximum(mUnitsAndLimits->maxXCoordinate());
    ui->addBarsDeltaXDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaXDecimals());
    ui->addBarsDeltaXDoubleSpinBox->clear();
    ui->addBarsDeltaXDoubleSpinBox->setValue(0.0);

    ui->addBarsDeltaYDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxYCoordinate());
    ui->addBarsDeltaYDoubleSpinBox->setMaximum(mUnitsAndLimits->maxYCoordinate());
    ui->addBarsDeltaYDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaYDecimals());
    ui->addBarsDeltaYDoubleSpinBox->clear();
    ui->addBarsDeltaYDoubleSpinBox->setValue(0.0);

    ui->areaDoubleSpinBox->setMinimum(mUnitsAndLimits->minArea());
    ui->areaDoubleSpinBox->setMaximum(mUnitsAndLimits->maxArea());
    ui->areaDoubleSpinBox->setDecimals(mUnitsAndLimits->areaDecimals());
    ui->areaDoubleSpinBox->clear();
    ui->areaDoubleSpinBox->setValue(mUnitsAndLimits->minArea());

    ui->modulusDoubleSpinBox->setMinimum(mUnitsAndLimits->minModulus());
    ui->modulusDoubleSpinBox->setMaximum(mUnitsAndLimits->maxModulus());
    ui->modulusDoubleSpinBox->setDecimals(mUnitsAndLimits->modulusDecimals());
    ui->modulusDoubleSpinBox->clear();
    ui->modulusDoubleSpinBox->setValue(mUnitsAndLimits->minModulus());

    ui->factorDoubleSpinBox->setMinimum(mUnitsAndLimits->minFactor());
    ui->factorDoubleSpinBox->setMaximum(mUnitsAndLimits->maxFactor());
    ui->factorDoubleSpinBox->setDecimals(mUnitsAndLimits->factorDecimals());
    ui->factorDoubleSpinBox->clear();
    ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());

    ui->unitWeightDoubleSpinBox->setMinimum(mUnitsAndLimits->minUnitWeight());
    ui->unitWeightDoubleSpinBox->setMaximum(mUnitsAndLimits->maxUnitWeight());
    ui->unitWeightDoubleSpinBox->setDecimals(mUnitsAndLimits->unitWeightDecimals());
    ui->unitWeightDoubleSpinBox->clear();
    ui->unitWeightDoubleSpinBox->setValue(mUnitsAndLimits->minUnitWeight());
}

void AddBarsDialog::updateFirstJointOptions()
{
    ui->fromJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->fromJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->fromJointComboBox->setCurrentIndex(-1);
    ui->toJointComboBox->clear();
    updateMirrorJointOptions();
}

void AddBarsDialog::updateAreaModulusFactorOption()
{
    if (mBarsTableView->model()->columnCount() == 3)
    {
        ui->areaDoubleSpinBox->clear();
        ui->areaDoubleSpinBox->setValue(mUnitsAndLimits->minArea());
        ui->areaDoubleSpinBox->setEnabled(false);
        ui->modulusDoubleSpinBox->clear();
        ui->modulusDoubleSpinBox->setValue(mUnitsAndLimits->minModulus());
        ui->modulusDoubleSpinBox->setEnabled(false);
        ui->unitWeightDoubleSpinBox->clear();
        ui->unitWeightDoubleSpinBox->setValue(mUnitsAndLimits->minUnitWeight());
        ui->unitWeightDoubleSpinBox->setEnabled(false);

        if (!ui->factorDoubleSpinBox->isEnabled())
        {
            ui->factorDoubleSpinBox->setEnabled(true);
        }

        ui->factorDoubleSpinBox->clear();
        ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());
    }
    else
    {
        ui->areaDoubleSpinBox->setEnabled(true);
        ui->modulusDoubleSpinBox->setEnabled(true);
        ui->unitWeightDoubleSpinBox->setEnabled(true);
        ui->factorDoubleSpinBox->clear();
        ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());
        ui->factorDoubleSpinBox->setEnabled(false);
    }
}

void AddBarsDialog::setSecondJointOptions(int firstJointIndex)
{
    ui->toJointComboBox->clear();

    //Determine connectable joints for given joint
    foreach (Joint *joint, *mJointsList)
    {
        //Skip given joint
        if (joint == mJointsList->at(firstJointIndex))
        {
            continue;
        }

        //Skip joint if already connected to given joint
        if (mJointsList->at(firstJointIndex)->connectedJoints()->contains(joint))
        {
            continue;
        }

        //Check for minimum and maximum bar length
        qreal firstX1 = mJointsList->at(firstJointIndex)->xCoordinate();
        qreal firstY1 = mJointsList->at(firstJointIndex)->yCoordinate();

        qreal firstX2 = joint->xCoordinate();
        qreal firstY2 = joint->yCoordinate();

        qreal firstXComponent = firstX2 - firstX1;
        qreal firstYComponent = firstY2 - firstY1;

        qreal firstLength = std::sqrt(std::pow(firstXComponent, 2.0) + std::pow(firstYComponent, 2.0));

        if (!((mUnitsAndLimits->minBarLength() < firstLength) && (mUnitsAndLimits->maxBarLength() > firstLength)))
        {
            continue;
        }

        //Check that implied bar will not coincide with any existing joint
        qreal epsilonMagnitudeSmall = 1.0e-12;
        qreal epsilonMagnitudeLarge = 1.0e+12;

        bool checkA = false;

        foreach (Joint *secondJoint, *mJointsList)
        {
            //Skip given joint
            if (secondJoint == mJointsList->at(firstJointIndex))
            {
                continue;
            }

            //Skip joint implied for connection
            if (secondJoint == joint)
            {
                continue;
            }

            qreal secondX2 = secondJoint->xCoordinate();
            qreal secondY2 = secondJoint->yCoordinate();

            qreal secondXComponent = secondX2 - firstX1;
            qreal secondYComponent = secondY2 - firstY1;

            qreal magnitudeCrossProduct = std::fabs(firstXComponent * secondYComponent
                                                    - firstYComponent * secondXComponent);

            if (magnitudeCrossProduct < epsilonMagnitudeSmall)
            {
                qreal quotient = 0.0;

                if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
                {
                    quotient = secondYComponent / firstYComponent;
                }
                else
                {
                    quotient = secondXComponent / firstXComponent;
                }

                if ((quotient > 0.0) && (quotient < 1.0))
                {
                    checkA = true;
                    break;
                }
            }
        }

        if (checkA)
        {
            continue;
        }

        //Confirm given joint if there are no bars
        if (mBarsList->isEmpty())
        {
            int jointNumber = mJointsList->indexOf(joint) + 1;
            ui->toJointComboBox->addItem(QString::number(jointNumber));
            ui->toJointComboBox->setCurrentIndex(-1);
            continue;
        }

        //Check that implied bar will not coincide with any existing bar
        qreal firstSlope;
        qreal firstIntercept;
        qreal secondSlope;
        qreal secondIntercept;

        checkA = false;

        foreach (Bar *bar, *mBarsList)
        {
            qreal secondX1 = bar->firstJoint()->xCoordinate();
            qreal secondY1 = bar->firstJoint()->yCoordinate();
            qreal secondX2 = bar->secondJoint()->xCoordinate();
            qreal secondY2 = bar->secondJoint()->yCoordinate();

            qreal secondXComponent = secondX2 - secondX1;
            qreal secondYComponent = secondY2 - secondY1;

            if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
            {
                firstSlope = epsilonMagnitudeLarge;
                firstIntercept = firstX1;
            }
            else
            {
                firstSlope = firstYComponent / firstXComponent;
                firstIntercept = (firstY2 * firstX1 - firstY1 * firstX2) / (-firstXComponent);
            }

            if (std::fabs(secondXComponent) < epsilonMagnitudeSmall)
            {
                secondSlope = epsilonMagnitudeLarge;
                secondIntercept = secondX1;
            }
            else
            {
                secondSlope = secondYComponent / secondXComponent;
                secondIntercept = (secondY2 * secondX1 - secondY1 * secondX2) / (-secondXComponent);
            }

            qreal magnitudeSlopeDifference = std::fabs(firstSlope - secondSlope);
            qreal magnitudeInterceptDifference = std::fabs(firstIntercept - secondIntercept);

            bool checkB = ((magnitudeSlopeDifference < epsilonMagnitudeSmall)
                               && (magnitudeInterceptDifference < epsilonMagnitudeSmall));

            std::vector<qreal> xCoordinates;
            std::vector<qreal> yCoordinates;

            std::vector<qreal>::iterator position;

            xCoordinates.push_back(firstX1);
            xCoordinates.push_back(firstX2);
            xCoordinates.push_back(secondX1);
            xCoordinates.push_back(secondX2);
            yCoordinates.push_back(firstY1);
            yCoordinates.push_back(firstY2);
            yCoordinates.push_back(secondY1);
            yCoordinates.push_back(secondY2);

            position = std::min_element(xCoordinates.begin(), xCoordinates.end());
            qreal xMin = *position;

            position = std::max_element(xCoordinates.begin(), xCoordinates.end());
            qreal xMax = *position;

            position = std::min_element(yCoordinates.begin(), yCoordinates.end());
            qreal yMin = *position;

            position = std::max_element(yCoordinates.begin(), yCoordinates.end());
            qreal yMax = *position;

            qreal xComponent = xMax - xMin;
            qreal yComponent = yMax - yMin;

            qreal length = std::sqrt(std::pow(xComponent, 2.0) + std::pow(yComponent, 2.0));

            qreal secondLength = std::sqrt(std::pow(secondXComponent, 2.0) + std::pow(secondYComponent, 2.0));

            bool checkC = ((firstLength + secondLength) - length) > epsilonMagnitudeSmall;

            if (checkB && checkC)
            {
                checkA = true;
                break;
            }
        }

        if (!checkA)
        {
            int jointNumber = mJointsList->indexOf(joint) + 1;
            ui->toJointComboBox->addItem(QString::number(jointNumber));
            ui->toJointComboBox->setCurrentIndex(-1);
        }
    }
}

void AddBarsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedJoints;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJoints.append(index.row() + 1);
    }

    ui->selectedJointsTextEdit->clear();

    foreach (int jointNumber, selectedJoints)
    {
        ui->selectedJointsTextEdit->append(QString::number(jointNumber));
    }
}

void AddBarsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    if (!selectedBars.isEmpty())
    {
        ui->fromJointComboBox->setCurrentIndex(-1);
        ui->fromJointComboBox->setEnabled(false);
        ui->toJointComboBox->setCurrentIndex(-1);
        ui->toJointComboBox->setEnabled(false);
        ui->repeatSpinBox->setMinimum(1);
        ui->repeatSpinBox->clear();
        ui->repeatSpinBox->setValue(1);

        if (mBarsTableView->model()->columnCount() == 3)
        {
            ui->factorDoubleSpinBox->clear();

            if (!mBarsList->isEmpty())
            {
                ui->factorDoubleSpinBox->setValue(mBarsList->last()->factor());
            }
            else
            {
                ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());
            }

            ui->factorDoubleSpinBox->setEnabled(false);
        }
        else
        {
            ui->areaDoubleSpinBox->clear();
            ui->modulusDoubleSpinBox->clear();
            ui->unitWeightDoubleSpinBox->clear();

            if (!mBarsList->isEmpty())
            {
                ui->areaDoubleSpinBox->setValue(mBarsList->last()->area());
                ui->modulusDoubleSpinBox->setValue(mBarsList->last()->modulus());
                ui->unitWeightDoubleSpinBox->setValue(mBarsList->last()->unitWeight());
            }

            ui->areaDoubleSpinBox->setEnabled(false);
            ui->modulusDoubleSpinBox->setEnabled(false);
            ui->unitWeightDoubleSpinBox->setEnabled(false);
        }
    }
    else
    {
        ui->fromJointComboBox->setEnabled(true);
        ui->toJointComboBox->setEnabled(true);
        ui->repeatSpinBox->setMinimum(0);
        ui->repeatSpinBox->clear();
        ui->repeatSpinBox->setValue(0);

        if (mBarsTableView->model()->columnCount() == 3)
        {
            ui->factorDoubleSpinBox->setEnabled(true);
        }
        else
        {
            ui->areaDoubleSpinBox->setEnabled(true);
            ui->modulusDoubleSpinBox->setEnabled(true);
            ui->unitWeightDoubleSpinBox->setEnabled(true);
        }
    }

    ui->selectedBarsTextEdit->clear();

    foreach (int barNumber, selectedBars)
    {
        ui->selectedBarsTextEdit->append(QString::number(barNumber));
    }
}

void AddBarsDialog::addBars()
{
    if (ui->optionTabWidget->currentIndex() == 0)
    {
        int firstJoint  = 0;
        int secondJoint = 0;

        if (ui->fromJointComboBox->isEnabled())
        {
            if ((ui->fromJointComboBox->currentIndex() < 0))
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("first joint : Input error"));
                messageBox.setInformativeText(tr("Not selected!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }
            else
            {
                firstJoint = ui->fromJointComboBox->currentText().toInt();
            }

            if ((ui->toJointComboBox->currentIndex() < 0))
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("second joint : Input error"));
                messageBox.setInformativeText(tr("Not selected!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }
            else
            {
                secondJoint = ui->toJointComboBox->currentText().toInt();
            }
        }

        int repeat   = 0;
        qreal deltaX = 0.0;
        qreal deltaY = 0.0;

        if (ui->repeatSpinBox->value() > 0)
        {
            repeat = ui->repeatSpinBox->value();
            deltaX = ui->addBarsDeltaXDoubleSpinBox->value();
            deltaY = ui->addBarsDeltaYDoubleSpinBox->value();

            bool check   = false;
            qreal length = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
            check = (mUnitsAndLimits->minBarLength() < length) && (length < mUnitsAndLimits->maxXCoordinate());

            if (!check)
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("%1x and/or %1y : Input error").arg(QString::fromUtf8("\u0394")));
                QString infoString = QString("Invalid input\n"
                                             "Entered : %1 %4\n"
                                             "Valid range\n"
                                             "Min : > %2 %4\n"
                                             "Max : < %3 %4")
                                .arg(QString::number(length, 'g', 3))
                                .arg(QString::number(mUnitsAndLimits->minBarLength(), 'g', 3))
                                .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                .arg(mUnitsAndLimits->coordinateUnit());

                messageBox.setInformativeText(infoString);

                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                ui->addBarsDeltaXDoubleSpinBox->setFocus();
                ui->addBarsDeltaXDoubleSpinBox->selectAll();
                return;
            }
        }

        if (ui->fromJointComboBox->isEnabled())
        {
            ui->repeatSpinBox->clear();
            ui->repeatSpinBox->setValue(0);
        }

        qreal area       = 0.0;
        qreal modulus    = 0.0;
        qreal factor     = 0.0;
        qreal unitWeight = 0.0;

        if (ui->areaDoubleSpinBox->isEnabled())
        {
            area = ui->areaDoubleSpinBox->value();
            modulus = ui->modulusDoubleSpinBox->value();
            unitWeight = ui->unitWeightDoubleSpinBox->value();
        }
        else
        {
            factor = ui->factorDoubleSpinBox->value();
        }

        emit inputsSignal(firstJoint,
                          secondJoint,
                          area,
                          modulus,
                          factor,
                          unitWeight,
                          repeat,
                          deltaX,
                          deltaY);
    }
    else if (ui->optionTabWidget->currentIndex() == 1)
    {
        QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();

        if (!itemSelectionModel->hasSelection())
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Bars to Mirror : Input error"));
            messageBox.setInformativeText(tr("No selection!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        if (ui->mirrorAxisComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Mirror Axis : Input error"));
            messageBox.setInformativeText(tr("Not selected!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        if (ui->mirrorJointComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Mirror Axis Joint : Input error"));
            messageBox.setInformativeText(tr("Not selected!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        emit inputsSignal(ui->mirrorAxisComboBox->currentText(), ui->mirrorJointComboBox->currentText().toInt());
    }
    else
    {
        QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

        if (!itemSelectionModel->hasSelection())
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Bars to Link : Input error"));
            messageBox.setInformativeText(tr("No selection!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        qreal area       = 0.0;
        qreal modulus    = 0.0;
        qreal factor     = 0.0;
        qreal unitWeight = 0.0;

        if (ui->areaDoubleSpinBox->isEnabled())
        {
            area = ui->areaDoubleSpinBox->value();
            modulus = ui->modulusDoubleSpinBox->value();
            unitWeight = ui->unitWeightDoubleSpinBox->value();
        }
        else
        {
            factor = ui->factorDoubleSpinBox->value();
        }

        emit inputsSignal(area,
                          modulus,
                          factor,
                          unitWeight);
    }
}

void AddBarsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
