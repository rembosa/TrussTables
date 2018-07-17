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

/* deletethermaleffectsdialog.h */

#ifndef DELETETHERMALEFFECTSDIALOG_H
#define DELETETHERMALEFFECTSDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "bar.h"
#include "thermaleffect.h"

namespace Ui
{
    class DeleteThermalEffectsDialog;
}

class DeleteThermalEffectsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit DeleteThermalEffectsDialog(QList<Bar *>           *barsList                = 0,
                                            QList<ThermalEffect *> *thermalEffectsList      = 0,
                                            QTableView             *thermalEffectsTableView = 0,
                                            QWidget                *parent                  = 0);

        ~DeleteThermalEffectsDialog();

        QPoint position() const;

    signals:
        void inputsSignal();

    public slots:
        void setSelectedThermalEffects(const QItemSelection &selected, const QItemSelection &deselected);
        void deleteThermalEffects();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::DeleteThermalEffectsDialog *ui;
        QList<Bar *>                   *mBarsList;
        QList<ThermalEffect *>         *mThermalEffectsList;
        QTableView                     *mThermalEffectsTableView;
        QPoint                         mPoint;
};

#endif // DELETETHERMALEFFECTSDIALOG_H
