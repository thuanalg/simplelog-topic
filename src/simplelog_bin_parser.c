#include "simplelog_bin_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int
spl_parse_binlog(SPL_PARSER_INPUT * const inp)
{
    int ret = 0;
    FILE *fp = 0;
    do {
        if(!inp) {
            ret = 1;
            break;
        }
        if(!inp->fname[0]) {
            ret = 1;
            break;
        }
        fp  = fopen(inp->fname, "rb");
        if(fp) {
            ret = 1;
            break;
        }
    } while(0);
    if(fp) {
        fclose(fp);
    }
    return ret;
}
/* The parser based on UTF-8 spirit. */
int main(int argc, char *argv[]) {
    SPL_PARSER_INPUT path = {0};
    int ret  = 0;
    if(argc < 2) {
        return 1;
    }
    snprintf(path.fname, sizeof(path.fname), "%s", argv[1]);
    ret = spl_parse_binlog(&path);
    return ret;
}