//********** coding: GBK **********/
//程序名: HuffmanTree.cpp
/**
* 版权信息: 该文件为数据结构实验作业，仅供完成数据结构实验作业以及学习交流使用
* 文件名: HuffmanTree.cpp
* 摘要: 定义哈夫曼树类、以及实现文本字符统计、压缩解压缩的功能, 并实现函数入口、控制台输出等
* @Version: 1.0
* @author: Cairenbin
* 完成日期: 2025.11.28
*/
#include "HuffmanTree.h"
#include "queue"
#include <sstream>

//HuffmanNode 部分
HuffmanNode::HuffmanNode(char c, int w):
    ch(c), weight(w), leftChild(nullptr), 
    rightChild(nullptr), next(nullptr) {}

HuffmanNode::~HuffmanNode() {} //析构函数

//HuffmanTree 部分

char HuffmanTree::getCharByCode(const std::string& code) {
    CodeMapNode* curr = codeMapHead;
    while(curr) {
        if(curr->code == code) {
            return curr->ch;
        }
        curr = curr->link;
    }
    return '\0'; // 未找到
}

HuffmanTree::HuffmanTree():
    root(nullptr), nodeListHead(nullptr), codeMapHead(nullptr) {}
HuffmanTree::~HuffmanTree() {
    destroy(root);
    while(codeMapHead) { //释放编码表
        CodeMapNode* temp = codeMapHead;
        codeMapHead = codeMapHead->link;
        delete temp;
    }
    while(nodeListHead) { //释放链表优先队列
        HuffmanNode* temp = nodeListHead;
        nodeListHead = nodeListHead->next;
        delete temp;

    }
}

//递归释放哈夫曼树
void HuffmanTree::destroy(HuffmanNode* node) {
    if(node) {
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
    }
}

//初始化哈夫曼树，读取字符并统计编码表
bool HuffmanTree::initalize(std::istream& is, int n) {
    //读取n个字符和权值，构造链表
    nodeListHead = nullptr;
    HuffmanNode* tail = nullptr;
    for(int i = 0; i < n; ++i) {
        char ch;
        int weight;
        is >> ch >> weight;
        HuffmanNode* newNode = new HuffmanNode(ch, weight);
        if(!nodeListHead) {
            nodeListHead = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    buildTree(); //构建哈夫曼树
    generateCodes(root, ""); //生成编码表
    return true;
}

//构建哈夫曼树
void HuffmanTree::buildTree() {
    while(nodeListHead && nodeListHead->next) {
        HuffmanNode* min1 = findMin();
        HuffmanNode* min2 = findMin();
        HuffmanNode* newNode = new HuffmanNode('*', min1->weight + min2->weight);
        newNode->leftChild = min1;
        newNode->rightChild = min2;
        //插入到链表头
        newNode->next = nodeListHead;
        nodeListHead = newNode;
    }
    root = nodeListHead; //最后剩下的节点为根节点
}

//找到链表中最小权值节点, 并将其移除出链表
HuffmanNode* HuffmanTree::findMin() {
    if(!nodeListHead) return nullptr;
    HuffmanNode* prevMin = nullptr; //最小节点的前驱
    HuffmanNode* minNode = nodeListHead; //最小节点
    HuffmanNode* prev = nullptr; //当前节点的前驱
    HuffmanNode* curr = nodeListHead; //当前节点
    int minValue = (minNode ? minNode->weight : 0);
    while(curr) {
        if(curr->weight < minValue) {
            minValue = curr->weight;
            prevMin = prev;
            minNode = curr;
        }
        prev = curr;
        curr = curr->next;
    }
    //从链表中移除最小节点
    if(prevMin) prevMin->next = minNode->next;
    else nodeListHead = minNode->next;

    minNode->next = nullptr; //断开与链表的连接
    return minNode;
}

//递归生成 codeMap
void HuffmanTree::generateCodes(HuffmanNode* node, std::string code) {
    if(!node) return;
    if(!node->leftChild && !node->rightChild) {
        insertCode(node->ch, code);
    }
    generateCodes(node->leftChild, code + "0");
    generateCodes(node->rightChild, code + "1");
}

//插入编码 map
void HuffmanTree::insertCode(char ch, const std::string& code) {
    CodeMapNode* newNode = new CodeMapNode(ch, code);
    newNode->link = codeMapHead;
    codeMapHead = newNode;
}

std::string HuffmanTree::getCode(char ch) {
    CodeMapNode* curr = codeMapHead;
    while(curr) {
        if(curr->ch == ch) {
            return curr->code; //找到对应编码
        }
        curr = curr->link;
    }
    return "\0"; //未找到
}

//编码文件
bool HuffmanTree::encodeFile(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream infile(inputPath, std::ios::in);
    if(!infile.is_open()) {
        std::cerr << "无法打开输入文件: " << inputPath << std::endl;
        return false;
    }
    std::ofstream outfile(outputPath, std::ios::out);
    if(!outfile.is_open()) {
        std::cerr << "无法打开输出文件: " << outputPath << std::endl;
        infile.close();
        return false;
    }
    char ch;
    int count = 0;
    while(infile.get(ch)) {
        std::string code = getCode(ch);
        outfile << code;
        count += code.length();
    }
    infile.close();
    outfile.close();
    return true;
}

//解码文件
bool HuffmanTree::decodeFile(const std::string& inputPath, const std::string& outputPath) {
    std::ifstream infile(inputPath, std::ios::in);
    if(!infile.is_open()) {
        std::cerr << "无法打开输入文件: " << inputPath << std::endl;
        return false;
    }
    std::ofstream outfile(outputPath, std::ios::out);
    if(!outfile.is_open()) {
        std::cerr << "无法打开输出文件: " << outputPath << std::endl;
        infile.close();
        return false;
    }
    std::string encoded, curCode;
    infile >> encoded;
    for(char bit : encoded) {
        curCode += bit;
        char decodedChar = getCharByCode(curCode);
        if(decodedChar != '\0') {
            outfile << decodedChar;
            curCode = "";
        }
    }
    infile.close();
    outfile.close();
    return true;
}

//
void HuffmanTree::printCodes(std::ofstream& os, int preLine) {
    CodeMapNode* curr = codeMapHead;
    int count = 0;
    while(curr) {
        os << curr->ch << ": " << curr->code << " ";
        if(++count % preLine == 0) {
            os << std::endl;
        }
        curr = curr->link;
    }
    os << std::endl;
}

void HuffmanTree::printCodesToFile(const std::string& filename, int preLine) {
    std::ofstream outfile(filename);
    if(outfile) {
        printCodes(outfile, preLine);
        outfile.close();
    }
}

//打印哈夫曼树到文件
void HuffmanTree::printTree(HuffmanNode* node, int depth, std::ofstream& outfile) {
    if(!node) return;
    for(int i = 0; i < depth; ++i) {
        std::cout << "    ";
        outfile << "    ";
    }
    std::cout << node->ch << "(" << node->weight << ")" << std::endl;
    outfile << node->ch << "(" << node->weight << ")" << std::endl;
    printTree(node->leftChild, depth + 1, outfile);
    printTree(node->rightChild, depth + 1, outfile);
}

void HuffmanTree::printTreeToConsole() {
    std::ofstream dummy("NUL");
    printTree(root, 0, dummy);
}

void HuffmanTree::printTreeToFile(const std::string& filename) {
    std::ofstream outfile(filename);
    if(outfile.is_open()) {
        printTree(root, 0, outfile);
        outfile.close();
    }
}
