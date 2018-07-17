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

/* editthermaleffectsdialog.cpp */

#include "editthermaleffectsdialog.h"
#include "ui_editthermaleffectsdialog.h"

EditThermalEffectsDialog::EditThermalEffectsDialog(UnitsAndLimits         *unitsAndLimits,
                                                   QList<Bar *>           *barsList,
                                                   QList<ThermalEffect *> *thermalEffectsList,
                                                   QTableView             *thermalEffectsTableView,
                                                   QList<Bar *>           *redundantBarsList,
                                                   QWidget                *parent) : QDialog(parent),
    ui(new Ui::EditThermalEffectsDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);
    ui->barComboBox->setEnabled(false);
    mUnitsAndLimits          = unitsAndLimits;
    mBarsList                = barsList;
    mThermalEffectsList      = thermalEffectsList;
    mThermalEffectsTableView = thermalEffectsTableView;
    mRedundantBarsList       = redundantBarsList;
    updateUnits();

    connect(ui->editThermalEffectsPushButton, SIGNAL(clicked()), this, SLOT(editThermalEffects()));
}

EditThermalEffectsDialog::~EditThermalEffectsDialog()
{
    delete ui;
}

QPoint EditThermalEffectsDialog::position() const
{
    return mPoint;
}

void EditThermalEffectsDialog::updateBars()
{
    ui->barComboBox->setEnabled(true);
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
    ui->barComboBox->setEnabled(false);
}

void EditThermalEffectsDialog::updateUnits()
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

void EditThermalEffectsDialog::setSelectedThermalEffects(const QItemSelection &selected,
                                                         const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mThermalEffectsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedThermalEffects;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedThermalEffects.append(index.row() + 1);
    }

    if (selectedThermalEffects.size() == 1)
    {
        if (!ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(true);
        }

        ThermalEffect *thermalEffect = mThermalEffectsList->at(selectedThermalEffects.first() - 1);
        Bar *bar                     = thermalEffect->thermalEffectBar();
        int barNumber                = mBarsList->indexOf(bar) + 1;
        int index                    = ui->barComboBox->findText(QString::number(barNumber));
        ui->barComboBox->setCurrentIndex(index);

        if (thermalEffect->temperatureChange() > 0.0)
        {
            ui->riseRadioButton->setChecked(true);
            ui->fallRadioButton->setChecked(false);
        }
        else
        {
            ui->riseRadioButton->setChecked(false);
            ui->fallRadioButton->setChecked(true);
        }

        ui->temperatureChangeDoubleSpinBox->clear();
        ui->temperatureChangeDoubleSpinBox->setValue(std::fabs(thermalEffect->temperatureChange()));
        ui->thermalCoefficientDoubleSpinBox->clear();
        ui->thermalCoefficientDoubleSpinBox->setValue(thermalEffect->thermalCoefficient());
    }
    else
    {
        if (!ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(true);
        }

        ui->barComboBox->setCurrentIndex(-1);
        ui->riseRadioButton->setChecked(true);
        ui->fallRadioButton->setChecked(false);
        ui->temperatureChangeDoubleSpinBox->clear();
        ui->temperatureChangeDoubleSpinBox->setValue(mUnitsAndLimits->minTemperatureChange());
        ui->thermalCoefficientDoubleSpinBox->clear();
        ui->thermalCoefficientDoubleSpinBox->setValue(mUnitsAndLimits->minThermalCoefficient());

        if (ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(false);
        }
    }

    ui->selectedThermalEffectsTextEdit->clear();

    foreach (int thermalEffectNumber, selectedThermalEffects)
    {
        ThermalEffect *thermalEffect = mThermalEffectsList->at(thermalEffectNumber - 1);
        Bar *bar                     = thermalEffect->thermalEffectBar();
        int barNumber                = mBarsList->indexOf(bar) + 1;
        ui->selectedThermalEffectsTextEdit->append(QString::number(thermalEffectNumber) + " @ bar "
                                                   + QString::number(barNumber));
    }
}

void EditThermalEffectsDialog::editThermalEffects()
{
    QItemSelectionModel *itemSelectionModel = mThermalEffectsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No thermal effect selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int barNumber            = 0;
    qreal temperatureChange  = 0.0;
    qreal thermalCoefficient = 0.0;

    if (ui->barComboBox->isEnabled())
    {
        barNumber = ui->barComboBox->currentText().toInt();
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

void EditThermalEffectsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
