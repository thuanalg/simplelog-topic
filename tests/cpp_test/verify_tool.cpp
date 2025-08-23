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
    sscanf(argv[2], "%d", &range);
    keysearch = argv[3];
    lenkey = strlen(keysearch);
    do {
        fp = fopen(filename, "r");
        if(!fp) {
            break;
        }
        while(1) {
            c  = getc(fp);
            if ( c == '\n' || c == EOF) {
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
                if( c == EOF) {
                    break;
                }                
                continue;               
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
	    fp = fopen(argv[4], "w+");
        sz = (int) m_map.size();
        for(i = 0; i < sz; ++i) 
        {
            if(m_map[i] < range) 
            {
                //fprintf(stderr, "error: i: %d, range %d != val: %d\n", 
                //    i, range, m_map[i]);
		        if (fp) {
					fprintf(fp, "error at [i: %d], range %d != val: %d\n", 
						i, range, m_map[i]);					
		        }
            }
        }
		if(fp) {
			fclose(fp);
		}
     }
    return 0;
}
//./verify_tool ./log_simple/2025/08/2025-08-20-simplelog_00000000.log 10 "My test log : "
//./verify_tool ./log_simple/2025/08/2025-08-20-simplelog_00000000.log 10 "My test log : "
//./verify_tool ./log_simple/2025/08/2025-08-20-simplelog_00000000.log 10 "My test log : "
//verify_tool "2025-08-23-testlog-sys-0000000.log" 20 "Log: "
//start dumb_fmt.exe "2025-08-24-testlog-exe-0000000.log" 30 "Log: "   zzzz_24_exe.txt 
//start dumb_fmt.exe "2025-08-24-testlog-lib-0000000.log" 30 "Log: "   zzzz_24_lib.txt 
//start dumb_fmt.exe "2025-08-24-testlog-nayax-0000000.log" 30 "Log: "   zzzz_24_nayax.txt 
//start dumb_fmt.exe "2025-08-24-testlog-sksgn-0000000.log" 30 "Log: "   zzzz_24_sksgn.txt 
//start dumb_fmt.exe "2025-08-24-testlog-sys-0000000.log" 30 "Log: "   zzzz_24_sys.txt 