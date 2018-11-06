#include <avlbst.h>
#include <string>


void testPublicFunctions()
{
	AVLTree<std::string, int> bst;

	bst.insert(std::pair<const std::string, int>("a", 5));
	bst.remove("a");

}

int main(int argc, char* argv[])
{
	testPublicFunctions();
	return 0;
}