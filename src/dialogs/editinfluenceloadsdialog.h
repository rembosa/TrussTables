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

/* editinfluenceloadsdialog.h */

#ifndef EDITINFLUENCELOADSDIALOG_H
#define EDITINFLUENCELOADSDIALOG_H

#include <cmath>

#include <QCloseEvent>
#include <QDialog>
#include <QItemSelection>
#include <QMessageBox>
#include <QTableView>

#include "influenceload.h"
#include "joint.h"
#include "unitsandlimits.h"

namespace Ui
{
    class EditInfluenceLoadsDialog;
}

class EditInfluenceLoadsDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit EditInfluenceLoadsDialog(UnitsAndLimits         *unitsAndLimits          = 0,
                                          QList<InfluenceLoad *> *influenceLoadsList      = 0,
                                          QList<Joint *>         *jointsList              = 0,
                                          QTableView             *jointsTableView         = 0,
                                          QTableView             *influenceLoadsTableView = 0,
                                          QWidget                *parent                  = 0);

        ~EditInfluenceLoadsDialog();

        QPoint position() const;

        void updateUnits();

    signals:
        void inputsSignal(QString      name,
                          QString      direction,
                          QList<qreal> pointLoads,
                          QList<qreal> pointLoadPositions);

    public slots:
        void setSelectedJoints(const QItemSelection &selected, const QItemSelection &deselected);
        void setSelectedInfluenceLoads(const QItemSelection &selected, const QItemSelection &deselected);
        void addPointLoad();
        void removePointLoad();
        void editInfluenceLoads();

    protected:
        void closeEvent(QCloseEvent *event);

    private:
        Ui::EditInfluenceLoadsDialog *ui;
        UnitsAndLimits               *mUnitsAndLimits;
        QList<InfluenceLoad *>       *mInfluenceLoadsList;
        QList<Joint *>               *mJointsList;
        QTableView                   *mJointsTableView;
        QTableView                   *mInfluenceLoadsTableView;
        QPoint                       mPoint;
        QString                      mName;
        QString                      mDirection;
        QList<int>                   mPath;
        QList<qreal>                 mPointLoads;
        QList<qreal>                 mPointLoadPositions;
};

#endif // EDITINFLUENCELOADSDIALOG_H
