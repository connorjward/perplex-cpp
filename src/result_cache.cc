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


#include "result_cache.h"

#include <cassert>
#include <iostream>

namespace perplexcpp
{
  ResultCache::ResultCache(const size_t capacity, const double rtol)
    : rtol(rtol), items(capacity)
  {}



  const MinimizeResult *const
  ResultCache::get(const double pressure, 
		   const double temperature, 
		   const std::vector<double>& composition)
  {
    for (auto it = this->items.cbegin(); it != this->items.cend(); ++it)
    {
      if (is_near_enough(pressure, (*it).pressure) &&
	  is_near_enough(temperature, (*it).temperature) &&
	  is_near_enough(composition, (*it).composition))
      {
	// Reorder the cache and return pointer to the result.
	this->items.erase(it);
	this->items.push_front(*it);
	return &(*it);
      }
    }
    return nullptr;
  }



  void
  ResultCache::put(const MinimizeResult& item)
  {
    // Add the new item to the front of the list and remove the last item.
    this->items.push_front(item);
    this->items.pop_back();
  }



  bool 
  ResultCache::is_near_enough(const double x, const double y)
  {
    return std::abs(x - y) / x <= this->rtol;
  }



  bool 
  ResultCache::is_near_enough(const std::vector<double> xs, 
		              const std::vector<double> ys)
  {
    assert(xs.size() == ys.size());

    for (size_t i = 0; i < xs.size(); ++i)
      if (!is_near_enough(xs[i], ys[i]))
	return false;
    return true;
  }
}
