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

#include <perplexcpp/utils.h>

namespace perplexcpp
{
  namespace utils
  {
    double convert_pascals_to_bar(const double pressure_in_pascals) {
      return pressure_in_pascals / 1e6;
    }

    double convert_bar_to_pascals(const double pressure_in_bar) {
      return pressure_in_bar * 1e6;
    }
  }
}
