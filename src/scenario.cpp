/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     scenario.cpp                                                        
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: Å Energi, www.ae.no

This software is released under the MIT license:

Copyright (c) <2024> <Å Energi, Bernt Viggo Matheussen>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#include "herss_scenario.h"

Scenario::Scenario(){
    stps = 0;
    dt = 0;
    idnr = 0;
    price = nullptr;
    action = nullptr;
    q_action = nullptr;
    inflow = nullptr;
    tot_outflow = nullptr;
    tot_inflow = nullptr;
    local_inflow = nullptr;
    up_inflow = nullptr;
    tunnelflow_m3s = nullptr;
    hatchflow_m3s = nullptr;
    overflow_m3s = nullptr;
    auto_qmin_m3s = nullptr;
    channel_storage_Mm3 = nullptr;
    res_Mm3 = nullptr;
    res_active_Mm3 = nullptr;
    res_masl = nullptr;
    res_fr = nullptr;
    profit = nullptr;
    overflow_Mm3 = nullptr;
    income = nullptr;
    cost = nullptr;
    cost_qmin = nullptr;
    startStopCost = nullptr;
    cost_lrw = nullptr;
    cost_fake_lrw = nullptr;
    adjust_cost = nullptr;
    cost_aggressive_actions = nullptr;
    Hbrutto = nullptr;
    Hnetto = nullptr;
    Power = nullptr;
    EstimatedEEKV = nullptr;
    year = nullptr;
    month = nullptr;
    day = nullptr;
    hour = nullptr;
    qmin_flag = nullptr;
}

Scenario::Scenario(size_t stps, size_t dt, size_t idnr){

    this->stps = stps;
    this->dt   = dt;
    this->idnr = idnr;
    restprice               = NOT_INIT;
    broken_lrw  = false;
    broken_qmin = false;
    days_with_production    = NOT_INIT;
    remaining_Mm3           = NOT_INIT;
    local_remaining_Mm3     = NOT_INIT;
    remaining_Euro          = NOT_INIT;
    remaining_MWh           = NOT_INIT;
    remaining_upstream_Mm3  = NOT_INIT;
    sum_prod_MWh            = NOT_INIT;
    sum_income_Euro         = NOT_INIT;
    sum_cost_Euro           = NOT_INIT;
    sum_profit_Euro         = NOT_INIT;
    sum_incoming_water_Mm3  = NOT_INIT;
    sum_local_inflow_Mm3    = NOT_INIT;
    sum_total_energy_MWh    = NOT_INIT;
    sum_overflow_Mm3        = NOT_INIT;

    try {
        auto init_double = [this](std::vector<double>& storage, double value) {
            storage.assign(this->stps, value);
            return storage.data();
        };
        auto init_int = [this](std::vector<int>& storage, int value) {
            storage.assign(this->stps, value);
            return storage.data();
        };

        action_storage.assign(stps, std::vector<double>(MAX_NR_NODES, NOT_INIT));
        action_rows.resize(stps);
        for(size_t t = 0; t < stps; ++t) {
            action_rows[t] = action_storage[t].data();
        }
        action = action_rows.data();

        price = init_double(price_storage, NOT_INIT);
        q_action = init_double(q_action_storage, NOT_INIT);
        inflow = init_double(inflow_storage, 0.0);
        tot_outflow = init_double(tot_outflow_storage, NOT_INIT);
        tot_inflow = init_double(tot_inflow_storage, NOT_INIT);
        local_inflow = init_double(local_inflow_storage, NOT_INIT);
        up_inflow = init_double(up_inflow_storage, 0.0);
        tunnelflow_m3s = init_double(tunnelflow_m3s_storage, NOT_INIT);
        hatchflow_m3s = init_double(hatchflow_m3s_storage, NOT_INIT);
        overflow_m3s = init_double(overflow_m3s_storage, NOT_INIT);
        auto_qmin_m3s = init_double(auto_qmin_m3s_storage, NOT_INIT);
        channel_storage_Mm3 = init_double(channel_storage_Mm3_storage, NOT_INIT);
        res_Mm3 = init_double(res_Mm3_storage, NOT_INIT);
        res_active_Mm3 = init_double(res_active_Mm3_storage, NOT_INIT);
        res_masl = init_double(res_masl_storage, NOT_INIT);
        res_fr = init_double(res_fr_storage, NOT_INIT);
        profit = init_double(profit_storage, NOT_INIT);
        overflow_Mm3 = init_double(overflow_Mm3_storage, NOT_INIT);
        income = init_double(income_storage, NOT_INIT);
        cost = init_double(cost_storage, NOT_INIT);
        cost_qmin = init_double(cost_qmin_storage, NOT_INIT);
        startStopCost = init_double(startStopCost_storage, NOT_INIT);
        cost_lrw = init_double(cost_lrw_storage, NOT_INIT);
        cost_fake_lrw = init_double(cost_fake_lrw_storage, NOT_INIT);
        adjust_cost = init_double(adjust_cost_storage, 0.0);
        cost_aggressive_actions = init_double(cost_aggressive_actions_storage, 0.0);
        Hbrutto = init_double(Hbrutto_storage, NOT_INIT);
        Hnetto = init_double(Hnetto_storage, NOT_INIT);
        Power = init_double(Power_storage, NOT_INIT);
        EstimatedEEKV = init_double(EstimatedEEKV_storage, NOT_INIT);
        year = init_int(year_storage, NOT_INIT);
        month = init_int(month_storage, NOT_INIT);
        day = init_int(day_storage, NOT_INIT);
        hour = init_int(hour_storage, NOT_INIT);
        qmin_flag = init_int(qmin_flag_storage, NOT_INIT);
    }
    catch(std::bad_alloc& exc) {
        printf("Error: memory allocation failed. \n"); 
        printf("file: %s  linenr: %d  function: %s \n", __FILE__ , __LINE__, __FUNCTION__);
		exit(EXIT_FAILURE);
    }
}
///////////////////////////////////////////////////////////////////////////////
Scenario::~Scenario(){
    price = nullptr;
    action = nullptr;
    q_action = nullptr;
    inflow = nullptr;
    tot_outflow = nullptr;
    tot_inflow = nullptr;
    local_inflow = nullptr;
    up_inflow = nullptr;
    res_Mm3 = nullptr;
    res_active_Mm3 = nullptr;
    res_masl = nullptr;
    res_fr = nullptr;
    profit = nullptr;
    overflow_Mm3 = nullptr;
    income = nullptr;
    cost = nullptr;
    cost_qmin = nullptr;
    startStopCost = nullptr;
    cost_lrw = nullptr;
    cost_fake_lrw = nullptr;
    Hbrutto = nullptr;
    Hnetto = nullptr;
    Power = nullptr;
    EstimatedEEKV = nullptr;
    year = nullptr;
    month = nullptr;
    day = nullptr;
    hour = nullptr;
    qmin_flag = nullptr;
    tunnelflow_m3s = nullptr;
    hatchflow_m3s = nullptr;
    overflow_m3s = nullptr;
    auto_qmin_m3s = nullptr;
    channel_storage_Mm3 = nullptr;
    adjust_cost = nullptr;
    cost_aggressive_actions = nullptr;
}
///////////////////////////////////////////////////////////////////////////////
