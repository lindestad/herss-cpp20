/********************************************************************************
Project:      The Hydraulic Economic River System Simulator (HERSS)
Filename:     herss_node.h
Developer:    Bernt Viggo Matheussen (Bernt.Viggo.Matheussen@aenergi.no)
Organization: A Energi, www.ae.no

This software is released under the MIT license.
********************************************************************************/

#ifndef _HERSS_NODE_H
#define _HERSS_NODE_H

#include "herss_config.h"
#include "herss_qmin.h"
#include "herss_scenario.h"

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

#endif
