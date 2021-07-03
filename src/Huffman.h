#ifndef HUFFMAN_H
#define HUFFMAXN_H

#include <unordered_map>
#include <vector>
#include <queue>

struct HuffmanTreeNode {
	HuffmanTreeNode* child[2];
	size_t Size;
	unsigned char Val;
	HuffmanTreeNode() :child{ nullptr,nullptr }, Size(0),Val(0) {}
	HuffmanTreeNode(const size_t& _Size,unsigned char _Val=0) : child{nullptr,nullptr}, Size(_Size),Val(_Val) {
	}
	~HuffmanTreeNode() {
		if(child[0]!=nullptr)
			delete child[0];
		if(child[1]!=nullptr)
			delete child[1];
	}
	bool isleaf(){return child[0]==nullptr; }
	bool operator<(const HuffmanTreeNode&other)const {
		return Size>other.Size;
	}
};

class Huffman {
private:
	HuffmanTreeNode*rt;
	unsigned int sum[256];//¼ÇÂ¼×Ö·û³öÏÖÆµÂÊ
	int LastLength;//³¤¶È

	typedef std::pair<unsigned int,unsigned int> bitval;

	void makeroot(HuffmanTreeNode*treenode,unsigned int dep,unsigned int Val,
		std::unordered_map<unsigned char,bitval>&);
	void treebuild();
	std::string treecode();//´æ´¢¹ş·òÂüÊ÷
	int build(const std::string&);//¹ş·òÂüÊ÷±àÂë¹¹½¨
public:
	Huffman() :rt(nullptr) {
	}

	std::string encode(const std::string&);//±àÂë
	std::string decode(const std::string&);
};

#endif 