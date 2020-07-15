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


#ifndef PERPLEXCPP_BASE_H
#define PERPLEXCPP_BASE_H


#include <string>
#include <vector>


namespace perplexcpp
{
  /**
   * A struct containing phase name information. In Perple_X, phases are represented
   * with 3 name: one specific to Perple_X, an abbreviated version and a full version.
   */
  struct PhaseName
  {
    /**
     * The name of the phase used by Perple_X.
     */
    std::string standard;


    /**
     * The shortened form of the phase name.
     */
    std::string abbreviated;


    /**
     * The long form of the phase name.
     */
    std::string full;
  };



  /**
   * A struct containg phase information.
   */
  struct Phase
  {
    /**
     * Corresponds to the phase index.
     */
    size_t id;


    /**
     * The phase name.
     */
    PhaseName name;


    /**
     * The fractional amount of the phase (by weight).
     */
    double weight_frac;


    /**
     * The fractional amount of the phase (by volume).
     */
    double volume_frac;


    /**
     * The fractional amount of the phase (by molar amount).
     */
    double molar_frac;


    /**
     * The amount of the phase (mol).
     */
    double n_moles;


    /**
     * The number of moles of each composition component per mole of phase. 
     *
     * @remark To find the amount of each composition component this must be
     *         scaled by the amount of phase.
     */
    std::vector<double> composition_ratio;


  };



  /**
   * @param
   *
   * @return
   */
  Phase find_phase(const std::vector<Phase> phases, const std::string name);



  /**
   * A struct containing the outputs from a call to minimize().
   */
  struct MinimizeResult
  {
    /**
     * The pressure (Pa).
     */
    double pressure;


    /**
     * The temperature (K).
     */
    double temperature;


    /**
     * The bulk composition.
     */
    std::vector<double> composition;


    /**
     * The resulting phases.
     */
    std::vector<Phase> phases;


    /**
     * The density (kg/m3).
     */
    double density;

    /**
     * The expansivity (1/K).
     */
    double expansivity;

    /**
     * The molar entropy (J/K).
     */
    double molar_entropy;

    /**
     * The molar heat capacity (J/K).
     */
    double molar_heat_capacity;
  };
}

#endif
