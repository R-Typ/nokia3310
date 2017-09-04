#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct Entry
{
    string name;
    string phone;
};

void createBackup(vector<Entry> entries);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: contimp <FILE.vcf>"<<endl;
        return 1;
    }

    ifstream file(argv[1]);
    string str, name;
    Entry entry;
    vector<Entry> entries;
    size_t pos;
    int mobileCnt(1), homeCnt(1), workCnt(1);
    while (getline(file, str))
    {
        pos = str.find("FN:");
        if (pos != std::string::npos)
        {
            name = str.substr(pos + 3);
            mobileCnt=1;
            homeCnt=1;
            workCnt=1;
        }
        pos = str.find("TEL;TYPE=");
        if (pos != std::string::npos)
        {
            string s = str.substr(pos + 9);
            pos = s.find(":");
            if (pos != std::string::npos)
            {
                string type = s.substr(0, pos-1);
                entry.phone = s.substr(pos+1);
                entry.name = name;
                if (type == "CELL")
                {
                    entry.name.append("_M");
                    if (mobileCnt > 1) entry.name.append(to_string(mobileCnt));
                    mobileCnt++;
                } else if (type == "WORK") {
                    entry.name.append("_W");
                    if (mobileCnt > 1) entry.name.append(to_string(workCnt));
                    workCnt++;
                } else if (type == "HOME") {
                    entry.name.append("_H");
                    if (homeCnt > 1) entry.name.append(to_string(homeCnt));
                    homeCnt++;
                }
                entries.push_back(entry);
            }
        }
    }
    createBackup(entries);
    return 0;
}

void createBackup(vector<Entry> entries)
{
    std::ofstream ofs;
    ofs.open ("backup.dat", std::ofstream::out | std::ofstream::trunc);
    for(size_t i=0; i< entries.size(); i++)
    {
        const Entry entry=entries.at(i);
        ofs << "BEGIN:VCARD" <<endl;
        ofs << "VERSION:2.1" <<endl;
        ofs << "N;ENCODING=QUOTED-PRINTABLE;CHARSET=UTF-8:;=" <<endl;
        ofs << entry.name << ";;;" <<endl;
        ofs << "TEL;VOICE;CELL:" << entry.phone <<endl;
        ofs << "END:VCARD" <<endl << endl;
    }
    ofs.close();
}
