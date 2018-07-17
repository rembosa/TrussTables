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

/* scaleforcesdialog.h */

#ifndef SCALEFORCESDIALOG_H
#define SCALEFORCESDIALOG_H

#include <QCloseEvent>
#include <QDialog>

namespace Ui
{
    class ScaleForcesDialog;
}

class ScaleForcesDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ScaleForcesDialog(QWidget *parent = 0);

        ~ScaleForcesDialog();

        QPoint position() const;

        qreal forceFactor();
        void setForceFactor(qreal factor);

    signals:
        void factorSignal(qreal factor);

    public slots:
        void onFactorChanged();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::ScaleForcesDialog *ui;
        QPoint                mPoint;
};

#endif // SCALEFORCESDIALOG_H
