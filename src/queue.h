#include "flags.h"

#define Q_SIZE 512
#define Q_ERR 0 //null

// par arg-flag
struct
afp {
	bbx_flags flags;
	char *str;
};

/* 
 * Esta struct e uma fila que guardara as strings a
 * serem lidas. Isto porque getopt() nao diferenciara
 * opcoes (-[a-zA-Z]) de numeros negativos (-[0-9]).
 * Por esse motivo, as strings serao lidas e enfileiradas
 * antes de getopt() lê-las, ja que reordena os elementos
 */
struct
bbx_queue{
	int head, tail, cap; //cap = capacidade
	struct afp args[0]; 
};

char *bbx_peek(struct bbx_queue *q);
char *bbx_dequeue(struct bbx_queue *q);
char *bbx_enqueue(struct bbx_queue *q, char *str);
struct bbx_queue *bbx_queue_init(int size);
void bbx_queue_show_stats(struct bbx_queue *q);
