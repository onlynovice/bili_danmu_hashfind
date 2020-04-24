#include <iostream>
#include <cstring>

using namespace std;

unsigned long Crc32Table[256];

int length(int num) {
    int len = 1;
    while (num / 10 != 0) {
        ++len;
        num /= 10;
    }
    return len;
}

char getIthChar(int n,int i){
    return to_string(n)[i];
}

void GetCrc32Table() {
    unsigned long Crc=0;
    int i=0, j=0;
    for (i = 0; i < 256; ++i) {
        Crc = i;
        for (j = 8; j > 0; --j) {
            if ((Crc & 1) == (unsigned int)1) {
                Crc =( (Crc >> 1) ^ (0xedb88320));
            } else {
                Crc = Crc>>1;
            }
        }
        Crc32Table[i] = Crc;
    }
}

unsigned long GetCrc32Str(unsigned int n) {
    unsigned long value = 0xffffffff;
    int len = length(n);
    for (int i = 0; i < len; i++) {
        value = (value >> 8) ^ Crc32Table[(value & 0xFF) ^ (getIthChar(n,i))];
    }
    return value ^ 0xffffffff;
}

int main() {
    memset(Crc32Table, 0, sizeof(Crc32Table));
    GetCrc32Table();
    unsigned int n;
    cout<<"输入用户uid:\n";
    cin>>n;
    cout<<"用户 uid:"<<n<<" 的hashID为:\n"<<hex<<GetCrc32Str(n)<<endl;
    return 0;
}
