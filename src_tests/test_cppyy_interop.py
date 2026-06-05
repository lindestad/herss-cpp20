#!/usr/bin/env python3

import argparse
import shutil
import sys
import tempfile
from pathlib import Path


EXPECTED_VALUE_FUNCTION = 7776257.30184


def write_global_file(path: Path, input_dir: Path, output_dir: Path) -> None:
    path.write_text(
        "\n".join(
            [
                "SYSTEMNAME uTAHPS",
                f"INPUTDIR {input_dir}/",
                "ACTIONFILE actions.txt",
                "INFLOWFILE inflow.txt",
                "PRICEFILE pricefile.txt",
                "TOPOLOGYFILE topology.txt",
                "STARTSTATEFILE start_state.txt",
                "DT 3600",
                "DT_LAST 3600",
                "OUTPUTFILE output_utahps.txt",
                "OUTSTATEFILE outstate_utahps.txt",
                "WRITE_NODEFILES 0",
                f"OUTPUTDIR {output_dir}/",
                "",
            ]
        )
    )


def run_cppyy_simulation(source_dir: Path, library: Path) -> None:
    import cppyy

    fixture_dir = source_dir / "src_tests" / "utahps_test"
    run_dir = Path(tempfile.mkdtemp(prefix="herss_cppyy_interop_"))
    output_dir = run_dir / "output"
    output_dir.mkdir()

    try:
        global_file = run_dir / "global.txt"
        write_global_file(global_file, fixture_dir, output_dir)

        cppyy.add_include_path(str(source_dir / "src"))
        cppyy.include("herss.h")
        cppyy.load_library(str(library))

        gc = cppyy.gbl.GlobalConfig()
        gc.globalfile = str(global_file)
        gc.readGlobalFile()
        gc.SetDirectoriesAndFilenames()
        gc.Diagnose()
        gc.checkNrSteps()
        gc.write_nodefiles = False

        data = cppyy.gbl.Dataset(gc)
        data.readAllData()

        herss = cppyy.gbl.Herss(gc)
        herss.prepaireSimulation(data)
        herss.Simulate()
        herss.CheckWaterBalance()
        herss.GlobalWaterBalance()
        herss.CalcAdjustmenCosts()

        value_function = herss.rs.CalcVF(data.restprice)
        if abs(value_function - EXPECTED_VALUE_FUNCTION) > 1e-2:
            raise AssertionError(
                f"cppyy value function changed: {value_function:.5f} != {EXPECTED_VALUE_FUNCTION:.5f}"
            )

        if herss.rs.nodes[0].S != herss.scen[0]:
            raise AssertionError("cppyy legacy node/scenario pointer interop changed")

        if data.price[0] <= 0.0 or data.inflow[0][0] <= 0.0:
            raise AssertionError("cppyy dataset raw views are not populated")

        print(f"cppyy uTAHPS value function = {value_function:.5f}")
    finally:
        shutil.rmtree(run_dir, ignore_errors=True)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source-dir", required=True, type=Path)
    parser.add_argument("--library", required=True, type=Path)
    args = parser.parse_args()

    run_cppyy_simulation(args.source_dir.resolve(), args.library.resolve())
    return 0


if __name__ == "__main__":
    sys.exit(main())
