#include "treenode.h"
#define ll long long int

Treenode::Treenode()
{
	this->parent = NULL;
	this->aset.clear();
	this->data.clear();
	this->children.clear();
	this->pos=0;
	this->neg=0;
}

std::vector< data_ds > Treenode::getData()
{
	return this->data;
}

void Treenode::setData(std::vector< data_ds > &data)
{
	this->data = data;
	int i,size;
	pos=0;
	neg=0;
	size=this->data.size();
	for(i=0;i<size;i++)
	{
		if(this->data[i].val==false)
		{
			neg++;
		}
		else
		{
			pos++;
		}
	}
}

int Treenode::getAno()
{
	return this->ano;
}

void Treenode::setAno(int no)
{
	this->ano = no;
}

Treenode* Treenode::getParent()
{
	return this->parent;
}

void Treenode::setParent(Treenode* parent)
{
	this->parent = parent;
}

std::vector<Treenode* > Treenode::getChildren()
{
	return this->children;
}

void Treenode::setChildren(std::vector<Treenode* >children)
{
	this->children = children;
}