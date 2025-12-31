#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "number_16.hh"

#ifndef character_hh
#define character_hh

namespace srs {

	enum vl_idx : uint8_t {
		BASE = 0b00,
		MAX  = 0b01,
		NOW  = 0b10,
	};

	class character {
		private:
			std::string __name;
			mutable std::vector<number_16> __hp;
			mutable std::vector<number_16> __atk;
			mutable std::vector<number_16> __mp;
			number_16 __id;              // ID stays immutable when character is const
			mutable number_16 __job_id;   // job_id can be modified even on const
			inline srs::number_16 __buff_debuff(const srs::number_16& vl, float effect_size) const;

		public:
			character(
				const std::string& name,
				const int16_t& base_hp, 
				const int16_t& base_atk, 
				const int16_t& base_mp, 
				const int16_t& id, 
				const int16_t& job_id
			);
			~character();
			character(const srs::character& me);

			std::string& name();
			const std::string& name() const;
			number_16& hp(vl_idx type) const;
			number_16& atk(vl_idx type) const;
			number_16& mp(vl_idx type) const;
			number_16& id();
			number_16& job_id();
			const number_16& id() const;
			number_16& job_id() const;

			void buff_hp(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
			void debuff_hp(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
			void buff_atk(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
			void debuff_atk(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
			void buff_mp(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
			void debuff_mp(const float& effect_size, const srs::vl_idx& type = vl_idx::NOW) const;
	};

}

#endif
