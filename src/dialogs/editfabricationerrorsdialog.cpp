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

/* editfabricationerrorsdialog.cpp */

#include "editfabricationerrorsdialog.h"
#include "ui_editfabricationerrorsdialog.h"

EditFabricationErrorsDialog::EditFabricationErrorsDialog(UnitsAndLimits            *unitsAndLimits,
                                                         QList<Bar *>              *barsList,
                                                         QList<FabricationError *> *fabricationErrorsList,
                                                         QTableView                *fabricationErrorsTableView,
                                                         QList<Bar *>              *redundantBarsList,
                                                         QWidget                   *parent) : QDialog(parent),
    ui(new Ui::EditFabricationErrorsDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);
    ui->barComboBox->setEnabled(false);
    mUnitsAndLimits             = unitsAndLimits;
    mBarsList                   = barsList;
    mFabricationErrorsList      = fabricationErrorsList;
    mFabricationErrorsTableView = fabricationErrorsTableView;
    mRedundantBarsList          = redundantBarsList;
    updateUnits();

    connect(ui->editFabricationErrorsPushButton, SIGNAL(clicked()), this, SLOT(editFabricationErrors()));
}

EditFabricationErrorsDialog::~EditFabricationErrorsDialog()
{
    delete ui;
}

QPoint EditFabricationErrorsDialog::position() const
{
    return mPoint;
}

void EditFabricationErrorsDialog::updateBars()
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

void EditFabricationErrorsDialog::updateUnits()
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

void EditFabricationErrorsDialog::setSelectedFabricationErrors(const QItemSelection &selected,
                                                               const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mFabricationErrorsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedFabricationErrors;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedFabricationErrors.append(index.row() + 1);
    }

    if (selectedFabricationErrors.size() == 1)
    {
        if (!ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(true);
        }

        FabricationError *fabricationError = mFabricationErrorsList->at(selectedFabricationErrors.first() - 1);
        Bar *bar                           = fabricationError->fabricationErrorBar();
        int barNumber                      = mBarsList->indexOf(bar) + 1;
        int index                          = ui->barComboBox->findText(QString::number(barNumber));
        ui->barComboBox->setCurrentIndex(index);

        if (fabricationError->lengthError() > 0.0)
        {
            ui->overLengthRadioButton->setChecked(true);
            ui->underLengthRadioButton->setChecked(false);
        }
        else
        {
            ui->overLengthRadioButton->setChecked(false);
            ui->underLengthRadioButton->setChecked(true);
        }

        ui->lengthErrorDoubleSpinBox->clear();
        ui->lengthErrorDoubleSpinBox->setValue(std::fabs(fabricationError->lengthError()));
    }
    else
    {
        if (!ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(true);
        }

        ui->barComboBox->setCurrentIndex(-1);
        ui->overLengthRadioButton->setChecked(true);
        ui->underLengthRadioButton->setChecked(false);
        ui->lengthErrorDoubleSpinBox->clear();
        ui->lengthErrorDoubleSpinBox->setValue(mUnitsAndLimits->minLengthError());

        if (ui->barComboBox->isEnabled())
        {
            ui->barComboBox->setEnabled(false);
        }
    }

    ui->selectedFabricationErrorsTextEdit->clear();

    foreach (int fabricationErrorNumber, selectedFabricationErrors)
    {
        FabricationError *fabricationError = mFabricationErrorsList->at(fabricationErrorNumber - 1);
        Bar *bar                           = fabricationError->fabricationErrorBar();
        int barNumber                      = mBarsList->indexOf(bar) + 1;
        ui->selectedFabricationErrorsTextEdit->append(QString::number(fabricationErrorNumber) + " @ bar "
                                                   + QString::number(barNumber));
    }
}

void EditFabricationErrorsDialog::editFabricationErrors()
{
    QItemSelectionModel *itemSelectionModel = mFabricationErrorsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No fabrication error selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int barNumber     = 0;
    qreal lengthError = 0.0;

    if (ui->barComboBox->isEnabled())
    {
        barNumber = ui->barComboBox->currentText().toInt();
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

void EditFabricationErrorsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
