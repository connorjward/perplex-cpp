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
  Phase find_phase(const std::vector<Phase> phases, const std::string name)
  {
    for (auto phase : phases) {
      if (name == phase.name.standard ||
	  name == phase.name.abbreviated ||
	  name == phase.name.full)
	return phase;
    }
    throw std::invalid_argument("The name '"+name+"' could not be found.");
  }
}
