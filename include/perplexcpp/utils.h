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

#ifndef PERPLEXCPP_UTILS_H
#define PERPLEXCPP_UTILS_H

namespace perplexcpp
{
  namespace utils
  {
    /**
     * @param pressure_in_pascals
     * @return                    The pressure in bar.
     */
    double convert_pascals_to_bar(const double pressure_in_pascals);


    /**
     * @param pressure_in_bar
     * @return                The pressure in pascals.
     */
    double convert_bar_to_pascals(const double pressure_in_bar);


    /**
     * Disable console output.
     *
     * @return The file descriptor pointing to stdout.
     */
    const int disable_stdout();


    /**
     * Enable console output.
     *
     * @param stdout_descriptor The file descriptor pointing to stdout.
     */
    void enable_stdout(const int stdout_descriptor) ;
  }
}

#endif
