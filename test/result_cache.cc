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

#include <gtest/gtest.h>


using namespace perplexcpp;


TEST(ResultCacheTest, GetReturnsNullPtrWithoutRtol)
{
  auto cache = ResultCache(3);

  MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(3, 2.1), 
    17.2,
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };
  cache.put(item);

  auto new_item = cache.get(2.1e8, 1788, std::vector<double>(3, 2.1));

  EXPECT_TRUE(new_item == nullptr);
}



TEST(ResultCacheTest, GetReturnsNullPtrWithRtol)
{
  auto cache = ResultCache(3, 0.1);

  const MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(2, 2.1), 
    12.5,
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };

  cache.put(item);

  auto result = cache.get(3.1e8, 1788, std::vector<double>(2, 2.1));

  EXPECT_TRUE(result == nullptr);
}



TEST(ResultCacheTest, GetReturnsExactMatch)
{
  auto cache = ResultCache(3);

  const MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(3, 5.1), 
    84.3,
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };
  cache.put(item);

  auto result = cache.get(2.05e8, 1788, std::vector<double>(3, 5.1));

  ASSERT_TRUE(result != nullptr);

  EXPECT_EQ(result->density, 1.2);
  EXPECT_EQ(result->molar_heat_capacity, 3.0);
}



TEST(ResultCacheTest, GetReturnsNearMatch)
{
  auto cache = ResultCache(3, 0.1);

  const MinimizeResult item { 
    2.03e8, 
    2095, 
    std::vector<double>(4, 7.93), 
    23.1,
    std::vector<Phase>(), 
    1.2, 1.0, 2.0, 3.0 
  };
  cache.put(item);

  auto result = cache.get(2.01e8, 2097, std::vector<double>(4, 7.89));

  ASSERT_TRUE(result != nullptr);

  EXPECT_EQ(result->density, 1.2);
  EXPECT_EQ(result->molar_heat_capacity, 3.0);
}
