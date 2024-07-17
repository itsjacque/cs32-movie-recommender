#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED
#include <vector>
#include <iostream>
template <typename KeyType, typename ValueType>
class TreeMultimap
{
    class Node;
  public:
    class Iterator
    {
      public:
        Iterator()
            : nodePtr(nullptr), value(nullptr)
        {
            m_valid = false;
        }
        
        Iterator(Node* node)
        : nodePtr(node), value(&node->value.front())
        {
            m_valid = true;
        }

        ValueType& get_value() const
        {
            return *value;
        }

        bool is_valid() const
        {
            if (value == nullptr)
                return false;
            if (value >= (&nodePtr->value.back() + 1))
                return false;
            else 
            return true;
        }

        void advance()
        {
            value++;
        }

      private:
        bool m_valid = true;
        Node* nodePtr;
        ValueType* value;
    };

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        cleanup(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_root == nullptr) {
            m_root = new Node;
            m_root->key = key;
            m_root->value.push_back(value);
            return;
        }
        Node* curr = m_root;
        for (;;) {
            if (key == curr->key) {
                curr->value.push_back(value);
                return;
            }
            else if (key > curr->key){
                if(curr->right == nullptr) {
                    curr->right = new Node;
                    curr->right->key = key;
                    curr->right->value.push_back(value);
                    return;
                }
                curr= curr->right;
                continue;
            }
            else if (key < curr->key){
                if(curr->left == nullptr) {
                    curr->left = new Node;
                    curr->left->key = key;
                    curr->left->value.push_back(value);
                    return;
                }
                curr= curr->left;
                continue;
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* curr = m_root;
        while(curr != nullptr) {
            if (curr->key == key){
                return Iterator(curr);
            } else if (key < curr->key ) {
                curr= curr->left;
            } else {
                curr= curr->right;
            }
        }
        return Iterator();
    }

  private:
    struct Node{
        KeyType key;
        std::vector<ValueType> value;
        Node* left = nullptr;
        Node* right = nullptr;
    };
    Node *m_root;
    void cleanup (Node* curr) {
        if (curr == nullptr)
            return;
        cleanup(curr->left);
        cleanup(curr->right);
        delete curr;
    }
};

#endif // TREEMULTIMAP_INCLUDED
