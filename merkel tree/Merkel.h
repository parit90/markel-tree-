#include <iostream>
#include <string>

using namespace std;
class TreeNode{
    public:
        string Hash;
        TreeNode* left;
        TreeNode* right;
};

class ToHash{
    string name;
    bool active;
};

string MakeHash(ToHash &_tohash);