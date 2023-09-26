#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  private:
    struct Node
    {
        Node(const KeyType& key, const ValueType& value)
        {
            m_key = key;
            m_values.push_back(value);
            m_right = nullptr;
            m_left = nullptr;
        }
        
        void addValue(const ValueType& value)
        {
            m_values.push_back(value);
        }
        
        // Key
        KeyType m_key;
        // Vector of values
        std::vector<ValueType> m_values;
        Node* m_right;
        Node* m_left;
    };
    
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            m_valid = false;
        }
        
        Iterator(Node* p)
        {
            it = p->m_values.begin();
            m_valid = true;
            m_treeNode = p;
        }

        ValueType& get_value() const
        {
            return *it;
        }

        bool is_valid() const
        {
            return m_valid;
        }

        void advance()
        {
            it++;
            if (it != m_treeNode->m_values.end())
                m_valid = true;
            else
                m_valid = false;
        }

      private:
        typename std::vector<ValueType>::iterator it;
        bool m_valid;
        Node* m_treeNode;
    };

    TreeMultimap()
    {
        // Initialize root to null pointer
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        // Delete all nodes in tree
        freeTree(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        // If empty, insert at root
        if (m_root == nullptr)
        {
            m_root = new Node(key, value);
            return;
        }
        // Traverse tree looking for place to insert value
        Node* p = m_root;
        for ( ; ; )
        {
            // If key is found, add to vector at node
            if (key == p->m_key)
            {
                p->addValue(value);
                return;
            }
            // Else, search for place to insert new node with value
            else if (key < p->m_key)
            {
                if (p->m_left != nullptr)
                    p = p->m_left;
                else
                {
                    p->m_left = new Node(key, value);
                    return;
                }
            }
            else
            {
                if (p->m_right != nullptr)
                    p = p->m_right;
                else
                {
                    p->m_right = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        // Traverse tree looking for key
        Node* p = m_root;
        while (p != nullptr)
        {
            if (key == p->m_key)
                return Iterator(p);
            else if (key < p->m_key)
                p = p->m_left;
            else
                p = p->m_right;
        }
        // If key not found, return invalid iterator
        return Iterator();
    }

  private:
    Node* m_root;
    
    // Recursive helper function for tree destructor
    void freeTree(Node* p)
    {
        if (p == nullptr)
            return;
        freeTree(p->m_left);
        freeTree(p->m_right);
        delete p;
    }
};

#endif // TREEMULTIMAP_INCLUDED
