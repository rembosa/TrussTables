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

/* addfabricationerrorsdialog.cpp */

#include "addfabricationerrorsdialog.h"
#include "ui_addfabricationerrorsdialog.h"

AddFabricationErrorsDialog::AddFabricationErrorsDialog(UnitsAndLimits *unitsAndLimits,
                                                       QList<Bar *>   *barsList,
                                                       QList<Bar *>   *redundantBarsList,
                                                       QTableView     *barsTableView,
                                                       QWidget        *parent) : QDialog(parent),
    ui(new Ui::AddFabricationErrorsDialog)
{
    ui->setupUi(this);
    mUnitsAndLimits    = unitsAndLimits;
    mBarsList          = barsList;
    mRedundantBarsList = redundantBarsList;
    mBarsTableView     = barsTableView;
    mPoint             = QPoint(0, 0);
    updateUnits();

    connect(ui->addFabricationErrorPushButton, SIGNAL(clicked()), this, SLOT(addFabricationErrors()));
}

AddFabricationErrorsDialog::~AddFabricationErrorsDialog()
{
    delete ui;
}

QPoint AddFabricationErrorsDialog::position() const
{
    return mPoint;
}

void AddFabricationErrorsDialog::updateBars()
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

void AddFabricationErrorsDialog::updateUnits()
{
    ui->lengthErrorLabel->setText(tr("%1L (%2)")
                                  .arg(QString::fromUtf8("\u0394"))
                                  .arg(mUnitsAndLimits->lengthErrorUnit()));

    ui->lengthErrorDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                "Min : %1 (%3)\n"
                                                "Max : %2 (%3)")
                                             .arg(QString::number(mUnitsAndLimits->minLengthError(), 'g', 3))
                                             .arg(QString::number(mUnitsAndLimits->maxLengthError(), 'g', 3))
                                             .arg(mUnitsAndLimits->lengthErrorUnit()));

    ui->lengthErrorDoubleSpinBox->setDecimals(mUnitsAndLimits->lengthErrorDecimals());
    ui->lengthErrorDoubleSpinBox->setMinimum(mUnitsAndLimits->minLengthError());
    ui->lengthErrorDoubleSpinBox->setMaximum(mUnitsAndLimits->maxLengthError());
    ui->lengthErrorDoubleSpinBox->clear();
    ui->lengthErrorDoubleSpinBox->setValue(mUnitsAndLimits->minLengthError());
}

void AddFabricationErrorsDialog::setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected)
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

void AddFabricationErrorsDialog::addFabricationErrors()
{
    int barNumber     = 0;
    qreal lengthError = 0.0;

    if (ui->barComboBox->isEnabled())
    {
        if (ui->barComboBox->currentIndex() < 0)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Length error bar : Input error"));
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

    if (ui->overLengthRadioButton->isChecked())
    {
        lengthError = ui->lengthErrorDoubleSpinBox->value();
    }
    else
    {
        lengthError = -ui->lengthErrorDoubleSpinBox->value();
    }

    emit inputsSignal(barNumber, lengthError);
}

void AddFabricationErrorsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
