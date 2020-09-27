#include <string.h>
#include "bbx-queue.h"

struct
bbx_queue{
	int head, tail;
	int idx[Q_SIZE]; 
};

inline int bbx_peek(struct bbx_queue *q){
	if( q->head != q->tail ){
		return q->idx[q->head];
	}
	else return Q_ERR; //vazia
}

inline int bbx_dequeue(struct bbx_queue *q){
	if( q->head != q->tail ){
		int res = q->idx[q->head];
		q->head= (q->head + 1) % Q_SIZE;
		return res;
	} else return Q_ERR; //vazia
}

inline int bbx_enqueue(struct bbx_queue *q){
	if( q->head != q->tail ){
		q->tail = (q->tail + 1) % Q_SIZE;
		if( q->head == -1 ) q->head+=1; //lista populada; agora head = 0, tail = 1
		return 0;
	} else return Q_ERR;
}

inline void bbx_queue_init(struct bbx_queue *q){
	q->head=-1;
	q->tail=0;
	memset(q->idx, 0, sizeof(int)*Q_SIZE);
}
