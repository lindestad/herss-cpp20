/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss.h                                                        
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

#ifndef _HERSS_H
#define _HERSS_H

// Compatibility header for cppyy and legacy C++ clients. Keep this parseable
// until a tagged release explicitly moves downstream users to a replacement API.

#include "herss_dataset.h"
#include "herss_scenario.h"
#include "herss_qmin.h"
#include "herss_routing.h"
#include "arraycurve.h"
#include "logger.h"

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class Node {
  public:
    Node();
    virtual ~Node();
    NodeType nodetype;
    size_t idnr; // Specified by the user, muste be correct calculation order (accumulation levels)
    Scenario *S;  // This will point to the correct scenario for the node.
    string nodename;

    Qmin qmin;  // We let all nodes have access to one Qmin object 
    bool qmin_in_use;  // Flag indicatin wether Qmin is used or not. 
    
    double up_res_Mm3;   // Upstream reservoir volume - used in Powerstation. 
    
    double remaining_Mm3;   
    double upstream_remaining_Mm3; // We accumulate as we go downward. 
    
    double remaining_active_Mm3;          // Means only water that can be produced 
    double upstream_remaining_active_Mm3; // We accumulate as we go downward.  Only water that is assumed to available for production. 

    size_t reservoir_idnr;  // Used so we can go from node idnr to reservoir number. 

    int pstation_idnr; // We use this to index pstations. 
    int max_adjustment_pr_day;
    double max_adjustment_cost;

    double local_energy_equivalent;  // kWh/m3
    double powstat_min_discharge;  // We must place them here so we can accoes them through node pointer. 
    double auto_qmin;
    double start_of_stp_masl;
    double end_of_stp_masl;
    
    bool downstream_node_in_use;
    bool outlet_hatch_in_use;
    bool outlet_tunnel_in_use;
    bool outlet_overflow_in_use;
    bool outlet_auto_qmin_in_use;

    int downstream_idnr; // Used to keep track of remaining water volumes. 
    int downstream_idnr_tunnel;
    int downstream_idnr_hatch;
    int downstream_idnr_overflow;
    int downstream_idnr_auto_qmin;

    Node *ptr_downstream_node;
    Node *ptr_downstream_node_tunnel;
    Node *ptr_downstream_node_hatch;
    Node *ptr_downstream_node_overflow;
    Node *ptr_downstream_node_auto_qmin;

    virtual int ReadNodeData(string filename);
    virtual int ReadStateFile(string filename);
    virtual int WriteStateFile(FILE *fp);

    virtual int Simulate(size_t t);
    virtual int initArrayCurves(void);
    virtual int CheckWaterBalance(class Herss *herss_obj); 
    virtual double GetStartWater_Mm3(void);
    virtual double GetEndWater_Mm3(void);
    virtual int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node 
    virtual double GetTunnelFLow(size_t t); // Used for reservoirs connected to a powerstation. 

    // Defining a function as virtual, means that it can be redefined in the child classes
    // This is an important feature since we can use the same function name, but execute different
    // taks depending on the child class.
};
/////////////////////////////////////////////////////////////////////////////////////////
class Reservoir: public Node {

    public:
    Reservoir();
    ~Reservoir();
    size_t stps;
    size_t dt;
    GlobalConfig *gc;
    double floodlevel_penalty;
    double floodlevel_cost;
    double reservoir_init_fr;
    double reservoir_init_masl;
    double reservoir_init_Mm3;
    double reservoir_init_active_Mm3;  // minus the filling at LRW
    double active_max_volume_Mm3;  // Filling at HRW minus the filling at LRW 
    double res_HRW;             //  Highest regulated water level [masl]
    double res_LRW;             //  Lowest regulated water level [masl]
    double filling_at_hrw_Mm3;  // Mm3
    double filling_at_lrw_Mm3;  // Mm3
    double filling_at_hatchlevel;
    double cost_lrw;
    double res_penalty;
    bool fast_overflow;         // If true, we use the fast overflow calculation.
    
    // Dynamic variables that change during the simulation
    double res_Mm3;             //  Reservoir filling [Mm3]
    double res_masl;            //  Reservoir filling [masl]
    double res_fr;              //  Reservoir filling as a fraction of full.


    // Reservoir geometry, we assume a trapezoidal shape
    // we use this to scip the reservoir curve. 
    bool use_reservoir_geometry;
    double width_m;
    double length_m;
    double theta; // angle of the sides of the reservoir, in degrees.
    double bottom_masl; // masl of the bottom of the reservoir.

    // We precompute this one to save time in the simulation.
    double slope_term; // tan((90.0 - theta) * PI / 180.0);   

    // We precompute this one to save time in the simulation.
    double geo_denom; 
    // geo_denom = length_m * (slope_term + width_m); 


    bool use_reservoir_curve; // If true, we use the reservoir curves for calculating the filling and the masl.
    double res_curve_masl[MAX_NR_POINTS_CURVE];
    double res_curve_Mm3[MAX_NR_POINTS_CURVE];
    size_t nr_points_res_curve;

    double ovefl_curve_masl[MAX_NR_POINTS_CURVE];
    double ovefl_curve_m3s[MAX_NR_POINTS_CURVE];
    size_t nr_points_ovefl_curve;
 
    double minQ_hatch, maxQ_hatch, hatch_masl;

    ArrayCurve ac_res_masl_2_Mm3;
    ArrayCurve ac_res_Mm3_2_masl;
    ArrayCurve ac_ovefl_masl_2_m3s;
    ArrayCurve ac_ovefl_m3s_2_masl;

    // VIRTUAL FUNCTIONS USED IN RESERVOIR/CHANNEL/PSTATION
    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    int initArrayCurves(void);
    //int CheckWaterBalance(void);
    int CheckWaterBalance(class Herss *herss_obj);  
    int GetStartWater(void);
    int WriteStateFile(FILE *fp);

    // Functions used only in Reservoir
    void InitReservoir(void);
    double CalcOverflow();   // Mm3
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node 
    double GetTunnelFLow(size_t t); // Used for reservoirs connected to a powerstation. 
    double GetReservoirFraction(size_t t);

    // We use this to check if the reservoir level is valid.
    void ValidateReservoirLevelMm3(size_t t, double level_Mm3);

    // We check if the settings for the reservoir are valid. 
    void ValidateReservoirSettings();  


    double calcResVolume(double masl);  // Returns Mm3 
    double calcResMasl(double Mm3);     // Returns masl



};
/////////////////////////////////////////////////////////////////////////////////////////
// CHANGE BY OVE - Initialize the Generator struct
struct Generator {
        std::vector<double> turb_virkn_Q;       // Turbine efficiency curve Q values
        std::vector<double> turb_virkn_psnt;    // Turbine efficiency curve psnt values
        ArrayCurve eff_curve; 
        std::vector<double> action;             // Actions for this generator
        double headlosscoef;                    // Head loss coefficient for this generator/penstock
        double max_discharge;                   // Maximum discharge for this generator
    };

class Powerstation: public Node {
    public:
    Powerstation();
    ~Powerstation();
    size_t stps;
    size_t dt;
    double init_Power;
    GlobalConfig *gc;

    double static_gen_efficiency;
    double headlosscoef;
    double powstat_masl;
    double powstat_startstop;
    bool shared_penstock;  // CHANGE BY OVE: true = shared penstock, false = separate penstocks

    size_t nr_generators;  // We can get this out of the vector, but its nice to have. 
    std::vector<Generator> generators; // CHANGE BY OVE: Implemented vectors to hold generators in a powerstation.

    // BVM 15 oct 2024.
    // In a river runoff powerstation, it is possible to empty the intake in one timestep. 
    // Since we do not allow this, and we override the action.
    // We give a marginal penalty so that we see a change in the VF.
    double aggressive_actions_cost;

    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    // int initArrayCurves(void);
    //int CheckWaterBalance(void);
    int CheckWaterBalance(class Herss *herss_obj);
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node 
    double GetTunnelFLow(size_t t); 
    int WriteStateFile(FILE *fp);
    double CalcAdjustmenCosts(void); // Only for Powerstation 
    void ValidatePowerstationSettings();  // We check if the settings for the powerstation are valid. For example, that the number of generators is not higher than the maximum allowed, and that the headloss coefficient is not negative.

};
/////////////////////////////////////////////////////////////////////////////////////////
class Channel: public Node {
    public:
    Channel();
    ~Channel();
    Channel(const Channel&) = delete;
    Channel& operator=(const Channel&) = delete;
    Channel(Channel&&) noexcept = default;
    Channel& operator=(Channel&&) noexcept = default;
    size_t stps;
    size_t dt;
    GlobalConfig *gc;

    CascadedReservoirs *casc_reservoirs;
    // We use a series of cascaded reservoirs to model the routing in the channel.

    std::vector<double> initial_storage_linres_Mm3; 

    double K_traveltime_hours;  // Travel time in hours, used in the cascaded reservoir routing model.
    size_t num_cascaded_reservoirs;  // Number of cascaded reservoirs used in the routing model.

    double decay;
    double waterflow_m3[MAX_TRAVELTIME_HOURS];  // Keeps track of how much water that is stored in the channel. [m3]
    double init_waterflow_m3[MAX_TRAVELTIME_HOURS];  // Keeps track of how much water that is stored in the channel. [m3]

    int ReadNodeData(string filename);
    int ReadStateFile(string filename);
    int Simulate(size_t t);
    int initArrayCurves(void);
    int CheckWaterBalance(class Herss *herss_obj);  
    double GetStartWater_Mm3(void);
    double GetEndWater_Mm3(void);
    int WriteNodeOutput(GlobalConfig *gc);  // Write output for each node 
    double GetTunnelFLow(size_t t);
    int WriteStateFile(FILE *fp);
    int SetStartState(void);
    void PrintChannelWater(void);
    void ValidateChannelSettings();  // We check if the settings for the channel are valid. For example, that the travel time is not longer than the maximum allowed.

private:
    std::unique_ptr<CascadedReservoirs> casc_reservoirs_owner;

};
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
/////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////


#endif
