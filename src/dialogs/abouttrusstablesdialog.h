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

/* abouttrusstablesdialog.h */

#ifndef ABOUTTRUSSTABLESDIALOG_H
#define ABOUTTRUSSTABLESDIALOG_H

#include <QDialog>
#include <config.h>

namespace Ui
{
    class AboutTrussTablesDialog;
}

class AboutTrussTablesDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit AboutTrussTablesDialog(QWidget *parent = 0);

        ~AboutTrussTablesDialog();

    private:
        Ui::AboutTrussTablesDialog *ui;
};

#endif // ABOUTTRUSSTABLESDIALOG_H
