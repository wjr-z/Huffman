#include "Huffman.h"
#include <iostream>

//#define DEBUG
using namespace std;
void Huffman::makeroot(HuffmanTreeNode* treenode,unsigned int dep,
	unsigned int Val, std::unordered_map<unsigned char,bitval>& wjr) {
	if (treenode == nullptr)return;
	if (treenode->isleaf()) {
		wjr[treenode->Val]={dep,Val};
		return;
	}
	makeroot(treenode->child[0],dep+1,Val<<1, wjr);
	makeroot(treenode->child[1],dep+1,Val<<1|1, wjr);
}

struct cmp {
	bool operator()(HuffmanTreeNode*lhs,HuffmanTreeNode*rhs)const{
		return (*lhs)<(*rhs);
	}
};

void Huffman::treebuild() {
	if(rt!=nullptr)delete rt,rt=nullptr;

	std::priority_queue<HuffmanTreeNode*,vector<HuffmanTreeNode*>,cmp>que;

	for (int i = 0; i < 256; ++i) {
		if(!sum[i])continue;
		que.push(new HuffmanTreeNode(sum[i],i));
	}

	int InQueSize = que.size();
	while (InQueSize > 1) {
		HuffmanTreeNode* top1 = que.top();
		que.pop();
		HuffmanTreeNode* top2 = que.top();
		que.pop();
		--InQueSize;

		HuffmanTreeNode* fa = new HuffmanTreeNode(top1->Size + top2->Size);
		fa->child[0] = top1;
		fa->child[1] = top2;
		que.push(fa);
	}
	rt = que.top();
	que.pop();
}

std::string Huffman::encode(const std::string&arr) {

	unsigned char*ql=(unsigned char*)(arr.c_str());
	unsigned int Length = arr.length();
	if (!Length)return "";

	memset(sum,0,sizeof(sum));

	for (int i = 0; i < Length; ++i) 
		++sum[ql[i]];

	treebuild();

	std::unordered_map<unsigned char,bitval>wjr;
	makeroot(rt,0,0, wjr);

	std::string _encode;
	unsigned int V=0;
	unsigned int valLen=0;

#ifdef DEBUG
	for (int i = 0; i < 256; ++i) {
		if(!sum[i])continue;
		unsigned int v=wjr[i].second;
		unsigned int Len=wjr[i].first;
		cout<<i<<' '<<Len<<' ';
		for(int j=Len-1;~j;--j)
			cout<<((v>>j)&1);
		cout<<endl;
	}
#endif
	
	if (wjr.size() == 1) {
		int arf=Length>>3;
		for (int i = 0; i < arf; ++i) 
			_encode.push_back(0);
		LastLength=Length&7;
		if(LastLength)_encode.push_back(0);
	}
	else {
		bitval _hash[256];
		for(int i=0;i<256;++i)
			if(sum[i])_hash[i]=wjr[i];
		for (int i = 0; i < Length; ++i) {
			auto it = _hash[ql[i]];
			unsigned int v = it.second;
			unsigned int Len = it.first;

			V = (V << Len) | v;
			valLen += Len;

			while (valLen >= 8) {
				_encode.push_back( (V >> (valLen - 8)));
				V &= valLen!=8?((1<<valLen-8)-1):0;
				valLen -= 8;
			}
		}
		LastLength = valLen?valLen:8;
		if (valLen)
			_encode .push_back( V<<(8-valLen));
	}

	return treecode().append(_encode);
}

void appendint(std::string& str, int Val) {
	str+=(Val>>24);
	str+=(Val>>16)&255;
	str+=(Val>>8)&255;
	str+=Val&255;
}

std::string Huffman::treecode() {//·µ»ØÊ÷±àÂë
	std::string str;
 	str+="wjrorz";//Í·
	str+=LastLength;
	for (int i = 0; i < 256; ++i) {
		if (sum[i]) {
			str+=i;
			appendint(str,sum[i]);
		}
	}
	str+="tql";//Î²
	return str;
}
int Huffman::build(const std::string&treecode) {
	if (treecode.substr(0, 6) != "wjrorz")
		return -1;
	int Length=treecode.size();
	memset(sum,0,sizeof(sum));
	unsigned char*ql=(unsigned char*)treecode.c_str();
	LastLength=ql[6];
	int i;
	for (i = 7;i<Length; i += 5) {
		if (ql[i] == 't'&&ql[i+1]=='q'&&ql[i+2]=='l')
			break;
		unsigned char X=ql[i];
		unsigned int _Sum=(ql[i+1]<<24)|(ql[i+2]<<16)|(ql[i+3]<<8)|ql[i+4];
		sum[X]=_Sum;
	}
	if(i>=Length)return -1;
	treebuild();
	return i+3;
}

std::string Huffman::decode(const std::string& str) {
	int Length=str.length();
	int head=build(str);
	if(head==-1)return "";

	HuffmanTreeNode*x=rt;
	std::string ans;
	for (int i = head; i < Length - 1; ++i) {
		unsigned char _ch=str[i];
		for (int j = 7;~j ; --j) {
			bool cho = (_ch>>j)&1;
			x = x->child[cho];
		#ifdef DEBUG
			cout<<cho;
		#endif
			if (x->isleaf()) {
				ans.push_back(x->Val);
				x = rt;
			}
		}
	}
	unsigned char _ch=str[Length-1];
	for (int j = 7; j >= 8 - LastLength; --j) {
		bool cho = (_ch >> j) & 1;
		x = x->child[cho];
	#ifdef DEBUG
		cout<<cho;
	#endif
		if (x->isleaf()) {
			ans.push_back(x->Val);
			x = rt;
		}
	}
#ifdef DEBUG
	cout<<endl;
#endif

	return ans;
}