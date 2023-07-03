//-----------------------------------------------------------------------------
// �m�����z�����֐�
// �쐬�ҁF���J��E��N
// �쐬�N�����F2023/6/6
// �w�肵���m���ɂȂ�悤�ɗ����𐶐�
//-----------------------------------------------------------------------------
#include <random>
#include <vector>
//-----------------------------------------------------------------------------
// How to Use
// 
// <������>
// std::vector<double> probabilities = {
//     5,
//     2,
//     3
// };//50% 20% 30%�̊m��
// 
// std::random_device rd;
// Probability probability(rd(), probabilities_);
// 
// <�g�p��>
// probability.GetProbability();//50%��0,20%��1,30%��2
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
	// �w�肳�ꂽ�m�����z�Ɋ�Â����l��Ԃ�
	// std::size_t 
	//-----------------------------------------------------------------------------
	std::size_t GetProbability();
};

