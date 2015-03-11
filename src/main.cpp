/*
 *  This file is part of Windows-Uptime.
 *
 *  Copyright (C) 2014-2015 R1tschY <r1tschy@yahoo.de>
 *
 *  Windows-Uptime is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  TrafficIndicator is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

#include <QApplication>
#include "utils.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  a.setOrganizationName(QStringLiteral("R1tschY"));
  a.setApplicationName(QStringLiteral("Windows-Uptime"));
  a.setApplicationVersion(QStringLiteral(STRINGIFY(PACKAGE_VERSION)));

  WinUptime::MainWindow w;
  w.show();

  return a.exec();
}
