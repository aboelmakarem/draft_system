//Ahmed Hussein
//ahussei4@jhu.edu

#ifndef TREENODE_H_
#define TREENODE_H_

#include "list"

using namespace std;

template <typename Type> class TreeNode
{
	public:
		TreeNode()
		{
			Initialize();
		}
		TreeNode(const TreeNode& oNode)
		{
			*this = oNode;
		}
		~TreeNode()
		{
			Reset();
		}
		TreeNode& operator=(const TreeNode& oNode)
		{
			m_poParent = oNode.m_poParent;
			m_lpoChildren = oNode.m_lpoChildren;
			m_iLevel = oNode.m_iLevel;
			m_oData = oNode.m_oData;
			m_sLabel = oNode.m_sLabel;
			m_bIsLeaf = oNode.m_bIsLeaf;
			return *this;
		}
		void Reset()
		{
			Initialize();
		}
		void SetParent(TreeNode<Type>* poParent)
		{
			m_poParent = poParent;
		}
		TreeNode<Type>* GetParent()
		{
			return m_poParent;
		}
		void AddChild(TreeNode<Type>* poChild)
		{
			if(poChild == NULL)
			{
				return;
			}
			m_lpoChildren.push_back(poChild);
		}
		void RemoveChild(TreeNode<Type>* poChild)
		{
			typename list< TreeNode<Type>* >::iterator liChildren = m_lpoChildren.begin();
			while(liChildren != m_lpoChildren.end())
			{
				if((*liChildren) == poChild)
				{
					liChildren = m_lpoChildren.erase(liChildren);
				}
				else
				{
					liChildren++;
				}
			}
		}
		void DeleteChildren()
		{
			typename list< TreeNode<Type>* >::iterator liChildren;
			for(liChildren = m_lpoChildren.begin() ; liChildren != m_lpoChildren.end() ; liChildren++)
			{
				if((*liChildren) != NULL)
				{
					(*liChildren)->DeleteChildren();
					delete (*liChildren);
				}
			}
			m_lpoChildren.clear();
		}
		list< TreeNode<Type>* >* GetChildren()
		{
			return &m_lpoChildren;
		}
		void SetLevel(const unsigned int& iLevel)
		{
			m_iLevel = iLevel;
		}
		unsigned int GetLevel() const
		{
			return m_iLevel;
		}
		void SetData(Type oData)
		{
			m_oData = oData;
		}
		Type GetData() const
		{
			return m_oData;
		}
		Type* GetDataPointer()
		{
			return &m_oData;
		}
		void SetLabel(const string& sLabel)
		{
			m_sLabel = sLabel;
		}
		string GetLabel() const
		{
			return m_sLabel;
		}
		void SetLeaf(const bool& bIsLeaf = true)
		{
			m_bIsLeaf = bIsLeaf;
		}
		bool IsLeaf()
		{
			return m_bIsLeaf;
		}
		
	private:
	
	protected:
		void Initialize()
		{
			m_poParent = NULL;
			m_lpoChildren.clear();
			m_iLevel = 0;
			m_sLabel = "";
			m_bIsLeaf = false;
		}
		TreeNode<Type>* m_poParent;
		list< TreeNode<Type>* > m_lpoChildren;
		unsigned int m_iLevel;
		Type m_oData;
		string m_sLabel;
		bool m_bIsLeaf;
};

#endif


