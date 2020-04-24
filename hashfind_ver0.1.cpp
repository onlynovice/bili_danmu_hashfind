#include <iostream>
#include <cstring>
using namespace std;

unsigned long Crc32Table[256];
char goal[15];

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
    unsigned long Crc;
    int i, j;
    for (i = 0; i < 256; ++i) {
        Crc = (long) i;
        for (j = 8; j > 0; --j) {
            if ((Crc & 1) == 1) {
                Crc = (Crc >> 1) ^ 0xEDB88320;
            } else {
                Crc >>= 1;
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

int charhex_to_int(const char a){
    if(a>=48&&a<=57){
        return a-48;
    } else if(a>=65&&a<=70){
        return a-55;
    } else{
        return a-87;
    }
}

unsigned long strhex_to_ulong(const char * str){
    int len=strlen(str);
    unsigned long ans=0;
    for(int i=0;i<len;++i){
        unsigned long x=charhex_to_int(goal[i]);
        for(int j=0;j<len-1-i;++j){
            x*=16;
        }
        ans+=x;
    }
    return ans;
}



int main() {
    cout << "输入目标hashID：\n";
    memset(goal, 0, sizeof(goal));
    cin >> goal;
    unsigned long decGoal=strhex_to_ulong(goal);
    memset(Crc32Table, 0, sizeof(Crc32Table));
    GetCrc32Table();

    unsigned int i=0;
    printf("开始破解...获取的hash是0x%s\n",goal);
    while(true){
        ++i;
        if(GetCrc32Str(i)==decGoal){
            printf("已找到目标UID : %d\n",i);
            break;
        }
        if(i%1000000==0){
            printf("%d : %lx\n",i,GetCrc32Str(i));
        }
        if(i>1000000000){
            printf("目标不存在\n");
        }
    }

    return 0;
}
