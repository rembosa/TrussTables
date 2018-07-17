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

/* scaledeflectionsdialog.cpp */

#include "scaledeflectionsdialog.h"
#include "ui_scaledeflectionsdialog.h"

ScaleDeflectionsDialog::ScaleDeflectionsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ScaleDeflectionsDialog)
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

ScaleDeflectionsDialog::~ScaleDeflectionsDialog()
{
    delete ui;
}

QPoint ScaleDeflectionsDialog::position() const
{
    return mPoint;
}

qreal ScaleDeflectionsDialog::deflectionFactor()
{
    return ui->factorDoubleSpinBox->value();
}

void ScaleDeflectionsDialog::setDeflectionFactor(qreal deflectionFactor)
{
    ui->factorDoubleSpinBox->setValue(deflectionFactor);
}

void ScaleDeflectionsDialog::onFactorChanged()
{
    qreal factor = 10000.0;
    emit factorSignal(ui->factorDoubleSpinBox->value() * factor);
}

void ScaleDeflectionsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
