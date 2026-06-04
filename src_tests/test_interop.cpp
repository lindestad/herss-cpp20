#include <gtest/gtest.h>
#include "test_paths.h"
#include "herss.h"

namespace {

GlobalConfig makeFixtureConfig()
{
    GlobalConfig gc;
    gc.globalfile = herssTestDataPath("global.txt");
    gc.readGlobalFile();
    gc.SetDirectoriesAndFilenames();
    gc.Diagnose();
    gc.checkNrSteps();
    gc.write_nodefiles = false;
    return gc;
}

}

TEST(InteropTest, DatasetKeepsLegacyRawPointerViews)
{
    GlobalConfig gc = makeFixtureConfig();
    Dataset dataset(&gc);
    dataset.readAllData();

    ASSERT_NE(dataset.price, nullptr);
    ASSERT_NE(dataset.inflow, nullptr);
    ASSERT_NE(dataset.action, nullptr);
    ASSERT_NE(dataset.year, nullptr);

    EXPECT_NEAR(dataset.price[0], 626.954, 0.001);
    EXPECT_NEAR(dataset.inflow[0][0], 0.36, 0.001);
    EXPECT_NEAR(dataset.action[0][1], 0.80, 0.001);
    EXPECT_EQ(dataset.year[0], 2022);

    dataset.inflow[0][0] = 1.25;
    EXPECT_DOUBLE_EQ(dataset.inflow[0][0], 1.25);
}

TEST(InteropTest, HerssPreparationKeepsNodeAndScenarioPointers)
{
    GlobalConfig gc = makeFixtureConfig();
    Dataset dataset(&gc);
    dataset.readAllData();

    Herss herss(&gc);
    herss.prepaireSimulation(&dataset);

    ASSERT_NE(herss.rs, nullptr);
    ASSERT_NE(herss.scen, nullptr);
    ASSERT_EQ(herss.rs->nr_nodes, gc.nr_nodes);

    for(size_t n = 0; n < gc.nr_nodes; ++n) {
        ASSERT_NE(herss.rs->nodes[n], nullptr);
        EXPECT_EQ(herss.rs->nodes[n]->S, herss.scen[n]);
    }

    EXPECT_EQ(herss.rs->nodes[0], static_cast<Node*>(&herss.rs->reservoirs[0]));
    EXPECT_EQ(herss.rs->nodes[1], static_cast<Node*>(&herss.rs->pstations[0]));
    EXPECT_EQ(herss.rs->nodes[2], static_cast<Node*>(&herss.rs->channels[0]));
}
