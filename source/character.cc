#include "../include/character.hh"
#include <cmath>

srs::character::character(
	const std::string& name,
	const int16_t& base_hp, 
	const int16_t& base_atk, 
	const int16_t& base_mp, 
	const int16_t& id, 
	const int16_t& job_id
):
	
	__name(name),
		__id(id),
      	__job_id(job_id),
      	__hp(3),
      	__atk(3),
      	__mp(3)
{
    __hp[vl_idx::BASE] = base_hp;
    __hp[vl_idx::MAX]  = base_hp;
    __hp[vl_idx::NOW]  = base_hp;

    __atk[vl_idx::BASE] = base_atk;
    __atk[vl_idx::MAX]  = base_atk;
    __atk[vl_idx::NOW]  = base_atk;

    __mp[vl_idx::BASE] = base_mp;
    __mp[vl_idx::MAX]  = base_mp;
    __mp[vl_idx::NOW]  = base_mp;
}

inline srs::number_16 srs::character::__buff_debuff(const srs::number_16& vl, float effect_size) const {
    return srs::number_16(vl.to_int16_t() * effect_size * 0.01 + 0.5);
}

srs::character::character(const character& me): 
	__name(me.__name),
	__id(me.__id),
	__job_id(me.__job_id),
  	__hp(me.__hp),
	__mp(me.__mp),
	__atk(me.__atk)
{}

srs::character::~character() {}

std::string& srs::character::name() { return __name; }
const std::string& srs::character::name() const { return __name; }

srs::number_16& srs::character::hp(srs::vl_idx type) const { return __hp[type]; }
srs::number_16& srs::character::atk(srs::vl_idx type) const { return __atk[type]; }
srs::number_16& srs::character::mp(srs::vl_idx type) const { return __mp[type]; }
srs::number_16& srs::character::id() { return __id; }
srs::number_16& srs::character::job_id() { return __job_id; }
const srs::number_16& srs::character::id() const { return __id; }
srs::number_16& srs::character::job_id() const { return __job_id; }

void srs::character::buff_hp(const float& effect_size, const srs::vl_idx& type) const {
	__hp[type] += __buff_debuff(__hp[type], effect_size);
	return;
}

void srs::character::debuff_hp(const float& effect_size, const srs::vl_idx& type) const {
	__hp[type] -= __buff_debuff(__hp[type], effect_size);
	return;
}

void srs::character::buff_atk(const float& effect_size, const srs::vl_idx& type) const {
	__atk[type] += __buff_debuff(__atk[type], effect_size);
	return;
}

void srs::character::debuff_atk(const float& effect_size, const srs::vl_idx& type) const {
	__atk[type] -= __buff_debuff(__atk[type], effect_size);
	return;
}

void srs::character::buff_mp(const float& effect_size, const srs::vl_idx& type) const {
    __mp[type] += __buff_debuff(__mp[type], effect_size);
	return;
}

void srs::character::debuff_mp(const float& effect_size, const srs::vl_idx& type) const {
    __mp[type] += __buff_debuff(__mp[type], effect_size);
	return;
}

