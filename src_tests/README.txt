########################################################################################################################
GOOGLE TESTING SUITE FOR HERSS
Dev: Ove A.
Date: 11.08.2025
########################################################################################################################

------------------------------------------------------- PURPOSE --------------------------------------------------------
The purpose of the testing suite is ensuring that future changes to HERSS do not impact the general functionality,
correctness and simulation time. Multiple source components are tested in addition to waterbalance, value function
and simulation time.

----------------------------------------------------- HOW TO USE -------------------------------------------------------
The test is run from the src folder by "make test". The test requires the testing scripts and utahps_test to be included
in the src_tests. In total, 101 tests are created.

-------------------------------------------------- COMPONENT TESTS -----------------------------------------------------
Below is a quick explanation of the tests implemented for each component. Other minor tests may be implemented without
being mentioned here (ex. constructor/destructor tests).

The tests check the correctness of:

# arraycurve.cpp
- Maximum/Minimum values of curves
- Normalization within [0.0 - 1.0]
- Interpolation returns
- Boundaries and out of bound conditions

# channel.cpp
- Traveltime = 0
- Travel time handling
- Penalty application
- Water balance

# dataset.cpp
- Read price/inflow/actions
- Multi_temporal_resolution
- Handling of input data

# globalconfig.cpp
- Initialization of members
- Error handling of non-existing files
- nr. of steps

# line.cpp
- Reading of input files

# powerstation.cpp
- Test of Multi_temporal_resolution
- Price loading
- Action = 0.0
- Efficiency calculation
- Head loss calculation
- Shared/Separate penstock configuration
- Start/Stop cost
- Multi-generator functionality
- Waterbalance
- Tunnelflow
- Qmin
- Aggressive action
- Negative action
- Income/profit calculation
- Minimum discharge
- Downstream node inflow
- General performance test

# reservoir.cpp
- Reservoir volume over time
- Overflow
- General performance test

# Valuefunction test
- Simulation of utahps_test to ensure src changes does not impact the valuefunction 

# Waterbalance test
- Checks two reservoirs to ensure changes in src does not impact the method used to calculate water volume in the reservoirs

# Runtime test
- Runs utahps_test repeatedly through the compiled executable and checks the average runtime

# Golden output test
- Runs the compiled executable on utahps_test and compares generated output files against the reference output
