#include "flags.h"

#define		NO_FLAGS		6	/* flags para impressao */
#define		NO_MFLAGS		4
#define 	NO_NTYPES 		7

struct bbx_queue *parse_opt(int argc, char *argv[]);

void bbx_chtop(bbx_flags fopt);
void bbx_chtof(bbx_flags fopt);
void bbx_opt_printf();
