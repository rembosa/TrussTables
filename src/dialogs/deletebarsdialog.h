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

/* deletebarsdialog.h */

#ifndef DELETEBARSDIALOG_H
#define DELETEBARSDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

namespace Ui
{
    class DeleteBarsDialog;
}

class DeleteBarsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit DeleteBarsDialog(QTableView *barsTableView = 0, QWidget *parent = 0);

        ~DeleteBarsDialog();

        QPoint position() const;

    signals:
        void inputsSignal();

    public slots:
        void deleteBars();
        void setSelectedBars(const QItemSelection &selected, const QItemSelection &deselected);

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::DeleteBarsDialog *ui;
        QTableView           *mBarsTableView;
        QPoint               mPoint;
};

#endif // DELETEBARSDIALOG_H
