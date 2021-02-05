#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include "String.h"
#include <fstream>

using namespace std;


// Количество строк на данный момент
int String::stringsNum = 0;

// Конструктор по умолчанию
String::String()
{
    stringsNum++;
    bufSize = DEFAULT_BUF_SIZE;
    length = 0;
    string = new char[bufSize];
    for (int i = 0; i < bufSize; i++)
        string[i] = '\0';
}

// Конструктор копирования
String::String(const String& str)
{
    stringsNum++;
    bufSize = str.bufSize;
    length = str.length;
    string = new char[bufSize];
    for (int i = 0; i < bufSize; i++)
        string[i] = str.string[i];
}

// Вставляет src в dst, начиная с index.
// Предполагается, что в dst выделено достаточно места
void String::insertTo(char* dst, int dstLen,
    const char* src, int srcLen, int index)
{
    for (int i = dstLen + srcLen - 1; i > index + srcLen - 1; i--)
        dst[i] = dst[i - srcLen];
    for (int i = index, j = 0; j < srcLen; i++, j++)
        dst[i] = src[j];
}

// Конструктор. bufSize - изначальный размер буфера
String::String(int bufSize)
{
    stringsNum++;
    this->bufSize = bufSize;
    length = 0;
    string = new char[bufSize];
    for (int i = 0; i < bufSize; i++)
        string[i] = '\0';
}

// Конструктор, создающий объект на основании const char*
String::String(const char* str)
{
    stringsNum++;
    bufSize = 0;
    length = 0;
    int charsNum = 0;
    for (int i = 0; str[i] != '\0'; i++)
        charsNum++;
    bufSize = countBufSize(charsNum);
    length = charsNum;
    string = new char[bufSize];
    for (int i = 0; i < charsNum; i++)
        string[i] = str[i];
    for (int i = charsNum; i < bufSize; i++)
        string[i] = '\0';
}

// Если передаваемая подстрока входит в строку, 
// то возвращает индекс перевого вхождения. 
// Иначе возвращает -1
int String::indexOf(const char* str)
{
    int index = -1;
    int j = -1;
    bool found = false;
    for (int i = 0; i < length && !found; i++)
    {
        if (index == -1)
        {
            if (string[i] == str[0])
            {
                index = i;
                j = 1;
            }
        }
        else
        {
            if (str[j] == '\0')
                found = true;
            else if (str[j] == string[i])
                j++;
            else
                index = -1;
        }
    }

    return index;
}

// Считает, сколько места нужно выделить для создания 
// строки указанной длины
int String::countBufSize(int charsToAddCount)
{
    int delta = charsToAddCount - bufSize + length;
    int addToBuf = ((int)(delta / DEFAULT_BUF_SIZE) + 1) * DEFAULT_BUF_SIZE;
    int newBufSize = bufSize + addToBuf;
    return newBufSize;
}

// Добавляет к строке подстроку
void String::add(const char* str)
{
    // Находим индекс, с которого начинаем добавление
    int startIndex = bufSize - 1;
    while (string[startIndex] != '\0')
        startIndex--;
    // Узнаём, сколько символов нужно добавить
    int charsToAddCount = 0;
    for (int i = 0; str[i] != '\0'; i++)
        charsToAddCount++;
    if ((length + charsToAddCount) >= bufSize)
    {
        // Считаем, сколько нужно места для новой строки и выделяем память
        int newBufSize = countBufSize(charsToAddCount);
        // Выделяем место под новую строку
        char* newString = new char[newBufSize];
        // Копируем старую строку в новый массив
        for (int i = 0; i < length; i++)
            newString[i] = string[i];
        // Добавляем новую строку
        for (int i = length, j = 0; j < charsToAddCount; i++, j++)
            newString[i] = str[j];
        // Заполняем остаток строки '\0'
        for (int i = length + charsToAddCount; i < newBufSize; i++)
            newString[i] = '\0';
        // Обновляем значения полей данных
        delete string;
        string = newString;
        length += charsToAddCount;
        bufSize = newBufSize;
    }
    else
    {
        // Копируем подстроку в нашу строку
        for (int i = length, j = 0; str[j] != '\0'; i++, j++)
            string[i] = str[j];
        length += charsToAddCount;
    }
}

// Заменяет символ с заданным индексом новым символом.
// Возвращает false, если символ таким индексом отсутствует
bool String::replace(int index, char c)
{
    if (index < bufSize)
    {
        if (string[index] == '\0')
            return false;
        else
        {
            string[index] = c;
            return true;
        }
    }
    else
        return false;
}

// Вставляет подстроку по указанному индексу.
// Возвращает false, если индекс отсутствует в текущей строке
bool String::insert(int index, const char* str)
{
    if (index > length)
        return false;
    // Узнаём, сколько символов нужно добавить
    int charsToAddCount = 0;
    for (int i = 0; str[i] != '\0'; i++)
        charsToAddCount++;

    if ((length + charsToAddCount) >= bufSize)
    {
        // Считаем, сколько нужно места для новой строки и выделяем память
        int newBufSize = countBufSize(charsToAddCount);
        // Выделяем место под новую строку
        char* newString = new char[newBufSize];
        // Копируем старую строку в новый массив
        for (int i = 0; i < length; i++)
            newString[i] = string[i];
        // Вставляем новую строку
        insertTo(newString, length, str, charsToAddCount, index);
        // Заполняем остаток строки '\0'
        for (int i = length + charsToAddCount; i < newBufSize; i++)
            newString[i] = '\0';
        // Обновляем значения полей данных
        delete string;
        string = newString;
        bufSize = newBufSize;
    }
    else
        // Вставляем подстроку в нашу строку
        insertTo(string, length, str, charsToAddCount, index);
    length += charsToAddCount;
    return true;
}

// Выводит строку на экран
void String::print()
{
    std::cout << string;
}

// Возвращает текущую длину строки
int String::getLength()
{
    return length;
}

// Возвращает количество существующих 
// на данный момент строк в программе
int String::getStringsNum()
{
    return stringsNum;
}

// Оператор присваивания
String& String::operator=(const String& str)
{
    if (str == *this)
        return *this;
    this->bufSize = str.bufSize;
    this->length = str.length;
    if (this->string != nullptr)
        delete this->string;
    this->string = new char[bufSize];
    for (int i = 0; i < bufSize; i++)
        this->string[i] = str.string[i];
    return *this;
}

// Деструктор
String::~String()
{
    delete string;
    stringsNum--;
}

// Оператор String + String
String operator+(const String& str1, const String& str2)
{
    String str(str1);
    str.add(str2.string);
    return str;
}

// Оператор const char* + String
const char* operator+(const char* str1, const String& str2)
{
    int str1Length = 0;
    for (int i = 0; str1[i] != '\0'; i++)
        str1Length++;
    char* str = new char[str1Length + str2.length + 1];
    for (int i = 0; i < str1Length; i++)
        str[i] = str1[i];
    for (int i = str1Length, j = 0; j < str2.length; i++, j++)
        str[i] = str2.string[j];
    str[str1Length + str2.length] = '\0';
    const char* str3 = str;
    return str3;
}

// Оператор String + const char*
String operator+(const String& str1, const char* str2)
{
    String str(str1);
    str.add(str2);
    return str;
}

// Оператор String > String
bool operator>(const String& str1, const String& str2)
{
    if (str1.length > str2.length)
        return true;
    else if (str1.length < str2.length)
        return false;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] > str2.string[i])
                return true;
            else if (str1.string[i] < str2.string[i])
                return false;
        }
    }
    return false;
}

// Оператор String < String
bool operator<(const String& str1, const String& str2)
{
    if (str1.length < str2.length)
        return true;
    else if (str1.length > str2.length)
        return false;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] < str2.string[i])
                return true;
            else if (str1.string[i] > str2.string[i])
                return false;
        }
    }
    return false;
}

// Оператор String >= String
bool operator>=(const String& str1, const String& str2)
{
    if (str1.length > str2.length)
        return true;
    else if (str1.length < str2.length)
        return false;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] < str2.string[i])
                return false;
            else if (str1.string[i] > str2.string[i])
                return true;
        }
    }
    return true;
}

// Оператор String <= String
bool operator<=(const String& str1, const String& str2)
{
    if (str1.length < str2.length)
        return true;
    else if (str1.length > str2.length)
        return false;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] > str2.string[i])
                return false;
            else if (str1.string[i] < str2.string[i])
                return true;
        }
    }
    return true;
}

// Оператор String == String
bool operator==(const String& str1, const String& str2)
{
    if (str1.length != str2.length)
        return false;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] != str2.string[i])
                return false;
        }
    }
    return true;
}

// Оператор String != String
bool operator!=(const String& str1, const String& str2)
{
    if (str1.length != str2.length)
        return true;
    else
    {
        for (int i = 0; i < str1.length; i++)
        {
            if (str1.string[i] != str2.string[i])
                return true;
        }
    }
    return false;
}

const char* String::getpp() {

    try {

        if (string == nullptr || !string) throw "Empty string";

        return string;
    }
    catch (const char* exception) {

        const char* s = " ";

        std::cerr << "Error: " << exception << std::endl << "_______________" << std::endl;
        return s;
    }
}


std::ostream& operator<< (std::ostream& out, const String& pp) {

    out << pp.string << endl;
    return out;
}


std::istream& operator>> (std::istream& is, String& pp) {

    is >> pp.string;
    return is;
}

bool String::write_binary(fstream& filename) {

        if (!filename.is_open()) {

            cout << "error of open" << endl;
            return 0;
        }
        else {

            cout << "file is open" << endl;

            filename << string;
            return 1;
        }
        filename.close();
   
}


bool String::read_binary(fstream& filename) {

        if (!filename.is_open()) {

            cout << "error of open" << endl;
            return 0;
        }
        else {

            cout << "file is open" << endl;

            int len = strlen(string);

            filename.seekg(-len - 2, ios::end);
            filename >> string;
            cout << string;
            return 1;
        }
        filename.close();
    
}

// Тестирование конструктора по умолчанию String()
String testDefaultConstructor()
{
    String str;
    cout << "\nСтрока создана!\n";
    cout << "Строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    return str;
}

// Тестирование String(const String& str)
String testCopyConstructor()
{
    cout << "\nВведите строку. На основании её будет "
        << "создана новая строка, а первая строка будет удалена.\n";
    string temp;
    getline(cin, temp);
    String* str1 = new String(temp.c_str());
    cout << "\nПервая строка создана! Количество строк в данный момент: "
        << String::getStringsNum();
    String str2(*str1);
    cout << "\nВызван конструктор копирования, вторая строка создана! "
        << "Количество строк в данный момент: " << String::getStringsNum();
    delete str1;
    cout << "\nПервая строка удалена! Количество строк в данный момент: "
        << String::getStringsNum();
    cout << "\nВведённая строка: ";
    str2.print();
    cout << "\nДлина строки: " << str2.getLength();
    return str2;
}

// Тестирование String(int bufSize)
String testBufSizeConstructor()
{
    cout << "\nВведите изначальный размер буфера, "
        << "с которым будет создана строка: ";
    string bufSizeStr = "";
    int bufSize = 0;
    bool entered = false;
    bool wasEnter = false;
    do
    {
        if (!wasEnter)
            wasEnter = true;
        else
            cout << "Введите, пожалуйста, целое число больше нуля: ";
        getline(cin, bufSizeStr);
        entered = true;
        for (unsigned int i = 0; i < bufSizeStr.length() && entered; i++)
        {
            if (bufSizeStr[i] < '0' || bufSizeStr[i] > '9')
                entered = false;
        }
        if (entered)
        {
            bufSize = stoi(bufSizeStr);
            if (bufSize <= 0)
                entered = false;
        }
    } while (!entered);
    String str(bufSize);
    cout << "\nСтрока создана!\n";
    cout << "Строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    return str;
}

// Тестирование String(const char* str)
String testCharPtrConstructor()
{
    cout << "\nВведите строку. На основании её будет создан объект String.\n";
    string temp;
    getline(cin, temp);
    String str(temp.c_str());
    cout << "\nСтрока создана!\n";
    cout << "Строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    return str;
}

// Выводит пользователю сообщение о вводе 
// любой клавиши и дожидается ввода
void pressAnyKey()
{
    cout << "\nНажмите любую клавишу для перехода "
        << "к следующему пункту тестирования...";
    _getch();
}

// Меню выбора тестирования конструктора
String testConstructor()
{
    cout << "1. Тестирование конструктора\n\n";
    cout << "Какой конструктор Вы хотите использовать?\n";
    cout << "1 — Конструктор по умолчанию\n";
    cout << "2 — Конструктор копирования\n";
    cout << "3 — Конструктор с заданием изначального размера буфера\n";
    cout << "4 — Конструктор, создающий объект из const char*\n";
    string option = "";
    bool optionEntered = false;
    do
    {
        if (!optionEntered)
            optionEntered = true;
        else
            cout << "Введите, пожалуйста, цифру от 1 до 4: ";
        getline(cin, option);
    } while (option != "1" && option != "2"
        && option != "3" && option != "4");
    switch (stoi(option))
    {
    case 1:
        return testDefaultConstructor();
    case 2:
        return testCopyConstructor();
    case 3:
        return testBufSizeConstructor();
    case 4:
        return testCharPtrConstructor();
    }
}

bool test_original_file(String& a) {

    const char* s = "test_Test-tEsT";
    bool rez = NULL;

    try {

        if (a.getpp() == nullptr && !a.getpp()) { throw "Test hasn't been complited "; }
        else {

            fstream fs;

            fs.open("txt.txt", fstream::in | fstream::out | fstream::app);

            if (!fs.is_open()) {

                cout << "error of open" << endl;
            }
            else {

                cout << "file is open" << endl;

                fs << a;
            }
            fs.close();


            fs.open("txt.txt", fstream::in | fstream::out | fstream::app);

            if (!fs.is_open()) {

                cout << "error of open" << endl;
            }
            else {

                cout << "file is open" << endl;

                int len = strlen(a.getpp());

                fs.seekg(-len - 2, ios::end);
                fs >> a;
                cout << a;
            }
            fs.close();

            if (a == s) {

                rez = true;
                cout << "Result test: " << "test original is OK" << endl << endl << endl << endl << "***************" << endl;
            }
            else {

                rez = false;
                cout << "Result test: " << "test original is BAD" << endl << endl << endl << endl << "***************" << endl;
            }
        }

        return (rez) ? true : false;
    }
    catch (const char* exception) {

        std::cerr << "Error: " << exception << std::endl << "_______________" << std::endl;
        return 0;
    }
}


bool test_binary_file(String& a, fstream& filename) {

    const char* s = "test_Test-tEsT";
    bool rez;

    try {

        if (a.getpp() == nullptr && !a.getpp()) { throw "Test hasn't been complited "; }
        else {

            if (a == s && a.read_binary(filename) && a.write_binary(filename)) {

                rez = true;
                cout << "Result test: " << "test binary is OK" << endl << endl << endl << endl << "***************" << endl;
            }
            else {

                rez = false;
                cout << "Result test: " << "test binary is BAD" << endl << endl << endl << endl << "***************" << endl;
            }
        }

        return (rez) ? true : false;
    }
    catch (const char* exception) {

        std::cerr << "Error: " << exception << std::endl << "_______________" << std::endl;
        return 0;
    }
}

// Тестирование метода добавления подстроки в конец строки
void testAdd(String& str)
{
    system("cls");
    cout << "2. Тестирование метода добавления подстроки в конец строки."
        << "\n\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength()
        << "\nВведите любую подстроку. Она будет "
        << "добавлена в конец текщуей строки: ";
    string substr;
    getline(cin, substr);
    str.add(substr.c_str());
    cout << "\n\nПодстрока добавлена!"
        << "\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    pressAnyKey();
}

// Тестирование метода замены символа в строке
void testReplace(String& str)
{
    system("cls");
    cout << "3. Тестирование метода замены символа в строке."
        << "\n\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength()
        << "\nВведите индекс символа, который должен быть заменён: ";
    int index = -1;
    string indexStr = "";
    bool entered = false;
    bool wasEnter = false;
    do
    {
        if (!wasEnter)
            wasEnter = true;
        else
        {
            cout << "\nВведите целое число от 0 до "
                << str.getLength() - 1 << ": ";
        }
        getline(cin, indexStr);
        entered = true;
        for (unsigned int i = 0; i < indexStr.length() && entered; i++)
        {
            if (indexStr[i] < '0' || indexStr[i] > '9')
                entered = false;
        }
        if (entered)
        {
            index = stoi(indexStr);
            if (index < 0 && index >= str.getLength())
                entered = false;
        }
    } while (!entered);
    cout << "Введите символ, который Вы "
        << "хотите установить по данному индексу: ";
    string temp = "";
    getline(cin, temp);
    char c;
    if (temp == "")
        c = ' ';
    else
        c = temp[0];
    str.replace(index, c);
    cout << "\n\nСимвол изменён!"
        << "\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    pressAnyKey();
}

// Тестирование метода вставки подстроки в строку
void testInsert(String& str)
{
    system("cls");
    cout << "4. Тестирование метода вставки подстроки в строку."
        << "\n\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength()
        << "\nВведите индекс начала вставки: ";
    int index = -1;
    string indexStr = "";
    bool entered = false;
    bool wasEnter = false;
    do
    {
        if (!wasEnter)
            wasEnter = true;
        else
        {
            cout << "\nВведите целое число от 0 до "
                << str.getLength() - 1 << ": ";
        }
        getline(cin, indexStr);
        entered = true;
        for (unsigned int i = 0; i < indexStr.length() && entered; i++)
        {
            if (indexStr[i] < '0' || indexStr[i] > '9')
                entered = false;
        }
        if (entered)
        {
            index = stoi(indexStr);
            if (index < 0 && index >= str.getLength())
                entered = false;
        }
    } while (!entered);
    cout << "Введите подстроку, которую "
        << "Вы хотите вставить по заданному индексу: ";
    string substr;
    getline(cin, substr);
    str.insert(index, substr.c_str());
    cout << "\n\nПодстрока вставлена!"
        << "\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    pressAnyKey();
}

// Тестирование метода поиска подстроки в строке
void testIndexOf(String& str)
{
    system("cls");
    cout << "5. Тестирование метода поиска подстроки в строке."
        << "\n\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength()
        << "\nВведите любую подстроку. "
        << "Если она присутсвтует в текущей строке, то "
        << "Вы узнаете индекс начала первого вхождения.\n";
    string substr;
    getline(cin, substr);
    int index = str.indexOf(substr.c_str());
    if (index != -1)
        cout << "\nИндекс первого вхождения подстроки в строку: " << index << ".\n";
    else
        cout << "\nПодстрока не найдена.\n";
    pressAnyKey();
}

// Тестирование операторов присваивания и сложения
void testAssignAdd(String& str)
{
    system("cls");
    cout << "6. Тестирование операторов \"+\" и \"=\"."
        << "\n\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength()
        << "\nВведите ещё одну строку. Она будет объединена с текущей "
        << "при помощи оператора \"+\" и присвоена ей "
        << "при помощи оператора \"=\".\n";
    string str2;
    getline(cin, str2);
    str = str + String(str2.c_str());
    cout << "Строки успешно объединены! \nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    cout << "\nТеперь введите ещё одну строку. "
        << "Она будет сохранена в char*, к которому будет присоединена "
        << "текущая строка. Результат Вы увидите на экране.\n";
    string str3StdString;
    getline(cin, str3StdString);
    const char* str3 = str3StdString.c_str();
    str3 = str3 + str;
    cout << "\nСтроки успешно объединены! "
        << "Получившаяся строка (хранится в char*):\n" << str3
        << "\nВведите ещё одну строку. На основании неё будет создан "
        << "новый объект String. К нему будет присоединена последняя "
        << "сформированная строка. Результат будет присвоен основной "
        << "строке при помощи оператора присваивания.\n";
    string str4StdString;
    getline(cin, str4StdString);
    String str4(str4StdString.c_str());
    str = str4 + str3;
    cout << "\nОбъединение строк произведено успешно!"
        << "\nТекущая строка: ";
    str.print();
    cout << "\nДлина строки: " << str.getLength();
    pressAnyKey();
}

// Тестирование операторов сравнения
void testComparison()
{
    system("cls");
    cout << "7. Тестирование операторов сравнения.";
    cout << "\n\nВведите строку: \n";
    string str1StdString;
    getline(cin, str1StdString);
    String str1(str1StdString.c_str());
    cout << "\nНовый объект String создан!"
        << "\nДлина Строки1: " << str1.getLength()
        << "\n\nВведите вторую строку. Она будет сравнена с первой "
        << "всеми перегруженными операторами сравнения.\n";
    string str2StdString;
    getline(cin, str2StdString);
    String str2(str2StdString.c_str());
    cout << "\nНовый объект String создан!"
        << "\nДлина Строки2: " << str1.getLength();
    cout << boolalpha;
    cout << "\n\nСтрока1 > Строка2 : " << (str1 > str2)
        << "\nСтрока1 < Строка2 : " << (str1 < str2)
        << "\nСтрока1 == Строка2 : " << (str1 == str2)
        << "\nСтрока1 != Строка2 : " << (str1 != str2)
        << "\nСтрока1 >= Строка2 : " << (str1 >= str2)
        << "\nСтрока1 <= Строка2 : " << (str1 <= str2);
}

int main()
{
    // Включение русского языка в консоли
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    /* String str = testConstructor();
    pressAnyKey();
   testAdd(str);
   testReplace(str);
   testInsert(str);
   testIndexOf(str);
   testAssignAdd(str);
   testComparison();
    cout << "\nТестирование завершено!\n";*/
    
        String a("test_Test-tEsT");

        fstream namefile("dat.dat", fstream::in | fstream::out | fstream::app | fstream::binary);

       test_original_file(a);

       test_binary_file(a, namefile);

    
}
