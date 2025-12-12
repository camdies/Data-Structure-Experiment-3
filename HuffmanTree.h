//********** coding: GBK **********/
//程序名: HuffmanTree.h
/*
* 版权信息: 该文件为数据结构实验作业，仅供完成数据结构实验作业以及学习交流使用
* 文件名: HuffmanTree.h
* 摘要: 定义哈夫曼树类、以及实现文本字符统计、压缩解压缩的功能
* @Version: 1.0
* @author: Cairenbin
* 完成日期: 2025.11.28
*/
#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H
#include <iostream>
#include <fstream>
#include <string>

/********** 结构体 HuffmanNode定义 **********/
struct HuffmanNode {
    char ch; //字符
    int weight; //权值
    HuffmanNode* leftChild; //左子节点
    HuffmanNode* rightChild; //右子节点
    HuffmanNode* next; //构建链表优先队列
    HuffmanNode(char c, int w);
    ~HuffmanNode(); //析构函数
};

/********** 类 HuffmanTree定义 **********/
class HuffmanTree {
private:
    HuffmanNode* root; //哈夫曼树根节点
    HuffmanNode* nodeListHead; //链表优先队列头指针

    //递归释放哈夫曼树
    void destroy(HuffmanNode* node);

    //链表构建哈夫曼树
    void buildTree();

    //前序遍历, 生成哈夫曼字符编码串
    void generateCodes(HuffmanNode*node, std::string code);

    //打印哈夫曼树
    void printTree(HuffmanNode* node, int depth, std::ofstream& outfile);

    HuffmanNode* findMin(); //找到链表中最小权值节点

    //存储字符对应编码(二叉链表加链表)
    struct CodeMapNode {
        char ch; //字符
        std::string code; //编码串
        CodeMapNode* link; //下一个编码节点指针
        CodeMapNode(char c, const std::string& cd)
            : ch(c), code(cd), link(nullptr) {}
    };
    CodeMapNode* codeMapHead;

    //插入字符编码到编码表
    void insertCode(char ch, const std::string& code);

    //根据字符获取编码
    std::string getCode(char ch);

    //根据编码获取字符
    char getCharByCode(const std::string& code);

public: 
    //构造函数
    HuffmanTree();

    //析构函数
    ~HuffmanTree();

    //初始化哈夫曼树，读取字符并统计编码表
    bool initalize(std::istream& is, int n);

    //文件操作使用函数
    bool encodeFile(const std::string& inputpath, const std::string& outputpath);
    bool decodeFile(const std::string& inputpath, const std::string& outputpath);

    //打印哈夫曼编码表
    void printCodes(std::ofstream& os, int preLine = 50);
    void printCodesToFile(const std::string& filename, int preLine = 50);

    //打印哈夫曼树
    void printTreeToFile(const std::string& filename);
    void printTreeToConsole();

    bool saveTree(const std::string& filename);
    bool loadTree(const std::string& filename);
};
#endif // HUFFMAN_TREE_H