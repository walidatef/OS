#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<cmath>
#include<iomanip>
#include<algorithm>
#include<utility>
#include<queue>
#include<stack>
#include<map>
#include<unordered_map>
#include<set>
#include<bitset>
#include<windows.h>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<string>
#include<iterator>
#include <sys/stat.h>

using namespace std;

// function to make code fast.
void FastCode()
{
    std::ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
}

bool IsPathExist(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

//store the path of directory
string dir = "";

// convert from int -> bytes & bytes -> int
class Convert {

public:

    vector<char> FromintTobyte(vector<int>v) {
        vector<char>val;
        for (int i = 0; i < v.size(); i++) {
            int n = v[i];
            if (n == -1) {
                val.push_back(1 + '0');
                val.push_back(1 + '0');
                val.push_back(1 + '0');
                val.push_back(1 + '0');
            }
            else {
                char v1 = n % 10;
                n /= 10;
                char v2 = n % 10;
                n /= 10;
                char v3 = n % 10;
                n /= 10;
                char v4 = n % 10;
                val.push_back(v4 + '0');
                val.push_back(v3 + '0');
                val.push_back(v2 + '0');
                val.push_back(v1 + '0');
            }
        }
        return val;
    }
    // convert from bytes to int
    vector<int> convert_f_bytes_t_int(vector<char>& ve_bytes) {
        vector<int>ve_int;
        int value = 0;
        for (int i = 0; i < ve_bytes.size(); i++) {
            value = value * 10 + int(ve_bytes[i] - '0');
            if ((i + 1) % 4 == 0) {
                if (value == 1111) {
                    ve_int.push_back(-1);
                }
                else {
                    ve_int.push_back(value);
                }
                value = 0;
            }
        }
        return ve_int;
    }
};

class Virtual_disk
{
private:
    fstream file;
    char value = '0'; // = 1 byte
    char fatTableValue = '*';
    char dataFileeValue = '#';

public:
    Virtual_disk() {
        if (!is_file_exist("OS.txt")) {

            file.open("OS.txt", ios::trunc | ios::out | ios::in);
            super_block();
            meta_data();
            data_file();

            file.close();
        }
    }

    bool is_file_exist(const char* fileName)
    {
        std::ifstream infile(fileName);
        return infile.good();
    }


    void super_block() { // size 1024 byte , first block
        for (int i = 0; i < 1024; i++) {
            file << value;
        }
    }

    // Fat table
    // to store  1024 number (int) , size = 1024 * 4
    // we need 4 blocks ->  number 2 ,3 ,4 and 5 in the file
    // every block = 256 int
    void meta_data() {
        for (int x = 1; x <= 4; x++) {
            for (int y = 0; y < 1024; y++) {
                file << fatTableValue;
            }
        }
    }

    //to store 1019 block , size = 1019 * 1024  byte
    void data_file() {
        for (int y = 0; y < 1019; y++) {
            for (int x = 0; x < 1024; x++) {
                file << dataFileeValue;
            }
        }
    }


    // T A S K 3
    void write_block(vector<char> bytes, int index) {
        if (is_file_exist("OS.txt")) {
            file.open("OS.txt");
            file.seekg(1024 * index);
            //write 1024 byte only
            for (int x = 0; x < bytes.size(); x++) {
                file << bytes[x];
            }
            file.close();
        }
        else {     // file couldn't be opened
            cout << "File not existed" << endl;
        }
    }

    vector<char> get_block(int index) {

        vector<char> table(1024);
        char* arr = table.data();
        if (is_file_exist("OS.txt")) {
            file.open("OS.txt");
            // To move on begin fat table
            file.seekg(1024 * index);
            file.read(arr, 1024);
            file.close();
        }
        else {     // file couldn't be opened
            cout << "File not existed" << endl;
        }
        return table;
    }
};

class Fat_table
{
private:
    fstream file;
    static const  int SIZE_FAT_BYTE = 4 * 1024;
    static const  int SIZE_FAT_INT = 1024;
    vector<int> fat_table;

public:
    Fat_table() {
        if (is_file_exist("OS.txt")) {
            fat_table = get_fatTable();
        }
        else {
            fat_table.resize(SIZE_FAT_INT, 0);
            initialize_fatTable();
        }
    }

    bool is_file_exist(const char* fileName)
    {
        std::ifstream infile(fileName);
        return infile.good();
    }

    //Fat table methods
    void initialize_fatTable() {
        // can't write in it index 0,1,2,3,4
        fat_table[0] = -1;
        fat_table[1] = -1;
        fat_table[2] = -1;
        fat_table[3] = -1;
        fat_table[4] = -1;

        //for Test
        /*for (int z = 5; z < 1024; z++) {
            fat_table[z] = z;
        }*/
    }
    vector<char> convert_f_int_t_byte(vector<int> fat_table) {
        vector<char>ve_bytes;
        for (int i = 0; i < fat_table.size(); i++) {
            if (fat_table[i] == -1) {
                for (int j = 0; j < 4; j++) {
                    ve_bytes.push_back('1');
                }
            }
            else {
                string s = to_string(fat_table[i]);
                if (s.size() == 4) {
                    for (int j = 0; j < 4; j++) {
                        ve_bytes.push_back(s[j]);
                    }
                }
                else {
                    for (int j = 0; j < 4 - s.size(); j++) {
                        ve_bytes.push_back('0');
                    }
                    for (int j = 0; j < s.size(); j++) {
                        ve_bytes.push_back(s[j]);
                    }
                }
            }
        }
        return ve_bytes;
    }
    void write_fatTable() {
        // Read from the text file
        file.open("OS.txt");
        // To move on begin fat table
        file.seekg(SIZE_FAT_INT);

        // C O N V E R T  Int (fat_table)  to  B y t e
        Convert convert;
        vector<char> bytes = convert.FromintTobyte(fat_table);

        for (auto& e : bytes) {
            file << e;
        }
        file.close();
    }

    // Read from the text file
    vector<int> get_fatTable() {
        vector<char> table(SIZE_FAT_BYTE);
        char* arr = table.data();

        Convert convert;
        if (is_file_exist("OS.txt")) {

            file.open("OS.txt");
            // To move on begin fat table
            file.seekg(SIZE_FAT_INT);
            file.read(arr, SIZE_FAT_BYTE);
            file.close();
        }
        else {     // file couldn't be opened
            cout << "File not existed" << endl;
        }
        //CONVERT  B y t e (table)  to  int
        return convert.convert_f_bytes_t_int(table);

    }

    //for test print fat table

    void print_fatTable() {

        for (auto& i : fat_table) {
            cout << i;
        }
    }

    //print free space - available blocks in fat table
    void available_blocks() {
        cout << endl;
        int free_space = 0;
        for (int y = 0; y < SIZE_FAT_INT; y++) {
            if (fat_table[y] == 0) {
                free_space++;
                cout << "Index: " << y << endl;
            }
        }
        cout << endl << "Free space = " << SIZE_FAT_INT * free_space << " byte" << endl;
    }

    //return numbers of available blocks in fat table
    int get_available_blocks() {
        int free_space = 0;
        for (int y = 0; y < SIZE_FAT_INT; y++) {

            if (fat_table[y] == 0) {
                free_space++;
            }

        }
        return free_space;
    }

    // T A S K 3

    //return first free block
    int get_Available_Block() {
        for (int y = 0; y < SIZE_FAT_INT; y++) {
            if (fat_table[y] == 0) {
                return y;
                break;
            }
        }
    }

    int get_next(int index) {
        return fat_table[index];
    }

    void set_next(int index, int value) {

        fat_table[index] = value;
        write_fatTable();
    }
};

class Directory_Entry {
public:
    char FileName[11], FileEmpty[12], FileAttribute;
    int  FileSize, Firstcluster;

    vector<char> get_bytes(Directory_Entry d) {
        vector<char>v;
        for (int i = 0; i < 11; i++) {
            v.push_back(d.FileName[i]);
        }
        for (int i = 0; i < 12; i++) {
            v.push_back(d.FileEmpty[i]);
        }
        v.push_back(d.FileAttribute);
        int n = d.FileSize;
        char v1 = n % 10;
        n /= 10;
        char v2 = n % 10;
        n /= 10;
        char v3 = n % 10;
        n /= 10;
        char v4 = n % 10;
        v.push_back(v4 + '0');
        v.push_back(v3 + '0');
        v.push_back(v2 + '0');
        v.push_back(v1 + '0');
        n = d.Firstcluster;
        v1 = n % 10;
        n /= 10;
        v2 = n % 10;
        n /= 10;
        v3 = n % 10;
        n /= 10;
        v4 = n % 10;
        v.push_back(v4 + '0');
        v.push_back(v3 + '0');
        v.push_back(v2 + '0');
        v.push_back(v1 + '0');

        return v;
    }

    Directory_Entry get_Directory_Entry(vector<char>v) {
        Directory_Entry d;
        int num1 = 0, num2 = 0;
        for (int i = 0; i < v.size(); i++) {
            if (i < 11) {
                d.FileName[i] = v[i];
            }
            else if (i < 23) {
                d.FileEmpty[i - 11] = v[i];
            }
            else if (i == 23) {
                d.FileAttribute = v[i];
            }
            else if (i < 28) {
                num1 = (num1 * 10) + (v[i] - '0');
            }
            else {
                num2 = (num2 * 10) + (v[i] - '0');
            }
        }
        d.FileSize = num1;
        d.Firstcluster = num2;
        return d;
    }
};

class Directory : public Directory_Entry
{
public:
    vector<Directory_Entry>list;
    Directory* Parent;

    Fat_table fat_table;

    Virtual_disk virtual_disk;

    void Write_directory() {
        if (list.size() != 0) {
            vector<char>dir_table_bytes;
            for (int i = 0; i < list.size(); i++) {
                vector<char>dir_ele = list[i].get_bytes(list[i]);
                for (int j = 0; j < 32; j++) {
                    dir_table_bytes.push_back(dir_ele[j]);
                }
            }
            int number_of_record_blockes = ceil((double)dir_table_bytes.size() / 1024.0);
            //cout << number_of_record_blockes << " " << list.size() << " " << dir_table_bytes.size()<< endl;
            int number_fill_blocks = dir_table_bytes.size() / 1024;
            int rem_bytes = dir_table_bytes.size() % 1024;
            if (fat_table.get_available_blocks() >= number_of_record_blockes) {
                for (int i = 0; i < number_fill_blocks; i++) {
                    vector<char>valuetowrite;
                    for (int j = i * 1024; j < (i + 1) * 1024; j++) {
                        valuetowrite.push_back(dir_table_bytes[j]);
                    }
                    int pos = fat_table.get_Available_Block();
                    if (pos != -1) {
                        virtual_disk.write_block(valuetowrite, pos);
                        fat_table.set_next(pos, -1);
                    }
                }
                vector<char>valuetowrite;
                for (int i = number_fill_blocks * 1024; i < (number_fill_blocks * 1024) + rem_bytes; i++) {
                    valuetowrite.push_back(dir_table_bytes[i]);
                }
                int pos = fat_table.get_Available_Block();
                //cout << pos << endl;
                virtual_disk.write_block(valuetowrite, pos);
                fat_table.set_next(pos, -1);
            }
            else {
                cout << "Net enough blockes\n";
            }
        }
        else {
            cout << " list is empty\n ";
        }
    }

    vector<Directory_Entry> read_directory() {
        vector<Directory_Entry> valdir;
        Directory_Entry d;
        vector<char> vas = ReadFormTo(5120);
        for (int i = 0; i < vas.size() / 32; i++) {
            vector<char>ele;
            for (int j = i * 32; j < 32 * (i + 1); j++) {
                ele.push_back(vas[j]);
            }
            valdir.push_back(d.get_Directory_Entry(ele));
            ele.clear();
        }
        return valdir;
    }

    int search_directory(string filename) {
        vector<char> vas(11);
        for (int i = 0; i < 11; i++) {
            if (filename.size() > i) {
                vas[i] = filename[i];
            }
            else {
                vas[i] = '0';
            }
        }
        int findval = -1;
        vector<Directory_Entry> valdir1 = read_directory();
        for (int i = 0; i < valdir1.size(); i++) {
            int ok = 1;
            for (int j = 0; j < 11; j++) {
                if (valdir1[i].FileName[j] != vas[j]) {
                    ok = 0;
                    break;
                }
            }
            if (ok) {
                findval = i;
                break;
            }
        }
        return findval;
    }

    void update_directory(Directory_Entry d) {
        string s = "";
        for (int i = 0; i < 11; i++) {
            s += d.FileName[i];
        }
        if (search_directory(s) != -1) {
            string block;
            fstream Filetxt("OS.txt");
            Filetxt >> block;
            int index = 0;
            for (int i = 5120; i < block.size(); i++) {
                if (block[i] == '#') {
                    i += 31;
                }
                else {
                    int ok = 1;
                    int ind = i;
                    for (int j = 0; j < 11 && ind < block.size(); j++) {
                        if (d.FileName[j] != block[ind]) {
                            ok = 0;
                            break;
                        }
                        ind++;
                    }
                    if (ok) {
                        index = i;
                    }
                }
                if (index) {
                    break;
                }
            }
            if (index != 0) {
                Filetxt.seekg(index, ios::beg);
                vector<char> dbytes = d.get_bytes(d);
                for (int i = 0; i < 32; i++) {
                    Filetxt << dbytes[i];
                }
            }
        }
        else {
            cout << "not found\n";
        }
    }
    void delete_directory(string filename) {
        vector<char> vas(11);
        for (int i = 0; i < 11; i++) {
            if (filename.size() > i) {
                vas[i] = filename[i];
            }
            else {
                vas[i] = '0';
            }
        }
        if (search_directory(filename) != -1) {
            string block;
            fstream Filetxt("OS.txt");
            Filetxt >> block;
            int index = 0;
            for (int i = 5120; i < block.size(); i++) {
                if (block[i] == '#') {
                    i += 31;
                }
                else {
                    int ok = 1;
                    int ind = i;
                    for (int j = 0; j < 11 && ind < block.size(); j++) {
                        if (vas[j] != block[ind]) {
                            ok = 0;
                            break;
                        }
                        ind++;
                    }
                    if (ok) {
                        index = i;
                    }
                }
                if (index) {
                    break;
                }
            }
            if (index != 0) {
                Filetxt.seekg(index, ios::beg);
                for (int i = 0; i < 32; i++) {
                    Filetxt << '#';
                }
            }
        }
        else {
            cout << "not found\n";
        }
    }

    vector<char> ReadFormTo(int start) {
        vector<char>val;
        string block;
        ifstream Filetxt("OS.txt");
        Filetxt >> block;
        for (int i = start; i < block.size(); i++) {
            if (block[i] == '#') {
                i += 31;
            }
            else {
                val.push_back(block[i]);
            }
        }
        Filetxt.close();
        return val;
    }
};

Directory direct;
class CMD {
public:
    // vector to store input as spilt words.
    vector<string>input;

    // all command and it's discribtion.
    map<string, pair<string, string>>command_discribtion;

    void commands_and_discribtion() {

        vector<string>command = {
                               "cd","cls","dir","help","quit",
                               "copy","del","md","rd","rename","type","import","export"
        };

        vector<string>discribtion = {
                                    "\tDisplays the name of or changes the current directory.",
                                    "\tClear the screen.","\tDisplays a list of files and subdirectories in a directory.",
                                    "\tProvides Help information for Windows commands.","\tQuit the shell.",
                                    "\tCopies one or more files to another location.","\tDeletes one or more files.",
                                    "\tCreates a directory.","\tRemoves a directory.","\tRename a file or files.",
                                    "\tDisplays the contents of a text file.","\timport text file(s) from your computer.",
                                    "\texport text file(s) to your computer."
        };

        vector<string>copy_discribtion = {
                                "\tDisplays the name of or changes the current directory.",
                                "\tClear the screen.","Displays a list of files and subdirectories in a directory.",
                                "\tProvides Help information for Windows commands.","\tQuit the shell.",
                                "\tCopies one or more files to another location.","\tDeletes one or more files.",
                                "\tCreates a directory.","\tRemoves a directory.","\tRename a file or files.",
                                "\tDisplays the contents of a text file.","import text file(s) from your computer.",
                                "\texport text file(s) to your computer."
        };

        for (int i = 0; i < command.size(); i++) {
            command_discribtion[command[i]] = make_pair(discribtion[i], copy_discribtion[i]);
        }
    }

    // check if command exist or not.
    bool find_command(string command) {

        // clear the old data in vector.
        input.clear();

        stringstream cmd(command);
        string word = "";

        // spilt the all command into words.
        while (cmd >> word)
        {
            input.push_back(word);
        }

        //search about command
        for (auto& i : command_discribtion) {
            if (i.first == input[0]) {
                return true;
            }
        }
        return false;
    }

    // function to close the console window.
    // command quit.
    void execute_quit()
    {
        ShowWindow(GetConsoleWindow(), SW_HIDE); //close the window
        exit(0);
    }

    // function to clear screen.
    // command cls.
    void execute_cls() {

        system("cls");
    }

    // function to execute command help.
    //command help.
    void execute_help() {

        for (auto& i : command_discribtion) {
            cout << i.first << "      " << i.second.first << "\n";
        }
    }

    //function to execute command cd.
    //command cd.
    void execute_cd() {
        if (input.size() == 1) {
            cout << dir << endl;
        }
        else if (input.size() == 2) {
            for (int i = 1; i < input.size(); i++) {
                if (direct.search_directory(input[i]) != -1) {
                    dir = dir + "\\" + input[i];
                }
                else {
                    cout << "A subdirectory or file " << input[i] << " Not exists.\n";
                }
            }
        }
        else {
            cout << "The syntax of the command is incorrect.\n";
        }
    }

    //function to execute command md.
    //command md.
    void execute_md() {
        if (input.size() == 1) {
            cout << "The syntax of the command is incorrect.\n";
        }
        else {
            for (int i = 1; i < input.size(); i++) {
                if (direct.search_directory(input[i]) == -1) {
                    Directory_Entry d;
                    for (int j = 0; j < 11; j++) {
                        if (input[i].size() > j) {
                            d.FileName[j] = input[i][j];
                        }
                        else {
                            d.FileName[j] = '0';
                        }
                    }
                    for (int j = 0; j < 12; j++) {
                        d.FileEmpty[j] = '0';
                    }
                    d.FileAttribute = '1';
                    d.FileSize = 0;
                    d.Firstcluster = 0;
                    direct.list.push_back(d);
                    direct.Write_directory();
                    direct.list.clear();
                }
                else {
                    cout << "A subdirectory or file " << input[i] << " already exists.\n";
                }
            }
        }
    }

    //function to execute command rd.
    //command rd.
    void execute_rd() {
        if (input.size() == 1) {
            cout << "The syntax of the command is incorrect.\n";
        }
        else {
            for (int i = 1; i < input.size(); i++) {
                if (direct.search_directory(input[i]) != -1) {
                    direct.delete_directory(input[i]);
                }
                else {
                    cout << "A subdirectory or file " << input[i] << " Not exists.\n";
                }
            }
        }
    }

    //function to execute command rd.
    //command rd.
    void execute_dir() {
        cout << "\nDirectory of " << dir << "\n\n";
        int nfile = 0, nfolder = 0, nsizefile = 0;
        vector<Directory_Entry>vv = direct.read_directory();
        for (int i = 0; i < vv.size(); i++) {
            if (vv[i].FileAttribute == '0') {
                cout << "\t\t\t";
                for (int j = 0; j < 11; j++) {
                    if (vv[i].FileName[j] != '0') {
                        cout << vv[i].FileName[j];
                    }
                }
                nfile++;
                nsizefile += vv[i].FileSize;
            }
            else {
                cout << "<DIR>\t";
                for (int j = 0; j < 11; j++) {
                    if (vv[i].FileName[j] != '0') {
                        cout << vv[i].FileName[j];
                    }
                }
                nfolder++;
            }
            cout << "\n";
        }
        cout << "\t" << nfile << " file(s) \t" << nsizefile << " bytes\n";
        cout << "\t" << nfolder << " Dir(s) \t" << (1024 * 1024) - nsizefile << " free bytes\n";
    }

    //function to execute command import.
    //command import.
    void execute_import() {
        if (input.size() == 2) {
            if (IsPathExist(input[1])) {
                string path = input[1], sname = "";
                for (int i = path.size() - 1; i >= 0; i--) {
                    if (path[i] == '\\') {
                        break;
                    }
                    else {
                        sname += path[i];
                    }

                }
                reverse(sname.begin(), sname.end());
                if (direct.search_directory(sname) == -1) {
                    Directory_Entry d;
                    for (int i = 0; i < 11; i++) {
                        if (sname.size() > i) {
                            d.FileName[i] = sname[i];
                        }
                        else {
                            d.FileName[i] = '0';
                        }
                    }
                    for (int i = 0; i < 12; i++) {
                        d.FileEmpty[i] = '0';
                    }
                    d.FileAttribute = '0';
                    fstream Filetxt(path);
                    string sz;
                    Filetxt >> sz;
                    d.FileSize = sz.size();
                    Filetxt.close();
                    d.Firstcluster = 12;
                    direct.list.push_back(d);
                    direct.Write_directory();
                    direct.list.clear();
                }
                else {
                    cout << "A subdirectory or file already exists.\n";
                }
            }
            else {
                cout << "Not Found Path\n";
            }
        }
        else {
            cout << "The syntax of the command is incorrect.\n";
        }
    }

    //function to execute command rename.
    //command rename.
    void execute_rename() {

        if (input.size() == 3) {
            string old = "", Newa = "", alls;
            int i;
            for (i = 0; i < 11; i++) {
                if (input[1].size() > i) {
                    old += input[1][i];
                }
                else {
                    old += '0';
                }
            }
            for (int i = 0; i < 11; i++) {
                if (input[2].size() > i) {
                    Newa += input[2][i];
                }
                else {
                    Newa += '0';
                }
            }
            if (direct.search_directory(input[1]) != -1) {
                fstream file("OS.txt");
                file >> alls;
                for (int i = 5120; i < alls.size(); i++) {
                    if (alls[i] == '#') {
                        i += 1023;
                    }
                    else {
                        int ok = 1, j, inds = i;
                        for (j = 0; j < 11; j++, inds++) {
                            if (alls[inds] != old[j]) {
                                ok = 0;
                                break;
                            }
                        }
                        if (ok) {
                            for (j = 0; j < 11; j++, i++) {
                                alls[i] = Newa[j];
                            }
                            break;
                        }
                        else {
                            i += 1023;
                        }
                    }
                }
                file.close();
                ofstream filef("OS.txt");
                filef << alls;
                filef.close();
            }
            else {
                cout << "A subdirectory or file " << input[i] << " Not exists.\n";
            }
        }
        input.clear();
    }

    //function to execute command delete.
    //command delete.
    void execute_deletef() {
        if (input[0] == "del")
        {
            map<string, int>mp;
            vector<Directory_Entry>vv = direct.read_directory();
            for (int i = 0; i < vv.size(); i++) {
                string s = "";
                for (int j = 0; j < 11; j++) {
                    if (vv[i].FileName[j] == '0') {
                        break;
                    }
                    else {
                        s += vv[i].FileName[j];
                    }
                }
                if (vv[i].FileAttribute == '1') {
                    mp[s] = 1;
                }
                else {
                    mp[s] = 0;
                }
            }
            if (input.size() == 1) {
                cout << "The syntax of the command is incorrect.\n";
            }
            else {
                for (int i = 1; i < input.size(); i++) {
                    string nameoffolder = input[i];
                    if (direct.search_directory(nameoffolder) != -1 && mp[nameoffolder] == 0) {
                        direct.delete_directory(nameoffolder);
                    }
                    else {
                        cout << "The system cannot find the file specified or it is folder.\n";
                    }
                }
            }
        }
    }

    // execute the all command.
    void execute_command() {
        string command = input[0];

        if (command == "cls") {
            execute_cls();
        }

        else if (command == "help") {
            // input may be contain one word or two.
            // if one word print all commands and it's discribtions.
            // if two words print the command and it's discribtion.
            if (input.size() == 1) {
                execute_help();
            }
            else {
                // convert the characters of second command to lowercase.
                string cmd = input[1];
                for (int i = 0; i < cmd.size(); i++) {
                    input[1][i] = tolower(cmd[i]);
                }

                //check if the second word is command or not.
                if (input[1] == "help" || input[1] == "cd" || input[1] == "cls" || input[1] == "quit"
                    || input[1] == "md" || input[1] == "rd" || input[1] == "dir" || input[1] == "import"
                    || input[1] == "rename" || input[1] == "del" || input[1] == "export" || input[1] == "type"
                    || input[1] == "copy") {

                    cout << command_discribtion[input[1]].second;
                }
                else {
                    cout << input[1] << ": is not recognized as an internal or external command,\noperable program or batch file.";
                }
            }
        }

        else if (command == "quit") {
            execute_quit();
        }

        else if (command == "cd") {
            execute_cd();
        }

        else if (command == "md") {
            execute_md();
        }

        else if (command == "rd") {
            execute_rd();
        }

        else if (command == "dir") {
            execute_dir();
        }

        else if (command == "import") {
            execute_import();
        }

        else if (command == "rename") {
            execute_rename();
        }

        else if (command == "del") {
            execute_deletef();
        }
    }
};


int main(int argc, char* argv[]) {

    FastCode();

    string cur_dir(argv[0]);
    int pos = cur_dir.find_last_of("/\\");
    dir = cur_dir.substr(0, pos);
    string cmd;

    Virtual_disk v;
    Fat_table fat;
    CMD cmD;

    fat.initialize_fatTable();
    fat.write_fatTable();

    cmD.commands_and_discribtion();

    while (1) {

        cout << dir << ">";
        getline(cin, cmd);
        if (count(cmd.begin(), cmd.end(), ' ') == cmd.size()) {
            continue;
        }
        else {
            //convert the characters of the command to lowercase.
            cmd += " ";
            int space_index = cmd.find(' ');
            string s = cmd.substr(0, space_index);
            string original_command = "";
            for (int i = 0; i < s.size(); i++) {
                original_command += cmd[i];
                cmd[i] = tolower(s[i]);
            }

            //call function find_command to check if the command exist or not.
            if (cmD.find_command(cmd)) {
                cmD.execute_command();
            }
            else {
                cout << original_command << ": is not recognized as an internal or external command,\noperable program or batch file.";
            }
        }
        cout << "\n\n";
    }

    return 0;
}