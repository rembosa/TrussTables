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

/* editinfluenceloadsdialog.cpp */

#include "editinfluenceloadsdialog.h"
#include "ui_editinfluenceloadsdialog.h"

EditInfluenceLoadsDialog::EditInfluenceLoadsDialog(UnitsAndLimits         *unitsAndLimits,
                                                   QList<InfluenceLoad *> *influenceLoadsList,
                                                   QList<Joint *>         *jointsList,
                                                   QTableView             *jointsTableView,
                                                   QTableView             *influenceLoadsTableView,
                                                   QWidget                *parent) : QDialog(parent),
    ui(new Ui::EditInfluenceLoadsDialog)
{
    ui->setupUi(this);
    ui->nameLineEdit->setEnabled(false);
    mUnitsAndLimits          = unitsAndLimits;
    mInfluenceLoadsList      = influenceLoadsList;
    mJointsList              = jointsList;
    mJointsTableView         = jointsTableView;
    mInfluenceLoadsTableView = influenceLoadsTableView;
    mPoint                   = QPoint(0, 0);
    updateUnits();

    connect(ui->addPointLoadPushButton, SIGNAL(clicked()), this, SLOT(addPointLoad()));
    connect(ui->removePointLoadPushButton, SIGNAL(clicked()), this, SLOT(removePointLoad()));
    connect(ui->editInfluenceLoadsPushButton, SIGNAL(clicked()), this, SLOT(editInfluenceLoads()));
}

EditInfluenceLoadsDialog::~EditInfluenceLoadsDialog()
{
    delete ui;
}

QPoint EditInfluenceLoadsDialog::position() const
{
    return mPoint;
}

void EditInfluenceLoadsDialog::updateUnits()
{
    ui->pointLoadLabel->setText(tr("V (%1)").arg(mUnitsAndLimits->loadUnit()));
    ui->pointLoadDoubleSpinBox->setToolTip(tr("Valid range\n"
                                              "Min : %1 (%3)\n"
                                              "Max : %2 (%3)")
                                           .arg(QString::number(mUnitsAndLimits->minPointLoad(), 'g', 3))
                                           .arg(QString::number(mUnitsAndLimits->maxPointLoad(), 'g', 3))
                                           .arg(mUnitsAndLimits->loadUnit()));

    ui->offsetLabel->setText(tr("Offset (%1)").arg(mUnitsAndLimits->coordinateUnit()));
    ui->offsetDoubleSpinBox->setToolTip(tr("Valid range\n"
                                           "Min : %1 (%3)\n"
                                           "Max : %2 (%3)")
                                        .arg(QString::number(mUnitsAndLimits->minOffset(), 'g', 3))
                                        .arg(QString::number(mUnitsAndLimits->maxOffset(), 'g', 3))
                                        .arg(mUnitsAndLimits->coordinateUnit()));

    ui->pointLoadDoubleSpinBox->setDecimals(mUnitsAndLimits->pointLoadDecimals());
    ui->pointLoadDoubleSpinBox->setMinimum(mUnitsAndLimits->minPointLoad());
    ui->pointLoadDoubleSpinBox->setMaximum(mUnitsAndLimits->maxPointLoad());
    ui->pointLoadDoubleSpinBox->clear();
    ui->pointLoadDoubleSpinBox->setValue(mUnitsAndLimits->minPointLoad());

    ui->offsetDoubleSpinBox->setDecimals(mUnitsAndLimits->offsetDecimals());
    ui->offsetDoubleSpinBox->setMinimum(mUnitsAndLimits->minOffset());
    ui->offsetDoubleSpinBox->setMaximum(mUnitsAndLimits->maxOffset());
    ui->offsetDoubleSpinBox->clear();
    ui->offsetDoubleSpinBox->setValue(mUnitsAndLimits->minOffset());

    ui->pointLoadsListWidget->clear();
    mPointLoads.clear();
    mPointLoadPositions.clear();
}

void EditInfluenceLoadsDialog::setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mInfluenceLoadsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedInfluenceLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedInfluenceLoads.append(index.row() + 1);
    }

    if (selectedInfluenceLoads.size() == 1)
    {
        itemSelectionModel = mJointsTableView->selectionModel();
        modelIndexList     = itemSelectionModel->selectedRows();

        QList<int> selectedJoints;

        foreach (QModelIndex index, modelIndexList)
        {
            selectedJoints.append(index.row() + 1);
        }

        if (selectedJoints.size() > 0)
        {
            mPath = selectedJoints;

            ui->selectedJointsTextEdit->clear();

            foreach (int jointNumber, selectedJoints)
            {
                ui->selectedJointsTextEdit->append(QString::number(jointNumber));
            }
        }
        else
        {
            InfluenceLoad *influenceLoad = mInfluenceLoadsList->at(selectedInfluenceLoads.first() - 1);

            mPath = influenceLoad->path();

            ui->selectedJointsTextEdit->clear();

            int count = influenceLoad->path().size();

            for (int i = 0; i < count; ++i)
            {
                ui->selectedJointsTextEdit->append(QString::number(influenceLoad->path().at(i)));
            }
        }
    }
    else
    {
        mPath.clear();
        ui->selectedJointsTextEdit->clear();
    }
}

void EditInfluenceLoadsDialog::setSelectedInfluenceLoads(const QItemSelection &selected,
                                                         const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mInfluenceLoadsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedInfluenceLoads;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedInfluenceLoads.append(index.row() + 1);
    }

    if (selectedInfluenceLoads.size() == 1)
    {
        if (!ui->nameLineEdit->isEnabled())
        {
            ui->nameLineEdit->setEnabled(true);
        }

        InfluenceLoad *influenceLoad = mInfluenceLoadsList->at(selectedInfluenceLoads.first() - 1);

        mName               = influenceLoad->name();
        mDirection          = influenceLoad->direction();
        mPointLoads         = influenceLoad->pointLoads();
        mPointLoadPositions = influenceLoad->pointLoadPositions();

        ui->nameLineEdit->setText(influenceLoad->name());
        ui->nameLineEdit->selectAll();
        ui->nameLineEdit->setFocus();

        if (influenceLoad->direction() == tr("LR"))
        {
            ui->leftToRightRadioButton->setChecked(true);
            ui->rightToLeftRadioButton->setChecked(false);
        }
        else
        {
            ui->leftToRightRadioButton->setChecked(false);
            ui->rightToLeftRadioButton->setChecked(true);
        }

        ui->pointLoadsListWidget->clear();

        int count = influenceLoad->pointLoads().size();

        for (int i = 0; i < count; ++i)
        {
            qreal pointLoadValue    = influenceLoad->pointLoads().at(i);
            qreal pointLoadPosition = influenceLoad->pointLoadPositions().at(i);
            ui->pointLoadsListWidget->addItem(tr("%1 %2 @ %3 %4")
                                              .arg(QString::number(pointLoadValue, 'f',
                                                                   mUnitsAndLimits->pointLoadDecimals()))
                                              .arg(mUnitsAndLimits->loadUnit())
                                              .arg(QString::number(pointLoadPosition, 'f',
                                                                   mUnitsAndLimits->offsetDecimals()))
                                              .arg(mUnitsAndLimits->coordinateUnit()));
        }

        itemSelectionModel = mJointsTableView->selectionModel();

        if (itemSelectionModel->hasSelection())
        {
            modelIndexList = itemSelectionModel->selectedRows();

            QList<int> selectedJoints;

            foreach (QModelIndex index, modelIndexList)
            {
                selectedJoints.append(index.row() + 1);
            }

            mPath = selectedJoints;

            ui->selectedJointsTextEdit->clear();

            foreach (int jointNumber, selectedJoints)
            {
                ui->selectedJointsTextEdit->append(QString::number(jointNumber));
            }
        }
        else
        {
            mPath = influenceLoad->path();

            ui->selectedJointsTextEdit->clear();

            int count = influenceLoad->path().size();

            for (int i = 0; i < count; ++i)
            {
                ui->selectedJointsTextEdit->append(QString::number(influenceLoad->path().at(i)));
            }
        }
    }
    else
    {
        mName.clear();
        mDirection = tr("LR");
        mPointLoads.clear();
        mPointLoadPositions.clear();
        mPath.clear();

        if (ui->nameLineEdit->isEnabled())
        {
            ui->nameLineEdit->clear();
            ui->nameLineEdit->setEnabled(false);
        }

        ui->leftToRightRadioButton->setChecked(true);
        ui->rightToLeftRadioButton->setChecked(false);
        ui->pointLoadsListWidget->clear();
        ui->selectedJointsTextEdit->clear();
    }

    ui->selectedInfluenceLoadsTextEdit->clear();

    foreach (int InfluenceLoadNumber, selectedInfluenceLoads)
    {
        ui->selectedInfluenceLoadsTextEdit->append(QString::number(InfluenceLoadNumber));
    }
}

void EditInfluenceLoadsDialog::addPointLoad()
{
    if (mPointLoads.size() == mUnitsAndLimits->maxPointLoads())
    {
        return;
    }

    qreal pointLoadValue = ui->pointLoadDoubleSpinBox->value();
    qreal offset         = 0.0;

    if (ui->pointLoadsListWidget->count() == 0)
    {
        mPointLoads.append(pointLoadValue);
        mPointLoadPositions.append(offset);

        ui->pointLoadsListWidget->addItem(tr("%1 %2 @ %3 %4")
                                          .arg(QString::number(pointLoadValue, 'f',
                                                               mUnitsAndLimits->pointLoadDecimals()))
                                          .arg(mUnitsAndLimits->loadUnit())
                                          .arg(QString::number(offset, 'f', mUnitsAndLimits->offsetDecimals()))
                                          .arg(mUnitsAndLimits->coordinateUnit()));
    }
    else
    {
        offset = ui->offsetDoubleSpinBox->value();
        mPointLoads.append(pointLoadValue);
        qreal pointLoadPosition = mPointLoadPositions.last() + offset;
        mPointLoadPositions.append(pointLoadPosition);
        ui->pointLoadsListWidget->addItem(tr("%1 %2 @ %3 %4")
                                          .arg(QString::number(pointLoadValue, 'f',
                                                               mUnitsAndLimits->pointLoadDecimals()))
                                          .arg(mUnitsAndLimits->loadUnit())
                                          .arg(QString::number(pointLoadPosition, 'f',
                                                               mUnitsAndLimits->offsetDecimals()))
                                          .arg(mUnitsAndLimits->coordinateUnit()));
    }
}

void EditInfluenceLoadsDialog::removePointLoad()
{
    if (ui->pointLoadsListWidget->count() > 0)
    {
        mPointLoads.removeLast();
        mPointLoadPositions.removeLast();
        int row = ui->pointLoadsListWidget->count() - 1;
        QListWidgetItem *item = ui->pointLoadsListWidget->takeItem(row);
        delete item;
    }
}

void EditInfluenceLoadsDialog::editInfluenceLoads()
{
    QItemSelectionModel *itemSelectionModel = mInfluenceLoadsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No influence load selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    if (ui->nameLineEdit->isEnabled())
    {
        if (ui->nameLineEdit->text().isEmpty())
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Name : Input error"));
            messageBox.setInformativeText(tr("No entry!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        mName = ui->nameLineEdit->text();

        int count = mPath.size();

        if (count < 2)
        {
            QMessageBox messageBox(this);
            messageBox.setWindowTitle(tr("Input Validation"));
            messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
            messageBox.setInformativeText(tr("At least two joints requred!"));
            messageBox.setIcon(QMessageBox::Warning);
            messageBox.exec();
            return;
        }

        int index = 0;

        forever
        {
            Joint *jointA = mJointsList->at(mPath.at(index) - 1);
            Joint *jointB = mJointsList->at(mPath.at(index + 1) - 1);
            qreal y1      = jointA->yCoordinate();
            qreal y2      = jointB->yCoordinate();
            qreal deltaY  = y2 - y1;

            if (!jointA->connectedJoints()->contains(jointB))
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
                messageBox.setInformativeText(tr("Path not connected!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }

            if (std::fabs(deltaY) > 0.0)
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
                messageBox.setInformativeText(tr("Path not horizontal!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }

            if (++index == (count - 1))
            {
                break;
            }
        }

        Joint *firstJoint           = mJointsList->at(mPath.first() - 1);
        qreal x1                    = firstJoint->xCoordinate();
        qreal y1                    = firstJoint->yCoordinate();
        qreal epsilonMagnitudeSmall = 1.0e-12;

        foreach (Joint *joint, *firstJoint->connectedJoints())
        {
            qreal x2     = joint->xCoordinate();
            qreal y2     = joint->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;

            bool checkA = deltaX < 0.0;
            bool checkB = (std::fabs(deltaY) < epsilonMagnitudeSmall);

            if (checkA && checkB)
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
                messageBox.setInformativeText(tr("Path length not maximum!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }
        }

        Joint *lastJoint = mJointsList->at(mPath.last() - 1);
        x1               = lastJoint->xCoordinate();
        y1               = lastJoint->yCoordinate();

        foreach (Joint *joint, *lastJoint->connectedJoints())
        {
            qreal x2     = joint->xCoordinate();
            qreal y2     = joint->yCoordinate();
            qreal deltaX = x2 - x1;
            qreal deltaY = y2 - y1;

            bool checkA = deltaX > 0.0;
            bool checkB = (std::fabs(deltaY) < epsilonMagnitudeSmall);

            if (checkA && checkB)
            {
                QMessageBox messageBox(this);
                messageBox.setWindowTitle(tr("Input Validation"));
                messageBox.setText(tr("Selected joints (Influence load path) : Input error"));
                messageBox.setInformativeText(tr("Path length not maximum!"));
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.exec();
                return;
            }
        }
    }

    if (ui->leftToRightRadioButton->isChecked())
    {
        mDirection = tr("LR");
    }
    else
    {
        mDirection = tr("RL");
    }

    if (ui->pointLoadsListWidget->count() == 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Point Loads : Input error"));
        messageBox.setInformativeText(tr("No entry!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    emit inputsSignal(mName,
                      mDirection,
                      mPointLoads,
                      mPointLoadPositions);
}

void EditInfluenceLoadsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
