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
       * @return The singleton instance of the wrapper.
       */
      static Wrapper& get_instance();

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
       * @return The number of composition components.
       */
      size_t get_n_composition_components() const;

      /**
       * @return The composition component names.
       */
      const std::vector<std::string>& get_composition_component_names() const;

      /**
       * @return The total number of moles of substance.
       */
      double get_n_moles() const;

      /**
       * @return The number of phases.
       */
      size_t get_n_phases() const;

      /**
       * @return The standard phase names.
       */
      const std::vector<std::string>& get_phase_names() const;

      /**
       * @return The abbreviated phase names.
       */
      const std::vector<std::string>& get_abbr_phase_names() const;

      /**
       * @return The full phase names.
       */
      const std::vector<std::string>& get_full_phase_names() const;

      /**
       * @return The phase weight fractions.
       */
      const std::vector<double>& get_phase_weight_fracs() const;

      /**
       * @return The phase volume fractions.
       */
      const std::vector<double>& get_phase_vol_fracs() const;

      /**
       * @return The phase molar fractions.
       */
      const std::vector<double>& get_phase_mol_fracs() const;

      /**
       * @return The phase amounts (mol).
       */
      const std::vector<double>& get_phase_amounts() const;

      /**
       * @return The phase compositions.
       */
      const std::vector<std::vector<double>>& get_phase_compositions() const;

      /**
       * Find the phase index for a given phase name.
       *
       * @param name The phase name
       */
      size_t find_phase_index_from_name(const std::string& name) const;

      /**
       * @return The system density (kg/m3).
       */
      double get_system_density() const;

      /**
       * @return The system expansivity (1/K).
       */
      double get_system_expansivity() const;

      /**
       * @return The system molar entropy (J/K).
       */
      double get_system_molar_entropy() const;

      /**
       * @return The system molar heat capacity (J/K).
       */
      double get_system_molar_heat_capacity() const;

    private:
      /**
       * A boolean indicating whether initialize() has been called.
       */
      bool initialized = false;

      /**
       * A boolean indicating whether minimize() has been called.
       */
      bool minimized = false;

      /**
       * The initial bulk composition specified in the file.
       */
      std::vector<double> initial_bulk_composition;

      /**
       * Construct the class.
       *
       * @remark This constructor is private to enforce the singleton pattern.
       */
      Wrapper() {};

      /**
       * Check if initialize() has been called and throw an exception if not.
       */
      void check_initialized() const;

      /**
       * Check if minimize() has been called and throw an exception if not.
       */
      void check_minimized() const;

      /**
       * @return The number of end phases.
       *
       * @remark This method is necessary because Perple_X differentiates between
       *         solution and end phases so the number of the latter can change.
       */
      size_t get_n_end_phases() const;

      /**
       * Read some phase quantity into an array.
       *
       * @param get_quantity A function pointer to the Perple_X interface function.
       * @param out          The output array that results are read into.
       */
      void load_phase_quantity(double (*get_quantity)(size_t),
	                       std::vector<double>& out) const;

      /**
       * @return A map where the keys are the solution phase indices and
       *         the values are the corresponding end phase indices.
       *
       * @remark This is necessary because Perple_X differentiates between the
       *         end phases and solution phases and their indexing is different.
       *
       */
      const std::unordered_map<size_t,size_t>& get_phase_index_mapping() const;
  };
}

#endif
