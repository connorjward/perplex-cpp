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


#ifndef PERPLEXCPP_WRAPPER_H
#define PERPLEXCPP_WRAPPER_H


#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>


namespace perplexcpp
{
  /**
   * A struct representing a composition component.
   */
  struct CompositionComponent
  {
    /**
     * The chemical name of the component.
     */
    std::string name;


    /**
     * The molar amount of the component.
     */
    double amount;
  };



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
    std::vector<CompositionComponent> composition;
  };



  struct MinimizeResult
  {
    /**
     * ???
     */
    std::vector<Phase> phases;


    /**
     * ???
     */
    double n_moles;

    /**
     * ???
     */
    double density;

    /**
     * ???
     */
    double expansivity;

    /**
     * ???
     */
    double molar_entropy;

    /**
     * ???
     */
    double molar_heat_capacity;
  };



  /**
   * A class that controls the access to the underlying Perple_X calculations
   * and results. It utilises the singleton design pattern (only a single
   * instance is ever created) because Perple_X relies heavily on global 
   * variables (COMMON blocks) so care must be taken to avoid concurrent access 
   * to the resources.
   */
  class Wrapper
  {
    public:

      /**
       * Initialize Perple_X.
       *
       * @param problem_file The Perple_X problem definition file.
       * @param working_dir  The directory containing the Perple_X files. If
       *                     not provided defaults to the current directory.
       */
      static void initialize(const std::string& problem_file, 
	                     const std::string& working_dir=".");


      /**
       * @return The singleton instance of the wrapper.
       */
      static Wrapper& get_instance();


      /**
       * The number of composition components.
       */
      const size_t n_composition_components;


      /**
       * The names of the composition components.
       */
      const std::vector<std::string> composition_component_names;


      /**
       * The initial bulk composition.
       */
      const std::vector<CompositionComponent> initial_composition;


      /**
       * ???
       */
      const size_t n_phases;


      /**
       * ???
       */
      const std::vector<PhaseName> phase_names;


      /**
       * Perform the minimization using MEEMUM. 
       *
       * @param pressure    The pressure (Pa).
       * @param temperature The temperature (K).
       * @param composition The bulk composition. 
       */
      MinimizeResult 
      minimize(const double pressure, 
	       const double temperature,
	       const std::vector<CompositionComponent>& composition) const;


      /**
       * Perform the minimization using MEEMUM. The composition in use is the
       * initial composition specified in the Perple_X problem definition file.
       *
       * @param pressure    The pressure (Pa).
       * @param temperature The temperature (K).
       */
      MinimizeResult 
      minimize(const double pressure, const double temperature) const;


      /**
       * Find the phase index for a given phase name.
       *
       * @param name The phase name
       */
      size_t find_phase_index_from_name(const std::string& name) const;


    private:

      /**
       * Flag indicating whether or not the wrapper has been initialized.
       */
      static bool initialized;


      /**
       * Construct the class.
       *
       * @remark This constructor is private to enforce the singleton pattern.
       */
      Wrapper();
  };
}

#endif
