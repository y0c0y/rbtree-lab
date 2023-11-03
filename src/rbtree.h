#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t; // 노드의 색을 나타내는 열거형

typedef int key_t; // 키의 타입을 int로 정의

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t; // 노드 구조체

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree; // 레드-블랙 트리 구조체

rbtree *new_rbtree(void); // rbtree 구조체를 동적 할당하는 함수

void rbtree_transplant(rbtree *, node_t *, node_t *); // 노드를 대체하는 함수
void left_rotate(rbtree *, node_t *); // 왼쪽으로 회전하는 함수
void right_rotate(rbtree *, node_t *); // 오른쪽으로 회전하는 함수

void free_node(node_t*); // 노드를 해제하는 함수
void delete_rbtree(rbtree *); // rbtree 구조체를 해제하는 함수

node_t *rbtree_insert(rbtree *, const key_t); // 노드를 삽입하는 함수
void rbtree_insert_fixup(rbtree *, node_t *); // 삽입 후 불균형을 해결하는 함수

node_t *rbtree_find(const rbtree *, const key_t); // 노드를 찾는 함수
void rbtree_delete_fixup(rbtree *, node_t *); // 삭제 후 불균형을 해결하는 함수

node_t *rbtree_min(const rbtree *); // 최소값을 가진 노드를 찾는 함수
node_t *rbtree_max(const rbtree *); //  최대값을 가진 노드를 찾는 함수

int rbtree_erase(rbtree *, node_t *); // 노드를 삭제하는 함수

int inorder_traversal(const rbtree *, node_t *, key_t *, int *, const size_t ); // 중위 순회하는 함수
int rbtree_to_array(const rbtree *, key_t *, const size_t); // 트리를 배열로 변환하는 함수

#endif  // _RBTREE_H_