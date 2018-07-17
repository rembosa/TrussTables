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

/* editsupportsettlementsdialog.cpp */

#include "editsupportsettlementsdialog.h"
#include "ui_editsupportsettlementsdialog.h"

EditSupportSettlementsDialog::EditSupportSettlementsDialog(UnitsAndLimits             *unitsAndLimits,
                                                           QList<Joint *>             *jointsList,
                                                           QList<Support *>           *supportsList,
                                                           QList<SupportSettlement *> *supportSettlementsList,
                                                           QTableView                 *supportSettlementsTableView,
                                                           QList<Support *>           *redundantSupportsList,
                                                           QWidget                    *parent) : QDialog(parent),
    ui(new Ui::EditSupportSettlementsDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);
    ui->settlementSupportComboBox->setEnabled(false);
    mUnitsAndLimits              = unitsAndLimits;
    mJointsList                  = jointsList;
    mSupportsList                = supportsList;
    mSupportSettlementsList      = supportSettlementsList;
    mSupportSettlementsTableView = supportSettlementsTableView;
    mRedundantSupportsList       = redundantSupportsList;
    updateUnits();

    connect(ui->editSupportSettlementsPushButton, SIGNAL(clicked()), this, SLOT(editSupportSettlements()));
}

EditSupportSettlementsDialog::~EditSupportSettlementsDialog()
{
    delete ui;
}

QPoint EditSupportSettlementsDialog::position() const
{
    return mPoint;
}

void EditSupportSettlementsDialog::updateSupports()
{
    ui->settlementSupportComboBox->setEnabled(true);
    ui->settlementSupportComboBox->clear();

    foreach (Support *support, *mSupportsList)
    {
        if (mRedundantSupportsList->contains(support))
        {
            bool checkA = (support->type() == UnitsAndLimits::FIXED_BOTTOM);
            bool checkB = (support->type() == UnitsAndLimits::ROLLER_BOTTOM);

            if (checkA || checkB)
            {
                int supportNumber = mSupportsList->indexOf(support) + 1;
                Joint *joint      = support->supportJoint();
                int jointNumber   = mJointsList->indexOf(joint) + 1;
                ui->settlementSupportComboBox->addItem(QString::number(supportNumber) + " @ joint "
                                                       + QString::number(jointNumber));
            }
        }
    }

    ui->settlementSupportComboBox->setCurrentIndex(-1);
    ui->settlementSupportComboBox->setEnabled(false);
}

void EditSupportSettlementsDialog::updateUnits()
{
    ui->settlementValueLabel->setText(tr("%1y (%2)")
                                      .arg(QString::fromUtf8("\u0394"))
                                      .arg(mUnitsAndLimits->supportSettlementUnit()));

    ui->settlementValueDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                    "Min : %1 (%3)\n"
                                                    "Max : %2 (%3)")
                                                 .arg(QString::number(mUnitsAndLimits->minSupportSettlement(), 'g' , 3))
                                                 .arg(QString::number(mUnitsAndLimits->maxSupportSettlement(), 'g' , 3))
                                                 .arg(mUnitsAndLimits->supportSettlementUnit()));

    ui->settlementValueDoubleSpinBox->setMinimum(mUnitsAndLimits->minSupportSettlement());
    ui->settlementValueDoubleSpinBox->setMaximum(mUnitsAndLimits->maxSupportSettlement());
    ui->settlementValueDoubleSpinBox->setDecimals(mUnitsAndLimits->supportSettlementDecimals());
    ui->settlementValueDoubleSpinBox->clear();
    ui->settlementValueDoubleSpinBox->setValue(mUnitsAndLimits->minSupportSettlement());
}

void EditSupportSettlementsDialog::setSelectedSupportSettlements(const QItemSelection &selected,
                                                                 const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mSupportSettlementsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedSupportSettlements;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupportSettlements.append(index.row() + 1);
    }

    if (selectedSupportSettlements.size() == 1)
    {
        if (!ui->settlementSupportComboBox->isEnabled())
        {
            ui->settlementSupportComboBox->setEnabled(true);
        }

        SupportSettlement *supportSettlement = mSupportSettlementsList->at(selectedSupportSettlements.first() - 1);
        Support *support                     = supportSettlement->settlementSupport();
        int supportNumber                    = mSupportsList->indexOf(support) + 1;
        Joint *joint                         = support->supportJoint();
        int jointNumber                      = mJointsList->indexOf(joint) + 1;
        int index                            = ui->settlementSupportComboBox->findText(QString::number(supportNumber)
                                                                                       + " @ joint "
                                                                                       + QString::number(jointNumber));

        ui->settlementSupportComboBox->setCurrentIndex(index);
        ui->settlementValueDoubleSpinBox->clear();
        ui->settlementValueDoubleSpinBox->setValue(supportSettlement->settlement());
    }
    else
    {
        if (!ui->settlementSupportComboBox->isEnabled())
        {
            ui->settlementSupportComboBox->setEnabled(true);
        }

        ui->settlementSupportComboBox->setCurrentIndex(-1);
        ui->settlementValueDoubleSpinBox->clear();
        ui->settlementValueDoubleSpinBox->setValue(mUnitsAndLimits->minSupportSettlement());

        if (ui->settlementSupportComboBox->isEnabled())
        {
            ui->settlementSupportComboBox->setEnabled(false);
        }
    }

    ui->selectedSupportSettlementsTextEdit->clear();

    foreach (int supportSettlementNumber, selectedSupportSettlements)
    {
        SupportSettlement *supportSettlement = mSupportSettlementsList->at(supportSettlementNumber - 1);
        Joint *joint                         = supportSettlement->settlementSupport()->supportJoint();
        int jointNumber                      = mJointsList->indexOf(joint) + 1;
        ui->selectedSupportSettlementsTextEdit->append(QString::number(supportSettlementNumber) + " @ joint "
                                                       + QString::number(jointNumber));
    }
}

void EditSupportSettlementsDialog::editSupportSettlements()
{
    QItemSelectionModel *itemSelectionModel = mSupportSettlementsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No support settlement selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int supportNumber = 0;
    qreal settlement  = 0.0;

    if (ui->settlementSupportComboBox->isEnabled())
    {
        QString currentText(ui->settlementSupportComboBox->currentText());
        supportNumber = currentText.split('@').first().toInt();
    }

    settlement = ui->settlementValueDoubleSpinBox->value();

    emit inputsSignal(supportNumber, settlement);
}

void EditSupportSettlementsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
