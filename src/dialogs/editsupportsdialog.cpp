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

/* editsupportsdialog.cpp */

#include "editsupportsdialog.h"
#include "ui_editsupportsdialog.h"

EditSupportsDialog::EditSupportsDialog(QList<Joint *>   *jointsList,
                                       QList<Support *> *supportsList,
                                       QTableView       *supportsTableView,
                                       QWidget          *parent) : QDialog(parent), ui(new Ui::EditSupportsDialog)
{
    ui->setupUi(this);
    mJointsList        = jointsList;
    mSupportsList      = supportsList;
    mSupportsTableView = supportsTableView;
    mPoint             = QPoint(0, 0);

    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_left.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_top.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_right.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/fixed_bottom.png"), QString("fixed support"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_inclined.png"), QString("inclined roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_left.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_top.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_right.png"), QString("roller"));
    ui->supportTypeComboBox->addItem(QIcon(":/icons/roller_bottom.png"), QString("roller"));
    ui->supportTypeComboBox->setCurrentIndex(-1);
    //ui->supportTypeComboBox->setIconSize(QSize(32, 32));

#ifdef Q_OS_WIN
    ui->supportTypeComboBox->setIconSize(QSize(32, 32));
#endif

#ifdef Q_OS_LINUX
    ui->supportTypeComboBox->setIconSize(QSize(25, 25));
#endif

    connect(ui->supportTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setAngleOption(int)));
    connect(ui->editSupportsPushButton, SIGNAL(clicked()), this, SLOT(editSupports()));
}

EditSupportsDialog::~EditSupportsDialog()
{
    delete ui;
}

QPoint EditSupportsDialog::position() const
{
    return mPoint;
}

void EditSupportsDialog::setAngleOption(int option)
{
    if (option == 4)
    {
        if (!ui->supportAngleDoubleSpinBox->isEnabled())
        {
            ui->supportAngleDoubleSpinBox->setEnabled(true);
            ui->supportAngleDoubleSpinBox->setValue(0.00);
        }
    }
    else
    {
        if (ui->supportAngleDoubleSpinBox->isEnabled())
        {
            ui->supportAngleDoubleSpinBox->clear();
            ui->supportAngleDoubleSpinBox->setValue(0.0);
            ui->supportAngleDoubleSpinBox->setEnabled(false);
        }

        switch (option)
        {
            case 0:
            case 5:
                ui->supportAngleDoubleSpinBox->setValue(90.0);
                break;
            case 1:
            case 6:
                ui->supportAngleDoubleSpinBox->setValue(180.0);
                break;
            case 2:
            case 7:
                ui->supportAngleDoubleSpinBox->setValue(270.0);
                break;
            case 3:
            case 8:
            case -1:
                ui->supportAngleDoubleSpinBox->setValue(0.0);
                break;
            default:
                break;
        }
    }
}

void EditSupportsDialog::setSelectedSupports(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected)
    Q_UNUSED(deselected)

    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();
    QModelIndexList modelIndexList          = itemSelectionModel->selectedRows();

    QList<int> selectedSupports;

    foreach (QModelIndex index, modelIndexList)
    {
        selectedSupports.append(index.row() + 1);
    }

    if (selectedSupports.size() == 0)
    {
        if (ui->supportJointComboBox->isEnabled())
        {
            ui->supportJointComboBox->clear();
            ui->supportJointComboBox->setEnabled(false);
        }

        ui->supportTypeComboBox->setCurrentIndex(-1);
        ui->selectedSupportsTextEdit->clear();
    }
    else if (selectedSupports.size() == 1)
    {
        int supportNumber                = selectedSupports.first();
        Support *support                 = mSupportsList->at(supportNumber - 1);
        Joint *supportJoint              = support->supportJoint();
        UnitsAndLimits::SupportType type = support->type();
        qreal angle                      = support->angle();

        if (!ui->supportJointComboBox->isEnabled())
        {
            ui->supportJointComboBox->setEnabled(true);
        }

        ui->supportJointComboBox->clear();

        foreach (Joint *joint, *mJointsList)
        {
            if (joint->isSupported() && (joint != supportJoint))
            {
                continue;
            }
            else
            {
                int jointNumber = mJointsList->indexOf(joint) + 1;
                ui->supportJointComboBox->addItem(QString::number(jointNumber));
            }
        }

        int supportJointNumber = mJointsList->indexOf(supportJoint) + 1;
        int index              = ui->supportJointComboBox->findText(QString::number(supportJointNumber));
        ui->supportJointComboBox->setCurrentIndex(index);

        switch (type)
        {
            case UnitsAndLimits::FIXED_LEFT:
                ui->supportTypeComboBox->setCurrentIndex(0);
                break;
            case UnitsAndLimits::ROLLER_LEFT:
                ui->supportTypeComboBox->setCurrentIndex(5);
                break;
            case UnitsAndLimits::FIXED_TOP:
                ui->supportTypeComboBox->setCurrentIndex(1);
                break;
            case UnitsAndLimits::ROLLER_TOP:
                ui->supportTypeComboBox->setCurrentIndex(6);
                break;
            case UnitsAndLimits::FIXED_RIGHT:
                ui->supportTypeComboBox->setCurrentIndex(2);
                break;
            case UnitsAndLimits::ROLLER_RIGHT:
                ui->supportTypeComboBox->setCurrentIndex(7);
                break;
            case UnitsAndLimits::FIXED_BOTTOM:
                ui->supportTypeComboBox->setCurrentIndex(3);
                break;
            case UnitsAndLimits::ROLLER_BOTTOM:
                ui->supportTypeComboBox->setCurrentIndex(8);
                break;
            case UnitsAndLimits::ROLLER:
                ui->supportTypeComboBox->setCurrentIndex(4);
                ui->supportAngleDoubleSpinBox->clear();
                ui->supportAngleDoubleSpinBox->setValue(angle);
            default:
                break;
        }

        ui->selectedSupportsTextEdit->clear();
        ui->selectedSupportsTextEdit->append(QString::number(supportNumber) + " @ joint "
                                             + QString::number(supportJointNumber));
    }
    else
    {
        ui->supportJointComboBox->clear();

        if (ui->supportJointComboBox->isEnabled())
        {
            ui->supportJointComboBox->setEnabled(false);
        }

        ui->supportTypeComboBox->setCurrentIndex(-1);
        ui->selectedSupportsTextEdit->clear();

        foreach (int supportNumber, selectedSupports)
        {
            Support *support = mSupportsList->at(supportNumber - 1);
            Joint *joint     = support->supportJoint();
            int jointNumber  = mJointsList->indexOf(joint) + 1;
            ui->selectedSupportsTextEdit->append(QString::number(supportNumber) + " @ joint "
                                                 + QString::number(jointNumber));
        }
    }
}

void EditSupportsDialog::editSupports()
{
    QItemSelectionModel *itemSelectionModel = mSupportsTableView->selectionModel();

    if (!itemSelectionModel->hasSelection())
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("No support selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }

    int supportJointNumber           = 0;
    UnitsAndLimits::SupportType type = UnitsAndLimits::FIXED_BOTTOM;
    qreal angle                      = 0.0;

    if (ui->supportJointComboBox->isEnabled())
    {
        supportJointNumber = ui->supportJointComboBox->currentText().toInt();
    }

    if (ui->supportTypeComboBox->currentIndex() < 0)
    {
        QMessageBox messageBox(this);
        messageBox.setWindowTitle(tr("Input Validation"));
        messageBox.setText(tr("Support type : Input error"));
        messageBox.setInformativeText(tr("No selection!"));
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();
        return;
    }
    else
    {
        int index = ui->supportTypeComboBox->currentIndex();

        switch (index)
        {
            case 0:
                type = UnitsAndLimits::FIXED_LEFT;
                break;
            case 1:
                type = UnitsAndLimits::FIXED_TOP;
                break;
            case 2:
                type = UnitsAndLimits::FIXED_RIGHT;
                break;
            case 3:
                type = UnitsAndLimits::FIXED_BOTTOM;
                break;
            case 4:
                type = UnitsAndLimits::ROLLER;
                break;
            case 5:
                type = UnitsAndLimits::ROLLER_LEFT;
                break;
            case 6:
                type = UnitsAndLimits::ROLLER_TOP;
                break;
            case 7:
                type = UnitsAndLimits::ROLLER_RIGHT;
                break;
            case 8:
                type = UnitsAndLimits::ROLLER_BOTTOM;
                break;
            default:
                break;
        }

        angle = ui->supportAngleDoubleSpinBox->value();
    }

    emit inputsSignal(supportJointNumber,
                      type,
                      angle);
}

void EditSupportsDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
