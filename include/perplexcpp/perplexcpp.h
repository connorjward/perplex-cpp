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

#ifndef PERPLEXCPP_PERPLEXCPP_H
#define PERPLEXCPP_PERPLEXCPP_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>


namespace perplexcpp
{
  /**
   * A struct containg phase information.
   */
  struct Phase
  {
    /**
     * The name of the phase used by Perple_X.
     */
    std::string standard_name;


    /**
     * The shortened form of the phase name.
     */
    std::string abbreviated_name;


    /**
     * The long form of the phase name.
     */
    std::string full_name;


    /**
     * The fractional amount of the phase (by weight).
     */
    double weight_frac;


    /**
     * The fractional amount of the phase (by volume).
     */
    double vol_frac;


    /**
     * The fractional amount of the phase (by molar amount).
     */
    double mol_frac;


    /**
     * The amount of the phase (mol).
     */
    double amount;


    /**
     * The phase composition.
     */
    std::vector<double> composition;
  };


  /**
   * Initialize Perple_X.
   *
   * @param problem_file The Perple_X problem definition file.
   * @param working_dir  The directory containing the Perple_X files. If
   *                     not provided defaults to the current directory.
   */
  void initialize(const std::string& problem_file, 
			 const std::string& working_dir=".");

  /**
   * Perform the minimization using MEEMUM. The composition in use is the
   * initial composition specified in the Perple_X problem definition file.
   *
   * @param pressure    The pressure (Pa).
   * @param temperature The temperature (K).
   * @param composition The bulk composition. Each element in the vector 
   *                    corresponds to the amount, in moles, of a
   *                    composition component.
   */
  void minimize(const double pressure, 
		const double temperature,
		const std::vector<double>& composition);


  /**
   * Perform the minimization using MEEMUM. The composition in use is the
   * initial composition specified in the Perple_X problem definition file.
   *
   * @param pressure    The pressure (Pa).
   * @param temperature The temperature (K).
   */
  void minimize(const double pressure, const double temperature);


  /**
   * The number of composition components.
   */
  size_t n_composition_components();


  /**
   * The names of the composition components.
   */
  std::vector<std::string> composition_component_names();


  /**
   * The initial bulk composition.
   */
  std::vector<double> initial_composition();


  /**
   * The number of phases.
   */
  size_t n_phases();


  /**
   * @return The total number of moles of substance.
   */
  double get_n_moles();


  /**
   * @return The phase results of a minimization.
   */
  std::vector<Phase> phases();


  /**
   * Find the phase index for a given phase name.
   *
   * @param name The phase name
   */
  size_t find_phase_index_from_name(const std::string& name);

  /**
   * @return The system density (kg/m3).
   */
  double get_system_density();

  /**
   * @return The system expansivity (1/K).
   */
  double get_system_expansivity();

  /**
   * @return The system molar entropy (J/K).
   */
  double get_system_molar_entropy();

  /**
   * @return The system molar heat capacity (J/K).
   */
  double get_system_molar_heat_capacity();
}

#endif
