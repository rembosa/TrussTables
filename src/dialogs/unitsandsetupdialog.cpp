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

/* unitsandsetupdialog.cpp */

#include "ui_unitsandsetupdialog.h"
#include "unitsandsetupdialog.h"

UnitsAndSetupDialog::UnitsAndSetupDialog(QString        *descriptionText,
                                         UnitsAndLimits *unitsAndLimits,
                                         QWidget        *parent) : QDialog(parent),
    ui(new Ui::UnitsAndSetupDialog)
{
    ui->setupUi(this);
    mDescriptionText = descriptionText;
    ui->descriptionPlainTextEdit->setPlainText(*mDescriptionText);
    ui->settlementLabel->setText(tr("%1S").arg(QString::fromUtf8("\u0394")));
    ui->temperatureChangeLabel->setText(tr("%1T").arg(QString::fromUtf8("\u0394")));
    ui->thermalCoefficientLabel->setText(tr("%1").arg(QString::fromUtf8("\u03B1")));
    ui->lengthErrorLabel->setText(tr("%1L").arg(QString::fromUtf8("\u0394")));
    mUnitsAndLimits = unitsAndLimits;

    if (mUnitsAndLimits->system() == tr("metric"))
    {
        setMetricUnits();
        ui->metricRadioButton->setChecked(true);
    }
    else
    {
        setImperialUnits();
        ui->imperialRadioButton->setChecked(true);
    }

    int index = ui->coordinatesComboBox->findText(mUnitsAndLimits->coordinateUnit());
    ui->coordinatesComboBox->setCurrentIndex(index);
    index = ui->areaComboBox->findText(mUnitsAndLimits->areaUnit());
    ui->areaComboBox->setCurrentIndex(index);
    index = ui->modulusComboBox->findText(mUnitsAndLimits->modulusUnit());
    ui->modulusComboBox->setCurrentIndex(index);
    index = ui->unitWeightComboBox->findText(mUnitsAndLimits->unitWeightUnit());
    ui->unitWeightComboBox->setCurrentIndex(index);
    index = ui->loadComboBox->findText(mUnitsAndLimits->loadUnit());
    ui->loadComboBox->setCurrentIndex(index);
    index = ui->settlementComboBox->findText(mUnitsAndLimits->supportSettlementUnit());
    ui->settlementComboBox->setCurrentIndex(index);
    index = ui->temperatureChangeComboBox->findText(mUnitsAndLimits->temperatureChangeUnit());
    ui->temperatureChangeComboBox->setCurrentIndex(index);
    index = ui->thermalCoefficientComboBox->findText(mUnitsAndLimits->thermalCoefficientUnit());
    ui->thermalCoefficientComboBox->setCurrentIndex(index);
    index = ui->lengthErrorComboBox->findText(mUnitsAndLimits->lengthErrorUnit());
    ui->lengthErrorComboBox->setCurrentIndex(index);

    connect(ui->metricRadioButton, SIGNAL(toggled(bool)), this, SLOT(onSystemOptionChanged(bool)));
    connect(ui->temperatureChangeComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(setThermalCoefficientUnits(int)));
    connect(ui->thermalCoefficientComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(setTemperatureChangeUnits(int)));
}

UnitsAndSetupDialog::~UnitsAndSetupDialog()
{
    delete ui;
}

void UnitsAndSetupDialog::setMetricUnits()
{
    ui->coordinatesComboBox->clear();
    ui->coordinatesComboBox->addItem(tr("m"));
    ui->coordinatesComboBox->addItem(tr("cm"));
    ui->coordinatesComboBox->addItem(tr("mm"));
    ui->coordinatesComboBox->setCurrentIndex(0);

    ui->areaComboBox->clear();
    ui->areaComboBox->addItem(tr("m%1").arg(QString::fromUtf8("\u00B2")));
    ui->areaComboBox->addItem(tr("cm%1").arg(QString::fromUtf8("\u00B2")));
    ui->areaComboBox->addItem(tr("mm%1").arg(QString::fromUtf8("\u00B2")));
    ui->areaComboBox->setCurrentIndex(2);

    ui->modulusComboBox->clear();
    ui->modulusComboBox->addItem(tr("N/m%1").arg(QString::fromUtf8("\u00B2")));
    ui->modulusComboBox->addItem(tr("N/mm%1").arg(QString::fromUtf8("\u00B2")));
    ui->modulusComboBox->addItem(tr("GPa"));
    ui->modulusComboBox->setCurrentIndex(2);

    ui->unitWeightComboBox->clear();
    ui->unitWeightComboBox->addItem(tr("kN/m%1").arg(QString::fromUtf8("\u00B3")));
    ui->unitWeightComboBox->setCurrentIndex(0);

    ui->loadComboBox->clear();
    ui->loadComboBox->addItem(tr("N"));
    ui->loadComboBox->addItem(tr("kN"));
    ui->loadComboBox->setCurrentIndex(1);

    ui->settlementComboBox->clear();
    ui->settlementComboBox->addItem(tr("m"));
    ui->settlementComboBox->addItem(tr("mm"));
    ui->settlementComboBox->setCurrentIndex(1);

    ui->temperatureChangeComboBox->clear();
    ui->temperatureChangeComboBox->addItem(tr("%1C").arg(QString::fromUtf8("\u00B0")));
    ui->temperatureChangeComboBox->addItem(tr("%1F").arg(QString::fromUtf8("\u00B0")));
    ui->temperatureChangeComboBox->setCurrentIndex(0);

    ui->thermalCoefficientComboBox->clear();
    ui->thermalCoefficientComboBox->addItem(tr("/%1C").arg(QString::fromUtf8("\u00B0")));
    ui->thermalCoefficientComboBox->addItem(tr("/%1F").arg(QString::fromUtf8("\u00B0")));
    ui->thermalCoefficientComboBox->setCurrentIndex(0);

    ui->lengthErrorComboBox->clear();
    ui->lengthErrorComboBox->addItem(tr("m"));
    ui->lengthErrorComboBox->addItem(tr("mm"));
    ui->lengthErrorComboBox->setCurrentIndex(1);
}

void UnitsAndSetupDialog::setImperialUnits()
{
    ui->coordinatesComboBox->clear();
    ui->coordinatesComboBox->addItem(tr("ft"));
    ui->coordinatesComboBox->addItem(tr("in."));
    ui->coordinatesComboBox->setCurrentIndex(0);

    ui->areaComboBox->clear();
    ui->areaComboBox->addItem(tr("ft%1").arg(QString::fromUtf8("\u00B2")));
    ui->areaComboBox->addItem(tr("in.%1").arg(QString::fromUtf8("\u00B2")));
    ui->areaComboBox->setCurrentIndex(1);

    ui->modulusComboBox->clear();
    ui->modulusComboBox->addItem(tr("lb/ft%1").arg(QString::fromUtf8("\u00B2")));
    ui->modulusComboBox->addItem(tr("lb/in.%1").arg(QString::fromUtf8("\u00B2")));
    ui->modulusComboBox->addItem(tr("kip/in.%1").arg(QString::fromUtf8("\u00B2")));
    ui->modulusComboBox->setCurrentIndex(1);

    ui->unitWeightComboBox->clear();
    ui->unitWeightComboBox->addItem(tr("lb/ft%1").arg(QString::fromUtf8("\u00B3")));
    ui->unitWeightComboBox->setCurrentIndex(0);

    ui->loadComboBox->clear();
    ui->loadComboBox->addItem(tr("lb"));
    ui->loadComboBox->addItem(tr("kip"));
    ui->loadComboBox->setCurrentIndex(1);

    ui->settlementComboBox->clear();
    ui->settlementComboBox->addItem(tr("ft"));
    ui->settlementComboBox->addItem(tr("in."));
    ui->settlementComboBox->setCurrentIndex(1);

    ui->temperatureChangeComboBox->clear();
    ui->temperatureChangeComboBox->addItem(tr("%1C").arg(QString::fromUtf8("\u00B0")));
    ui->temperatureChangeComboBox->addItem(tr("%1F").arg(QString::fromUtf8("\u00B0")));
    ui->temperatureChangeComboBox->setCurrentIndex(1);

    ui->thermalCoefficientComboBox->clear();
    ui->thermalCoefficientComboBox->addItem(tr("/%1C").arg(QString::fromUtf8("\u00B0")));
    ui->thermalCoefficientComboBox->addItem(tr("/%1F").arg(QString::fromUtf8("\u00B0")));
    ui->thermalCoefficientComboBox->setCurrentIndex(1);

    ui->lengthErrorComboBox->clear();
    ui->lengthErrorComboBox->addItem(tr("ft"));
    ui->lengthErrorComboBox->addItem(tr("in."));
    ui->lengthErrorComboBox->setCurrentIndex(1);
}

void UnitsAndSetupDialog::onSystemOptionChanged(bool checked)
{
    if (checked)
    {
        setMetricUnits();
    }
    else
    {
        setImperialUnits();
    }
}

void UnitsAndSetupDialog::setThermalCoefficientUnits(int index)
{
    ui->thermalCoefficientComboBox->setCurrentIndex(index);
}

void UnitsAndSetupDialog::setTemperatureChangeUnits(int index)
{
    ui->temperatureChangeComboBox->setCurrentIndex(index);
}

void UnitsAndSetupDialog::accept()
{
    if (ui->metricRadioButton->isChecked())
    {
        mUnitsAndLimits->setSystem(tr("metric"));
    }
    else
    {
        mUnitsAndLimits->setSystem(tr("imperial"));
    }

    mUnitsAndLimits->setCoordinateUnit(ui->coordinatesComboBox->currentText());
    mUnitsAndLimits->setAreaUnit(ui->areaComboBox->currentText());
    mUnitsAndLimits->setModulusUnit(ui->modulusComboBox->currentText());
    mUnitsAndLimits->setUnitWeightUnit(ui->unitWeightComboBox->currentText());
    mUnitsAndLimits->setLoadUnit(ui->loadComboBox->currentText());
    mUnitsAndLimits->setSupportSettlementUnit(ui->settlementComboBox->currentText());
    mUnitsAndLimits->setTemperatureChangeUnit(ui->temperatureChangeComboBox->currentText());
    mUnitsAndLimits->setThermalCoefficientUnit(ui->thermalCoefficientComboBox->currentText());
    mUnitsAndLimits->setLengthErrorUnit(ui->lengthErrorComboBox->currentText());
    *mDescriptionText = ui->descriptionPlainTextEdit->toPlainText();
    QDialog::accept();
}
