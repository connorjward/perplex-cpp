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


TEST(ResultCacheTest, GetReturnsMinusOneWithoutRtol)
{
  auto cache = ResultCache(3);

  MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(3, 2.1), 
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };
  cache.put(item);

  MinimizeResult result;
  EXPECT_EQ(cache.get(2.1e8, 1788, std::vector<double>(3, 2.1), result), -1);
}



TEST(ResultCacheTest, GetReturnsMinusOneWithRtol)
{
  auto cache = ResultCache(3, 0.1);

  const MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(2, 2.1), 
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };

  cache.put(item);

  MinimizeResult result;
  EXPECT_EQ(cache.get(3.1e8, 1788, std::vector<double>(2, 2.1), result), -1);
}



TEST(ResultCacheTest, GetReturnsExactMatch)
{
  auto cache = ResultCache(3);

  const MinimizeResult item { 
    2.05e8, 
    1788, 
    std::vector<double>(3, 5.1), 
    std::vector<Phase>(), 
    1.2, 
    1.0, 
    2.0, 
    3.0 
  };
  cache.put(item);

  MinimizeResult result;
  ASSERT_EQ(cache.get(2.05e8, 1788, std::vector<double>(3, 5.1), result), 0);

  EXPECT_EQ(result.density, 1.2);
  EXPECT_EQ(result.molar_heat_capacity, 3.0);
}



TEST(ResultCacheTest, GetReturnsNearMatch)
{
  auto cache = ResultCache(3, 0.1);

  const MinimizeResult item { 
    2.03e8, 
    2095, 
    std::vector<double>(4, 7.93), 
    std::vector<Phase>(), 
    1.2, 1.0, 2.0, 3.0 
  };
  cache.put(item);

  MinimizeResult result;
  ASSERT_EQ(cache.get(2.01e8, 2097, std::vector<double>(4, 7.89), result), 0);

  EXPECT_EQ(result.density, 1.2);
  EXPECT_EQ(result.molar_heat_capacity, 3.0);
}



TEST(ResultCacheTest, GetsWorksTwiceInARow)
{
  auto cache = ResultCache(3, 0.1);

  const MinimizeResult item { 
    2.03e8, 
    2095, 
    std::vector<double>(4, 7.93), 
    std::vector<Phase>(), 
    1.2, 1.0, 2.0, 3.0 
  };
  cache.put(item);

  MinimizeResult result;
  ASSERT_EQ(cache.get(2.01e8, 2097, std::vector<double>(4, 7.89), result), 0);

  EXPECT_EQ(result.density, 1.2);
  EXPECT_EQ(result.molar_heat_capacity, 3.0);

  ASSERT_EQ(cache.get(2.01e8, 2097, std::vector<double>(4, 7.89), result), 0);

  EXPECT_EQ(result.density, 1.2);
  EXPECT_EQ(result.molar_heat_capacity, 3.0);
}
