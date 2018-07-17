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

/* modelareadialog.cpp */

#include "modelareadialog.h"
#include "ui_modelareadialog.h"

ModelAreaDialog::ModelAreaDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ModelAreaDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);

    connect(ui->marginHorizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(setMarginValueLabelText(int)));
    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(setModelArea()));
}

ModelAreaDialog::~ModelAreaDialog()
{
    delete ui;
}

QPoint ModelAreaDialog::position() const
{
    return mPoint;
}

int ModelAreaDialog::widthFactor()
{
    return ui->widthFactorSpinBox->value();
}

void ModelAreaDialog::setWidthFactor(int widthFactor)
{
    ui->widthFactorSpinBox->setValue(widthFactor);
}

int ModelAreaDialog::heightFactor()
{
    return ui->heightFactorSpinBox->value();
}

void ModelAreaDialog::setHeightFactor(int heightFactor)
{
    ui->heightFactorSpinBox->setValue(heightFactor);
}

int ModelAreaDialog::margin()
{
    return ui->marginHorizontalSlider->value();
}

void ModelAreaDialog::setMargin(int margin)
{
    ui->marginHorizontalSlider->setValue(margin);
}

void ModelAreaDialog::setMarginValueLabelText(int value)
{
    ui->marginValueLabel->setText(tr("%1 px.").arg(QString::number(value)));
}

void ModelAreaDialog::setModelArea()
{
    int w      = ui->widthFactorSpinBox->value();
    int h      = ui->heightFactorSpinBox->value();
    int margin = ui->marginHorizontalSlider->value();

    emit inputsSignal(w,
                      h,
                      margin);
}

void ModelAreaDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
