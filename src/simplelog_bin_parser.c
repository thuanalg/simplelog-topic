#include "simplelog_bin_parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPL_PARSER_BUF (1024)
void
spl_parser_dump(SPL_HEADER *);

int
spl_parse_binlog(SPL_PARSER_INPUT * const inp)
{
    int ret = 0;
	char buf[SPL_PARSER_BUF] = {0};
    FILE *fp = 0;
    char determine = 0;
    int n = 0;
    SPL_HEADER *p = 0;
    int k = 0;
    int step = 0;
    int count = 0;
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
        if(!fp) {
            ret = 1;
            break;
        }
	    /*You have to check litile/big endian, 32/64 bit for parsing with different machine/arch.*/
	    /*However, if is sample PC/arch, it is unnecessary. */
	    determine = fgetc(fp);
	    /*determine is important to parse from different ARCH.*/
	    while (1) {
		    memset(buf, 0, sizeof(buf));
		    step = 0;
		    n = 0;
		    k = 0;
		    n = fread(buf + step, 1, sizeof(SPL_HEADER), fp);
		    if (n < sizeof(SPL_HEADER)) {
			    break;
		    }
		    step = n;
		    p = (SPL_HEADER *)buf;
		    k = p->total - sizeof(SPL_HEADER);
		    /*Please note about over buffer. Here just a sample.*/
		    n = fread(buf + step, 1, k, fp);
		    if (n < k) {
			    break;
		    }
		    /*Now we have binary data.*/
		    spl_parser_dump((SPL_HEADER *)buf);
		    ++count;
	    }
    } while(0);
    fprintf(stdout, "\ncount: %d.\n", count);
    if(fp) {
        fclose(fp);
    }
    return ret;
}
#if 0
typedef struct SPL_HEADER_TAG {
	int total; /* Total package size in bytes. */
	char spec_key[2]; /* Special key to help detect or recover corrupted data. */
	unsigned short type_id; /* Identifier to determine the data type. */
	LLU timestamp; /* Timestamp in nanoseconds. */
} SPL_HEADER;
#endif

void
spl_parser_dump(SPL_HEADER *p)
{
	char spec_key[3] = {0};

	spec_key[0] = p->spec_key[0];
	spec_key[1] = p->spec_key[1];

	do {
		if (SPL_PARSER_TEXT == p->type_id) {
			char *pt = (char *)p->data;
			fprintf(stdout, "%s\n", pt);
			break;
		}
		if (SPL_PARSER_GPS == p->type_id) {
			SPL_BIN_GEO *pt = (SPL_BIN_GEO *)p->data;
			fprintf(stdout, "longitute: %f\n", pt->longitute);
			break;
		}
	} while (0);


	fprintf(stdout, "{total, spec_key, type_id, timestamp} = {%d, %s, %d, %llu},\n", 
        p->total, spec_key, (int)p->type_id, p->timestamp);

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