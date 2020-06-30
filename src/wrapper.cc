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

#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

#include <perplexcpp/wrapper.h>
#include <perplexcpp/utils.h>

#include "c_interface.h"

namespace
{
  /**
   * @return The file descriptor pointing to stdout.
   */
  const int disable_stdout() 
  {
    // Flush stdout.
    fflush(stdout); 

    // Get file descriptors.
    const int stdout_descriptor = dup(1);
    const int null_descriptor = open("/dev/null", O_WRONLY);

    // Reassign stdout to /dev/null.
    dup2(null_descriptor, 1);
    close(null_descriptor);

    return stdout_descriptor;
  }

  /**
   * @param stdout_descriptor The file descriptor pointing to stdout.
   */
  void enable_stdout(const int stdout_descriptor) 
  {
    // Flush stdout.
    fflush(stdout);

    // Reassign descriptor.
    dup2(stdout_descriptor, 1);
    close(stdout_descriptor);
  }
}

namespace perplexcpp
{
  Wrapper& Wrapper::get_instance()
  {
    static Wrapper instance;
    return instance;
  }

  void Wrapper::initialize(const std::string& problem_file, 
     	                   const std::string& working_dir)
  {
    // Save the current working directory then change it to the location of the Perple_X files.
    char initial_dir[256];
    if (getcwd(initial_dir, sizeof(initial_dir)) == NULL)
      throw std::runtime_error("Could not get the current directory.");
    
    if (chdir(working_dir.c_str()) != 0)
      throw std::invalid_argument("Could not change directory.");

#ifndef ALLOW_PERPLEX_OUTPUT
    // Disable stdout to prevent Perple_X dominating stdout.
    const int fd = disable_stdout();
#endif

    // Check that the problem file ends in '.dat' and then strip it before passing it
    // to Perple_X.
    size_t extension = problem_file.rfind(".");
    if (problem_file.substr(extension) != ".dat")
      throw std::invalid_argument("Problem file given does not end in '.dat'.");
    solver_init(problem_file.substr(0, extension).c_str());

#ifndef ALLOW_PERPLEX_OUTPUT
    enable_stdout(fd);
#endif

    // Return to the original working directory.
    if (chdir(initial_dir) != 0)
      throw std::invalid_argument("Could not change directory.");

    // Save that initialization is complete.
    initialized = true;

    // Save the initial bulk composition.
    for (size_t i = 0; i < get_n_composition_components(); ++i)
      initial_bulk_composition.push_back(bulk_props_get_composition(i));
  }

  void Wrapper::minimize(const double pressure, 
                         const double temperature,
			 const std::vector<double>& composition)
  {
    if (composition.size() != get_n_composition_components())
      throw std::invalid_argument("Specified bulk composition is the wrong size.");

    for (size_t i = 0; i < get_n_composition_components(); ++i)
      bulk_props_set_composition(i, composition[i]);

    solver_set_pressure(utils::convert_pascals_to_bar(pressure));
    solver_set_temperature(temperature);

#ifndef ALLOW_PERPLEX_OUTPUT
    // Disable stdout to prevent Perple_X dominating stdout.
    const int fd = disable_stdout();
#endif

    solver_minimize();

#ifndef ALLOW_PERPLEX_OUTPUT
    enable_stdout(fd);
#endif

    // Save that the minimization is complete.
    minimized = true;
  }

  void Wrapper::minimize(const double pressure, const double temperature)
  {
    minimize(pressure, temperature, initial_bulk_composition);
  }

  size_t Wrapper::get_n_composition_components() const
  {
    return composition_props_get_n_components();
  }

  const std::vector<std::string>& 
  Wrapper::get_composition_component_names() const
  {
    static std::vector<std::string> names;

    names.clear();

    for (size_t i = 0; i < get_n_composition_components(); ++i)
      names.push_back(std::string(composition_props_get_name(i)));
    return names;
  }

  double Wrapper::get_n_moles() const
  {
    double n_moles = 0.0;
    for (double composition_component : initial_bulk_composition)
      n_moles += composition_component;
    return n_moles;
  }

  size_t Wrapper::get_n_phases() const
  {
    return soln_phase_props_get_n();
  }

  const std::vector<std::string>& 
  Wrapper::get_phase_names() const
  {
    static std::vector<std::string> names;

    names.clear();

    for (size_t i = 0; i < get_n_phases(); ++i)
      names.push_back(std::string(soln_phase_props_get_name(i)));
    return names;
  }

  const std::vector<std::string>& 
  Wrapper::get_abbr_phase_names() const
  {
    static std::vector<std::string> names;

    names.clear();

    for (size_t i = 0; i < get_n_phases(); ++i)
      names.push_back(std::string(soln_phase_props_get_abbr_name(i)));
    return names;
  }

  const std::vector<std::string>& 
  Wrapper::get_full_phase_names() const
  {
    static std::vector<std::string> names;

    names.clear();

    for (size_t i = 0; i < get_n_phases(); ++i)
      names.push_back(std::string(soln_phase_props_get_full_name(i)));
    return names;
  }

  const std::vector<double>& Wrapper::get_phase_weight_fracs() const
  {
    static std::vector<double> fracs;

    load_phase_quantity(res_phase_props_get_weight_frac, fracs);
    return fracs;
  }

  const std::vector<double>& Wrapper::get_phase_vol_fracs() const
  {
    static std::vector<double> fracs;

    load_phase_quantity(res_phase_props_get_vol_frac, fracs);
    return fracs;
  }

  const std::vector<double>& Wrapper::get_phase_mol_fracs() const
  {
    static std::vector<double> fracs;

    load_phase_quantity(res_phase_props_get_mol_frac, fracs);
    return fracs;
  }

  const std::vector<double>& Wrapper::get_phase_amounts() const
  {
    static std::vector<double> amounts;

    load_phase_quantity(res_phase_props_get_mol, amounts);
    return amounts;
  }

  const std::vector<std::vector<double>>&
  Wrapper::get_phase_compositions() const
  {
    static std::vector<std::vector<double>> compositions;

    compositions.clear();

    std::unordered_map<size_t,size_t> idx_map = get_phase_index_mapping();

    for (size_t i = 0; i < get_n_phases(); ++i) {
      std::vector<double> composition;

      // Check to see if the solution phase is present in the end phases.
      // If it is, load the composition. If not, set it to be zeros.
      if (idx_map.find(i) != idx_map.end()) {
	for (size_t j = 0; j < get_n_composition_components(); ++j)
	  composition.push_back(res_phase_props_get_composition(idx_map[i], j));
      } else {
	for (size_t j = 0; j < get_n_composition_components(); ++j)
	  composition.push_back(0.0);
      }

      compositions.push_back(composition);
    }
    return compositions;
  }

  size_t Wrapper::find_phase_index_from_name(const std::string& phase_name) const
  {
    auto standard_names = get_phase_names();
    auto abbr_names = get_abbr_phase_names();
    auto full_names = get_full_phase_names();
    
    assert(standard_names.size() == get_n_phases());
    assert(abbr_names.size() == get_n_phases());
    assert(full_names.size() == get_n_phases());

    for (size_t i = 0; i < get_n_phases(); ++i) {
      if (phase_name == standard_names[i] ||
	  phase_name == abbr_names[i] || 
	  phase_name == full_names[i]) {
	return i;
      }
    }
    throw std::invalid_argument("The phase name was not found.");
  }

  double Wrapper::get_system_density() const
  {
    return sys_props_get_density();
  }

  double Wrapper::get_system_expansivity() const
  {
    return sys_props_get_expansivity();
  }

  double Wrapper::get_system_molar_entropy() const
  {
    return sys_props_get_mol_entropy();
  }
  
  double Wrapper::get_system_molar_heat_capacity() const
  {
    return sys_props_get_mol_heat_capacity();
  }

  void Wrapper::check_initialized() const
  {
    if (!initialized)
      throw std::logic_error("initialize() has not been called yet.");
  }

  void Wrapper::check_minimized() const
  {
    if (!minimized)
      throw std::logic_error("minimize() has not been called yet.");
  }

  size_t Wrapper::get_n_end_phases() const
  {
    return res_phase_props_get_n();
  }

  void Wrapper::load_phase_quantity(double (*get_quantity)(size_t), 
                                    std::vector<double>& out) const
  {
    out.clear();

    auto idx_map = get_phase_index_mapping();

    for (size_t i = 0; i < get_n_phases(); ++i) {
      // Check to see if the solution phase is present in the end phases.
      // If it is, load the quantity. If not, set it to be zeros.
      if (idx_map.find(i) != idx_map.end())
	out.push_back(get_quantity(idx_map[i]));
      else
	out.push_back(0.0);
    }
  }

  const std::unordered_map<size_t,size_t>& Wrapper::get_phase_index_mapping() const
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
}
