#include "rbtree.h"

#include <stdlib.h>
#include <stdio.h> // for debugging

// 트리를 생성하는 함수
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); // 트리 구조체를 할당
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t)); // NIL 노드를 할당

  if(p == NULL || NIL == NULL) return NULL; // 할당에 실패하면 NULL 반환

  NIL->color = RBTREE_BLACK; // NIL 노드는 항상 검은색
  NIL->parent = NIL->left = NIL->right = NIL; // NIL 노드는 자기 자신을 가리키도록 함
  
  p->nil = NIL; // NIL 노드를 가리키도록 함
  p->root = NIL; // root 노드를 NIL 노드로 초기화
  return p; // 트리 구조체 반환
}

// 왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t *x) {
 node_t *y = x->right; // y는 x의 오른쪽 자식 노드
 x->right = y->left; // y의 왼쪽 자식 노드를 x의 오른쪽 자식 노드로 만듦
 
 // y의 왼쪽 자식 노드가 NIL 노드가 아니면 y의 왼쪽 자식 노드의 부모 노드를 x로 만듦
 if(y->left != t->nil) y->left->parent = x; 
 
 y->parent = x->parent; // y의 부모 노드를 x의 부모 노드로 만듦
 
 if(x->parent == t->nil) t->root = y; // x의 부모 노드가 NIL 노드이면 y를 root 노드로 만듦
 else if(x == x->parent->left) x->parent->left = y; // x가 x의 부모 노드의 왼쪽 자식 노드이면 y를 x의 부모 노드의 왼쪽 자식 노드로 만듦
 else x->parent->right = y; // x가 x의 부모 노드의 오른쪽 자식 노드이면 y를 x의 부모 노드의 오른쪽 자식 노드로 만듦

 y->left = x; // y의 왼쪽 자식 노드를 x로 만듦
 x->parent = y; // x의 부모 노드를 y로 만듦
}

// 오른쪽으로 회전하는 함수
void right_rotate(rbtree *t, node_t *x) {
  node_t* y = x->left; // y는 x의 왼쪽 자식 노드
  x->left = y->right; // y의 오른쪽 자식 노드를 x의 왼쪽 자식 노드로 만듦

  if(y->right != t->nil) y->right->parent = x; // y의 오른쪽 자식 노드가 NIL 노드가 아니면 y의 오른쪽 자식 노드의 부모 노드를 x로 만듦
  y->parent = x->parent; // y의 부모 노드를 x의 부모 노드로 만듦
  
  if(x->parent==t->nil) t->root = y; // x의 부모 노드가 NIL 노드이면 y를 root 노드로 만듦
  else if(x == x->parent->right) x->parent->right = y; // x가 x의 부모 노드의 오른쪽 자식 노드이면 y를 x의 부모 노드의 오른쪽 자식 노드로 만듦
  else x->parent->left = y; // x가 x의 부모 노드의 왼쪽 자식 노드이면 y를 x의 부모 노드의 왼쪽 자식 노드로 만듦
  
  y->right = x; // y의 오른쪽 자식 노드를 x로 만듦
  x->parent = y; // x의 부모 노드를 y로 만듦
}

// 노드를 이동하는 함수
void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {

  if (u->parent == t->nil) t->root = v; // u의 부모 노드가 NIL 노드이면 v를 root 노드로 만듦
  else if (u == u->parent->left) u->parent->left = v; // u가 u의 부모 노드의 왼쪽 자식 노드이면 v를 u의 부모 노드의 왼쪽 자식 노드로 만듦
  else u->parent->right = v; // u가 u의 부모 노드의 오른쪽 자식 노드이면 v를 u의 부모 노드의 오른쪽 자식 노드로 만듦
  
  v->parent = u->parent; // v의 부모 노드를 u의 부모 노드로 만듦
}

// 노드를 해제하는 함수
void free_node(node_t *p, node_t *NIL) {
  if(p == NIL) return; // p가 NULL이거나 NIL이면 함수 종료

  free_node(p->left, NIL); // p의 왼쪽 자식 노드를 인자로 재귀 호출
  free_node(p->right, NIL); // p의 오른쪽 자식 노드를 인자로 재귀 호출
  free(p); // p를 해제
}

// 트리를 삭제하는 함수
void delete_rbtree(rbtree *t) {
  free_node(t->root, t->nil); // root 노드와 NIL 노드를 인자로 전달
  free(t->nil); // NIL 노드를 해제
  free(t);
}

// 삽입 후 불균형을 해결하는 함수
void rbtree_insert_fixup(rbtree *t, node_t *z) {
 while(z->parent->color == RBTREE_RED) // z의 부모 노드의 색이 빨간색인 동안 반복
 {
   if(z->parent == z->parent->parent->left) // z의 부모 노드가 z의 부모 노드의 부모 노드의 왼쪽 자식 노드이면
   {
     node_t* y = z->parent->parent->right; // y는 z의 부모 노드의 부모 노드의 오른쪽 자식 노드
     if(y->color == RBTREE_RED) // y의 색이 빨간색이면
     {
       z->parent->color = RBTREE_BLACK; // z의 부모 노드의 색을 검은색으로 만듦
       y->color = RBTREE_BLACK; // y의 색을 검은색으로 만듦
       z->parent->parent->color = RBTREE_RED; // z의 부모 노드의 부모 노드의 색을 빨간색으로 만듦
       z = z->parent->parent; // z를 z의 부모 노드의 부모 노드로 만듦
     }
     else
     {
        if(z == z->parent->right) // z가 z의 부모 노드의 오른쪽 자식 노드이면
        {
          z = z->parent; // z를 z의 부모 노드로 만듦
          left_rotate(t, z); // z를 기준으로 왼쪽으로 회전
        }
        z->parent->color = RBTREE_BLACK; // z의 부모 노드의 색을 검은색으로 만듦
        z->parent->parent->color = RBTREE_RED; // z의 부모 노드의 부모 노드의 색을 빨간색으로 만듦
        right_rotate(t, z->parent->parent); // z의 부모 노드의 부모 노드를 기준으로 오른쪽으로 회전
     }
   }
   else
   {
      node_t* y = z->parent->parent->left; // y는 z의 부모 노드의 부모 노드의 왼쪽 자식 노드
      if(y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK; // z의 부모 노드의 색을 검은색으로 만듦
        y->color = RBTREE_BLACK; // y의 색을 검은색으로 만듦
        z->parent->parent->color = RBTREE_RED; // z의 부모 노드의 부모 노드의 색을 빨간색으로 만듦
        z = z->parent->parent; // z를 z의 부모 노드의 부모 노드로 만듦
      }
      else
      {
          if(z == z->parent->left)
          {
            z = z->parent; // z를 z의 부모 노드로 만듦
            right_rotate(t, z); // z를 기준으로 오른쪽으로 회전
          }
          z->parent->color = RBTREE_BLACK; // z의 부모 노드의 색을 검은색으로 만듦
          z->parent->parent->color = RBTREE_RED; // z의 부모 노드의 부모 노드의 색을 빨간색으로 만듦  
          left_rotate(t, z->parent->parent); // z의 부모 노드의 부모 노드를 기준으로 왼쪽으로 회전
      }
    }
  }
  t->root->color = RBTREE_BLACK; // root 노드의 색을 검은색으로 만듦
}

// 트리에 노드를 삽입하는 함수
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t* y = t->nil; // y는 NIL 노드
  node_t* x = t->root; // x는 root 노드
  
  while(x != t->nil) // x가 NIL 노드가 아닌 동안 반복
  {
    y = x; // y를 x로 만듦
    if(key < x->key) x = x->left; // key가 x의 키보다 작으면 x를 x의 왼쪽 자식 노드로 만듦
    else x = x->right; // 그렇지 않으면 x를 x의 오른쪽 자식 노드로 만듦
  }

  node_t* z = (node_t *)calloc(1, sizeof(node_t)); // z는 새로운 노드
  
  if(z == NULL) return NULL; // 할당에 실패하면 NULL 반환
  
  z->key = key; // z의 키를 key로 만듦
  z->parent = y; // z의 부모 노드를 y로 만듦

  if (y == t->nil) t->root = z; // y가 NIL 노드이면 z를 root 노드로 만듦
  else if (key < y->key) y->left = z; // key가 y의 키보다 작으면 z를 y의 왼쪽 자식 노드로 만듦
  else y->right = z; // 그렇지 않으면 z를 y의 오른쪽 자식 노드로 만듦

  z->left = t->nil; // z의 왼쪽 자식 노드를 NIL 노드로 만듦
  z->right = t->nil; // z의 오른쪽 자식 노드를 NIL 노드로 만듦
  z->color = RBTREE_RED; // z의 색을 빨간색으로 만듦

  rbtree_insert_fixup(t, z); // 삽입 후 불균형을 해결
  return z; // 삽입한 노드 반환
}

 // 트리에서 노드를 찾는 함수(중복 값이 있을 때)
node_t *rbtree_find(const rbtree *t, const key_t key)
{
    node_t *p = t->root;

    while (p != t->nil && key != p->key) 
    {
        if (key < p->key) p = p->left;
        else p = p->right;
    }

    if (p != t->nil && p->key == key) return p; // 노드를 찾으면 해당 노드 반환
    else return NULL; // 찾지 못하면 NULL 반환  
}

// 트리에서 최소값을 찾는 함수
node_t *rbtree_min(const rbtree *t) {
  if(t->root == t->nil) return t->nil; // root 노드가 NIL 노드이면 NIL 노드 반환
  node_t* x = t->root;
  while(x->left != t->nil) x = x->left; // x를 x의 왼쪽 자식 노드로 업데이트
  return x; // 가장 왼쪽 끝에 있는 노드 (최소값을 가진 노드) 반환
}

// 트리에서 최대값을 찾는 함수
node_t *rbtree_max(const rbtree *t) {
  if(t->root == t->nil) return t->nil; // root 노드가 NIL 노드이면 NIL 노드 반환
  node_t* x = t->root;
  while(x->right != t->nil) x = x->right; // x를 x의 오른쪽 자식 노드로 업데이트
  return x; // 가장 오른쪽 끝에 있는 노드 (최대값을 가진 노드) 반환

}

// 삭제 후 불균형을 해결하는 함수
void rbtree_delete_fixup(rbtree *t, node_t *x) {
  node_t* w = NULL;// w는 x의 형제 노드
  while(x != t->root && x->color == RBTREE_BLACK) // x가 root 노드가 아니고 x의 색이 검은색인 동안 반복
  {
    if(x == x->parent->left) // x가 x의 부모 노드의 왼쪽 자식 노드이면
    {
      w = x->parent->right; // w는 x의 형제 노드
      if(w->color == RBTREE_RED) // w의 색이 빨간색이면
      {
        w->color = RBTREE_BLACK; // w의 색을 검은색으로 만듦
        x->parent->color = RBTREE_RED; // x의 부모 노드의 색을 빨간색으로 만듦
        left_rotate(t, x->parent); // x의 부모 노드를 기준으로 왼쪽으로 회전
        w = x->parent->right; // w를 x의 부모 노드의 오른쪽 자식 노드로 만듦
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) // w의 왼쪽 자식 노드와 오른쪽 자식 노드의 색이 모두 검은색이면
      {
        w->color = RBTREE_RED; // w의 색을 빨간색으로 만듦
        x = x->parent; // x를 x의 부모 노드로 만듦
      }
      else
      {
        if(w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK; // w의 왼쪽 자식 노드의 색을 검은색으로 만듦
          w->color = RBTREE_RED; // w의 색을 빨간색으로 만듦
          right_rotate(t, w); // w를 기준으로 오른쪽으로 회전
          w = x->parent->right; // w를 x의 부모 노드의 오른쪽 자식 노드로 만듦
        }
        w->color = x->parent->color; // w의 색을 x의 부모 노드의 색으로 만듦
        x->parent->color = RBTREE_BLACK; // x의 부모 노드의 색을 검은색으로 만듦
        w->right->color = RBTREE_BLACK; // w의 오른쪽 자식 노드의 색을 검은색으로 만듦
        left_rotate(t, x->parent); // x의 부모 노드를 기준으로 왼쪽으로 회전
        x = t->root; // x를 root 노드로 만듦
      }
    }
    else // x == x->parent->right
    {
      w = x->parent->left; // w는 x의 형제 노드
      if(w->color == RBTREE_RED) // w의 색이 빨간색이면
      {
        w->color = RBTREE_BLACK; // w의 색을 검은색으로 만듦
        x->parent->color = RBTREE_RED; // x의 부모 노드의 색을 빨간색으로 만듦
        right_rotate(t, x->parent); // x의 부모 노드를 기준으로 오른쪽으로 회전
        w = x->parent->left; // w를 x의 부모 노드의 왼쪽 자식 노드로 만듦
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) // w의 오른쪽 자식 노드와 왼쪽 자식 노드의 색이 모두 검은색이면
      {
        w->color = RBTREE_RED; // w의 색을 빨간색으로 만듦
        x = x->parent; // x를 x의 부모 노드로 만듦
      }
      else
      {
        if(w->left->color == RBTREE_BLACK) // w의 왼쪽 자식 노드의 색이 검은색이면
        {
          w->right->color = RBTREE_BLACK; // w의 오른쪽 자식 노드의 색을 검은색으로 만듦
          w->color = RBTREE_RED; // w의 색을 빨간색으로 만듦
          left_rotate(t, w); // w를 기준으로 왼쪽으로 회전
          w = x->parent->left; // w를 x의 부모 노드의 왼쪽 자식 노드로 만듦
        }
        w->color = x->parent->color; // w의 색을 x의 부모 노드의 색으로 만듦
        x->parent->color = RBTREE_BLACK; // x의 부모 노드의 색을 검은색으로 만듦
        w->left->color = RBTREE_BLACK; // w의 왼쪽 자식 노드의 색을 검은색으로 만듦
        right_rotate(t, x->parent); // x의 부모 노드를 기준으로 오른쪽으로 회전
        x = t->root; // x를 root 노드로 만듦
      } 
    }
  }
  x->color = RBTREE_BLACK; // x의 색을 검은색으로 만듦
}

// 트리에서 노드를 삭제하는 함수
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z; // y는 삭제할 노드
  node_t *x; // x는 y의 자식 노드

  color_t y_original_color = y->color; // y의 색을 저장
  if (z->left == t->nil)
  {
    x = z->right; // x는 z의 오른쪽 자식 노드
    rbtree_transplant(t, z, z->right); // z를 z의 오른쪽 자식 노드로 대체
  }
  else if(z->right == t->nil)
  {
    x = z->left; // x는 z의 왼쪽 자식 노드
    rbtree_transplant(t, z, z->left); // z를 z의 왼쪽 자식 노드로 대체
  }
  else //
  {
    y = z->right; // y는 z의 오른쪽 자식 노드
    while(y->left != t->nil) y = y->left; // y를 y의 왼쪽 자식 노드로 업데이트
    y_original_color = y->color; // y의 색을 저장
    x = y->right; // x는 y의 오른쪽 자식 노드
    if(y->parent == z) x->parent = y; // y의 부모 노드가 z이면 x의 부모 노드를 y로 만듦
    else // y->parent != z
    {
      rbtree_transplant(t, y, y->right); // y를 y의 오른쪽 자식 노드로 대체
      y->right = z->right; // y의 오른쪽 자식 노드를 z의 오른쪽 자식 노드로 만듦
      y->right->parent = y; // y의 오른쪽 자식 노드의 부모 노드를 y로 만듦
    }
    rbtree_transplant(t, z, y); // z를 y로 대체
    y->left = z->left; // y의 왼쪽 자식 노드를 z의 왼쪽 자식 노드로 만듦
    y->left->parent = y; // y의 왼쪽 자식 노드의 부모 노드를 y로 만듦
    y->color = z->color; // y의 색을 z의 색으로 만듦
  }
  if(y_original_color == RBTREE_BLACK) rbtree_delete_fixup(t, x); // y의 색이 검은색이면 불균형을 해결

  if(t->root == z) t->root = (y_original_color == RBTREE_BLACK) ? x : y;
  free(z); // z를 해제
  return 0; // 성공적으로 삭제하면 0을 반환
}

// 트리를 중위 순회하는 함수 (재귀적)
int inorder_traversal(const rbtree *t, node_t *x, key_t *arr, int *index, const size_t n) {
  if(x != t->nil) // x가 NIL 노드가 아닌 동안 반복
  {
    if(!inorder_traversal(t, x->left, arr, index, n)) return 0; // 왼쪽 서브트리 방문

    if(*index >= n) return 0; // 배열의 크기를 초과하면 0 반환
    arr[(*index)++] = x->key; // x의 키를 배열에 저장

    if(!inorder_traversal(t, x->right, arr, index, n)) return 0; // 오른쪽 서브트리 방문
  }
  return 1; // 성공적으로 순회하면 1 반환
} 

// 트리를 배열로 변환하는 함수
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) 
{
  int index = 0;
  if(!inorder_traversal(t, t->root, arr, &index, n)) return -1;
  return 0;
}