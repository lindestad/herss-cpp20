/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_qmin.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_QMIN_H
#define _HERSS_QMIN_H

#include "herss_common.h"

//////////////////////////////////////////////////////////////////////////////////////////
class QminPeriod {
    public:
    QminPeriod(){};
    ~QminPeriod(){};
    double min_discharge;
    int start_day;
    int start_month;
    int end_day;
    int end_month;
    double penalty_cost;
};
//////////////////////////////////////////////////////////////////////////////////////////
class Qmin {
  public:
    Qmin();
    ~Qmin();
    bool qmin_flag;
    QminPeriod timeperiods[MAX_NUMBER_OF_QMIN_PERIODS];
    int nr_periods;
    double calcQminRequirement(int year, int month, int day, double *cost );
};

#endif
