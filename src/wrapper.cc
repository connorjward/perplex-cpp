#include <cassert>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>

#include <perplex/wrapper.h>
#include <perplex/utils.h>

#include "c_interface.h"

namespace
{
  const int disable_stdout() {
    // flush stdout
    fflush(stdout);

    // get file descriptors
    const int stdout_descriptor = dup(1);
    const int null_descriptor = open("/dev/null", O_WRONLY);

    // reassign stdout to /dev/null
    dup2(null_descriptor, 1);
    close(null_descriptor);

    return stdout_descriptor;
  }

  void enable_stdout(const int stdout_descriptor) {
    // flush stdout
    fflush(stdout);

    // reassign descriptor
    dup2(stdout_descriptor, 1);
    close(stdout_descriptor);
  }
}

namespace perplex
{
  Wrapper& Wrapper::get_instance()
  {
    static Wrapper instance;
    return instance;
  }

  void Wrapper::initialize(const std::string& filename)
  {
    // disable stdout to prevent Perple_X dominating stdout
    /* const int fd = disable_stdout(); */
    solver_init(filename.c_str());
    /* enable_stdout(fd); */

    // save that initialization is complete
    initialized = true;
  }

  void Wrapper::minimize(const double pressure, const double temperature)
  {
    solver_set_pressure(utils::convert_pascals_to_bar(pressure));
    solver_set_temperature(temperature);

    // TODO fix this bug
    // disable stdout to prevent Perple_X dominating stdout
    /* const int fd = disable_stdout(); */
    solver_minimize();
    /* enable_stdout(fd); */

    // save that the minimization is complete
    minimized = true;
  }

  size_t Wrapper::get_n_composition_components() const
  {
    return composition_props_get_n_components();
  }

  const std::vector<std::string>& 
  Wrapper::get_composition_component_names() const
  {
    static std::vector<std::string> names;

    names.resize(get_n_composition_components());
    for (size_t i = 0; i < names.size(); ++i)
      names[i] = std::string{composition_props_get_name(i)};
    return names;
  }

  const std::vector<double>& Wrapper::get_bulk_composition() const
  {
    static std::vector<double> composition;
    
    composition.resize(get_n_composition_components());
    for (size_t i = 0; i < composition.size(); ++i)
      composition[i] = bulk_props_get_composition(i);
    return composition;
  }

  void Wrapper::set_bulk_composition(const std::vector<double>& composition)
  {
    if (composition.size() != get_n_composition_components())
      throw std::invalid_argument("Specified bulk composition is the wrong size.");

    for (size_t i = 0; i < composition.size(); ++i)
      bulk_props_set_composition(i, composition[i]);
  }

  size_t Wrapper::get_n_phases() const
  {
    return soln_phase_props_get_n();
  }

  const std::vector<std::string>& 
  Wrapper::get_abbr_phase_names() const
  {
    static std::vector<std::string> names;

    names.resize(get_n_phases());
    for (size_t i = 0; i < names.size(); ++i)
      names[i] = std::string(soln_phase_props_get_abbr_name(i));
    return names;
  }

  const std::vector<std::string>& 
  Wrapper::get_full_phase_names() const
  {
    static std::vector<std::string> names;

    names.clear();
    for (size_t i = 0; i < get_n_phases(); ++i)
      names.push_back(std::string{soln_phase_props_get_full_name(i)});
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

    compositions.resize(get_n_phases());

    std::unordered_map<size_t,size_t> idx_map = get_phase_index_mapping();
    for (size_t i = 0; i < compositions.size(); ++i) {
      std::vector<double> composition = compositions[i];
      composition.resize(get_n_composition_components());

      bool found = idx_map.find(i) != idx_map.end();

      for (size_t j = 0; j < composition.size(); ++j) {
	if (found)
	  composition[j] = res_phase_props_get_composition(idx_map[i], j);
	else
	  composition[j] = 0.0;
      }
    }
    return compositions;
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

  void Wrapper::load_phase_quantity(double (*get_quantity)(size_t), 
                                  std::vector<double>& out) const
  {
    out.resize(get_n_phases());

    std::unordered_map<size_t,size_t> idx_map = get_phase_index_mapping();
    for (size_t i = 0; i < out.size(); ++i) {
      auto search = idx_map.find(i);
      if (search != idx_map.end())
	out[i] = get_quantity(idx_map[i]);
      else
	out[i] = 0.0;
    }
  }

  const std::unordered_map<size_t,size_t>& Wrapper::get_phase_index_mapping() const
  {
    static std::unordered_map<size_t,size_t> index_map;

    index_map.clear();
    for (size_t i = 0; i < get_n_end_phases(); ++i) {
      std::string end_phase_name = res_phase_props_get_name(i);
      index_map.emplace(find_phase_index_from_name(end_phase_name), i);
    }
    return index_map;
  }

  size_t Wrapper::find_phase_index_from_name(const std::string& phase_name) const
  {
    std::vector<std::string> abbr_names = get_abbr_phase_names();
    std::vector<std::string> full_names = get_full_phase_names();
    
    assert(abbr_names.size() == get_n_phases());
    assert(full_names.size() == get_n_phases());

    for (size_t i = 0; i < get_n_phases(); ++i) {
      if (phase_name == abbr_names[i] || phase_name == full_names[i])
	return i;
    }
    throw std::invalid_argument("The phase name was not found.");
  }

  size_t Wrapper::get_n_end_phases() const
  {
    return res_phase_props_get_n();
  }
}
