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

/* editbarsdialog.cpp */

#include "editbarsdialog.h"
#include "ui_editbarsdialog.h"

EditBarsDialog::EditBarsDialog(UnitsAndLimits *unitsAndLimits,
                               QList<Joint *> *jointsList,
                               QList<Bar *>   *barsList,
                               QTableView     *barsTableView,
                               QWidget        *parent) : QDialog(parent), ui(new Ui::EditBarsDialog)
{
    ui->setupUi(this);

    ui->fromJointComboBox->setEnabled(false);
    ui->toJointComboBox->setEnabled(false);
    ui->areaDoubleSpinBox->setEnabled(false);
    ui->modulusDoubleSpinBox->setEnabled(false);
    ui->factorDoubleSpinBox->setEnabled(false);
    ui->unitWeightDoubleSpinBox->setEnabled(false);

    mUnitsAndLimits = unitsAndLimits;
    mJointsList     = jointsList;
    mBarsList       = barsList;
    mBarsTableView  = barsTableView;
    mPoint          = QPoint(0, 0);
    updateUnits();

    connect(ui->fromJointComboBox, SIGNAL(activated(int)), this, SLOT(setSecondJointOptions(int)));
    connect(ui->editBarsPushButton, SIGNAL(pressed()), this, SLOT(editBars()));
}

EditBarsDialog::~EditBarsDialog()
{
    delete ui;
}

QPoint EditBarsDialog::position() const
{
    return mPoint;
}

void EditBarsDialog::updateUnits()
{
    ui->areaLabel->setText(tr("Area of section (%1)").arg(mUnitsAndLimits->areaUnit()));
    ui->modulusLabel->setText(tr("Modulus of elasticity (%1)").arg(mUnitsAndLimits->modulusUnit()));
    ui->unitWeightLabel->setText(tr("Unit weight (%1)").arg(mUnitsAndLimits->unitWeightUnit()));

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
    ui->unitWeightDoubleSpinBox->setDecimals(mUnitsAndLimits->factorDecimals());
    ui->unitWeightDoubleSpinBox->clear();
    ui->unitWeightDoubleSpinBox->setValue(mUnitsAndLimits->minUnitWeight());
}

void EditBarsDialog::updateFirstJointOptions()
{
    mBarsTableView->clearSelection();
    ui->fromJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->fromJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->fromJointComboBox->setCurrentIndex(-1);
}

void EditBarsDialog::setSecondJointOptions(int firstJointIndex)
{
    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();
    QList<int> selectedBars;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedBars.append(index.row() + 1);
    }

    int barIndex       = selectedBars.first() - 1;
    Bar *editingBar    = mBarsList->at(barIndex);
    Joint *firstJoint  = editingBar->firstJoint();
    Joint *secondJoint = editingBar->secondJoint();

    ui->toJointComboBox->clear();

    //Determine connectable joints for given joint
    foreach (Joint *joint, *mJointsList)
    {
        //Skip given joint
        if (joint == mJointsList->at(firstJointIndex))
        {
            continue;
        }

        //Skip joint if already connected to given joint and is not current second joint
        if (mJointsList->at(firstJointIndex)->connectedJoints()->contains(joint))
        {
            bool checkA = false;

            if ((mJointsList->at(firstJointIndex) == firstJoint) && (joint == secondJoint))
            {
                checkA = true;
            }

            if ((mJointsList->at(firstJointIndex) == secondJoint) && (joint == firstJoint))
            {
                checkA = true;
            }

            if (!checkA)
            {
                continue;
            }
        }

        //Check for minimum and maximum bar length
        qreal firstX1 = mJointsList->at(firstJointIndex)->xCoordinate();
        qreal firstY1 = mJointsList->at(firstJointIndex)->yCoordinate();
        qreal firstX2 = joint->xCoordinate();
        qreal firstY2 = joint->yCoordinate();

        qreal firstXComponent = firstX2 - firstX1;
        qreal firstYComponent = firstY2 - firstY1;
        qreal firstLength     = std::sqrt(std::pow(firstXComponent, 2.0) + std::pow(firstYComponent, 2.0));

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

        //Check that implied bar will not coincide with any existing bar
        qreal firstSlope;
        qreal firstIntercept;
        qreal secondSlope;
        qreal secondIntercept;

        checkA = false;

        foreach (Bar *bar, *mBarsList)
        {
            if (bar == editingBar)
            {
                continue;
            }

            qreal secondX1         = bar->firstJoint()->xCoordinate();
            qreal secondY1         = bar->firstJoint()->yCoordinate();
            qreal secondX2         = bar->secondJoint()->xCoordinate();
            qreal secondY2         = bar->secondJoint()->yCoordinate();
            qreal secondXComponent = secondX2 - secondX1;
            qreal secondYComponent = secondY2 - secondY1;

            if (std::fabs(firstXComponent) < epsilonMagnitudeSmall)
            {
                firstSlope     = epsilonMagnitudeLarge;
                firstIntercept = firstX1;
            }
            else
            {
                firstSlope     = firstYComponent / firstXComponent;
                firstIntercept = (firstY2 * firstX1 - firstY1 * firstX2) / (-firstXComponent);
            }

            if (std::fabs(secondXComponent) < epsilonMagnitudeSmall)
            {
                secondSlope     = epsilonMagnitudeLarge;
                secondIntercept = secondX1;
            }
            else
            {
                secondSlope     = secondYComponent / secondXComponent;
                secondIntercept = (secondY2 * secondX1 - secondY1 * secondX2) / (-secondXComponent);
            }

            qreal magnitudeSlopeDifference     = std::fabs(firstSlope - secondSlope);
            qreal magnitudeInterceptDifference = fabs(firstIntercept - secondIntercept);

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

            position         = std::min_element(xCoordinates.begin(), xCoordinates.end());
            qreal xMin       = *position;
            position         = std::max_element(xCoordinates.begin(), xCoordinates.end());
            qreal xMax       = *position;
            position         = std::min_element(yCoordinates.begin(), yCoordinates.end());
            qreal yMin       = *position;
            position         = std::max_element(yCoordinates.begin(), yCoordinates.end());
            qreal yMax       = *position;
            qreal xComponent = xMax - xMin;
            qreal yComponent = yMax - yMin;
            qreal length     = std::sqrt(std::pow(xComponent, 2.0) + std::pow(yComponent, 2.0));

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
            int number = mJointsList->indexOf(joint) + 1;
            ui->toJointComboBox->addItem(QString::number(number));
            ui->toJointComboBox->setCurrentIndex(-1);
        }
    }
}

void EditBarsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
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

    if (selectedBars.size() == 0)
    {
        ui->fromJointComboBox->setCurrentIndex(-1);
        ui->fromJointComboBox->setEnabled(false);
        ui->toJointComboBox->clear();
        ui->toJointComboBox->setEnabled(false);

        if (mBarsTableView->model()->columnCount() == 3)
        {
            ui->factorDoubleSpinBox->clear();
            ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());
            ui->factorDoubleSpinBox->setEnabled(false);
        }
        else
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
        }
    }
    else
    {
        if (mBarsTableView->model()->columnCount() == 3)
        {
            if (!ui->factorDoubleSpinBox->isEnabled())
            {
                ui->factorDoubleSpinBox->setEnabled(true);
            }
        }
        else
        {
            if (!ui->areaDoubleSpinBox->isEnabled())
            {
                ui->areaDoubleSpinBox->setEnabled(true);
                ui->modulusDoubleSpinBox->setEnabled(true);
                ui->unitWeightDoubleSpinBox->setEnabled(true);
            }
        }

        if (selectedBars.size() == 1)
        {
            if (!ui->fromJointComboBox->isEnabled())
            {
                ui->fromJointComboBox->setEnabled(true);
                ui->toJointComboBox->setEnabled(true);
            }

            int barIndex     = selectedBars.first() - 1;
            Bar *bar         = mBarsList->at(barIndex);
            qreal area       = bar->area();
            qreal modulus    = bar->modulus();
            qreal factor     = bar->factor();
            qreal unitWeight = bar->unitWeight();

            int firstJointNumber  = mJointsList->indexOf(bar->firstJoint()) + 1;
            int secondJointNumber = mJointsList->indexOf(bar->secondJoint()) + 1;

            int index = ui->fromJointComboBox->findText(QString::number(firstJointNumber));
            ui->fromJointComboBox->setCurrentIndex(index);

            ui->toJointComboBox->clear();
            ui->toJointComboBox->addItem(QString::number(secondJointNumber));
            ui->toJointComboBox->setCurrentIndex(0);

            if (mBarsTableView->model()->columnCount() == 3)
            {
                ui->factorDoubleSpinBox->clear();
                ui->factorDoubleSpinBox->setValue(factor);
            }
            else
            {
                ui->areaDoubleSpinBox->clear();
                ui->areaDoubleSpinBox->setValue(area);
                ui->modulusDoubleSpinBox->clear();
                ui->modulusDoubleSpinBox->setValue(modulus);
                ui->unitWeightDoubleSpinBox->clear();
                ui->unitWeightDoubleSpinBox->setValue(unitWeight);
            }
        }
        else
        {
            if (ui->fromJointComboBox->isEnabled())
            {
                ui->fromJointComboBox->setCurrentIndex(-1);
                ui->fromJointComboBox->setEnabled(false);
                ui->toJointComboBox->clear();
                ui->toJointComboBox->setEnabled(false);
            }

            if (mBarsTableView->model()->columnCount() == 3)
            {
                ui->factorDoubleSpinBox->clear();
                ui->factorDoubleSpinBox->setValue(mUnitsAndLimits->minFactor());
            }
            else
            {
                ui->areaDoubleSpinBox->clear();
                ui->areaDoubleSpinBox->setValue(mUnitsAndLimits->minArea());
                ui->modulusDoubleSpinBox->clear();
                ui->modulusDoubleSpinBox->setValue(mUnitsAndLimits->minModulus());
                ui->unitWeightDoubleSpinBox->clear();
                ui->unitWeightDoubleSpinBox->setValue(mUnitsAndLimits->minUnitWeight());
            }
        }
    }

    ui->selectedBarsTextEdit->clear();

    foreach (int barNumber, selectedBars)
    {
        ui->selectedBarsTextEdit->append(QString::number(barNumber));
    }
}

void EditBarsDialog::editBars()
{
    QItemSelectionModel *itemSelectionModel = mBarsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No bar selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int firstJointNumber  = 0;
    int secondJointNumber = 0;

    if (ui->fromJointComboBox->isEnabled())
    {
        firstJointNumber = ui->fromJointComboBox->currentText().toInt();

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
            secondJointNumber = ui->toJointComboBox->currentText().toInt();
        }
    }

    qreal area       = 0.0;
    qreal modulus    = 0.0;
    qreal factor     = 0.0;
    qreal unitWeight = 0.0;

    if (ui->areaDoubleSpinBox->isEnabled())
    {
        area       = ui->areaDoubleSpinBox->value();
        modulus    = ui->modulusDoubleSpinBox->value();
        unitWeight = ui->unitWeightDoubleSpinBox->value();
    }
    else
    {
        if (ui->factorDoubleSpinBox->isEnabled())
        {
            factor = ui->factorDoubleSpinBox->value();
        }
    }

    emit inputsSignal(firstJointNumber,
                      secondJointNumber,
                      area,
                      modulus,
                      factor,
                      unitWeight);
}

void EditBarsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
