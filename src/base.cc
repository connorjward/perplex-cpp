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


#include <perplexcpp/base.h>

#include <stdexcept>


namespace perplexcpp
{
  std::vector<CompositionComponent>
  make_composition(std::vector<std::string> names,
		   std::vector<double> amounts)
  {
    if (names.size() != amounts.size())
      throw std::invalid_argument("Vector sizes do not match.");

    std::vector<CompositionComponent> comp;
    for (size_t i = 0; i < names.size(); ++i)
      comp.push_back({names[i], amounts[i]});
    return comp;
  }


  std::vector<CompositionComponent>
  make_empty_composition(std::vector<std::string> names)
  {
    std::vector<CompositionComponent> comp;
    for (size_t i = 0; i < names.size(); ++i)
      comp.push_back({names[i], 0.0});
    return comp;
  }
}
