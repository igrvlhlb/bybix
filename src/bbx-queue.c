#include "bbx-queue.h"
#include <stdlib.h>
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
		q->head = (q->head + 1) % q->cap;
		return res;
	} else return Q_ERR; //vazia
}

inline char *bbx_enqueue(struct bbx_queue *q, char *str){
	if( q->head != q->tail ){
		q->args[q->tail] = str; //adiciona ponteiro para string
		q->tail = (q->tail + 1) % q->cap;
		if( q->head == -1 ) q->head+=1; //lista populada; agora head = 0, tail = 1
		return str;
	} else return Q_ERR;
}

inline struct bbx_queue *bbx_queue_init(int size){
	struct bbx_queue *q = malloc(sizeof(struct bbx_queue)+(sizeof(char*)*size));
	q->head = -1;
	q->tail = 0;
	q->cap = size;
	return q;
}

inline void bbx_queue_show_stats(struct bbx_queue *q){
	fprintf(stderr, "head:\t%d\n"
			"tail:\t%d\n"
			"*head:\t%s\n"
			"*tail:\t%s\n"
			"cap:\t%d\n",
			q->head, q->tail,
			q->args[q->head],
			q->args[q->tail],
			q->cap);
}
