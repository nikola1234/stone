#include <iostream>
#include <string>

// 结点类
class TreeNode
{
public:
    TreeNode() {
        left = nullptr;
        right = nullptr;
        data = "";
    }
    ~TreeNode() {
        if (left) {
            delete left;
            left = nullptr;
        }
        if (right) {
            delete right;
            right = nullptr;
        }
    }

    TreeNode* get_left(void) const {
        return left;
    }

    TreeNode* get_right(void) const {
        return right;
    }

    std::string get_data(void) const {
        return data;
    }

    void set_left(TreeNode* value) {
        left = value;
    }

    void set_right(TreeNode* value) {
        right = value;
    }

    void set_data(const std::string & value) {
        data = value;
    }
private:
    TreeNode* left;
    TreeNode* right;
    /*data*/
    std::string data;
};

//树管理类
class Tree {
public:
    Tree() {}
    ~Tree() {}

    // 初始化树，获取根结点
    TreeNode* InitTree(void) {
        TreeNode* node = new TreeNode();
        if (node) {
            std::cout << "根结点初始化成功。" << std::endl;
            std::cout << "请输入根结点数据：";
            std::string str;
            getline(std::cin, str);
            node->set_data(str);
        } else {
            std::cout << "根结点初始化失败。" << std::endl;
            return nullptr;
        }
        return node;
    }

    // 添加结点 
    // 参数 treeNode 树根结点
    void AddTreeNode(TreeNode* treeNode) {
        TreeNode* node = new TreeNode();
        if (node) {
            std::cout << "结点初始化成功。" << std::endl;
            std::cout << "请输入结点数据：";
            std::string str;
            getline(std::cin, str);
            node->set_data(str);
            std::cout << "请输入该结点父结点数据：";
            std::string p_str;
            getline(std::cin, p_str);
            TreeNode*  p_node = TreeFindNode(treeNode, p_str);
            if (!p_node) {
                std::cout << "未找到父结点，添加失败。" << std::endl;
                delete node;
                node = nullptr;
            }
            std::cout << "输入“1”设置该结点为左子结点，输入“2”设置该结点为右子结点：";
            std::string num_str = "";
            do {
                getline(std::cin, num_str);
                if (num_str == "1") {
                    if (p_node->get_left()) {
                        std::cout << "该结点已经存在左子结点。" << std::endl;
                    } else {
                        p_node->set_left(node);
                    }
                } else if (num_str == "2") {
                  if (p_node->get_right()) {
                        std::cout << "该结点已经存在右子结点。" << std::endl;
                    } else {
                        p_node->set_right(node);
                    }
                }
            } while (num_str != "1" && num_str != "2" );
        } else {
            std::cout << "结点初始化失败。" << std::endl;
        }
    }
    // 根据结点数据查询结点
    TreeNode* TreeFindNode(TreeNode* treeNode, std::string data) {
        if(!treeNode) {
            return nullptr;
        }
        TreeNode* node = nullptr; 
        if (treeNode->get_data() == data) {
            return treeNode;
        } else {
            if (node = TreeFindNode(treeNode->get_left(), data)) {  //递归调用寻找左子结点
                return node; 
            } else if (node = TreeFindNode(treeNode->get_right(), data)) {  //递归调用寻找右子结点
                return node; 
            } else {
                return nullptr;
            }
        }
        return node;
    }
    // 返回左子结点
    TreeNode* TreeLeftNode(TreeNode* node) {
        if (!node) {
            std::cout << "获取该结点的左子结点---结点为空。" << std::endl;
        }
        return node->get_left();
    }
    // 返回右子结点
    TreeNode* TreerightNode(TreeNode* node) {
        if (!node) {
            std::cout << "获取该结点的右子结点---结点为空。" << std::endl;
        }
        return node->get_right();
    }
    // 查询树是否为空
    bool TreeIsEmpty(TreeNode* node) {
        bool ret = false;
        if (!node) { 
            std::cout << "该结点为空。" << std::endl;
            ret = true;
        }
        return ret;
    }
    //计算二叉树的深度
    int TreeDepth(TreeNode* node) {
        int depth = 0;
        if (node == nullptr) {
            return depth;
        }
        int left = TreeDepth(node->get_left());
        int right = TreeDepth(node->get_right());
        depth = (right > left) ? (right + 1) : (left + 1);
        return depth;
    }
    //清空二叉树
    void ClearTree(TreeNode* node) {
        if (node) {
            ClearTree(node->get_left());
            ClearTree(node->get_right());
            delete node;
            node = nullptr;
        }
    }
    //显示结点数据
    void TreeNodeData(TreeNode* node) {
        std::cout << node->get_data() <<" ";
    }
    // 先序遍历算法 : 前序遍历的递归定义：先根节点，后左子树，再右子树。
    //       node -> left -> right
    void DLRTree(TreeNode* node) {
        if (node) {
            TreeNodeData(node);
            DLRTree(node->get_left());
            DLRTree(node->get_right());
        }
    }
    // 中序遍历算法 : 中序遍历的递归定义：先左子树，后根节点，再右子树。
    //       left -> node -> right
    void LDRTree(TreeNode* node) {
        if (node) {
            LDRTree(node->get_left());
            TreeNodeData(node);
            LDRTree(node->get_right());
        }
    }
    // 后序遍历算法 : 后序遍历的递归定义：先左子树，后右子树，再根节点.
    //       left -> right -> node
    void LRDTree(TreeNode* node) {
        if (node) {
            LRDTree(node->get_left());
            LRDTree(node->get_right());
            TreeNodeData(node);
        }
    }
    // 按层遍历算法
};

/*
                                                           [root]
                                      [1-left]                                           [1-right]
                      [2.1-left]                   [2.1-right]                [2.2-left]          [ 2.2-right] 
        [3.1-left]             [3.1-right]   [3.2-left] [3.2-right]   [3.3-left]  [3.3-right]  [3.4-left]  [3.4-right] 
 [4.1-left]  [4.1-right]   [4.2-left] 
*/
int main(int argc, char const *argv[])
{
    Tree tree;
    std::string cli_str;
    TreeNode* root = tree.InitTree();
    //添加结点
    do {
        std::cout << "请选择菜单 0:退出， 1:添加结点 :";
        getline(std::cin, cli_str);
        if (cli_str == "1") {
            tree.AddTreeNode(root);
        }
    } while (cli_str != "0");
    // 遍历
    do {
        std::cout << "请选择菜单 0:退出， 1:先序遍历, 2:中序遍历, 3:后序遍历 :";
        getline(std::cin, cli_str);
        if (cli_str == "1") {
            tree.DLRTree(root);
            std::cout <<"."<< std::endl;
        } else if (cli_str == "2") {
            tree.LDRTree(root);
            std::cout <<"."<< std::endl;
        } else if (cli_str == "3") {
            tree.LRDTree(root);
            std::cout <<"."<< std::endl;
        }
    } while (cli_str != "0");
    delete root;
    root =nullptr;
    return 0;
}