#pragma once
#include <vector>
#include <memory>
#include <string>
#include <QVariant>
#include <map>
#include "../qtproxies/DataSerializer.h"
#include "../qtproxies/DataDeserializer.h"

namespace SimpleTree
{

template <typename T>
class Node
{
public:

    Node(const std::shared_ptr<T> &nodeValue = std::shared_ptr<T>())
    {
        m_value = nodeValue;
    }
    void appendNodes(const std::vector<SimpleTree::Node<T> > &nodes)
    {
        m_nodes.insert(m_nodes.end(), nodes.begin(), nodes.end());
    }
    void appendNode(const Node<T> &node)
    {
        m_nodes.push_back(node);
    }
    void removeNode(const int& index)
    {
        typename std::vector<SimpleTree::Node<T> >::iterator it = m_nodes.begin()+index;
        m_nodes.erase(it);
    }
    std::vector<SimpleTree::Node<T> > &getNodes()
    {
        return m_nodes;
    }
    const std::vector<SimpleTree::Node<T> > &getNodes() const
    {
        return m_nodes;
    }
    Node<T> &getNode(unsigned index)
    {
        return m_nodes.at(index);
    }
    void setName(const std::string &name)
    {
        m_name = name;
    }
    std::string getName() const
    {
        return m_name;
    }

    unsigned size() const
    {
        return m_nodes.size();
    }

    void appendNodeValue(const std::shared_ptr<T> &nodeValue)
    {
        m_value = nodeValue;
    }

    void emplaceNode(const std::shared_ptr<T> &nodeValue)
    {
        Node<T> node(nodeValue);
        m_nodes.push_back(node);
    }

    std::shared_ptr<T> getNodeValue() const
    {
        return m_value;
    }

    std::shared_ptr<T> operator->()
    {
        return m_value;
    }

    std::shared_ptr<T> operator*()
    {
        return m_value;
    }

    bool operator==(const Node<T>& otherNode) const
    {
        if(m_name!=m_name)
            return false;
        if(m_nodes.size() != otherNode.size())
            return false;
        if(m_value.get() && !otherNode.getNodeValue().get())
            return false;
        if(!m_value.get() && otherNode.getNodeValue().get())
            return false;

        auto otherNodes = otherNode.getNodes();
        for(int i=0; i<m_nodes.size(); i++)
        {
            if(!(m_nodes.at(i)==otherNodes.at(i)))
                return false;
        }
        return true;
    }

    QVariant getInfo(quint8 role) const
    {
        return m_infos.at(role);
    }

    void setInfo(unsigned role, const QVariant &value)
    {
        m_infos[role] = value;
    }

    std::map<quint8, QVariant> getInfos() const
    {
        return m_infos;
    }

    void setInfos(std::map<quint8, QVariant> p_infos)
    {
        this->m_infos = p_infos;
    }


private:
    std::string m_name;
    std::map<quint8, QVariant> m_infos;
    std::shared_ptr<T> m_value;
    std::vector<SimpleTree::Node<T> > m_nodes;
};

}
