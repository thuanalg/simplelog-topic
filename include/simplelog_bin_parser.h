/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* Email:
 *		<nguyenthaithuanalg@gmail.com> - Nguyễn Thái Thuận
 * Mobile:
 *		<+084.799.324.179>
 * Skype:
 *		<nguyenthaithuanalg>
 * Date:
 *		<2026-July-04>
 *		<2026-July-11>
 * The lasted modified date:
 * Decription:
 *		This file is to be an example for parsing a binlog file.
 *		It learnt from UTF-8.
 *		You have to do for your own, and MAY base on this example.
 */
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef ___SIMPLE_PARSE_BIN_LOG__
#define ___SIMPLE_PARSE_BIN_LOG__ 
#include "simplelog.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct __SPL_BIN_GEO__ {
	double longitute;
	double latitude;
	float speed;
	unsigned char satellites_used;
	unsigned char fix_quality;
} SPL_BIN_GEO;

typedef enum __SPL_PARSER_ID__ {
	SPL_PARSER_TEXT = 0,
	SPL_PARSER_GPS,

	SPL_PARSER_PEAK,
} SPL_PARSER_ID;

typedef struct __SPL_PARSER_INPUT__ {
	char fname[1024];
} SPL_PARSER_INPUT;
/* It is just a sample, parsing depends on your log.*/
/* It is just a sample, parsing depends on your log.*/
DLL_API_SIMPLE_LOG int
spl_parse_binlog(SPL_PARSER_INPUT *inp);

#ifdef __cplusplus
}
#endif
#endif
