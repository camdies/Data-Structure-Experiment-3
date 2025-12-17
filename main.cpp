//********** coding: GBK **********/
//程序名: main.cpp
/*
* 版权信息: 该文件为数据结构实验作业，仅供完成数据结构实验3作业以及学习交流使用
* 文件名: main.cpp
* 摘要: 实现哈夫曼树类的功能调用入口, 包括初始化、编码、译码、打印等功能, 并提供菜单和用户交互界面
* @Version: 1.0
* @author: Cairenbin
* 完成日期: 2025.11.28
*/
#include "HuffmanTree.cpp"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <limits>
using namespace std;

/********************* 指定文件路径, 提前赋默认值***********************/
string g_SrcFile = "ToBeTran";      // 待编码源文件
string g_CodeFile = "CodeFile";     // 编码结果文件 (二进制)
string g_DestFile = "TextFile";     // 译码结果文件 (还原文本)
string g_CodePrin = "CodePrin";     // 编码打印文件 (可视化的01串)
string g_TreePrin = "TreePrin";     // 树结构打印文件


// 辅助工具：统计文件字符频率并生成初始化数据
void generateFrequencyForCode(const string& filename) {
    ifstream file(filename.c_str(), ios::in); // 文本模式读取
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    // [修改点1]使用 string 作为键，以支持多字节字符(中文)
    map<char, int> freqMap;
    char ch;
    while (file >> noskipws >> ch) {
        // Ignore \r on Windows
        if (ch == '\r') continue;

        // Ignore Chinese characters (GBK: first byte > 127)
        if ((unsigned char)ch > 127) {
            char nextCh;
            file >> noskipws >> nextCh; // Skip the second byte
            continue;
        }

        freqMap[ch]++;
    }
    file.close();

    cout << "========== Summary ==========" << endl;
    cout << freqMap.size() << endl;

    for (map<char, int>::iterator it = freqMap.begin(); it != freqMap.end(); ++it) {
        char key = it->first;
        int val = it->second;

        if (key == ' ') cout << "Space " << val << " ";
        else if (key == '\n') cout << "Enter " << val << " ";
        else if (key == '\t') cout << "Tab " << val << " ";
        else if (key == '#') cout << "Hash " << val << " ";
        else cout << key << " " << val << " ";
    }
    cout << endl << "=======================================================" << endl;
}
/*
void generateFrequencyForCode(const string& filename) {
    ifstream file(filename.c_str(), ios::in); // 文本模式读取
    if (!file.is_open()) {
        cout << "无法打开文件: " << filename << endl;
        return;
    }

    map<char, int> freqMap;
    char ch;
    // 使用 noskipws 读取所有字符，包括空格、换行、Tab
    while (file >> noskipws >> ch) {
        // Windows下忽略 \r
        if (ch == '\r') continue;

        // [新增逻辑]忽略中文字符 (GBK编码)
        // 将 char 强转为 unsigned char，如果大于 127 (0x7F)，说明是多字节字符的首字节
        if ((unsigned char)ch > 127) {
            // 读取下一个字节并丢弃 (GBK中文通常是双字节，防止第二个字节被误读为ASCII)
            char nextCh;
            file >> noskipws >> nextCh; 
            continue; 
        }

        freqMap[ch]++;
    }
    file.close();

    cout << "========== 统计结果 (请复制以下内容进行初始化) ==========" << endl;
    cout << freqMap.size() << endl; // 先输出字符总数 n

    // 使用迭代器遍历 map
    for (map<char, int>::iterator it = freqMap.begin(); it != freqMap.end(); ++it) {
        char key = it->first;
        int val = it->second;

        if (key == ' ') cout << "Space " << val << " ";
        else if (key == '\n') cout << "Enter " << val << " ";
        else if (key == '\t') cout << "Tab " << val << " ";
        else if (key == '#') cout << "Hash " << val << " ";
        else cout << key << " " << val << " ";
    }
    cout << endl << "=======================================================" << endl;
}
*/


// 辅助函数：获取用户输入文件名，支持回车保留原值
void inputFileName(string& configVar, const string& prompt) {
    cout << prompt << " [Now: " << configVar << "]: ";
    
    // 检查缓冲区是否有残留换行符，如果有则忽略，但不能盲目忽略导致阻塞
    // 这里采用 getline 读取整行的方式更安全
    string input;
    getline(cin, input);

    // 如果输入为空（用户直接回车），则不修改 configVar
    if (!input.empty()) {
        // 查找第一个非空白字符
        size_t first = input.find_first_not_of(" \t\r\n");
        if (first != string::npos) {
            // 查找最后一个非空白字符
            size_t last = input.find_last_not_of(" \t\r\n");
            configVar = input.substr(first, (last - first + 1));
        }
    }
}


//函数类型: 公有类外函数, 作为HuffmanTree类的辅助函数
//函数功能: 显示菜单
//函数操作对象: 无
//函数参数: 无
//参数返回值: 无
void showMenu() {
    cout << " Huffman Tree Coding System" << endl;
    //cout << " 哈夫曼树编码系统" << endl;
    cout << "1. Initialize Huffman tree (input characters and weights, save to hfmTree) " << endl;
    //cout << "1. 初始化哈夫曼树 (输入字符和权值, 保存到 hfmTree) " << endl;
    cout << "2. Encode (ToBeTran to CodeFile) " << endl;
    //cout << "2. 编码 (ToBeTran 到 CodeFile) " << endl;
    cout << "3. Decode (CodeFile to TextFile) " << endl;
    //cout << "3. 译码 (CodeFile 到 TextFile) " << endl;
    cout << "4. Print code file (CodeFile to screen, 50 per line, also write to CodePrin) " << endl;
    //cout << "4. 打印代码文件 (CodeFile 到屏幕, 每行50个, 同时写入 CodePrin) " << endl;
    cout << "5. Print Huffman tree (screen + TreePrin) " << endl;
    //cout << "5. 打印哈夫曼树 (屏幕 + TreePrin) " << endl;
    cout << "6. [Tool] Count file frequency (generate initialization data)" << endl;
    //cout << "6. [工具] 统计文件频率 (生成初始化数据)" << endl;
    cout << "7. [Settings] Configure global file paths (source/target files)" << endl;
    //cout << "7. [设置] 配置全局文件路径 (源文件/目标文件)" << endl;
    cout << "-1 Exit" << endl;
    //cout << "-1 退出" << endl;
    cout << "Please select function number: ";
}
////////////////////////////////////////////////////////


//函数类型: 主函数
//函数功能: 实现哈夫曼树类的功能调用入口, 包括初始化
//函数操作对象: 无
//函数参数: 无
//参数返回值: int, 程序退出状态码
int main() { 
    HuffmanTree ht; //创建哈夫曼树对象
    int choice; //用户选择变量
    while (true) {
        showMenu();
        //增加输入检查，防止死循环
        if (!(cin >> choice)) {
            cout << "Invalid input, please enter a number!" << endl;
            cin.clear(); // 清除错误标志
            cin.ignore(1024, '\n'); // 丢弃缓冲区中的剩余字符
            continue; // 重新显示菜单
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
        //处理用户选择

        case -1:
        //选项功能: 退出程序
        //涉及文件操作: 无
            cout << "Program ended." << endl;
            return 0;
        /////////////////////////////////////////////////////

        case 1: {
        //选项功能: 初始化哈夫曼树, 接受用户输入字符集大小, 而后接受字符和权值, 构建哈夫曼树
        //涉及文件操作: 写入文件hfmTree
            int n; //存储字符集的大小
            cout << "Please enter the character set size n: ";
            // 检查n的输入是否合法
            if (!(cin >> n)) {
                cout << "Input error: please enter an integer." << endl;
                cin.clear(); // 清除错误标志
                cin.ignore(1024, '\n'); // 丢弃缓冲区内容
                break;
            }
            cout << "Please enter n characters and weights in order (e.g. a 5 b 7 ...):" << endl;
            if (ht.initalize(cin, n)) {
                cout << "Huffman tree has been built and saved to hfmTree." << endl;
            } else {
                cout << "Initialization failed! Please check if the input data is complete and formatted correctly." << endl;
                // 初始化失败后，输入流可能处于错误状态，需要重置以便下一次输入
                cin.clear(); // 清除错误标志
                cin.ignore(1024, '\n'); // 丢弃缓冲区内容
            }
            break;
        }
        /////////////////////////////////////////////////////

        case 2: {
        //选项功能: 编码文件ToBeTran到CodeFile
        //涉及文件操作: 读取ToBeTran, 写入CodeFile
            cout << "Encoding " << g_SrcFile << " ..." << endl;
            if (ht.encodeFile(g_SrcFile, g_CodeFile))
                cout << "Encoding complete, result written to " << g_CodeFile << endl;
            break;
        }
        /////////////////////////////////////////////////////

        case 3: {
        //选项功能: 译码文件CodeFile到TextFile
        //涉及文件操作: 读取CodeFile, 写入TextFile
            cout << "Decoding " << g_CodeFile << " ..." << endl;
            if (ht.decodeFile(g_CodeFile, g_DestFile))
                cout << "Decoding complete, result written to " << g_DestFile << endl;
            break;
        }
        /////////////////////////////////////////////////////

        case 4: {
        //选项功能: 打印代码文件CodeFile到屏幕, 每行50个, 同时写入CodePrin
        //涉及文件操作: 读取CodeFile, 写入CodePrin
            cout << "Printing " << g_CodeFile << " to " << g_CodePrin << "..." << endl;
            ht.printCodeFileAndWriteCodePrin(g_CodeFile, g_CodePrin, 50);
            cout << "Print complete." << endl;
            break;
        }
        /////////////////////////////////////////////////////

        case 5: {
        //选项功能: 打印哈夫曼树到屏幕和TreePrin文件
        //涉及文件操作: 写入TreePrin
            cout << "Printing Huffman tree to " << g_TreePrin << "..." << endl;
            cout << "Huffman tree structure:" << endl;
            ht.printTreeToConsole();
            ht.printTreeToFile(g_TreePrin);
            cout << "Huffman tree has been written to " << g_TreePrin << endl;
            break;
        }
        /////////////////////////////////////////////////////

        case 6: {
            string filename;
            cout << "Please enter the source code file name to count [default: " << g_SrcFile << "]: ";
            char peek = cin.peek();
            if (peek == '\n') {
                filename = g_SrcFile;
                cin.ignore();
            } else {
                cin >> filename;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            generateFrequencyForCode(filename);
            break;
        }
        /////////////////////////////////////////////////////

        case 7: {
            cout << "\n--- Configure global file paths (press Enter to keep current value) ---" << endl;
            inputFileName(g_SrcFile, "1. Source file to be encoded (Source)");
            inputFileName(g_CodeFile, "2. Encoded result file (Code)");
            inputFileName(g_DestFile, "3. Decoded result file (Dest)");
            inputFileName(g_CodePrin, "4. Encoded print file (Print)");
            inputFileName(g_TreePrin, "5. Tree structure print file (Tree)");
            cout << "--- Configuration updated ---\n" << endl;
            break;
        }
        /////////////////////////////////////////////////////

        default:
        //选项功能: 无效输入处理
        //涉及文件操作: 无
            cout << "Invalid selection, please re-enter!" << endl;
            break;
        }
        cout << endl;
    }
    return 0;
}
//D:\C++\Grade_2.1\Data_Structure\Experiment\3\