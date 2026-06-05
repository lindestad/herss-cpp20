/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_system.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_SYSTEM_H
#define _HERSS_SYSTEM_H

#include "herss_channel.h"
#include "herss_dataset.h"
#include "herss_powerstation.h"
#include "herss_reservoir.h"

/////////////////////////////////////////////////////////////////////////////////////////
// This class models a hydropower system
class Riversystem {

public:
    Riversystem();                   // Default constructor
    Riversystem(GlobalConfig *gc);
    ~Riversystem();                  // Default destructor
    GlobalConfig *gc;
    size_t nr_nodes;
    size_t nr_reservoirs;
    size_t nr_pstations;
    size_t nr_channels;

    double start_water_Mm3;
    double end_water_Mm3;
    double inflow_volume_Mm3;
    double outgoing_Mm3;
    double waterbalance;
    int nodes_idnrs[MAX_NR_NODES];
    double sum_prod_MWh;
    double sum_total_MWh; // Production plus remaining in whole riversystem
    double adjust_cost;   // Adjustment cost

    // For use in ValueFunction calculations
    //double effective_remaining_available_Mm3;  // Means only water that can be produced
    //double total_effective_remaining_available_Mm3;  // Means only water that can be produced
    //double effective_upstream_remaining_available_Mm3; // We accumulate as we go downward.  Only water that is assumed to available for production.

    double tot_remaining_Mm3;
    double tot_remaining_MWh;
    double tot_remaining_Euro;
    double tot_active_remaining_Mm3;  // Total upstream is included
    double tot_income_Euro;
    double tot_cost_Euro;
    double tot_profit_Euro;
    double valuefunction_Euro;
    double sum_production;
    double avg_price;
    double sum_startstopcost;
    double sum_max_adjustment_cost;
    double sum_lrw_cost;
    double sum_qmin_cost;
    double ValueFunction[500];  // We store the value function for each timestep.
    double WaterValue[500];  // We store the water value for each timestep.

    // Array of Nodes (reservoirs, powerstations, channels)
    Node **nodes;
    Reservoir *reservoirs;
    Powerstation *pstations;
    Channel *channels;
    double Simulate(int id);
    double CalcVF(double restprice);
    double CalcVF_atEndOfStp(double restprice, size_t stp);  // Calculate the value function at a specific timestep
    double CalcSimulationProfit();
    int WriteRiverSystemData(double restprice);
    void WriteReservoirData();
    void PrintReservoirData2Screen();
    int WriteSelectedOutputMatrix();
    double GetEndingReservoirLevel(size_t r_idnr);
    void PrintEconomicInfo(class Herss *herss_obj);
    void DiagnoseRiversystemConfiguration();   // Run checks to see if the configuration of the riversystem is correct.

private:
    std::vector<Node*> node_storage;
    std::vector<Reservoir> reservoir_storage;
    std::vector<Powerstation> pstation_storage;
    std::vector<Channel> channel_storage;

};
/////////////////////////////////////////////////////////////////
// A class that models Input, Scenarios and riversystem
class Herss {
public:
    Herss();
    Herss(GlobalConfig *gc);
    GlobalConfig *gc;
    Dataset *data;  // Store reference to dataset for variable timesteps
    ~Herss();
    size_t dt;          // Delta time step in seconds
    size_t stps;        // How many time steps used in each scenario, and in the optimization step.
    size_t nr_nodes;
    Riversystem *rs;
    Scenario  **scen;

    int prepaireSimulation(Dataset *data); // Read in final data and set pointers.
    void SetPointers();
    void ReadTopologyFile(string filename);
    int Simulate();
    int CheckWaterBalance();
    int GlobalWaterBalance();
    int WriteNodeOutput();  // Write output for each node
    int WriteStateFile();  // Write output for each node
    int CalcAdjustmenCosts();

    void SetAction(size_t node_idnr, size_t gen_idnr, size_t t, double value);

    double GetAction(size_t node_idnr, size_t gen_idnr, size_t t);
    void PrintActions();
    void PrintReservoirLevels_fr();
    void PrintRemainingChannelWater_Mm3();
    double GetRestPrice();
    double GetPrice(size_t t);
    void SetPrice(size_t t, double price, double restprice);
    double GetReservoir_Init_fr(size_t idnr);  // Get starting reservoir fraction.
    void SetReservoir_Init_fr(size_t idnr, double value);  // Set starting reservoir fraction.
    double GetReservoirLevel_fr(size_t node_idnr, size_t t);
    void PrintInflowSeries(size_t t);
    void PrintState();
    void SetInflowInNode(size_t t, size_t nodenr, double value);
    double GetInflowInNode(size_t t, size_t nodenr);
    void PrintAllInput();
    double CalcWaterValue_atEndofStp(size_t t);  // Calculate the water value in the system at a specific timestep
    double GetValueFunction_atStp(size_t t);
    void SetDate(size_t t, int Y, int M, int D, int H);
    int getDeltaT(size_t timestep);

private:
    std::unique_ptr<Riversystem> rs_owner;
    std::vector<std::unique_ptr<Scenario>> scenario_owner;
    std::vector<Scenario*> scenario_views;

};

#endif
