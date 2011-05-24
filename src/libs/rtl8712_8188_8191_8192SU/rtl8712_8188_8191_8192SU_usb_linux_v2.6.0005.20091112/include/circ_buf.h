#ifndef __CIRC_BUF_H_
#define __CIRC_BUF_H_ 1

#define CIRC_CNT(head,tail,size) (((head) - (tail)) & ((size)-1))

#define CIRC_SPACE(head,tail,size) CIRC_CNT((tail),((head)+1),(size))

#endif //_CIRC_BUF_H_
