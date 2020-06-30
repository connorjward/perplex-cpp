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

#include <perplexcpp/perplexcpp.h>

#include <gtest/gtest.h>
#include <perplexcpp/utils.h>


using namespace perplexcpp;


class SimpleDataTest : public ::testing::Test {
  protected:
    void SetUp() override {
      const std::string problem_file = "test.dat";
      const std::string working_dir = "./simple";

      initialize(problem_file, working_dir);

      const double pressure = utils::convert_bar_to_pascals(20000);
      const double temperature = 1500;
      const std::vector<double> composition = {
	38.500,
	2.820,
	50.500,
	5.880
      };

      minimize(pressure, temperature, composition);
    }
};


TEST_F(SimpleDataTest, CheckNCompositionComponents)
{
  EXPECT_EQ(n_composition_components(), 4);
}


TEST_F(SimpleDataTest, CheckCompositionComponentNames)
{
  auto names = composition_component_names();

  EXPECT_STREQ(names[0].c_str(), "SiO2");
  EXPECT_STREQ(names[1].c_str(), "CaO");
  EXPECT_STREQ(names[2].c_str(), "MgO");
  EXPECT_STREQ(names[3].c_str(), "FeO");
}


TEST_F(SimpleDataTest, CheckNPhases) 
{
  EXPECT_EQ(n_phases(), 4);
}

TEST_F(SimpleDataTest, CheckPhases)
{
  std::vector<Phase> phases = perplexcpp::phases();

  EXPECT_STREQ(phases[0].standard_name.c_str(), "Cpx(HGP)");
  EXPECT_STREQ(phases[0].abbreviated_name.c_str(), "Cpx");
  EXPECT_STREQ(phases[0].full_name.c_str(), "clinopyroxene");
  EXPECT_NEAR(phases[0].weight_frac*100, 13.44, 5e-3);
  EXPECT_NEAR(phases[0].vol_frac*100, 13.56, 5e-3);
  EXPECT_NEAR(phases[0].mol_frac*100, 10.36, 5e-3);
  EXPECT_NEAR(phases[0].amount, 3.07, 5e-3);

  EXPECT_STREQ(phases[1].standard_name.c_str(), "melt(HGP)");
  EXPECT_STREQ(phases[1].abbreviated_name.c_str(), "Melt");
  EXPECT_STREQ(phases[1].full_name.c_str(), "liquid");
  EXPECT_NEAR(phases[1].weight_frac*100, 0.00, 5e-3);
  EXPECT_NEAR(phases[1].vol_frac*100, 0.00, 5e-3);
  EXPECT_NEAR(phases[1].mol_frac*100, 0.0, 5e-3);
  EXPECT_NEAR(phases[1].amount, 0.0, 5e-3);

  EXPECT_STREQ(phases[2].standard_name.c_str(), "O(HGP)");
  EXPECT_STREQ(phases[2].abbreviated_name.c_str(), "Ol");
  EXPECT_STREQ(phases[2].full_name.c_str(), "olivine");
  EXPECT_NEAR(phases[2].weight_frac*100, 62.02, 5e-3);
  EXPECT_NEAR(phases[2].vol_frac*100, 61.68, 5e-3);
  EXPECT_NEAR(phases[2].mol_frac*100, 69.931, 5e-3);
  EXPECT_NEAR(phases[2].amount, 20.7, 5e-2);

  EXPECT_STREQ(phases[3].standard_name.c_str(), "Opx(HGP)");
  EXPECT_STREQ(phases[3].abbreviated_name.c_str(), "Opx");
  EXPECT_STREQ(phases[3].full_name.c_str(), "orthopyroxene");
  EXPECT_NEAR(phases[3].weight_frac*100, 24.54, 5e-3);
  EXPECT_NEAR(phases[3].vol_frac*100, 24.75, 5e-3);
  EXPECT_NEAR(phases[3].mol_frac*100, 19.70, 5e-3);
  EXPECT_NEAR(phases[3].amount, 5.83, 5e-3);
}

TEST_F(SimpleDataTest, CheckSystemDensity) {
  EXPECT_NEAR(get_system_density(), 3249.3, 0.05);
}

TEST_F(SimpleDataTest, CheckSystemExpansivity) {
  EXPECT_NEAR(get_system_expansivity(), 0.38575e-4, 5e-9);
}

TEST_F(SimpleDataTest, CheckSystemMolarEntropy) {
  EXPECT_NEAR(get_system_molar_entropy(), 11996, 0.5);
}
 
TEST_F(SimpleDataTest, CheckSystemMolarHeatCapacity) {
  EXPECT_NEAR(get_system_molar_heat_capacity(), 6244.7, 0.05);
} 
