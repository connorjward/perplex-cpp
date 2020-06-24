/*
 * Copyright (C) 2020 Connor Ward.
 *
 * This file is part of PerpleX-cpp.
 *
 * PerpleX-cpp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PerpleX-cpp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PerpleX-cpp.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <perplexcpp/wrapper.h>

#include <gtest/gtest.h>
#include <perplexcpp/utils.h>

using namespace perplexcpp;

class WrapperSimpleDataTest : public ::testing::Test {
  protected:
    void SetUp() override {
      const std::string problem_file = "test.dat";
      const std::string working_dir = "./simple";
      const double pressure = utils::convert_bar_to_pascals(20000);
      const double temperature = 1500;
      std::vector<double> composition;
      composition.push_back(38.500);
      composition.push_back(2.820);
      composition.push_back(50.500); 
      composition.push_back(5.880);

      Wrapper wrap = Wrapper::get_instance();

      wrap.initialize(problem_file, working_dir);
      wrap.set_bulk_composition(composition);
      wrap.minimize(pressure, temperature);
    }
};

TEST_F(WrapperSimpleDataTest, CheckNCompositionComponents)
{
  EXPECT_EQ(Wrapper::get_instance().get_n_composition_components(), 4);
}

TEST_F(WrapperSimpleDataTest, CheckCompositionComponentNames)
{
  auto names = Wrapper::get_instance().get_composition_component_names();

  EXPECT_STREQ(names[0].c_str(), "SiO2");
  EXPECT_STREQ(names[1].c_str(), "CaO");
  EXPECT_STREQ(names[2].c_str(), "MgO");
  EXPECT_STREQ(names[3].c_str(), "FeO");
}

TEST_F(WrapperSimpleDataTest, CheckBulkComposition)
{
  auto composition = Wrapper::get_instance().get_bulk_composition();

  EXPECT_NEAR(composition[0], 38.500, 5e-4);
  EXPECT_NEAR(composition[1], 2.820, 5e-4);
  EXPECT_NEAR(composition[2], 50.500, 5e-4);
  EXPECT_NEAR(composition[3], 5.880, 5e-4);
}

TEST_F(WrapperSimpleDataTest, CheckNPhases) 
{
  EXPECT_EQ(Wrapper::get_instance().get_n_phases(), 4);
}

TEST_F(WrapperSimpleDataTest, CheckPhaseNames)
{
  auto names = Wrapper::get_instance().get_phase_names();

  EXPECT_STREQ(names[0].c_str(), "Cpx(HGP)");
  EXPECT_STREQ(names[1].c_str(), "melt(HGP)");
  EXPECT_STREQ(names[2].c_str(), "O(HGP)");
  EXPECT_STREQ(names[3].c_str(), "Opx(HGP)");
}

TEST_F(WrapperSimpleDataTest, CheckAbbrPhaseNames)
{
  auto names = Wrapper::get_instance().get_abbr_phase_names();

  EXPECT_STREQ(names[0].c_str(), "Cpx");
  EXPECT_STREQ(names[1].c_str(), "Melt");
  EXPECT_STREQ(names[2].c_str(), "Ol");
  EXPECT_STREQ(names[3].c_str(), "Opx");
}

TEST_F(WrapperSimpleDataTest, CheckFullPhaseNames)
{
  auto names = Wrapper::get_instance().get_full_phase_names();

  EXPECT_STREQ(names[0].c_str(), "clinopyroxene");
  EXPECT_STREQ(names[1].c_str(), "liquid");
  EXPECT_STREQ(names[2].c_str(), "olivine");
  EXPECT_STREQ(names[3].c_str(), "orthopyroxene");
}

TEST_F(WrapperSimpleDataTest, CheckPhaseWeightFracs)
{
  auto fracs = Wrapper::get_instance().get_phase_weight_fracs();

  EXPECT_NEAR(fracs[0]*100, 13.44, 5e-3);
  EXPECT_NEAR(fracs[1]*100, 0.00, 5e-3);
  EXPECT_NEAR(fracs[2]*100, 62.02, 5e-3);
  EXPECT_NEAR(fracs[3]*100, 24.54, 5e-3);
}

TEST_F(WrapperSimpleDataTest, CheckPhaseVolFracs)
{
  auto fracs = Wrapper::get_instance().get_phase_vol_fracs();

  EXPECT_NEAR(fracs[0]*100, 13.56, 5e-3);
  EXPECT_NEAR(fracs[1]*100, 0.00, 5e-3);
  EXPECT_NEAR(fracs[2]*100, 61.68, 5e-3);
  EXPECT_NEAR(fracs[3]*100, 24.75, 5e-3);
}

TEST_F(WrapperSimpleDataTest, CheckPhaseMolFracs)
{
  auto fracs = Wrapper::get_instance().get_phase_mol_fracs();

  EXPECT_NEAR(fracs[0]*100, 10.36, 5e-3);
  EXPECT_NEAR(fracs[1]*100, 0.0, 5e-3);
  EXPECT_NEAR(fracs[2]*100, 69.931, 5e-3);
  EXPECT_NEAR(fracs[3]*100, 19.70, 5e-3);
}

TEST_F(WrapperSimpleDataTest, CheckPhaseAmounts) 
{
  auto amounts = Wrapper::get_instance().get_phase_amounts();

  EXPECT_NEAR(amounts[0], 3.07, 5e-3);
  EXPECT_NEAR(amounts[1], 0.0, 5e-3);
  EXPECT_NEAR(amounts[2], 20.7, 5e-2);
  EXPECT_NEAR(amounts[3], 5.83, 5e-3);
}

TEST_F(WrapperSimpleDataTest, CheckSystemDensity) {
  EXPECT_NEAR(Wrapper::get_instance().get_system_density(), 3249.3, 0.05);
}

TEST_F(WrapperSimpleDataTest, CheckSystemExpansivity) {
  EXPECT_NEAR(Wrapper::get_instance().get_system_expansivity(), 0.38575e-4, 5e-9);
}

TEST_F(WrapperSimpleDataTest, CheckSystemMolarEntropy) {
  EXPECT_NEAR(Wrapper::get_instance().get_system_molar_entropy(), 11996, 0.5);
}
 
TEST_F(WrapperSimpleDataTest, CheckSystemMolarHeatCapacity) {
  EXPECT_NEAR(Wrapper::get_instance().get_system_molar_heat_capacity(), 6244.7, 0.05);
} 
