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

#include <perplexcpp/base.h>
#include <perplexcpp/utils.h>

#include "c_interface.h"


namespace perplexcpp
{
  namespace
  {
    /**
     * @return The composition component names.
     */
    std::vector<std::string> make_composition_component_names()
    {
      std::vector<std::string> names;
      for (size_t i = 0; i < composition_props_get_n_components(); ++i)
	names.push_back(std::string(composition_props_get_name(i)));
      return names;
    }


    /**
     * @return The bulk composition.
     */
    std::vector<double> make_bulk_composition()
    {
      std::vector<double> bulk_composition;
      for (size_t i = 0; i < composition_props_get_n_components(); ++i)
	bulk_composition.push_back(bulk_props_get_composition(i));
      return bulk_composition;
    }


    /**
     * @param phase_index The phase index.
     *
     * @return The phase name.
     */
    PhaseName get_phase_name(const size_t phase_index)
    {
	return PhaseName {
	  soln_phase_props_get_name(phase_index),  // standard
	  soln_phase_props_get_abbr_name(phase_index),  // abbreviated
	  soln_phase_props_get_full_name(phase_index)  // full
	};
    }


    /**
     * @return The phase names.
     */
    std::vector<PhaseName> get_phase_names()
    {
      std::vector<PhaseName> names;
      for (size_t i = 0; i < soln_phase_props_get_n(); ++i)
	names.push_back(get_phase_name(i));
      return names;
    }


    /**
     * @param end_phase_index The index for the phase for the result phase array.
     *
     * @return The phase composition.
     */
    std::vector<double> get_phase_composition(const size_t end_phase_index)
    {
      std::vector<double> composition;
      for (size_t i = 0; i < composition_props_get_n_components(); ++i)
	composition.push_back(res_phase_props_get_composition(end_phase_index, i));
      return composition;
    }


    /**
     * Find the phase index for a given phase name.
     *
     * @param phase_name The phase name.
     *
     * @return The corresponding phase index.
     */
    size_t find_phase_index_from_name(const std::string& phase_name)
    {
      for (size_t i = 0; i < soln_phase_props_get_n(); ++i) {
	if (phase_name == get_phase_name(i).standard ||
	    phase_name == get_phase_name(i).abbreviated || 
	    phase_name == get_phase_name(i).full) {
	  return i;
	}
      }
      throw std::invalid_argument("The phase name '" +
	                          phase_name +
				  "' was not found among the solution models.");
    }


    /**
     * @return A map mapping the phase index in the solution array in Perple_X (key)
     *         with the phase index in the result array (value).
     */
    std::unordered_map<size_t,size_t> get_phase_index_mapping()
    {
      std::unordered_map<size_t,size_t> idx_map;

      for (size_t i = 0; i < res_phase_props_get_n(); ++i) {
	std::string phase_name = res_phase_props_get_name(i);

	// If the Perple_X models are poorly suited to the problem at hand they may
	// sometimes return phases that are not among the solution models (e.g. faTL).
	// These phases are often present in extremely small amounts and so can be
	// disregarded. However, if you are seeing lots of error messages or if the 
	// fraction of material that is unrecognised is large it implies
	// that you need to edit your parameter files.
	try {
	  idx_map.emplace(find_phase_index_from_name(phase_name), i);
	} 
	catch (const std::invalid_argument& e) {
	  std::cerr << e.what() 
	            << std::endl
	            << phase_name << " constitutes " << res_phase_props_get_mol_frac(i)*100
	            << "\% of the end phases. If this number is large you may need to "
	            << "edit your Perple_X problem definition file.";
	}
      }
      return idx_map;
    }

    std::vector<Phase> get_phases()
    {
      std::vector<Phase> phases;

      auto map = get_phase_index_mapping();
      for (size_t i = 0; i < soln_phase_props_get_n(); ++i) {
	Phase phase = { 
	  get_phase_name(i),  // name
	  0.0,  // weight_frac
	  0.0,  // vol_frac
	  0.0,  // mol_frac
	  0.0,  // amount
	  std::vector<double>(composition_props_get_n_components(), 0.0)  // composition
	};

	// Check to see if the solution phase is present in the end phases.
	// If they are then load the quantities.
	if (map.find(i) != map.end()) {
	  phase.weight_frac = res_phase_props_get_weight_frac(map[i]);
	  phase.vol_frac = res_phase_props_get_vol_frac(map[i]);
	  phase.mol_frac = res_phase_props_get_mol_frac(map[i]);
	  phase.amount = res_phase_props_get_mol(map[i]);
	  phase.composition = get_phase_composition(map[i]);
	}

	phases.push_back(phase);
      }
      return phases;
    }


    double get_n_moles()
    {
      double n = 0.0;
      for (size_t i = 0; i < res_phase_props_get_n(); ++i)
	n += res_phase_props_get_mol(i);
      return n;
    }
  }


  void Wrapper::initialize(const std::string& problem_file, 
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

    // Save that initialization is complete.
    initialized = true;
  }


  Wrapper& Wrapper::get_instance()
  {
    if (!initialized)
      throw std::logic_error("Perple_X has not been initialized.");

    static Wrapper instance;
    return instance;
  }


  MinimizeResult 
  Wrapper::minimize(const double pressure, 
                    const double temperature,
		    const std::vector<double>& composition) const
  {
    if (pressure <= 0.0)
      throw std::invalid_argument("The pressure must be a positive number.");

    if (temperature <= 0.0)
      throw std::invalid_argument("The temperature must be a positive number.");

    if (composition.size() != n_composition_components)
      throw std::invalid_argument("Specified bulk composition is the wrong size.");

    for (size_t i = 0; i < n_composition_components; ++i)
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

    return MinimizeResult {
      get_phases(),  // phases
      composition,  // composition
      get_n_moles(),  // n_moles
      sys_props_get_density(),  // density
      sys_props_get_expansivity(),  // expansivity
      sys_props_get_mol_entropy(),  // molar_entropy
      sys_props_get_mol_heat_capacity()  // molar_heat_capacity
    };
  }


  MinimizeResult
  Wrapper::minimize(const double pressure, const double temperature) const
  {
    return minimize(pressure, temperature, this->initial_composition);
  }


  // Static variables cannot be instantiated in the header file.
  bool Wrapper::initialized = false;


  Wrapper::Wrapper() 
  : n_composition_components(composition_props_get_n_components()),
    composition_component_names(make_composition_component_names()),
    initial_composition(make_bulk_composition()),
    n_phases(soln_phase_props_get_n()),
    phase_names(get_phase_names())
  {}
}
