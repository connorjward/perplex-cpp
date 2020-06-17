#pragma once

#include <cstddef>

#include <string>
#include <unordered_map>
#include <vector>

namespace perplex
{
  class Wrapper
  {
    public:
      /**
       * Retrieve singleton instance of state. A singleton is used because
       * Perple_X relies heavily on global variables (COMMON blocks) so care
       * must be taken to avoid concurrent access to the resources.
       *
       * @return The singleton instance of the state.
       */
      static Wrapper& get_instance();

      /**
       * Initialize Perple_X
       *
       * @param filename The Perple_X problem definition file.
       */
      void initialize(const std::string& filename);

      /**
       * Perform the minimization using MEEMUM.
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
       * @return The bulk composition.
       */
      const std::vector<double>& get_bulk_composition() const;

      /**
       * @param composition The new bulk composition.
       */
      void set_bulk_composition(const std::vector<double>& composition);

      /**
       * @return The number of phases.
       */
      size_t get_n_phases() const;

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
       * A boolean indicating whether Perple_X has been initialized or not.
       */
      bool initialized = false;

      /**
       * A boolean indicating whether minimize() has been called or not.
       */
      bool minimized = false;

      /**
       * Construct the class.
       *
       * @remark This constructor is private to enforce the singleton pattern.
       */
      Wrapper() {};

      /**
       *
       */
      void check_initialized() const;

      /**
       *
       */
      void check_minimized() const;

      /**
       *
       */
      void load_phase_quantity(double (*get_quantity)(size_t),
	                       std::vector<double>& out) const;

      /**
       *
       */
      const std::unordered_map<size_t,size_t>& get_phase_index_mapping() const;

      /**
       * TODO
       */
      size_t find_phase_index_from_name(const std::string& name) const;

      /**
       *
       */
      size_t get_n_end_phases() const;
  };
}
