
#include <iostream>
#include <optional>

namespace srs::game::hit::effect {
    constexpr double critical = 1.0;
    constexpr double near     = 0.5;
    constexpr double fk       = 0.0;
}

namespace srs::sys {
    u_int32_t hit_calc(const double& effect_size, const u_int32_t& max_cs) {
        return max_cs * effect_size;
    }
    
    constexpr std::optional<double> r_reciprocal(int64_t n) {
        if (n == 0) {
            return std::nullopt;
        } else {
            return 1.0 / n;
        }
    }
}

int main() {
    double effect_size = srs::game::hit::effect::critical;
    u_int32_t max_score = 1000;
    u_int32_t max_combo = 10;

    u_int32_t max_cs = max_score / max_combo;

    std::cout << "Hit Score: " << srs::sys::hit_calc(effect_size, max_cs) << std::endl;
}
