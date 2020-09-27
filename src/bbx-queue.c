#include "bbx-queue.h"
#include <stdio.h>

inline char *bbx_peek(struct bbx_queue *q){
	if( q->head != q->tail ){
		return q->args[q->head];
	}
	else return Q_ERR; //vazia
}

inline char *bbx_dequeue(struct bbx_queue *q){
	if( q->head != q->tail ){
		char *res = q->args[q->head];
		q->head = (q->head + 1) % Q_SIZE;
		return res;
	} else return Q_ERR; //vazia
}

inline char *bbx_enqueue(struct bbx_queue *q, char *str){
	if( q->head != q->tail ){
		q->args[q->tail] = str; //adiciona ponteiro para string
		q->tail = (q->tail + 1) % Q_SIZE;
		if( q->head == -1 ) q->head+=1; //lista populada; agora head = 0, tail = 1
		return str;
	} else return Q_ERR;
}

inline void bbx_queue_init(struct bbx_queue *q){
	q->head=-1;
	q->tail=0;
}

inline void bbx_queue_stats(struct bbx_queue *q){
	fprintf(stderr, "head:\t%d\n"
			"tail:\t%d\n"
			"*head:\t%s\n"
			"*tail:\t%s\n",
			q->head, q->tail,
			q->args[q->head],
			q->args[q->tail]);
}
