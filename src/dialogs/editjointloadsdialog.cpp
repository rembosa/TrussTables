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

/* editjointloadsdialog.cpp */

#include "editjointloadsdialog.h"
#include "ui_editjointloadsdialog.h"

EditJointLoadsDialog::EditJointLoadsDialog(UnitsAndLimits     *unitsAndLimits,
                                           QList<Joint *>     *jointsList,
                                           QList<JointLoad *> *jointLoadsList,
                                           QTableView         *jointLoadsTableView,
                                           QWidget            *parent) : QDialog(parent),
    ui(new Ui::EditJointLoadsDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);

    ui->positionComboBox->addItem(tr("head"));
    ui->positionComboBox->addItem(tr("tail"));
    ui->positionComboBox->setCurrentIndex(-1);

    mUnitsAndLimits      = unitsAndLimits;
    mJointsList          = jointsList;
    mJointLoadsList      = jointLoadsList;
    mJointLoadsTableView = jointLoadsTableView;
    updateUnits();

    connect(ui->editJointLoadsPushButton, SIGNAL(clicked()), this, SLOT(editJointLoads()));
}

EditJointLoadsDialog::~EditJointLoadsDialog()
{
    delete ui;
}

QPoint EditJointLoadsDialog::position() const
{
    return mPoint;
}

void EditJointLoadsDialog::updateJoints()
{
    ui->loadJointComboBox->setEnabled(true);
    ui->loadJointComboBox->clear();

    foreach (Joint *joint, *mJointsList)
    {
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->loadJointComboBox->addItem(QString::number(jointNumber));
    }

    ui->loadJointComboBox->setCurrentIndex(-1);
    ui->loadJointComboBox->setEnabled(false);
}

void EditJointLoadsDialog::updateUnits()
{
    ui->horizontalComponentLabel->setText(tr("H (%1)").arg(mUnitsAndLimits->loadUnit()));
    ui->verticalComponentLabel->setText(tr("V (%1)").arg(mUnitsAndLimits->loadUnit()));

    ui->horizontalComponentDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                        "Min : %1 (%3)\n"
                                                        "Max :  %2 (%3)")
                                                     .arg(QString::number(-mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                          , 3))
                                                     .arg(QString::number(+mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                          , 3))
                                                     .arg(mUnitsAndLimits->loadUnit()));

    ui->verticalComponentDoubleSpinBox->setToolTip(tr("Valid range\n"
                                                      "Min : %1 (%3)\n"
                                                      "Max :  %2 (%3)")
                                                   .arg(QString::number(-mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                        , 3))
                                                   .arg(QString::number(+mUnitsAndLimits->maxJointLoadComponent(), 'g'
                                                                        , 3))
                                                   .arg(mUnitsAndLimits->loadUnit()));

    ui->horizontalComponentDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxJointLoadComponent());
    ui->horizontalComponentDoubleSpinBox->setMaximum(+mUnitsAndLimits->maxJointLoadComponent());
    ui->horizontalComponentDoubleSpinBox->setDecimals(mUnitsAndLimits->jointLoadDecimals());
    ui->horizontalComponentDoubleSpinBox->clear();
    ui->horizontalComponentDoubleSpinBox->setValue(0.0);

    ui->verticalComponentDoubleSpinBox->setMinimum(-mUnitsAndLimits->maxJointLoadComponent());
    ui->verticalComponentDoubleSpinBox->setMaximum(+mUnitsAndLimits->maxJointLoadComponent());
    ui->verticalComponentDoubleSpinBox->setDecimals(mUnitsAndLimits->jointLoadDecimals());
    ui->verticalComponentDoubleSpinBox->clear();
    ui->verticalComponentDoubleSpinBox->setValue(0.0);
}

void EditJointLoadsDialog::setSelectedJointLoads(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mJointLoadsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedJointLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedJointLoads.append(index.row() + 1);
    }

    if (selectedJointLoads.size() == 1)
    {
        if (!ui->loadJointComboBox->isEnabled())
        {
            ui->loadJointComboBox->setEnabled(true);
        }

        JointLoad *load     = mJointLoadsList->at(selectedJointLoads.first() - 1);
        Joint *loadJoint    = load->loadJoint();
        int loadJointNumber = mJointsList->indexOf(loadJoint) + 1;
        int index           = ui->loadJointComboBox->findText(QString::number(loadJointNumber));
        ui->loadJointComboBox->setCurrentIndex(index);

        ui->horizontalComponentDoubleSpinBox->clear();
        ui->horizontalComponentDoubleSpinBox->setValue(load->horizontalComponent());
        ui->verticalComponentDoubleSpinBox->clear();
        ui->verticalComponentDoubleSpinBox->setValue(load->verticalComponent());

        if (load->position() == QString("head"))
        {
            ui->positionComboBox->setCurrentIndex(0);
        }
        else
        {
            ui->positionComboBox->setCurrentIndex(1);
        }
    }
    else
    {
        if (!ui->loadJointComboBox->isEnabled())
        {
            ui->loadJointComboBox->setEnabled(true);
        }

        ui->loadJointComboBox->setCurrentIndex(-1);
        ui->horizontalComponentDoubleSpinBox->clear();
        ui->horizontalComponentDoubleSpinBox->setValue(0.0);
        ui->verticalComponentDoubleSpinBox->clear();
        ui->verticalComponentDoubleSpinBox->setValue(0.0);
        ui->positionComboBox->setCurrentIndex(-1);

        if (ui->loadJointComboBox->isEnabled())
        {
            ui->loadJointComboBox->setEnabled(false);
        }
    }

    ui->selectedLoadsTextEdit->clear();

    foreach (int loadNumber, selectedJointLoads)
    {
        JointLoad *load = mJointLoadsList->at(loadNumber - 1);
        Joint *joint    = load->loadJoint();
        int jointNumber = mJointsList->indexOf(joint) + 1;
        ui->selectedLoadsTextEdit->append(QString::number(loadNumber) + " @ joint " + QString::number(jointNumber));
    }
}

void EditJointLoadsDialog::editJointLoads()
{
    QItemSelectionModel *itemSelectionModel = mJointLoadsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No load selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int loadJointNumber       = 0;
    qreal horizontalComponent = 0.0;
    qreal verticalComponent   = 0.0;
    QString position;

    if (ui->loadJointComboBox->isEnabled())
    {
        loadJointNumber = ui->loadJointComboBox->currentText().toInt();
    }

    horizontalComponent = ui->horizontalComponentDoubleSpinBox->value();
    verticalComponent   = ui->verticalComponentDoubleSpinBox->value();

    qreal magnitude = std::sqrt(std::pow(horizontalComponent, 2.0) + std::pow(verticalComponent, 2.0));
    bool checkA     = (magnitude < mUnitsAndLimits->minJointLoadResultant());
    bool checkB     = (magnitude > mUnitsAndLimits->maxJointLoadComponent());

    if (checkA || checkB)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Load magnitude : Input error"));
        QString infoString = QString("Invalid input\n"
                                     "Entered : %1 (%4)\n"
                                     "Valid range\n"
                                     "Min : %2 (%4)\n"
                                     "Max : %3 (%4)")
                .arg(QString::number(magnitude, 'g', 3))
                .arg(QString::number(mUnitsAndLimits->minJointLoadResultant(), 'g', 3))
                .arg(QString::number(mUnitsAndLimits->maxJointLoadComponent(), 'g', 3))
                .arg(mUnitsAndLimits->loadUnit());

        messageBox.setInformativeText(infoString);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    if (ui->positionComboBox->currentIndex() < 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Load position : Input error"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        position = ui->positionComboBox->currentText();
    }

    emit inputsSignal(loadJointNumber,
                      horizontalComponent,
                      verticalComponent,
                      position);
}

void EditJointLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
