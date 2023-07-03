//-----------------------------------------------------------------------------
// 確率分布乱数関数
// 作成者：長谷川勇一朗
// 作成年月日：2023/6/6
// 指定した確率になるように乱数を生成
//-----------------------------------------------------------------------------
#include <random>
#include <vector>
//-----------------------------------------------------------------------------
// How to Use
// 
// <初期化>
// std::vector<double> probabilities = {
//     5,
//     2,
//     3
// };//50% 20% 30%の確率
// 
// std::random_device rd;
// Probability probability(rd(), probabilities_);
// 
// <使用時>
// probability.GetProbability();//50%で0,20%で1,30%で2
//-----------------------------------------------------------------------------
#pragma once

class Probability {
	std::mt19937 mt;
	std::discrete_distribution<std::size_t> dist;
public:
	Probability(long seed_, std::vector<double> probabilities_)
		: mt(seed_)
		, dist(probabilities_.begin(), probabilities_.end())
	{}

	~Probability() {}

	//-----------------------------------------------------------------------------
	// 指定された確率分布に基づいた値を返す
	// std::size_t 
	//-----------------------------------------------------------------------------
	std::size_t GetProbability();
};

