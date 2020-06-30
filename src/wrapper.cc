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

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include <perplexcpp/utils.h>

#include "c_interface.h"


namespace perplexcpp
{
  namespace
  {
    /**
     * Flag indicating whether or not Perple_X has been initialized.
     */
    bool initialized_ = false;


    /**
     * A boolean indicating whether minimize() has been called.
     */
    bool minimized_ = false;


    std::vector<double> initial_composition_;

    /**
     * @return The number of end phases.
     *
     * @remark This method is necessary because Perple_X differentiates between
     *         solution and end phases so the number of the latter can change.
     */
    size_t get_n_end_phases()
    {
      return res_phase_props_get_n();
    }

    /**
     * @return A map where the keys are the solution phase indices and
     *         the values are the corresponding end phase indices.
     *
     * @remark This is necessary because Perple_X differentiates between the
     *         end phases and solution phases and their indexing is different.
     *
     */
    const std::unordered_map<size_t,size_t>& phase_index_mapping()
    {
      static std::unordered_map<size_t,size_t> idx_map;

      idx_map.clear();

      for (size_t i = 0; i < get_n_end_phases(); ++i) {
	std::string phase_name = res_phase_props_get_name(i);

	// If the Perple_X models are poorly suited to the problem at hand they may
	// sometimes return phases that are not among the solution models (e.g. faTL).
	// These phases are often present in extremely small amounts and so can be
	// disregarded. However, if you are seeing lots of error messages that implies
	// that you need to edit your parameter files.
	try {
	  idx_map.emplace(find_phase_index_from_name(phase_name), i);
	} catch (const std::invalid_argument& e) {
	  std::cerr << e.what() << std::endl;
	  std::cerr << phase_name << " : " << res_phase_props_get_mol_frac(i) << std::endl;
	}
      }
      return idx_map;
    }
    /**
     * Read some phase quantity into an array.
     *
     * @param get_quantity A function pointer to the Perple_X interface function.
     * @param out          The output array that results are read into.
     */
    double find_phase_quantity(const size_t phase_index,
			       double (*quantity)(size_t))
    {
      // Check to see if the solution phase is present in the end phases.
      // If it is, return the quantity. If not, return zero.
      auto map = phase_index_mapping();
      if (map.find(phase_index) != map.end())
	return quantity(map[phase_index]);
      else
	return 0.0;
    }


    /**
     * doc
     */
    double phase_weight_frac(const size_t phase_index)
    {
      return find_phase_quantity(phase_index, res_phase_props_get_weight_frac);
    }


    /**
     * doc
     */
    double phase_vol_frac(const size_t phase_index)
    {
      return find_phase_quantity(phase_index, res_phase_props_get_vol_frac);
    }


    /**
     * doc
     */
    double phase_mol_frac(const size_t phase_index)
    {
      return find_phase_quantity(phase_index, res_phase_props_get_mol_frac);
    }


    /**
     * doc
     */
    double phase_amount(const size_t phase_index)
    {
      return find_phase_quantity(phase_index, res_phase_props_get_mol);
    }


    /**
     * @return The composition of a phase.
     */
    std::vector<double> phase_composition(const size_t phase_index)
    {
      std::vector<double> composition;

      // Check to see if the solution phase is present in the end phases.
      // If it is, return the quantity. If not, return zero.
      auto map = phase_index_mapping();
      if (map.find(phase_index) != map.end())
	for (size_t i = 0; i < n_composition_components(); ++i)
	  composition.push_back(res_phase_props_get_composition(map[phase_index], i));
      else
	composition.push_back(0.0);
      return composition;
    }

    void check_initialized()
    {
      if (!initialized_)
	throw std::logic_error("Perple_X has not been initialized.");
    }

    /**
     * Check if minimize() has been called and throw an exception if not.
     */
    void check_minimized()
    {
      if (!minimized_)
	throw std::logic_error("minimize() has not been called yet.");
    }


  }


  void initialize(const std::string& problem_file, 
			   const std::string& working_dir)
  {
    // Save the current working directory.
    char initial_dir[256];
    if (getcwd(initial_dir, sizeof(initial_dir)) == NULL)
      throw std::runtime_error("Could not get the current directory.");

    // Change working directory to the location of the Perple_X files.
    if (chdir(working_dir.c_str()) != 0)
      throw std::invalid_argument("Could not change directory.");

#ifndef ALLOW_PERPLEX_OUTPUT
    // Disable stdout to prevent Perple_X dominating stdout.
    const int fd = utils::disable_stdout();
#endif

    // Check that the problem file ends in '.dat' and then strip it before passing it
    // to Perple_X.
    size_t suffix = problem_file.rfind(".");
    if (problem_file.substr(suffix) != ".dat")
      throw std::invalid_argument("Problem file given does not end in '.dat'.");
    solver_init(problem_file.substr(0, suffix).c_str());

#ifndef ALLOW_PERPLEX_OUTPUT
    utils::enable_stdout(fd);
#endif

    // Return to the original working directory.
    if (chdir(initial_dir) != 0)
      throw std::invalid_argument("Could not change directory.");

    // Save the initial composition.
    for (size_t i = 0; i < composition_props_get_n_components(); ++i)
      initial_composition_.push_back(bulk_props_get_composition(i));

    // Save that initialization is complete.
    initialized_ = true;
  }


  void minimize(const double pressure, 
                         const double temperature,
			 const std::vector<double>& composition)
  {
    check_initialized();

    if (composition.size() != n_composition_components())
      throw std::invalid_argument("Specified bulk composition is the wrong size.");

    for (size_t i = 0; i < n_composition_components(); ++i)
      bulk_props_set_composition(i, composition[i]);

    solver_set_pressure(utils::convert_pascals_to_bar(pressure));
    solver_set_temperature(temperature);

#ifndef ALLOW_PERPLEX_OUTPUT
    // Disable stdout to prevent Perple_X dominating stdout.
    const int fd = utils::disable_stdout();
#endif

    solver_minimize();

#ifndef ALLOW_PERPLEX_OUTPUT
    utils::enable_stdout(fd);
#endif

    // Save that the minimization is complete.
    minimized_ = true;
  }


  void minimize(const double pressure, const double temperature)
  {
    check_initialized();

    minimize(pressure, temperature, initial_composition_);
  }

  size_t n_composition_components()
  {
    return composition_props_get_n_components();
  }


  std::vector<std::string> composition_component_names()
  {
    std::vector<std::string> names;
    for (size_t i = 0; i < composition_props_get_n_components(); ++i)
      names.push_back(std::string(composition_props_get_name(i)));
    return names;
  }


  std::vector<double> initial_composition()
  {
    return initial_composition_;
  }


  size_t n_phases()
  {
    return soln_phase_props_get_n();
  }

  double get_n_moles()
  {
    double n_moles = 0.0;
    for (double composition_component : initial_composition_)
      n_moles += composition_component;
    return n_moles;
  }


  std::vector<Phase> phases()
  {
    check_minimized();

    std::vector<Phase> phases;
    for (size_t i = 0; i < n_phases(); ++i) {
      Phase phase {
	soln_phase_props_get_name(i),       // standard_name
	soln_phase_props_get_abbr_name(i),  // abbreviated_name
	soln_phase_props_get_full_name(i),  // full_name
	phase_weight_frac(i),               // weight_frac
	phase_vol_frac(i),                  // vol_frac
	phase_mol_frac(i),                  // mol_frac
	phase_amount(i),                    // amount
	phase_composition(i)                // composition
      };

      phases.push_back(phase);
    }
    return phases;
  }


  size_t find_phase_index_from_name(const std::string& phase_name)
  {
    for (size_t i = 0; i < n_phases(); ++i) {
      if (phase_name == soln_phase_props_get_name(i) ||
	  phase_name == soln_phase_props_get_abbr_name(i) || 
	  phase_name == soln_phase_props_get_full_name(i))
	return i;
    }
    throw std::invalid_argument("The phase name was not found.");
  }


  double get_system_density()
  {
    return sys_props_get_density();
  }


  double get_system_expansivity()
  {
    return sys_props_get_expansivity();
  }


  double get_system_molar_entropy()
  {
    return sys_props_get_mol_entropy();
  }
  

  double get_system_molar_heat_capacity()
  {
    return sys_props_get_mol_heat_capacity();
  }
}
