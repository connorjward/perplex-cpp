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
#include <perplexcpp/result_cache.h>


namespace perplexcpp
{
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
	                     const std::string& working_dir=".",
			     const size_t cache_capacity=0, 
			     const double cache_rtol=0.0);

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
      const std::vector<double> initial_composition;


      /**
       * The number of phases.
       */
      const size_t n_phases;


      /**
       * The phase names.
       */
      const std::vector<PhaseName> phase_names;


      /**
       * The minimum pressure accepted (Pa).
       */
      const double min_pressure;

      /**
       * The maximum pressure accepted (Pa).
       */
      const double max_pressure;


      /**
       * The minimum temperature accepted (K).
       */
      const double min_temperature;


      /**
       * The maximum temperature accepted (K).
       */
      const double max_temperature;


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
	       const std::vector<double>& composition) const;


      /**
       * Perform the minimization using MEEMUM. The composition in use is the
       * initial composition specified in the Perple_X problem definition file.
       *
       * @param pressure    The pressure (Pa).
       * @param temperature The temperature (K).
       */
      MinimizeResult 
      minimize(const double pressure, const double temperature) const;


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
       * ???
       */
      static size_t cache_capacity;


      /**
       * ???
       */
      static double cache_rtol;


      /**
       * A LRU cache to store the results of previous computations.
       */
      mutable ResultCache cache;


      /**
       * Construct the class.
       *
       * @remark This constructor is private to enforce the singleton pattern.
       */
      Wrapper();
  };
}

#endif
