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


#include <perplexcpp/result_cache.h>

#include <cassert>
#include <iostream>

namespace perplexcpp
{
  ResultCache::ResultCache(const size_t capacity, const double rtol)
    : capacity(capacity),
      rtol(rtol)
  {
      if (capacity < 0)
	throw std::invalid_argument("The capacity must be a non-negative number");

      if (rtol < 0.0 || rtol > 1.0)
	throw std::invalid_argument("The tolerance must be between 0 and 1");
  }



  int
  ResultCache::get(const double pressure, 
		   const double temperature, 
		   const std::vector<double> &composition,
		   MinimizeResult &out)
  {
    for (auto it = this->items.cbegin(); it != this->items.cend(); ++it)
    {
      if (is_near_enough(pressure, (*it).pressure) &&
	  is_near_enough(temperature, (*it).temperature) &&
	  is_near_enough(composition, (*it).composition))
      {
	MinimizeResult item = *it;

	// Reorder the cache if it contains more than one item.
	if (this->items.size() > 1)
	{
	  this->items.erase(it);
	  this->items.push_front(item);
	}

	out = item;
	return 0;
      }
    }
    return -1;
  }



  void
  ResultCache::put(const MinimizeResult& item)
  {
    // Add the new item to the front of the list and remove the last item.
    if (items.size() == this->capacity)
      this->items.pop_back();

    this->items.push_front(item);
  }



  size_t
  ResultCache::size()
  {
    return this->items.size();
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
