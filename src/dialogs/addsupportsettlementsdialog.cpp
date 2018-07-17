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

/* addsupportsettlementsdialog.cpp */

#include "addsupportsettlementsdialog.h"
#include "ui_addsupportsettlementsdialog.h"

AddSupportSettlementsDialog::AddSupportSettlementsDialog(UnitsAndLimits   *unitsAndLimits,
                                                         QList<Joint *>   *jointsList,
                                                         QList<Support *> *supportsList,
                                                         QList<Support *> *redundantSupportsList,
                                                         QWidget          *parent) : QDialog(parent),
    ui(new Ui::AddSupportSettlementsDialog)
{
    ui->setupUi(this);
    mPoint                 = QPoint(0, 0);
    mUnitsAndLimits        = unitsAndLimits;
    mJointsList            = jointsList;
    mSupportsList          = supportsList;
    mRedundantSupportsList = redundantSupportsList;
    updateUnits();

    connect(ui->addSupportSettlementsPushButton, SIGNAL(clicked()), this, SLOT(addSupportSettlements()));
}

AddSupportSettlementsDialog::~AddSupportSettlementsDialog()
{
    delete ui;
}

QPoint AddSupportSettlementsDialog::position() const
{
    return mPoint;
}

void AddSupportSettlementsDialog::updateSupports()
{
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
                Joint *joint = support->supportJoint();
                int jointNumber = mJointsList->indexOf(joint) + 1;
                ui->settlementSupportComboBox->addItem(QString::number(supportNumber) + " @ joint "
                                                       + QString::number(jointNumber));
            }
        }
    }

    ui->settlementSupportComboBox->setCurrentIndex(-1);
}

void AddSupportSettlementsDialog::updateUnits()
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

    ui->settlementValueDoubleSpinBox->setDecimals(mUnitsAndLimits->supportSettlementDecimals());
    ui->settlementValueDoubleSpinBox->setMinimum(mUnitsAndLimits->minSupportSettlement());
    ui->settlementValueDoubleSpinBox->setMaximum(mUnitsAndLimits->maxSupportSettlement());
    ui->settlementValueDoubleSpinBox->clear();
    ui->settlementValueDoubleSpinBox->setValue(mUnitsAndLimits->minSupportSettlement());
}

void AddSupportSettlementsDialog::addSupportSettlements()
{
    int supportNumber = 0;
    qreal settlement  = 0.0;

    if (ui->settlementSupportComboBox->currentIndex() < 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Support : Input error"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        QString currentText(ui->settlementSupportComboBox->currentText());
        supportNumber = currentText.split('@').first().toInt();
    }

    settlement = ui->settlementValueDoubleSpinBox->value();

    emit inputsSignal(supportNumber, settlement);
}

void AddSupportSettlementsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
