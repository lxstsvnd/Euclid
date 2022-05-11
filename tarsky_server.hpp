#include"polynoms.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace Kirill
{
	void uniquying(std::vector<Polynom>&);
	std::vector<std::vector<int>> get_format_table(std::vector<Polynom> DNF_polynoms, std::vector<int> fd, int sockListener);
	std::vector<Polynom> derivation_saturation(std::vector<Polynom> unsaturated);
	std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated, std::vector<int> fd, int sockListener);
	std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated, std::vector<int> fd, int sockListener);
	void degree_sort(std::vector<Polynom>&);
	std::vector<std::vector<int>> tars_table(std::vector<Polynom>);
}
