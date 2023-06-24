#ifndef RANK_H
#define RANK_H

#include "general.h"

// ~~~~~~~~~~ rank ~~~~~~~~~ //
class Rank {
private:
	map<string, int> rank_value;
public:
	const string rank_type[6] = { ROOKIE_RANK , BEGINER_RANK , AVERGE_RANK ,ADVANCE_RANK ,EXPERT_RANK , GOD_RANK };
	Rank();
	const map<string, int>& get_rank_value_map()const;
	const int get_rank_value(const string rank) const;
};

#endif