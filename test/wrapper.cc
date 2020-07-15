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

      Wrapper::initialize(problem_file, working_dir, 10, 0.1);
      Wrapper& wrapper = Wrapper::get_instance();

      const double pressure = utils::convert_bar_to_pascals(20000);
      const double temperature = 1500;

      result = wrapper.minimize(pressure, temperature);
    }

    
    MinimizeResult result;
};



TEST_F(WrapperSimpleDataTest, CheckNCompositionComponents)
{
  EXPECT_EQ(Wrapper::get_instance().n_composition_components, 4);
}


TEST_F(WrapperSimpleDataTest, CheckCompositionComponentNames)
{
  auto names = Wrapper::get_instance().composition_component_names;

  ASSERT_EQ(names.size(), 4);

  EXPECT_STREQ(names[0].c_str(), "SiO2");
  EXPECT_STREQ(names[1].c_str(), "CaO");
  EXPECT_STREQ(names[2].c_str(), "MgO");
  EXPECT_STREQ(names[3].c_str(), "FeO");
}


TEST_F(WrapperSimpleDataTest, CheckInitialBulkComposition)
{
  auto comp = Wrapper::get_instance().initial_bulk_composition;

  ASSERT_EQ(comp.size(), 4);

  EXPECT_NEAR(comp[0], 38.500, 5e-4);
  EXPECT_NEAR(comp[1], 2.820, 5e-4);
  EXPECT_NEAR(comp[2], 50.500, 5e-4);
  EXPECT_NEAR(comp[3], 5.880, 5e-4);
}


TEST_F(WrapperSimpleDataTest, CheckNPhases) 
{
  EXPECT_EQ(Wrapper::get_instance().n_phases, 4);
}


TEST_F(WrapperSimpleDataTest, CheckPhaseNames)
{
  auto names = Wrapper::get_instance().phase_names;

  ASSERT_EQ(names.size(), 4);

  EXPECT_STREQ(names[0].standard.c_str(), "Cpx(HGP)");
  EXPECT_STREQ(names[0].abbreviated.c_str(), "Cpx");
  EXPECT_STREQ(names[0].full.c_str(), "clinopyroxene");

  EXPECT_STREQ(names[1].standard.c_str(), "melt(HGP)");
  EXPECT_STREQ(names[1].abbreviated.c_str(), "Melt");
  EXPECT_STREQ(names[1].full.c_str(), "liquid");

  EXPECT_STREQ(names[2].standard.c_str(), "O(HGP)");
  EXPECT_STREQ(names[2].abbreviated.c_str(), "Ol");
  EXPECT_STREQ(names[2].full.c_str(), "olivine");

  EXPECT_STREQ(names[3].standard.c_str(), "Opx(HGP)");
  EXPECT_STREQ(names[3].abbreviated.c_str(), "Opx");
  EXPECT_STREQ(names[3].full.c_str(), "orthopyroxene");
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultComposition)
{
  auto comp = result.composition;

  ASSERT_EQ(comp.size(), 4);

  EXPECT_NEAR(comp[0], 38.500, 5e-4);
  EXPECT_NEAR(comp[1], 2.820, 5e-4);
  EXPECT_NEAR(comp[2], 50.500, 5e-4);
  EXPECT_NEAR(comp[3], 5.880, 5e-4);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseNames)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_STREQ(phases[0].name.standard.c_str(), "Cpx(HGP)");
  EXPECT_STREQ(phases[0].name.abbreviated.c_str(), "Cpx");
  EXPECT_STREQ(phases[0].name.full.c_str(), "clinopyroxene");

  EXPECT_STREQ(phases[1].name.standard.c_str(), "melt(HGP)");
  EXPECT_STREQ(phases[1].name.abbreviated.c_str(), "Melt");
  EXPECT_STREQ(phases[1].name.full.c_str(), "liquid");

  EXPECT_STREQ(phases[2].name.standard.c_str(), "O(HGP)");
  EXPECT_STREQ(phases[2].name.abbreviated.c_str(), "Ol");
  EXPECT_STREQ(phases[2].name.full.c_str(), "olivine");

  EXPECT_STREQ(phases[3].name.standard.c_str(), "Opx(HGP)");
  EXPECT_STREQ(phases[3].name.abbreviated.c_str(), "Opx");
  EXPECT_STREQ(phases[3].name.full.c_str(), "orthopyroxene");
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseWeightFractions)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[0].weight_frac*100, 13.44, 5e-3);
  EXPECT_NEAR(phases[1].weight_frac*100, 0.00, 5e-3);
  EXPECT_NEAR(phases[2].weight_frac*100, 62.02, 5e-3);
  EXPECT_NEAR(phases[3].weight_frac*100, 24.54, 5e-3);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseVolumeFractions)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[0].volume_frac*100, 13.56, 5e-3);
  EXPECT_NEAR(phases[1].volume_frac*100, 0.00, 5e-3);
  EXPECT_NEAR(phases[2].volume_frac*100, 61.68, 5e-3);
  EXPECT_NEAR(phases[3].volume_frac*100, 24.75, 5e-3);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseMolarFractions)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[0].molar_frac*100, 10.36, 5e-3);
  EXPECT_NEAR(phases[1].molar_frac*100, 0.0, 5e-3);
  EXPECT_NEAR(phases[2].molar_frac*100, 69.931, 5e-3);
  EXPECT_NEAR(phases[3].molar_frac*100, 19.70, 5e-3);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseAmounts)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[0].n_moles, 3.07, 5e-3);
  EXPECT_NEAR(phases[1].n_moles, 0.0, 5e-3);
  EXPECT_NEAR(phases[2].n_moles, 20.7, 5e-2);
  EXPECT_NEAR(phases[3].n_moles, 5.83, 5e-3);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultPhaseCompositionRatio)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[1].composition_ratio[1], 0.00000, 5e-6);
  EXPECT_NEAR(phases[2].composition_ratio[2], 1.77645, 5e-6);
  EXPECT_NEAR(phases[3].composition_ratio[3], 0.17159, 5e-6);
}

TEST_F(WrapperSimpleDataTest, CheckMinResultPhaseDensity)
{
  auto phases = result.phases;

  ASSERT_EQ(phases.size(), 4);

  EXPECT_NEAR(phases[0].density, 3220.916, 5e-4);
  EXPECT_NEAR(phases[1].density, 0.0, 1e-8);
  EXPECT_NEAR(phases[2].density, 3266.898, 5e-4);
  EXPECT_NEAR(phases[3].density, 3221.036, 5e-4);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultDensity)
{
  EXPECT_NEAR(result.density, 3249.3, 0.05);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultExpansivity)
{
  EXPECT_NEAR(result.expansivity, 0.38575e-4, 5e-9);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultMolarEntropy)
{
  EXPECT_NEAR(result.molar_entropy, 11996, 0.5);
}


TEST_F(WrapperSimpleDataTest, CheckMinimizeResultMolarHeatCapacity)
{
  EXPECT_NEAR(result.molar_heat_capacity, 6244.7, 0.05);
}



TEST_F(WrapperSimpleDataTest, CheckResultCompositionsSumToBulkComposition)
{
  auto& perplex_wrapper = Wrapper::get_instance();

  double phase_sum = 0.0;
  for (size_t p = 0; p < perplex_wrapper.n_phases; p++)
    for (size_t c = 0; c < perplex_wrapper.n_composition_components; c++)
      phase_sum += result.phases[p].n_moles * result.phases[p].composition_ratio[c];

  double bulk_sum = 0.0;
  for (size_t c = 0; c < perplex_wrapper.n_composition_components; c++)
    bulk_sum += perplex_wrapper.initial_bulk_composition[c];

  EXPECT_NEAR(phase_sum, bulk_sum, 1e-8);
}
