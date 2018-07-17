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

/* deletethermaleffectsdialog.cpp */

#include "deletethermaleffectsdialog.h"
#include "ui_deletethermaleffectsdialog.h"

DeleteThermalEffectsDialog::DeleteThermalEffectsDialog(QList<Bar *>           *barsList,
                                                       QList<ThermalEffect *> *thermalEffectsList,
                                                       QTableView             *thermalEffectsTableView,
                                                       QWidget                *parent) : QDialog(parent),
    ui(new Ui::DeleteThermalEffectsDialog)
{
    ui->setupUi(this);
    mPoint                   = QPoint(0, 0);
    mBarsList                = barsList;
    mThermalEffectsList      = thermalEffectsList;
    mThermalEffectsTableView = thermalEffectsTableView;

    connect(ui->deleteThermalEffectsPushButton, SIGNAL(clicked()), this, SLOT(deleteThermalEffects()));
}

DeleteThermalEffectsDialog::~DeleteThermalEffectsDialog()
{
    delete ui;
}

QPoint DeleteThermalEffectsDialog::position() const
{
    return mPoint;
}

void DeleteThermalEffectsDialog::setSelectedThermalEffects(const QItemSelection &selected,
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

void DeleteThermalEffectsDialog::deleteThermalEffects()
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

    QMessageBox messageBox(this);
    messageBox.setWindowTitle(tr("Thermal Effect Deletion"));
    messageBox.setText(tr("Confirm Thermal Effect(s) Deletion!"));
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    if (messageBox.exec() == QMessageBox::Cancel)
    {
        return;
    }

    emit inputsSignal();
}

void DeleteThermalEffectsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
