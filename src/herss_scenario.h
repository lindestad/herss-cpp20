/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_scenario.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_SCENARIO_H
#define _HERSS_SCENARIO_H

#include "herss_common.h"

///////////////////////////////////////////////////////////////////////////////////////////
class Scenario {

public:
    Scenario();
    Scenario(size_t stps, size_t dt, size_t idnr);
    ~Scenario();

    size_t stps;
    size_t dt;
    size_t idnr;  // This is the same idnr as used in node.
    double restprice;
    bool broken_lrw;
    bool broken_qmin;

    double days_with_production;
    double remaining_Mm3;
    double local_remaining_Mm3;  // The remaining volume in the Node.
    double remaining_Euro;
    double remaining_MWh;
    double remaining_upstream_Mm3;
    double sum_prod_MWh;
    double sum_income_Euro;
    double sum_cost_Euro;
    double sum_profit_Euro;
    double sum_incoming_water_Mm3;
    double sum_local_inflow_Mm3;
    double sum_total_energy_MWh;
    double sum_overflow_Mm3;  // We check overflow over the optimization horizon.

    // Arrays
    double *price;
    double **action;
    double *q_action;
    double *inflow;
    double *tot_outflow;
    double *tot_inflow;
    double *local_inflow;
    double *up_inflow;
    double *tunnelflow_m3s;
    double *hatchflow_m3s;
    double *overflow_m3s;
    double *auto_qmin_m3s;
    double *channel_storage_Mm3;

    double *res_Mm3;          // Reservoir filling in Mm3
    double *res_active_Mm3;   //  Reservoir filling [Mm3] minus filling at LRW
    double *res_masl;         // Reservoir filling in meters above sea level (masl)
    double *res_fr;           // Reservoir filling as a fraction of full
    double *profit;
    double *overflow_Mm3;
    double *income;
    double *cost;
    double *cost_qmin;
    double *startStopCost;
    double *cost_lrw;
    double *cost_fake_lrw;
    double *adjust_cost;
    double *cost_aggressive_actions;

    double *Hbrutto;  // Hydraulic head brutto
    double *Hnetto;   // Hydraulic head netto
    double *Power;
    double *EstimatedEEKV;  // Estimated energy equivalent

    int *year;
    int *month;
    int *day;
    int *hour;
    int *qmin_flag;

private:
    std::vector<double> price_storage;
    std::vector<std::vector<double>> action_storage;
    std::vector<double*> action_rows;
    std::vector<double> q_action_storage;
    std::vector<double> inflow_storage;
    std::vector<double> tot_outflow_storage;
    std::vector<double> tot_inflow_storage;
    std::vector<double> local_inflow_storage;
    std::vector<double> up_inflow_storage;
    std::vector<double> tunnelflow_m3s_storage;
    std::vector<double> hatchflow_m3s_storage;
    std::vector<double> overflow_m3s_storage;
    std::vector<double> auto_qmin_m3s_storage;
    std::vector<double> channel_storage_Mm3_storage;
    std::vector<double> res_Mm3_storage;
    std::vector<double> res_active_Mm3_storage;
    std::vector<double> res_masl_storage;
    std::vector<double> res_fr_storage;
    std::vector<double> profit_storage;
    std::vector<double> overflow_Mm3_storage;
    std::vector<double> income_storage;
    std::vector<double> cost_storage;
    std::vector<double> cost_qmin_storage;
    std::vector<double> startStopCost_storage;
    std::vector<double> cost_lrw_storage;
    std::vector<double> cost_fake_lrw_storage;
    std::vector<double> adjust_cost_storage;
    std::vector<double> cost_aggressive_actions_storage;
    std::vector<double> Hbrutto_storage;
    std::vector<double> Hnetto_storage;
    std::vector<double> Power_storage;
    std::vector<double> EstimatedEEKV_storage;
    std::vector<int> year_storage;
    std::vector<int> month_storage;
    std::vector<int> day_storage;
    std::vector<int> hour_storage;
    std::vector<int> qmin_flag_storage;

};

#endif
