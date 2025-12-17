//********** coding: GBK **********/
//程序名: HuffmanTree.h
/*
* 版权信息: 该文件为数据结构实验作业, 仅供完成数据结构实验3作业以及学习交流使用
* 文件名: HuffmanTree.h
* 摘要: 定义哈夫曼树类, 以及实现文本字符统计, 压缩解压缩的功能函数的声明
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
//类功能: 构建哈夫曼树节点
//*** */
struct HuffmanNode {
    char ch; //字符, 类型为char
    int weight; //权值, 类型为int
    HuffmanNode* leftChild; //左子节点
    HuffmanNode* rightChild; //右子节点
    HuffmanNode* next; //构建链表优先队列
    HuffmanNode(char c, int w); //构造函数
    ~HuffmanNode(); //析构函数
};

/********** 类 HuffmanTree定义 **********/
//类功能: 实现哈夫曼树的各种操作, 包括构建哈夫曼树, 生成编码表, 文件压缩解压缩等功能
/** */
class HuffmanTree {
private:
    HuffmanNode* root; //哈夫曼树根节点
    HuffmanNode* nodeListHead; //链表优先队列头指针


    //函数功能: 递归释放哈夫曼树
    //函数操作对象: HuffmanTree类对象
    //函数参数: HuffmanNode* node, 指向当前节点的指针
    //参数返回值: 无
    void destroy(HuffmanNode* node);


    //函数功能: 链表构建哈夫曼树
    //函数操作对象: HuffmanTree类对象
    //函数参数: 无
    //参数返回值: 无
    void buildTree();


    //函数功能: 递归生成编码表
    //函数操作对象: HuffmanTree类对象
    //函数参数: HuffmanNode* node, 指向当前节点的指针
    //          std::string code, 当前编码串
    //参数返回值: 无
    void generateCodes(HuffmanNode*node, std::string code);


    //函数功能: 打印哈夫曼树
    //函数操作对象: HuffmanTree类对象
    //函数参数: HuffmanNode* node, 指向当前节点的指针
    //          int depth, 当前节点深度
    //          ofstream& outfile, 输出文件流引用
    //参数返回值: 无
    void printTree(HuffmanNode* node, int depth, std::ofstream& outfile);


    //函数功能: 辅助函数, 用于递归打印直观的树结构
    //函数操作对象: HuffmanTree类对象
    //函数参数: HuffmanNode* node, 指向当前节点的指针
    //          int depth, 当前节点深度
    //          std::string type, 节点类型标识 ("L" 左子节点, "R" 右子节点, "Root" 根节点)
    //          std::ostream& os, 输出流引用
    //参数返回值: 无
    void printTreeStructure(HuffmanNode* node, int depth, std::string type, std::ostream& os);
    //void printTreeStructure(HuffmanNode* node, int depth, std::string type);

    
    //函数功能: 找到链表中最小权值节点
    //函数操作对象: HuffmanTree类对象
    //函数参数: 无
    //参数返回值: HuffmanNode*, 指向最小权值节点的指针
    HuffmanNode* findMin();

    //********** 私有结构体 CodeMapNode定义 **********/
    //结构体功能: 存储字符对应编码(二叉链表加链表)
    ///*** */
    struct CodeMapNode {
        char ch; //字符
        std::string code; //编码串
        CodeMapNode* link; //下一个编码节点指针
        CodeMapNode(char c, const std::string& cd)
            : ch(c), code(cd), link(nullptr) {}
    };
    CodeMapNode* codeMapHead; //编码表头指针


    //函数功能: 插入字符编码到编码表
    //函数操作对象: HuffmanTree类对象
    //函数参数: char ch, 字符
    //          std::string code, 编码串
    //参数返回值: 无
    void insertCode(char ch, const std::string& code);


    //函数功能: 根据字符获取编码
    //函数操作对象: HuffmanTree类对象
    //函数参数: char ch, 字符
    //参数返回值: std::string, 编码串
    std::string getCode(char ch);


    //函数功能: 根据编码获取字符
    //函数操作对象: HuffmanTree类对象
    //函数参数: std::string code, 编码串
    //参数返回值: char, 字符
    char getCharByCode(const std::string& code);

public: 
    //构造函数
    HuffmanTree();

    //析构函数
    ~HuffmanTree();

    //函数功能: 初始化哈夫曼树, 读取字符并统计编码表
    //函数操作对象: HuffmanTree类对象
    //函数参数: istream& is, 输入流引用
    //          int n, 字符数量
    //参数返回值: bool, 初始化是否成功
    bool initalize(std::istream& is, int n);


    //函数功能: 文件操作使用函数
    //函数操作对象: HuffmanTree类对象
    //函数参数: const std::string& inputpath, 输入文件路径
    //          const std::string& outputpath, 输出文件路径
    //参数返回值: bool, 操作是否成功
    bool encodeFile(const std::string& inputpath, const std::string& outputpath);


    //函数功能: 文件操作使用函数
    //函数操作对象: HuffmanTree类对象
    //函数参数: const std::string& inputpath, 输入文件路径
    //          const std::string& outputpath, 输出文件路径
    //参数返回值: bool, 操作是否成功
    bool decodeFile(const std::string& inputpath, const std::string& outputpath);


    //函数功能: 打印哈夫曼编码表
    //函数操作对象: HuffmanTree类对象
    //函数参数: ofstream& os, 输出流引用
    //          int preLine, 每行打印多少个编码对, 默认50
    //参数返回值: 无
    void printCodes(std::ofstream& os, int preLine = 50);


    //函数功能: 打印哈夫曼编码表到文件
    //函数操作对象: HuffmanTree类对象
    //函数参数: const std::string& filename, 输出文件路径
    //          int preLine, 每行打印多少个编码对, 默认50
    //参数返回值: 无
    void printCodesToFile(const std::string& filename, int preLine = 50);


    //函数功能: 打印哈夫曼编码表到控制台
    //函数操作对象: HuffmanTree类对象
    //             读取codeFileName文件, 默认CodeFile
    //             写入codePrinFileName文件, 默认CodePrin
    //函数参数: int preLine, 每行打印多少个编码对, 本次强制默认50
    //参数返回值: 无
    void printCodeFileAndWriteCodePrin(const std::string& codeFileName, const std::string& codePrinFileName, int perLine = 50);


    //函数功能: 打印哈夫曼树
    //函数操作对象: HuffmanTree类对象
    //             写入文件filename, 默认TreePrin
    //函数参数: const std::string& filename, 输出文件路径
    //参数返回值: 无
    void printTreeToFile(const std::string& filename);


    //函数功能: 打印哈夫曼树到控制台
    //函数操作对象: HuffmanTree类对象
    //参数返回值: 无
    void printTreeToConsole();


    //函数功能: 保存哈夫曼树到文件
    //函数操作对象: HuffmanTree类对象
    //函数参数: const std::string& filename, 输出文件路径
    //参数返回值: bool, 保存是否成功
    bool saveTree(const std::string& filename);


    //函数功能: 加载哈夫曼树从文件
    //函数操作对象: HuffmanTree类对象
    //函数参数: const std::string& filename, 输入文件路径
    //参数返回值: bool, 加载是否成功
    bool loadTree(const std::string& filename);
};
#endif // HUFFMAN_TREE_H