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


#ifndef _perplexcpp_f2c_h
#define _perplexcpp_f2c_h


#include <cstddef>


namespace f2c
{
extern "C" 
{

/**
 * Initialize the solver.
 *
 * @param filename The name of the Perple_X problem definition file.
 */
void solver_init(const char* filename);

/**
 * Perform the minimization. 
 */
void solver_minimize();

/**
 * @param pressure The pressure used in the minimization (bar).
 */
void solver_set_pressure(const double pressure);

/**
 * @param temperature The temperature used in the minimization (K).
 */
void solver_set_temperature(const double temperature);


/**
 * @return The minimum pressure (bar).
 */
double get_min_pressure();


/**
 * @return The maximum pressure (bar).
 */
double get_max_pressure();


/**
 * @return The minimum temperature (K).
 */
double get_min_temperature();


/**
 * @return The maximum temperature (K).
 */
double get_max_temperature();


/* ------------------------------------------------------------ */
/* ------------------ COMPOSITION PROPERTIES ------------------ */
/* ------------------------------------------------------------ */

/**
 * @return Number of composition components.
 */
size_t composition_props_get_n_components();

/**
 * @param component_idx Composition component index.
 * @return              Name of a composition component.
 */
char* composition_props_get_name(size_t component_idx);

/**
 * @return the molar mass in kg/mol
 */
double 
get_composition_molar_mass(const size_t component_idx);

/* ----------------------------------------------------------- */
/* --------------------- BULK PROPERTIES --------------------- */
/* ----------------------------------------------------------- */

/**
 * @param component_idx Composition component index.
 * @return              Amount of component (mol). 
 */
double bulk_props_get_composition(size_t component_idx);

/**
 * @param component_idx Composition component index.
 * @param amount        Amount of component (mol).
 */
void bulk_props_set_composition(size_t component_idx, double amount);

/* --------------------------------------------------------- */
/* --------------- SOLUTION PHASE PROPERTIES --------------- */
/* --------------------------------------------------------- */

/**
 * @return Number of solution phases.
 */
size_t soln_phase_props_get_n();

/**
 * @param idx Solution phase index.
 * @return	The standard name of the solution phase.
 */
char* soln_phase_props_get_name(size_t idx);

/**
 * @param idx Solution phase index.
 * @return	The abbreviated name of the solution phase.
 */
char* soln_phase_props_get_abbr_name(size_t idx);

/**
 * @param idx Solution phase index.
 * @return	The full name of the solution phase.
 */
char* soln_phase_props_get_full_name(size_t idx);



/* ----------------------------------------------------------- */
/* ----------------- RESULT PHASE PROPERTIES ----------------- */
/* ----------------------------------------------------------- */

/**
 * @return Number of result phases.
 */
size_t res_phase_props_get_n();

/**
 * @param res_phase_idx Result phase index.
 * @return              Result phase name.
 *
 * @remark The returned name can be either the short or long version.
 */
char* res_phase_props_get_name(size_t res_phase_idx);

/**
 * @param res_phase_idx Result phase index.
 * @return              Result phase fractional weight.
 */
double res_phase_props_get_weight_frac(size_t res_phase_idx);

/**
 * @param res_phase_idx Result phase index.
 * @return              Result phase fractional volume.
 */
double res_phase_props_get_vol_frac(size_t res_phase_idx);

/**
 * @param res_phase_idx Result phase index.
 * @return              Result phase fractional molar amount.
 */
double res_phase_props_get_mol_frac(size_t res_phase_idx);

/**
 * @param res_phase_idx Result phase index.
 * @return              Result phase molar amount.
 */
double res_phase_props_get_mol(size_t res_phase_idx);

/*
 * @param endmember_idx Result phase index.
 * @param component_idx Composition component index.
 * @return              Result phase density
 */
double 
get_endmember_composition_ratio(const size_t endmember_idx,
				const size_t component_idx);
      

/**
 * @param endmember_idx Result phase index.
 * @return              Result phase density
 */
double
get_endmember_density(const size_t endmember_idx);


/* ----------------------------------------------------------- */
/* -------------------- SYSTEM PROPERTIES -------------------- */
/* ----------------------------------------------------------- */

/**
 * @return System density (kg/m3).
 */
double sys_props_get_density();

/**
 * @return System expansivity (1/K).
 */
double sys_props_get_expansivity();

/**
 * @return System molar entropy (J/K).
 */
double sys_props_get_mol_entropy();

/**
 * @return System molar heat capacity (J/K).
 */
double sys_props_get_mol_heat_capacity();

}  // extern
}  // namespace

#endif
