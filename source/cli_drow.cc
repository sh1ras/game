#include "../include/cli_drow.hh"

namespace srs {
    srs::drow sdorw;
}    

inline void srs::drow::__status_drow(
        const std::vector<srs::character>& character_data, 
        const std::vector<u_int16_t>& name_len,
        const std::string& label,
        auto get_value,
        const int& n
    ) {
    for (size_t i = 0; i < character_data.size(); ++i) {
        std::string value_str = label + " " + get_value(character_data[i]);
        std::cout << value_str;
        for (int j  = value_str.length(); j <= name_len[i]; ++j) {
            std::cout << ' ';
        } if(n > 0 && i == n) {
            std::cout << "    ";
        }
    }
    std::cout << "\n";
}

void srs::drow::vector_dorw(const std::vector<std::string>& drow_vector) {
    for(const auto& str : drow_vector) {
        std::cout << str << std::endl;
    } return;
}

void srs::drow::character_drow(const std::vector<srs::character>& character_data, const int& n) {
    std::vector<u_int16_t> name_len(character_data.size());
    for (size_t i = 0; i < character_data.size(); ++i) {
        std::string character_name("===  " + std::to_string(i) +  character_data[i].name() + "  ===");
        std::cout << character_name;
        if(n > 0 && i == n) {
            std::cout << "     ";
        } else {
            std::cout << ' ';
        }
        name_len[i] = character_name.length();
    } std::cout << "\n";
    __status_drow(character_data, name_len, "HP  :", [](const srs::character& ch){ 
        return ch.hp(srs::NOW).to_string(); 
    }, n);
    
    __status_drow(character_data, name_len, "ATK :", [](const srs::character& ch){ 
        return ch.atk(srs::NOW).to_string(); 
    }, n);
    __status_drow(character_data, name_len, "MP  :", [](const srs::character& ch){ 
        return ch.mp(srs::NOW).to_string(); 
    }, n);

    return;
}

void srs::drow::action_drow(const std::vector<std::string>& action) {
    for (const auto& act : action) {
        std::cout << "===" << act << "=== ";
    }
    std::cout << std::endl;
    return;
}

int main() {
    srs::character player("Hero" ,15000, 3000, 1, 101, 1);
    srs::sdorw.character_drow({player, player, player, player, player, player}, 2);
    player.buff_atk(0.1, srs::NOW);
    srs::sdorw.character_drow({player, player, player, player, player, player}, 2);
    //srs::sdorw.action_drow({"Attack", "Defend", "MyItem"});
    return 0;
}