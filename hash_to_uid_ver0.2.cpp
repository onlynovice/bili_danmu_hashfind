#include <iostream>
#include <cstring>

#define ulong unsigned long
#define uint unsigned long

using namespace std;

ulong Crc32Table[256];
char goal[15];
string buffer;/*NOLINT*/

void GetCrc32Table() {
    ulong Crc;
    uint i, j;
    for (i = 0; i < 256; ++i) {
        Crc = (ulong) i;
        for (j = 8; j > 0; --j) {
            if ((Crc & 1) == 1) {/*NOLINT*/
                Crc = (Crc >> 1) ^ 0xEDB88320;/*NOLINT*/
            } else {
                Crc >>= 1;/*NOLINT*/
            }
        }
        Crc32Table[i] = Crc;
    }
}

ulong GetCrc32Str() {
    ulong value = 0xffffffff;
    uint len = buffer.length();
    for (uint i = 0; i < len; i++) {
        value = (value >> 8) ^ Crc32Table[(value & 0xFF) ^ buffer[i]];/*NOLINT*/
    }
    return value ^ 0xffffffff;
}

uint hex_char_to_int(const char &a) {
    if (a >= 48 && a <= 57) {
        return a - 48;
    } else if (a >= 65 && a <= 70) {
        return a - 55;
    } else {
        return a - 87;
    }
}

ulong hex_str_to_ulong(const char *str) {
    uint len = strlen(str);
    ulong ans = 0;
    for (uint i = 0; i < len; ++i) {
        ulong x = hex_char_to_int(goal[i]);
        for (uint j = 0; j < len - 1 - i; ++j) {
            x *= 16;
        }
        ans += x;
    }
    return ans;
}


int main() {
    memset(Crc32Table, 0, sizeof(Crc32Table));
    GetCrc32Table();

    while (true) {
        cout << "输入目标hashID：\n";
        memset(goal, 0, sizeof(goal));
        cin >> goal;
        ulong decGoal = hex_str_to_ulong(goal);
        uint i = 0;
        printf("开始破解...获取的hash是0x%s\n", goal);
        while (true) {
            ++i;
            buffer = to_string(i);
            if (GetCrc32Str() == decGoal) {
                printf("已找到目标UID : %lu\n", i);
                break;
            }
            if (!((i % 1000000) ^ 0)) { /*NOLINT*/
                printf("%lu : %lx\n", i, GetCrc32Str());
            }
            if (i > 1000000000) {
                printf("目标不存在\n");
            }
        }

        cout<<"就此结束？(y/n):\n";
        char a;
        cin>>a;
        if(a=='y'){
            break;
        }
    }

    return 0;
}
