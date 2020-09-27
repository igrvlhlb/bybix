#define Q_SIZE 512
#define Q_ERR (((int) ~0) >> 1)

struct bbx_queue;

inline int bbx_peek(struct bbx_queue *q);
inline int bbx_dequeue(struct bbx_queue *q);
inline int bbx_enqueue(struct bbx_queue *q);
inline void bbx_queue_init(struct bbx_queue *q);
