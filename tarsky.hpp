#include"polynoms.hpp"

namespace Kirill
{
	void uniquying(std::vector<Polynom>&);
	std::vector<Polynom> derivation_saturation(std::vector<Polynom> unsaturated);
	std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated);
	std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated);
	void degree_sort(std::vector<Polynom>&);
	std::vector<std::vector<int>> tars_table(std::vector<Polynom>);
}
