//********** coding: GBK **********/
//程序名: HuffmanTree.cpp
/**
* 版权信息: 该文件为数据结构实验作业, 仅供完成数据结构实验3作业以及学习交流使用
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
////////////////////////////////////////////////////////


//HuffmanTree 部分

//函数类型: 私有
//函数功能: 递归释放哈夫曼树
//函数操作对象: HuffmanTree类对象
//函数参数: HuffmanNode* node, 指向当前节点的指针
//参数返回值: 无
void HuffmanTree::destroy(HuffmanNode* node) {
    if(node) {
        destroy(node->leftChild);
        destroy(node->rightChild);
        delete node;
    }
}
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 链表构建哈夫曼树
//函数操作对象: HuffmanTree类对象
//函数参数: 无
//参数返回值: 无
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
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 递归生成 codeMap
//函数操作对象: HuffmanTree类对象
//函数参数: HuffmanNode* node, 指向当前节点的指针
//          std::string code, 当前节点对应的编码串
//参数返回值: 无
void HuffmanTree::generateCodes(HuffmanNode* node, std::string code) {
    if(!node) return;
    if(!node->leftChild && !node->rightChild) {
        insertCode(node->ch, code);
    }
    generateCodes(node->leftChild, code + "0");
    generateCodes(node->rightChild, code + "1");
}
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 打印哈夫曼树到文件
//函数操作对象: HuffmanTree类对象, 
//函数参数: HuffmanNode* node, 指向当前节点的指针
//          int depth, 当前节点深度
//          ofstream& outfile, 输出文件流引用
//参数返回值: 无
void HuffmanTree::printTree(HuffmanNode* node, int depth, std::ofstream& outfile) {
    if(!node) return;
    for(int i = 0; i < depth; ++i) {
        //std::cout << "    ";
        outfile << "    ";
    }
    //std::cout << node->ch << "(" << node->weight << ")" << std::endl;
    outfile << node->ch << "(" << node->weight << ")" << std::endl;
    printTree(node->leftChild, depth + 1, outfile);
    printTree(node->rightChild, depth + 1, outfile);
}
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 辅助函数, 用于递归打印直观的树结构
//函数操作对象: HuffmanTree类对象
//函数参数: HuffmanNode* node, 指向当前节点的指针
//          int depth, 当前节点深度
//          std::string type, 节点类型标识 ("L" 左子节点, "R" 右子节点, "Root" 根节点)
//          std::ostream& os, 输出流引用
//参数返回值: 无
void HuffmanTree::printTreeStructure(HuffmanNode* node, int depth, std::string type, std::ostream& os) {
    if (node == nullptr) {
        return;
    }

    // 1. 打印缩进 (每一层缩进4个空格)
    for (int i = 0; i < depth; ++i) {
        os << "    |"; // 使用 | 辅助对齐
    }

    // 2. 打印节点连接符
    if (depth > 0) {
        os << "----";
    }

    // 3. 打印节点内容
    // 判断是否为叶子节点 (没有左右孩子)
    if (node->leftChild == nullptr && node->rightChild == nullptr) {
        // 叶子节点: 显示 [字符] 和 (权值)
        os << type << ": [" << node->ch << "] (weight:" << node->weight << ")" << std::endl;
    } else {
        // 内部节点: 只显示 <权值之和>
        os << type << ": <weight:" << node->weight << ">" << std::endl;
    }

    // 4. 递归打印子树
    // 先左后右, 深度+1
    printTreeStructure(node->leftChild, depth + 1, "L", os);
    printTreeStructure(node->rightChild, depth + 1, "R", os);
}
/*
void HuffmanTree::printTreeStructure(HuffmanNode* node, int depth, std::string type) {
    if (node == nullptr) {
        return;
    }

    // 1. 打印缩进 (每一层缩进4个空格)
    for (int i = 0; i < depth; ++i) {
        std::cout << "    |"; // 使用 | 辅助对齐
    }

    // 2. 打印节点连接符
    if (depth > 0) {
        std::cout << "----";
    }

    // 3. 打印节点内容
    // 判断是否为叶子节点 (没有左右孩子)
    if (node->leftChild == nullptr && node->rightChild == nullptr) {
        // 叶子节点: 显示 [字符] 和 (权值)
        std::cout << type << ": [" << node->ch << "] (权值:" << node->weight << ")" << std::endl;
    } else {
        // 内部节点: 只显示 <权值之和>
        std::cout << type << ": <权值:" << node->weight << ">" << std::endl;
    }

    // 4. 递归打印子树
    // 先左后右, 深度+1
    printTreeStructure(node->leftChild, depth + 1, "L");
    printTreeStructure(node->rightChild, depth + 1, "R");
}
*/
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 找到链表中最小权值节点
//函数操作对象: HuffmanTree类对象
//函数参数: 无
//参数返回值: HuffmanNode*, 指向最小权值节点的指针
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
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 插入字符编码到编码表
//函数操作对象: HuffmanTree类对象
//函数参数: char ch, 字符
//          std::string code, 编码串
//参数返回值: 无
void HuffmanTree::insertCode(char ch, const std::string& code) {
    CodeMapNode* newNode = new CodeMapNode(ch, code);
    newNode->link = codeMapHead;
    codeMapHead = newNode;
}
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 根据字符获取编码
//函数操作对象: HuffmanTree类对象
//函数参数: char ch, 字符
//参数返回值: std::string, 编码串
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
////////////////////////////////////////////////////////


//函数类型: 私有
//函数功能: 根据编码获取字符
//函数操作对象: HuffmanTree类对象
//函数参数: std::string code, 编码串
//参数返回值: char, 字符
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
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 构造函数
//函数操作对象: HuffmanTree类对象
//函数参数: 无
//参数返回值: 无
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
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 初始化哈夫曼树, 读取字符并统计编码表, 便于后续文件编码压缩和解压缩
//函数操作对象: HuffmanTree类对象
//函数参数: istream& is, 输入流引用
//          int n, 字符数量
//参数返回值: bool, 初始化是否成功
bool HuffmanTree::initalize(std::istream& is, int n) {
    // 先清理可能存在的旧数据, 防止多次初始化导致内存泄漏
    destroy(root);
    root = nullptr;
    while (codeMapHead) {
        CodeMapNode* temp = codeMapHead;
        codeMapHead = codeMapHead->link;
        delete temp;
    }
    codeMapHead = nullptr;

    //读取n个字符和权值, 构造链表
    nodeListHead = nullptr;
    HuffmanNode* tail = nullptr;
    for(int i = 0; i < n; ++i) {
        char ch;
        int weight;
        std::string tempStr;
        
        // 1. 尝试读取字符字符串
        if (!(is >> tempStr)) {
            std::cerr << "[Error!] Not enough input data! Expected " << n << " groups, but interrupted at group " << i + 1 << " 组字符时中断!" << std::endl;
            // 清理已创建的链表节点
            while (nodeListHead) {
                HuffmanNode* temp = nodeListHead;
                nodeListHead = nodeListHead->next;
                delete temp;
            }
            return false;
        }
        // 处理 Space, Enter 等特殊字符串
        if (tempStr == "Space" || tempStr == "SPACE" || tempStr == "space") {
            ch = ' ';  // 将 "Space" 识别为空格字符
        } else if (tempStr == "Enter" || tempStr == "ENTER" || tempStr == "enter") {
            ch = '\n'; // 将 "Enter" 识别为换行符
        } else if(tempStr == "Tab" || tempStr == "TAB" || tempStr == "tab") {
            ch = '\t'; // 将 "Tab" 识别为制表符
        } else if(tempStr == "Hash" || tempStr == "HASH" || tempStr == "hash") {
            ch = '#'; // 将 "Hash" 识别为#字符
        } else {
            ch = tempStr[0]; 
        }

        // 2. 尝试读取权值
        if (!(is >> weight)) {
            //std::cerr << "[Error!]读取权值失败！请检查第 " << i + 1 << " 个字符 [" << tempStr << "] 后的数值格式!" << std::endl;
            std::cerr << "[Error!] Failed to read weight! Please check the value format after character [" << tempStr << "] at group " << i + 1 << "." << std::endl;
            // 清理已创建的链表节点
            while (nodeListHead) {
                HuffmanNode* temp = nodeListHead;
                nodeListHead = nodeListHead->next;
                delete temp;
            }
            return false;
        }

        HuffmanNode* newNode = new HuffmanNode(ch, weight);
        if(!nodeListHead) {
            nodeListHead = newNode; //链表为空的时候初始化头指针
            tail = newNode; //初始化尾指针
        } else {
            tail->next = newNode; //插入到链表尾部
            tail = newNode; //更新尾指针
        }
    }
    buildTree(); //构建哈夫曼树
    generateCodes(root, ""); //生成编码表
    saveTree("hfmTree"); //保存哈夫曼树到文件
    return true;
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 编码文件, 将文本压缩为二进制编码文件存入CodeFile
//函数操作对象: HuffmanTree类对象, 读取inputPath文件, 写入outputPath文件
//函数参数: const std::string& inputPath, 输入文件路径
//          const std::string& outputPath, 输出文件路径
//参数返回值: bool, 编码是否成功
bool HuffmanTree::encodeFile(const std::string& inputPath, const std::string& outputPath) {
    if (!root) { //检查哈夫曼树是否在内存中
        if (!loadTree("hfmTree")) { //尝试从文件加载哈夫曼树
            std::cerr << "Huffman tree not in memory and cannot be loaded from file!" << std::endl;
            return false;
        }
    }
    std::ifstream infile(inputPath, std::ios::in);
    if(!infile.is_open()) {
        std::cerr << "Cannot open input file: " << inputPath << std::endl;
        return false;
    }
    std::ofstream outfile(outputPath, std::ios::out | std::ios::binary);
    if(!outfile.is_open()) {
        std::cerr << "Cannot open output file: " << outputPath << std::endl;
        infile.close();
        return false;
    }
    long long totalBits = 0; // 1. 预留空间存储总比特数 (long long 类型, 8字节)
    outfile.write(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));
    char buffer = 0; // 字节缓冲区
    int bitCount = 0; // 当前缓冲区已填入的位数

    char ch;
    while (infile >> std::noskipws >> ch) {
        //if (ch == '\n' || ch == '\r') continue;
        // 跳过中文字符（GBK编码下首字节大于127的字符及其后一个字节）
        if ((unsigned char)ch > 127) {
            char skipCh;
            infile >> std::noskipws >> skipCh; // 跳过第二个字节
            continue;
        }
        std::string code = getCode(ch);
        if (code.empty()) {
            std::cerr << "Warning: Character [" << ch << "] not defined, skipped." << std::endl;
            continue;
        }

        // 2. 将字符串 "01" 转换为比特位
        for (char bitChar : code) {
            if (bitChar == '1') {
                // 将对应位置 1 (从高位到低位)
                buffer |= (1 << (7 - bitCount));
            }
            // 如果是 '0', 对应位默认为0, 无需操作
            
            bitCount++;
            totalBits++;

            // 缓冲区满8位, 写入文件
            if (bitCount == 8) {
                outfile.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }

    // 3. 处理最后一个未满的字节
    if (bitCount > 0) {
        outfile.put(buffer);
    }

    // 4. 回到文件开头, 写入实际的总比特数: 
    outfile.seekp(0);
    outfile.write(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

    infile.close();
    outfile.close();
    return true;
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 解码文件, 将二进制编码文件解压为文本存入outputPath
//函数操作对象: HuffmanTree类对象, 读取inputPath文件, 写入outputPath文件
//函数参数: const std::string& inputPath, 输入文件路径
//          const std::string& outputPath, 输出文件路径
//参数返回值: bool, 解码是否成功
bool HuffmanTree::decodeFile(const std::string& inputPath, const std::string& outputPath) {
    if (!root) { //检查哈夫曼树是否在内存中
        if (!loadTree("hfmTree")) { //尝试从文件加载哈夫曼树!
            std::cerr << "Huffman tree not in memory and cannot be loaded from file!" << std::endl;
            return false;
        }
    }
    std::ifstream infile(inputPath, std::ios::in | std::ios::binary);
    if(!infile.is_open()) {
        std::cerr << "Cannot open input file: " << inputPath << std::endl;
        return false;
    }
    std::ofstream outfile(outputPath, std::ios::out);
    if(!outfile.is_open()) {
        std::cerr << "Cannot open output file: " << outputPath << std::endl;
        infile.close();
        return false;
    }

    // 1. 读取总比特数
    long long totalBits = 0;
    infile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));

    char buffer;
    long long bitsRead = 0;
    std::string curCode = "";

    // 2. 逐字节读取
    while (infile.get(buffer)) {
        // 提取字节中的每一位
        for (int i = 0; i < 8; ++i) {
            if (bitsRead >= totalBits) break; // 读够了就停止, 忽略填充位

            // 检查当前位是 0 还是 1
            int bit = (buffer >> (7 - i)) & 1;
            
            curCode += (bit == 1 ? '1' : '0');
            bitsRead++;

            char decodedChar = getCharByCode(curCode);
            if (decodedChar != '\0') {
                outfile << decodedChar;
                curCode = "";
            }
        }
    }
    infile.close();
    outfile.close();
    return true;
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 打印编码表到输出流
//函数操作对象: HuffmanTree类对象
//函数参数: ofstream& os, 输出流引用
//          int preLine, 每行打印多少个编码对
//参数返回值: 无
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
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 打印编码表到文件
//函数操作对象: HuffmanTree类对象
//函数参数: const std::string& filename, 输出文件路径
//          int preLine, 每行打印多少个编码对
//参数返回值: 无
void HuffmanTree::printCodesToFile(const std::string& filename, int preLine) {
    std::ofstream outfile(filename);
    if(outfile) {
        printCodes(outfile, preLine);
        outfile.close();
    }
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 打印编码表到控制台
//函数操作对象: HuffmanTree类对象
//             读取codeFileName文件, 默认CodeFile
//             写入codePrinFileName文件, 默认CodePrin
//函数参数: int preLine, 每行打印多少个编码对, 本次强制默认50
//参数返回值: 无
void HuffmanTree::printCodeFileAndWriteCodePrin(const std::string& codeFileName, const std::string& codePrinFileName, int perLine) {
    std::ifstream codeFile(codeFileName, std::ios::in | std::ios::binary);
    std::ofstream codePrinFile(codePrinFileName);
    if (!codeFile.is_open()) {
        std::cerr << "Cannot open CodeFile: " << codeFileName << std::endl;
        return;
    }
    if (!codePrinFile.is_open()) {
        std::cerr << "Cannot open CodePrin: " << codePrinFileName << std::endl;
        codeFile.close();
        return;
    }

    // 1. 读取总比特数
    long long totalBits = 0;
    codeFile.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits)); //读取二进制文件总比特数

    char buffer; //作为字节缓冲区使用
    long long bitsRead = 0; //已读取比特数标记
    int lineCount = 0; //当前行已打印比特数标记

    // 2. 逐位还原并打印
    while (codeFile.get(buffer)) {
        for (int i = 0; i < 8; ++i) {
            if (bitsRead >= totalBits) break; // 读够了就停止, 忽略填充位

            int bit = (buffer >> (7 - i)) & 1; //提取当前位
            char charBit = (bit == 1 ? '1' : '0'); //转换为字符 '0' 或 '1'

            std::cout << charBit; //打印到控制台
            codePrinFile << charBit; //打印到文件

            bitsRead++;
            lineCount++;

            // 换行控制
            if (lineCount % perLine == 0) {
                std::cout << std::endl;
                codePrinFile << std::endl;
            }
        }
    }
    if (lineCount % perLine != 0) {
        std::cout << std::endl;
        codePrinFile << std::endl;
    }
    codeFile.close();
    codePrinFile.close();
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 打印哈夫曼树到文件
//函数操作对象: HuffmanTree类对象
//             写入文件filename, 默认TreePrin
//函数参数: const std::string& filename, 输出文件路径
//参数返回值: 无
void HuffmanTree::printTreeToFile(const std::string& filename) {
    std::ofstream outfile(filename);
    if(outfile.is_open()) {
        outfile << "========== Huffman Tree Structure (Indented) ==========" << std::endl;
        outfile << "Note: < > means internal node weight sum, [ ] means leaf node character" << std::endl;
        // 传入文件流 outfile
        printTreeStructure(root, 0, "Root", outfile); //传入文件流 outfile
        outfile << "===============================================" << std::endl;
        outfile.close();
    } else {
        std::cerr << "Cannot open file:"  << filename << " for writing." << std::endl;
    }
}
/*
void HuffmanTree::printTreeToFile(const std::string& filename) {
    std::ofstream outfile(filename);
    if(outfile.is_open()) {
        printTree(root, 0, outfile);
        outfile.close();
    }
}
*/
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 打印哈夫曼树到控制台
//函数操作对象: HuffmanTree类对象
//             读取TreePrin文件
//函数参数: 无
//参数返回值: 无
void HuffmanTree::printTreeToConsole() {
    if (root == nullptr) {
        std::cout << "Huffman tree is empty! Please initialize first." << std::endl;
        return;
    }
    //            ========== 哈夫曼树结构 (凹入表示法) ==========
    std::cout << "========== Huffman Tree Structure (Indented) ==========" << std::endl;
    std::cout << "Note: < > means internal node weight sum" << std::endl;
    std::cout << "[ ] means leaf node character" << std::endl;
    // 传入标准输出流 std::cout
    printTreeStructure(root, 0, "Root", std::cout);
    std::cout << "===============================================" << std::endl;
}
/*
void HuffmanTree::printTreeToConsole() {
    if (root == nullptr) {
        std::cout << "哈夫曼树为空！请先初始化!" << std::endl;
        return;
    }
    std::cout << "========== 哈夫曼树结构 (凹入表示法) ==========" << std::endl;
    std::cout << "说明: < >代表内部节点权值和, [ ]代表叶子节点字符" << std::endl;
    printTreeStructure(root, 0, "Root");
    std::cout << "===============================================" << std::endl;
}
*/
////////////////////////////////////////////////////////


//函数类型: 公有类外函数, 作为HuffmanTree类的辅助函数
//函数功能: 辅助递归保存节点
//函数操作对象: HuffmanTree类对象
//函数参数: ofstream& ofs, 输出文件流引用
//          HuffmanNode* node, 指向当前节点的指针
//参数返回值: 无
void saveNode(std::ofstream& ofs, HuffmanNode* node) {
    if (!node) {
        ofs << "# ";
        return;
    }
    // 保存当前节点信息
    if (node->ch == ' ') {
        ofs << "Space" << " " << node->weight << " "; //空格字符用SPACE表示
    } else if(node->ch == '\n'){
        ofs << "Enter" << " " << node->weight << " "; //换行字符用Enter表示
    } else if(node->ch == '\t') {
        ofs << "Hash" << " " << node->weight << " "; //#字符用Hash表示
    } else if(node->ch == '#') {
        ofs << "Tab" << " " << node->weight << " "; //制表符用Tab表示
    } else { //其他字符直接保存
        ofs << node->ch << " " << node->weight << " "; //保存字符和权值
    }
    saveNode(ofs, node->leftChild);
    saveNode(ofs, node->rightChild);
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 保存哈夫曼树到文件
//函数操作对象: HuffmanTree类对象
//函数参数: const std::string& filename, 输出文件路径
//参数返回值: bool, 保存是否成功
bool HuffmanTree::saveTree(const std::string& filename) {
    std::ofstream ofs(filename); //打开文件会自动覆盖原文件
    if (!ofs.is_open()) return false;
    saveNode(ofs, root);
    ofs.close();
    return true;
}
////////////////////////////////////////////////////////


//函数类型: 公有类外函数, 作为HuffmanTree类的辅助函数
//函数功能: 辅助递归加载节点
//函数操作对象: HuffmanTree类对象
//函数参数: istream& ifs, 输入文件流引用
//参数返回值: HuffmanNode*, 指向当前节点的指针
HuffmanNode* loadNode(std::istream& ifs) {
    std::string token;
    if (!(ifs >> token)) return nullptr;
    if (token == "#") return nullptr;
    char ch;
    if (token == "Space") { 
        ch = ' '; 
    } else if(token == "Enter") {
        ch = '\n'; 
    } else if(token == "Tab") {
        // 新增还原制表符
        ch = '\t';
    } else if(token == "Hash") {
        ch = '#'; 
    } else {
        ch = token[0]; 
    }
    int weight;
    ifs >> weight;
    HuffmanNode* node = new HuffmanNode(ch, weight);
    node->leftChild = loadNode(ifs);
    node->rightChild = loadNode(ifs);
    return node;
}
////////////////////////////////////////////////////////


//函数类型: 公有
//函数功能: 加载哈夫曼树从文件
//函数操作对象: HuffmanTree类对象
//函数参数: const std::string& filename, 输入文件路径
//参数返回值: bool, 加载是否成功
bool HuffmanTree::loadTree(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return false;
    destroy(root); // 释放原树
    root = loadNode(ifs);
    codeMapHead = nullptr;
    generateCodes(root, ""); // 重新生成编码表
    ifs.close();
    return true;
}
////////////////////////////////////////////////////////