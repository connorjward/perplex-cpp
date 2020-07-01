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

#include <perplexcpp/base.h>


namespace perplexcpp
{
  /**
   * A struct containing the outputs from a call to minimize().
   */
  struct MinimizeResult
  {
    /**
     * The resulting phases.
     */
    std::vector<Phase> phases;


    /**
     * The bulk composition.
     */
    std::vector<CompositionComponent> composition;

    /**
     * The total number of moles in the bulk composition.
     */
    double n_moles;

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
       * The number of phases.
       */
      const size_t n_phases;


      /**
       * The phase names.
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


      // Disable copy constructors because the object is a singleton. 
      // These are public to improve error messages.
      // (source: https://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
      Wrapper(Wrapper const&) = delete;
      void operator=(Wrapper const&) = delete;

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
