#include <set>
#include <vector>
struct Country
{
	public:
		std::set<Country*> neighbors;
		int color = 0;
		char name = '\0';
};
void BFS(char** &_map, bool**&visited, std::vector<Country*>& countries, int & rows, int & cols);

void findColors(Country *country);