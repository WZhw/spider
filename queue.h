#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

typedef struct{
    char* queue[MAXN];
    int head, tail;
}Queue;

Queue *queue_init(){
    Queue *q;
	if((q = malloc(sizeof(Queue))) == NULL){
		return NULL;
	}
	memset(q, 0, sizeof(Queue));
	q->head = 0;
	q->tail = 0;
	return q;
}

int isEmpty(Queue *q){
    return ((q->head - q->tail) == 0) ? 1 : 0;
}

int isFull(Queue *q){
    return ((q->head - q->tail + MAXN)%MAXN == 1);
}

int enQueue(Queue *q, AC_TREE node){
    if(isFull(q)){
        return 0;
    }
    else{
        q->queue[q->tail] = node;
        q->tail = (q->tail + 1) % MAXN;
        return 1;
    }
}

AC_TREE deQueue(Queue *q){
    if(isEmpty(q)){
        return NULL;
    }
    else{
        int temp = q->head;
        q->head = (q->head + 1) % MAXN;
        return q->queue[temp];
    }
}

#endif // QUEUE_H_INCLUDED
