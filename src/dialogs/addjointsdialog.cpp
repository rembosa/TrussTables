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

/* addjointsdialog.cpp */

#include "addjointsdialog.h"
#include "ui_addjointsdialog.h"

AddJointsDialog::AddJointsDialog(UnitsAndLimits *unitsAndLimits,
                                 QList<Joint *> *jointsList,
                                 QTableView     *jointsTableView,
                                 QWidget        *parent) : QDialog(parent), ui(new Ui::AddJointsDialog)
{
    ui->setupUi(this);

    ui->mirrorAxisComboBox->addItem(tr("Horizontal"));
    ui->mirrorAxisComboBox->addItem(tr("Vertical"));
    ui->mirrorAxisComboBox->setCurrentIndex(-1);

    mUnitsAndLimits  = unitsAndLimits;
    mJointsList      = jointsList;
    mJointsTableView = jointsTableView;
    mPoint           = QPoint(0, 0);
    updateUnits();
    setDisableInput();

    connect(ui->addJointsPushButton, SIGNAL(clicked()), this, SLOT(addJoints()));
}

AddJointsDialog::~AddJointsDialog()
{
    delete ui;
}

QPoint AddJointsDialog::position() const
{
    return mPoint;
}

void AddJointsDialog::updateJoints()
{
    ui->mirrorJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->mirrorJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->mirrorAxisComboBox->setCurrentIndex(-1);
    ui->mirrorJointComboBox->setCurrentIndex(-1);
}

void AddJointsDialog::updateUnits()
{
    ui->addJointsDeltaXLabel->setText(tr("%1x (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));
    ui->addJointsDeltaYLabel->setText(tr("%1y (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));

    ui->addJointsDeltaXDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                    "Min : %1 (%3)\n"
                                                    "Max :  %2 (%3)")
                                                 .arg(QString::number(-mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                                 .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                                 .arg(mUnitsAndLimits->coordinateUnit()));

    ui->addJointsDeltaYDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                    "Min : %1 (%3)\n"
                                                    "Max :  %2 (%3)")
                                                 .arg(QString::number(-mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                                 .arg(QString::number(mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                                 .arg(mUnitsAndLimits->coordinateUnit()));

    ui->addJointsDeltaXDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxXCoordinate());
    ui->addJointsDeltaXDoubleSpinBox->setMaximum(mUnitsAndLimits->maxXCoordinate());
    ui->addJointsDeltaXDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaXDecimals());
    ui->addJointsDeltaXDoubleSpinBox->setValue(0.0);

    ui->addJointsDeltaYDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxYCoordinate());
    ui->addJointsDeltaYDoubleSpinBox->setMaximum(mUnitsAndLimits->maxYCoordinate());
    ui->addJointsDeltaYDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaYDecimals());
    ui->addJointsDeltaYDoubleSpinBox->setValue(0.0);
}

void AddJointsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
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

void AddJointsDialog::setDisableInput()
{

    ui->addJointsDeltaXDoubleSpinBox->clear();
    ui->addJointsDeltaXDoubleSpinBox->setValue(0.0);
    ui->addJointsDeltaYDoubleSpinBox->clear();
    ui->addJointsDeltaYDoubleSpinBox->setValue(0.0);
    ui->jointsCountSpinBox->clear();
    ui->jointsCountSpinBox->setValue(1);
    ui->addJointsDeltaXDoubleSpinBox->setEnabled(false);
    ui->addJointsDeltaYDoubleSpinBox->setEnabled(false);
    ui->jointsCountSpinBox->setEnabled(false);
}

void AddJointsDialog::setEnableInput()
{
    ui->addJointsDeltaXDoubleSpinBox->setEnabled(true);
    ui->addJointsDeltaYDoubleSpinBox->setEnabled(true);
    ui->jointsCountSpinBox->setEnabled(true);
}

void AddJointsDialog::addJoints()
{
    if (ui->optionTabWidget->currentIndex() == 0)
    {
        qreal deltaX = 0.0;
        qreal deltaY = 0.0;
        int count    = 0;

        if (mJointsList->isEmpty())
        {
            emit inputsSignal(deltaX, deltaY, count);
            ui->addJointsDeltaXDoubleSpinBox->setFocus();
            ui->addJointsDeltaXDoubleSpinBox->selectAll();
            return;
        }

        deltaX = ui->addJointsDeltaXDoubleSpinBox->value();
        deltaY = ui->addJointsDeltaYDoubleSpinBox->value();
        count  = ui->jointsCountSpinBox->value();

        qreal deltaLength = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

        bool checkMin = mUnitsAndLimits->minBarLength() < deltaLength;
        bool checkMax = deltaLength < mUnitsAndLimits->maxXCoordinate();

        if (!checkMin || !checkMax)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("%1x and/or %1y : Input error").arg(QString::fromUtf8("\u0394")));
            QString infoText = QString("Invalid input\n"
                                       "Entered : %1 %4\n"
                                       "Valid range\n"
                                       "Min : > %2 %4\n"
                                       "Max : < %3 %4")
                    .arg(QString::number(deltaLength, 'g', 3))
                    .arg(QString::number(mUnitsAndLimits->minBarLength(), 'g', 3))
                    .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                    .arg(mUnitsAndLimits->coordinateUnit());

            messageBox.setInformativeText(infoText);
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            ui->addJointsDeltaXDoubleSpinBox->setFocus();
            ui->addJointsDeltaXDoubleSpinBox->selectAll();
            return;
        }

        emit inputsSignal(deltaX, deltaY, count);

        ui->addJointsDeltaXDoubleSpinBox->setFocus();
        ui->addJointsDeltaXDoubleSpinBox->selectAll();
        ui->jointsCountSpinBox->clear();
        ui->jointsCountSpinBox->setValue(1);
    }
    else
    {
        QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

        if (!itemSelectionModel->hasSelection())
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Joints to Mirror : Input error"));
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
}

void AddJointsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
