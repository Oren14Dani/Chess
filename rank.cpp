#include "rank.h"

Rank::Rank() {
	for (int i = 0; i < 6; i++) {
		rank_value[rank_type[i]] = SCORE_PER_RANK * (i + 1);
		//value_rank[SCORE_PER_RANK * (i + 1)] = rank_type[i];
	}
}

const map<string, int>& Rank::get_rank_value_map() const { return rank_value; }

const int Rank::get_rank_value(const string rank) const {
	try { return rank_value.at(rank); }
	catch (out_of_range) {
		cout << "invalid rank" << endl;
		return NOT_MATCH;
	}
}