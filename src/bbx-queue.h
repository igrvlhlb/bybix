#define Q_SIZE 512
#define Q_ERR 0 //null

/* 
 * Esta struct e uma fila que guardara as strings a
 * serem lidas. Isto porque getopt() nao diferenciara
 * opcoes (-[a-zA-Z]) de numeros negativos (-[0-9]).
 * Por esse motivo, as strings serao lidas e enfileiradas
 * antes de getopt() lê-las, ja que reordena os elementos
 */
struct
bbx_queue{
	int head, tail;
	char *args[Q_SIZE]; 
};

char *bbx_peek(struct bbx_queue *q);
char *bbx_dequeue(struct bbx_queue *q);
char *bbx_enqueue(struct bbx_queue *q, char *str);
void bbx_queue_init(struct bbx_queue *q);
void bbx_queue_stats(struct bbx_queue *q);
