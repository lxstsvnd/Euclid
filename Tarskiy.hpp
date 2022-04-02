#include"polynoms.hpp"

void uniquying(std::vector<Polynom>&);
std::vector<Polynom> derivation_saturation(std::vector<Polynom> unsaturated);
std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated);
std::vector<Polynom> elder_coefficient_saturation(std::vector<Polynom> unsaturated);
std::vector<Polynom> drop_elder_coefficient_saturation(std::vector<Polynom> unsaturated);
std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated);
void range(std::vector<Polynom>&);
void degree_sort(std::vector<Polynom>&);
std::vector<std::vector<int>> tars_table(std::vector<Polynom>);
