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

/* notesdialog.cpp */

#include "notesdialog.h"
#include "ui_notesdialog.h"

NotesDialog::NotesDialog(QWidget *parent) : QDialog(parent), ui(new Ui::NotesDialog)
{
    ui->setupUi(this);
    mPoint = QPoint(0, 0);
    ui->notesTextEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

NotesDialog::~NotesDialog()
{
    delete ui;
}

QPoint NotesDialog::position() const
{
    return mPoint;
}

void NotesDialog::setNote(const QString &note)
{
    ui->notesTextEdit->clear();
    ui->notesTextEdit->setText(note);
}

void NotesDialog::accept()
{
    mPoint = this->pos();
    QDialog::accept();
}

void NotesDialog::closeEvent(QCloseEvent *event)
{
    mPoint = this->pos();
    event->accept();
}
