#include "OrgChart.hpp"
using namespace std;
namespace ariel
{

    OrgChart &OrgChart::add_root(string const &value)
    {
        if (root == nullptr)
        {
            this->root = new Node();
        }
        this->root->info = value;
        data.push_back(root);
        return *this;
    }

    OrgChart &OrgChart::add_sub(string const &value1, string const &value2)
    {
        Node *child = new Node();
        child->info = value2;
        bool check = false;
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i]->info == value1)
            {
                data[i]->add_child(child);
                child->parent = data[i];
                check = true;
                break;
            }
        }
        if (!check)
        {
            throw std::invalid_argument("value does not exit");
        }
        data.push_back(child);
        return *this;
    }

    bool OrgChart::check_in(vector<Node *> a, string const &b)
    {
        for (unsigned int i = 0; i < a.size(); i++)
        {
            if (a[i]->info == b)
            {
                return true;
            }
        }
        return false;
    }

    void OrgChart::iterator::create_vec_level_order(Node *ptr = nullptr)
    {
        if (ptr != nullptr)
        {
            vector<Node *> queue;
            this->iter.clear();
            queue.push_back(this->pointer_to_current_node);
            this->iter.push_back(this->pointer_to_current_node);
            while (!queue.empty())
            {
                Node *tmp = queue[0];
                queue.erase(queue.begin());
                for (unsigned int i = 0; i < tmp->children.size(); i++)
                {
                    if (!check_in(this->iter, tmp->children[i]->info))
                    {
                        this->iter.push_back(tmp->children[i]);
                    }
                    queue.push_back(tmp->children[i]);
                }
            }
        }
    }

    void OrgChart::iterator::create_vec_reverse_order(Node *ptr = nullptr)
    {
        if (ptr != nullptr)
        {
            this->iter.clear();
            vector<Node *> queue;
            queue.push_back(this->pointer_to_current_node);
            this->iter.insert(this->iter.begin(), this->pointer_to_current_node);
            while (!queue.empty())
            {
                Node *tmp = queue[0];
                queue.erase(queue.begin());
                for (unsigned int i = 0; i < tmp->children.size(); i++)
                {
                    unsigned int j = tmp->children.size() - i - 1;
                    if (!check_in(this->iter, tmp->children[j]->info))
                    {
                        this->iter.insert(this->iter.begin(), tmp->children[j]);
                    }
                    queue.push_back(tmp->children[j]);
                }
            }
            this->pointer_to_current_node = this->iter[0];
        }
    }

    void OrgChart::iterator::create_vec_preorder(Node *ptr = nullptr)
    {
        if (ptr != nullptr)
        {
            this->iter.clear();
            vector<Node *> stack;
            stack.insert(stack.begin(), this->pointer_to_current_node);
            while (!stack.empty())
            {
                Node *curr = stack[0];
                this->iter.push_back(curr);
                stack.erase(stack.begin());
                for (unsigned int i = 0; i < curr->children.size(); i++)
                {
                    unsigned int j = curr->children.size() - i - 1;
                    stack.insert(stack.begin(), curr->children[j]);
                }
            }
        }
    }

    OrgChart::iterator &OrgChart::iterator::operator++()
    {
        this->index++;
        this->pointer_to_current_node = this->iter[this->index];
        return *this;
    }

    OrgChart::iterator OrgChart::iterator::operator++(int a)
    {
        iterator tmp{type, this->iter[this->index]};
        index++;
        return tmp;
    }

    bool OrgChart::iterator::operator==(const iterator &rhs) const
    {
        return pointer_to_current_node == rhs.pointer_to_current_node;
    }

    bool OrgChart::iterator::operator!=(const iterator &rhs) const
    {
        return pointer_to_current_node != rhs.pointer_to_current_node;
    }

    string &OrgChart::iterator::operator*()
    {
        return pointer_to_current_node->info;
    }

    string *OrgChart::iterator::operator->() const
    {
        return &(this->pointer_to_current_node->info);
    }

    OrgChart::iterator OrgChart::begin()
    {
        return iterator{"level", root};
    }

    OrgChart::iterator OrgChart::end()
    {
        return iterator{"level", nullptr};
    }

    OrgChart::iterator OrgChart::begin_level_order()
    {
        return iterator{"level", root};
    }

    OrgChart::iterator OrgChart::end_level_order()
    {
        return iterator{"level", nullptr};
    }

    OrgChart::iterator OrgChart::begin_reverse_order()
    {
        return iterator{"reverse", root};
    }

    OrgChart::iterator OrgChart::reverse_order()
    {
        return iterator{"reverse", nullptr};
    }

    OrgChart::iterator OrgChart::begin_preorder()
    {
        return iterator{"preorder", root};
    }

    OrgChart::iterator OrgChart::end_preorder()
    {

        return iterator{"preorder", nullptr};
    }

    ostream &operator<<(ostream &out, const OrgChart &a)
    {
        auto it=OrgChart::iterator{"level",a.root};
        out<<"The root:"<<a.root->info<<endl;
        for(unsigned int i=0;i<it.iter.size();i++)
        {
            if(it.iter[i]->children.empty())
            {
                continue;
            }
            out<<"children of "<<it.iter[i]->info<<":\n";
            for(unsigned int j=0;j<it.iter[i]->children.size();j++)
            {
                out<<it.iter[i]->children[j]->info<<" ";
            }
            out<<"\n";
        }
        return out;
        
    }

}