#include <type_traits> // C++11
#include <concepts> // C++20
#include <ranges> // C++20
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_map>
#include <map>
#include <cassert>
#include <utility>
#include <concepts>
#include <stdio.h>
#include <string.h>

int range = 0;
char *filename = 0;
char *keysearch = 0;
#define BUFF_LINE       2048
char buffline[BUFF_LINE + 1];

int main(int argc, char *argv[]) {
    FILE *fp = 0;
    std::map<int, int> m_map;
    char *tmpstr = 0;
    int lenkey = 0;
    int n = 0;
    char c = 0;
    if(argc < 4) {
        return 1;
    }
    filename = argv[1];
    sscanf(argv[2], "%d", range);
    keysearch = argv[3];
    lenkey = strlen(keysearch);
    do {
        fp = fopen(filename, "r");
        if(!fp) {
            break;
        }
        while(1) {
            c  = getc(fp);
            if ( c == '\n') {
                if(n > 1) {
                    tmpstr = strstr(buffline, keysearch);
                    if(tmpstr) 
                    {
                        int fre = -1;
                        tmpstr += lenkey;
                        sscanf(tmpstr, "%d", &fre);
                        if(fre > -1) 
                        {
                            m_map[fre]++;
                        }
                    }
                }
                n = 0;
                memset(buffline, 0, sizeof(buffline));                
            }
            if( c == EOF) {
                break;
            }
            buffline[n] = c;
            ++n;
        }
    } while(0);
    if(fp) {
        fclose(fp);
    }
    if(m_map.size()) {
        int i = 0;
        int sz = 0;
        sz = (int) m_map.size();
        for(i = 0; i < sz; ++i) {
            if(m_map[i] < range) {
                fprintf(stderr, "\nerror: i: %d\n", i);
                break;
            }
        }
     }
    return 0;
}
