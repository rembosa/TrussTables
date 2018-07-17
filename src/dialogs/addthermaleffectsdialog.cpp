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

/* addthermaleffectsdialog.cpp */

#include "addthermaleffectsdialog.h"
#include "ui_addthermaleffectsdialog.h"

AddThermalEffectsDialog::AddThermalEffectsDialog(UnitsAndLimits *unitsAndLimits,
                                                 QList<Bar *>   *barsList,
                                                 QList<Bar *>   *redundantBarsList,
                                                 QTableView     *barsTableView,
                                                 QWidget        *parent) : QDialog(parent),
    ui(new Ui::AddThermalEffectsDialog)

{
    ui->setupUi(this);
    mUnitsAndLimits    = unitsAndLimits;
    mBarsList          = barsList;
    mRedundantBarsList = redundantBarsList;
    mBarsTableView     = barsTableView;
    mPoint             = QPoint(0, 0);
    updateUnits();

    connect(ui->addThermalEffectsPushButton, SIGNAL(clicked()), this, SLOT(addThermalEffects()));
}

AddThermalEffectsDialog::~AddThermalEffectsDialog()
{
    delete ui;
}

QPoint AddThermalEffectsDialog::position() const
{
    return mPoint;
}

void AddThermalEffectsDialog::updateBars()
{
    ui->barComboBox->clear();

    foreach (Bar *bar, *mBarsList)
    {
        if (mRedundantBarsList->contains(bar))
        {
            int barNumber = mBarsList->indexOf(bar) + 1;
            ui->barComboBox->addItem(QString::number(barNumber));
        }
    }

    ui->barComboBox->setCurrentIndex(-1);
}

void AddThermalEffectsDialog::updateUnits()
{
    ui->temperatureChangeLabel->setText(tr("%1t (%2)")
                                        .arg(QString::fromUtf8("\u0394"))
                                        .arg(mUnitsAndLimits->temperatureChangeUnit()));
    ui->thermalCoefficientLabel->setText(tr("%1 (%2)")
                                         .arg(QString::fromUtf8("\u03B1"))
                                         .arg(mUnitsAndLimits->thermalCoefficientUnit()));

    ui->temperatureChangeDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                      "Min : %1 (%3)\n"
                                                      "Max : %2 (%3)")
                                                   .arg(QString::number(mUnitsAndLimits->minTemperatureChange(), 'g'
                                                                        , 3))
                                                   .arg(QString::number(mUnitsAndLimits->maxTemperatureChange(), 'g'
                                                                        , 3))
                                                   .arg(mUnitsAndLimits->temperatureChangeUnit()));

    ui->thermalCoefficientDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                       "Min : %1 (%3)\n"
                                                       "Max : %2 (%3)")
                                                    .arg(QString::number(mUnitsAndLimits->minThermalCoefficient(), 'g'
                                                                         , 3))
                                                    .arg(QString::number(mUnitsAndLimits->maxThermalCoefficient(), 'g'
                                                                         , 3))
                                                    .arg(mUnitsAndLimits->thermalCoefficientUnit()));

    ui->temperatureChangeDoubleSpinBox->setDecimals(mUnitsAndLimits->temperatureChangeDecimals());
    ui->temperatureChangeDoubleSpinBox->setMinimum(mUnitsAndLimits->minTemperatureChange());
    ui->temperatureChangeDoubleSpinBox->setMaximum(mUnitsAndLimits->maxTemperatureChange());
    ui->temperatureChangeDoubleSpinBox->clear();
    ui->temperatureChangeDoubleSpinBox->setValue(mUnitsAndLimits->minTemperatureChange());

    ui->thermalCoefficientDoubleSpinBox->setDecimals(mUnitsAndLimits->thermalCoefficientDecimals());
    ui->thermalCoefficientDoubleSpinBox->setMinimum(mUnitsAndLimits->minThermalCoefficient());
    ui->thermalCoefficientDoubleSpinBox->setMaximum(mUnitsAndLimits->maxThermalCoefficient());
    ui->thermalCoefficientDoubleSpinBox->clear();
    ui->thermalCoefficientDoubleSpinBox->setValue(mUnitsAndLimits->minThermalCoefficient());
}

void AddThermalEffectsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
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
        if (!ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(true);
        }
    }
    else
    {
        if (ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setCurrentIndex(-1);
            ui->barComboBox->setEnabled(false);
        }
    }

    ui->selectedBarsTextEdit->clear();

    foreach (int barNumber, selectedBars)
    {
        ui->selectedBarsTextEdit->append(QString::number(barNumber));
    }
}

void AddThermalEffectsDialog::addThermalEffects()
{
    int barNumber            = 0;
    qreal temperatureChange  = 0.0;
    qreal thermalCoefficient = 0.0;

    if (ui->barComboBox->isEnabled())
    {
        if (ui->barComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Thermal effect bar : Input error"));
            messageBox.setInformativeText(tr("No selection!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }
        else
        {
            barNumber = ui->barComboBox->currentText().toInt();
        }
    }

    if (ui->riseRadioButton->isChecked())
    {
        temperatureChange = ui->temperatureChangeDoubleSpinBox->value();
    }
    else
    {
        temperatureChange = -ui->temperatureChangeDoubleSpinBox->value();
    }

    thermalCoefficient = ui->thermalCoefficientDoubleSpinBox->value();

    emit inputsSignal(barNumber,
                      temperatureChange,
                      thermalCoefficient);
}

void AddThermalEffectsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
