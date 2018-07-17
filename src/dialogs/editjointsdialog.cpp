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

/* editjointsdialog.cpp */

#include "editjointsdialog.h"
#include "ui_editjointsdialog.h"

EditJointsDialog::EditJointsDialog(UnitsAndLimits *unitsAndLimits,
                                   QTableView     *jointsTableView,
                                   QWidget        *parent) : QDialog(parent), ui(new Ui::EditJointsDialog)
{
    ui->setupUi(this);
    mUnitsAndLimits  = unitsAndLimits;
    mJointsTableView = jointsTableView;
    mPoint           = QPoint(0, 0);
    updateUnits();

    connect(ui->editJointsPushButton, SIGNAL(clicked()), this, SLOT(editJoints()));
}

EditJointsDialog::~EditJointsDialog()
{
    delete ui;
}

QPoint EditJointsDialog::position() const
{
    return mPoint;
}

void EditJointsDialog::updateUnits()
{
    ui->editJointsDeltaXLabel->setText(tr("%1x (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));
    ui->editJointsDeltaYLabel->setText(tr("%1y (%2)").arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->coordinateUnit()));

    ui->editJointsDeltaXDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                     "Min : %1 (%3)\n"
                                                     "Max :  %2 (%3)")
                                                  .arg(QString::number(-mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                                  .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                                                  .arg(mUnitsAndLimits->coordinateUnit()));

    ui->editJointsDeltaYDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                     "Min : %1 (%3)\n"
                                                     "Max :  %2 (%3)")
                                                  .arg(QString::number(-mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                                  .arg(QString::number(mUnitsAndLimits->maxYCoordinate(), 'g', 3))
                                                  .arg(mUnitsAndLimits->coordinateUnit()));

    ui->editJointsDeltaXDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxXCoordinate());
    ui->editJointsDeltaXDoubleSpinBox->setMaximum(mUnitsAndLimits->maxXCoordinate());
    ui->editJointsDeltaXDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaXDecimals());
    ui->editJointsDeltaXDoubleSpinBox->clear();
    ui->editJointsDeltaXDoubleSpinBox->setValue(0.0);

    ui->editJointsDeltaYDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxYCoordinate());
    ui->editJointsDeltaYDoubleSpinBox->setMaximum(mUnitsAndLimits->maxYCoordinate());
    ui->editJointsDeltaYDoubleSpinBox->setDecimals(mUnitsAndLimits->deltaYDecimals());
    ui->editJointsDeltaYDoubleSpinBox->clear();
    ui->editJointsDeltaYDoubleSpinBox->setValue(0.0);
}

void EditJointsDialog::editJoints()
{
    QItemSelectionModel *itemSelectionModel = mJointsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No joint selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    qreal deltaX = ui->editJointsDeltaXDoubleSpinBox->value();
    qreal deltaY = ui->editJointsDeltaYDoubleSpinBox->value();

    qreal deltaLength = std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));

    bool checkMin = 0.0 < deltaLength;
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
                .arg(QString::number(0.0, 'g', 3))
                .arg(QString::number(mUnitsAndLimits->maxXCoordinate(), 'g', 3))
                .arg(mUnitsAndLimits->coordinateUnit());

        messageBox.setInformativeText(infoText);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        ui->editJointsDeltaXDoubleSpinBox->setFocus();
        ui->editJointsDeltaXDoubleSpinBox->selectAll();
        return;
    }

    emit inputsSignal(deltaX, deltaY);

    ui->editJointsDeltaXDoubleSpinBox->setFocus();
    ui->editJointsDeltaXDoubleSpinBox->selectAll();
}

void EditJointsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
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

void EditJointsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
