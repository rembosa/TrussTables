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

/* scaleforcesdialog.cpp */

#include "scaleforcesdialog.h"
#include "ui_scaleforcesdialog.h"

ScaleForcesDialog::ScaleForcesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScaleForcesDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);

    ui->factorDoubleSpinBox->setToolTip(tr("Valid range\n"
                                           "Min : %1\n"
                                           "Max : %2")
                                        .arg(QString::number(ui->factorDoubleSpinBox->minimum(), 'f', 2))
                                        .arg(QString::number(ui->factorDoubleSpinBox->maximum(), 'f', 2)));

    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(onFactorChanged()));
}

ScaleForcesDialog::~ScaleForcesDialog()
{
    delete ui;
}

QPoint ScaleForcesDialog::position() const
{
    return mPoint;
}

qreal ScaleForcesDialog::forceFactor()
{
    return ui->factorDoubleSpinBox->value();
}

void ScaleForcesDialog::setForceFactor(qreal factor)
{
    ui->factorDoubleSpinBox->setValue(factor);
}

void ScaleForcesDialog::onFactorChanged()
{
    emit factorSignal(ui->factorDoubleSpinBox->value());
}

void ScaleForcesDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
