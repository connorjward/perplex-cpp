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
   * @param
   *
   * @return
   */
  Phase find_phase(const std::vector<Phase> phases, const std::string name);
}

#endif
