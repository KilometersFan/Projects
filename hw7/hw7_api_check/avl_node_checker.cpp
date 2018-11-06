#include <avlbst.h>
#include <string>

template<typename Key, typename Value>
class InheritedAVLNode : public AVLNode<Key, Value>
{
public:
	InheritedAVLNode(const Key& key, const Value& value);
	void test();
};

template<typename Key, typename Value>
InheritedAVLNode<Key, Value>::InheritedAVLNode(const Key& key, const Value& value)
		: AVLNode<Key, Value>(key, value, nullptr)
{
}

template<typename Key, typename Value>
void InheritedAVLNode<Key, Value>::test()
{
	this->mHeight = 6003;
}

void testProtectedMembers()
{
	InheritedAVLNode<int, double> AVLNode(25, 3.5);
	AVLNode.test();
}

template<typename Key, typename Value>
void testConstFunctions(const AVLNode<Key, Value>& node)
{
	std::pair<const std::string, int> item = node.getItem();

	if (node.getKey() == item.first
		&& node.getHeight() == 3)
	{
		AVLNode<Key, Value>* parent = node.getParent();
		AVLNode<Key, Value>* left = node.getLeft();
		AVLNode<Key, Value>* right = node.getRight();
		parent = left;
		left = right;
		right = parent;
	}
}

template<typename Key, typename Value>
void testNonConstFunctions(AVLNode<Key, Value>& AVLNode)
{
	AVLNode.setParent(nullptr);
	AVLNode.setLeft(nullptr);
	AVLNode.setRight(nullptr);
	AVLNode.setHeight(7);
}

void testPublicFunctions()
{
	AVLNode<std::string, int> AVLNode("a", 1, nullptr);
	testNonConstFunctions<std::string, int>(AVLNode);
	testConstFunctions<std::string, int>(AVLNode);
}

int main(int argc, char* argv[])
{
	testPublicFunctions();
	testProtectedMembers();
	return 0;
}