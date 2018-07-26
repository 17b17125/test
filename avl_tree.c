#include <stdio.h>
#include <stdlib.h>


struct tree {
  struct tree *l;
  int v;
  struct tree *r;
  int h;
};

typedef struct tree tree_t;

int count;

int tree_height(tree_t *t){
  if(t == NULL) return -1;
  if(t->l == NULL && t->r == NULL) return 0;
  if(t->l == NULL) return t->r->h + 1;
  if(t->r == NULL) return t->l->h + 1;
  return t->l->h > t->r->h ? t->l->h + 1 : t->r->h + 1;
}

/*
  2分探索木 l の要素は必ず v より小さい
  2分探索木 r の要素は必ず v より大きい
*/
tree_t *tree_create(tree_t *l, int v, tree_t *r){
  tree_t *t = malloc(sizeof(tree_t));
  count++;
  t->l = l;
  t->r = r;
  t->v = v;
  t->h = tree_height(l) > tree_height(r) ? tree_height(l) + 1 : tree_height(r) + 1;
  return t;
}

void tree_free(tree_t *t){
  if(t != NULL){
    free(t);
    count--;
  }
}

int tree_size(tree_t *t){
  if(t == NULL) return 0;
  return tree_size(t->l) + tree_size(t->r) + 1;
}

void tree_print_elements(tree_t *t){
  if(t == NULL){
  }else{
    if(t->l == NULL){
      printf("%d ", t->v);
      tree_print_elements(t->r);
    }else{
      tree_print_elements(t->l);
      printf("%d ", t->v);
      tree_print_elements(t->r);
    }
  }
}

/*
  tree_height(l) <= tree_height(r) + 2 &&
  tree_height(r) <= tree_height(l) + 2
  が true であると仮定してよい
*/

//発展的課題
tree_t *tree_bal(tree_t *l, int v, tree_t *r) {
  tree_t *ll, *lr, *lrl, *lrr;
  tree_t *rl, *rr, *rll, *rlr;
  int lv, lrv;
  int rv, rlv;

  //if(tree_height(r) <= 1) return tree_create(l, v, r);
  if(l == NULL || r == NULL) return tree_create(l, v, r);
  if(tree_height(l) >= tree_height(r)){
    ll = l->l;
    lr = l->r;
    lv = l->v;
    if(tree_height(ll) >= tree_height(lr)) return tree_create(ll, lv, tree_create(lr, v, r));
    lrl = lr->l;
    lrr = lr->r;
    lrv = lr->v;
    return tree_create(tree_create(ll, lv, tree_create(lrl, lrv, lrr)), v, r);
  }else{  //lの高さがrの高さ未満
    rl = r->l;
    rr = r->r;
    rv = r->v;
    if(tree_height(rl) <= tree_height(rr)) return tree_create(tree_create(l, v, rl), rv, rr);
    rll = rl->l;
    rlr = rl->r;
    rlv = rl->v;
    return tree_create(tree_create(l, v, rll), rlv, tree_create(rlr, rv, rr));
  }
}


int tree_mem(int x, tree_t *t){
  if(t == NULL) return 0;
  if(x == t->v) return 1;
  if(x < t->v){
    if(t->l == NULL) return 0;
    return tree_mem(x, t->l);
  }else{
    if(t->r == NULL) return 0;
    return tree_mem(x, t->r);
  }
}

tree_t *tree_add(int x, tree_t *t) {
  tree_t *tl,*tr;
  int tv;

  if(t == NULL) return tree_bal(NULL, x, NULL);
  if(t->v == x) return t;
  tl = t->l;
  tr = t->r;
  tv = t->v;
  //tree_free(t);
  if(x < tv){
    return tree_bal(tree_add(x, tl), tv, tr);
  }else{
    return tree_bal(tl, tv, tree_add(x, tr));
  }
}

int tree_min(tree_t *t){
  if(t == NULL) return -1;
  if(t->l != NULL) return tree_min(t->l);
  return t->v;
}

int tree_max(tree_t *t){
  if(t == NULL) return -1;
  if(t->r != NULL) return tree_max(t->r);
  return t->v;
}

tree_t *tree_remove_min(tree_t *t){
  tree_t *tl,*tr;
  int tv;

  if(t == NULL) return NULL;
  tl = t->l;
  tr = t->r;
  tv = t->v;
  if(tl != NULL) {
    //tree_free(t);
    return tree_bal(tree_remove_min(tl), tv, tr);
  }else{
    return tr;
  }
}

/*
  2分探索木 t1 の要素は必ず2分探索木 t2 の要素より小さい
*/
tree_t *tree_merge(tree_t *t1, tree_t *t2){
  tree_t *tmp1,*tmp2;

  tmp1 = t1;
  tmp2 = t2;
  //tree_free(t1);
  //tree_free(t2);
  return tree_bal(tmp1, tree_min(tmp2), tree_remove_min(tmp2));
}

tree_t *tree_remove(int x, tree_t *t){
  tree_t *tl, *tr;
  int tv;

  if(t == NULL) return NULL;
  tl = t->l;
  tr = t->r;
  tv = t->v;
  //tree_free(t);
  if(x == tv) {
    if(tr != NULL) return tree_bal(tl, tree_min(tr), tree_remove_min(tr));
    return tl;
  }
  if(x < tv) return tree_bal(tree_remove(x, tl), tv, tr);
  return tree_bal(tl, tv, tree_remove(x, tr));
}


//平衡性のチェック(発展的課題)
int tree_check(tree_t *t){
  if(t == NULL) return 1;
  return tree_height(t->l) <= tree_height(t->r)+1 &&
  tree_height(t->r) <= tree_height(t->l)+1 && tree_check(t->l) && tree_check(t->r);
}

void tree_tree(tree_t *t){
  if(t == NULL) return;
  if(t->l != NULL) tree_tree(t->l);
  else printf("(noleft) ");
  printf("%d ", t->v);
  if(t->r != NULL) tree_tree(t->r);
  else printf("(noright) ");
}

int main(){
  int i;
  tree_t *t = NULL, *test = NULL;
  int err = 0;
  count = 0;

  /*for(i = 0; i < 7919; i++){
    t = tree_add(i*4000 % 7919, t);
  }*/

  //tree_print_elements(t);

//勝手に追加した tree_remove_minのチェック
  /*for(i = 0; i < 7919; i++){
    if(tree_min(t) != i) printf("%d vs %d\n", i, tree_min(t));
    t = tree_remove_min(t);
  }
  printf("removed_mins: size = %d\n", tree_size(t));
  */

// サイズが7919かチェック
  printf("size = %d\n", tree_size(t));
  for(i = 0; i < 7919; i++){
    t = tree_remove(i*5000 % 7919, t);
  }

  tree_print_elements(t);
// サイズが0かチェック
  printf("size = %d\n", tree_size(t));


//  平衡性のチェック
  for(i = 0; i < 100000; i++) {
    int rnd = rand()%1000;
    if(rand() % 2)
      t = tree_remove(rnd, t);
    else
      t = tree_add(rnd, t);
    if(!tree_check(t)){
      if(i <= 20) printf("%d\n", i);
      if(i == 3){
        tree_tree(t);
        printf("\nt->v %d\nt->r->v %d\nt->r->l->v %d\nmin. %d\nmax. %d\n", t->v, t->r->v, t->r->l->v, tree_min(t), tree_max(t));
        test = tree_create(tree_create(t->l, t->v, t->r->l), t->r->v, t->r->r);
        tree_tree(test);
        printf("\ntst->v %d\ntst->l->v %d\ntst->l->r->v %d\nmin. %d\nmax. %d\n", test->v, test->l->v, test->l->r->v, tree_min(t), tree_max(t));
      }
      if(i == 4){
        tree_tree(t);
        printf("\nrnd %d\n", rnd);
      }
      err++;
      //puts("ERROR");
    }
  }
  printf("Error: %d\n", err);

//  確保しているノードの領域の数と木のサイズが一致するかチェック
  //printf("count = %d\n", count);
  tree_print_elements(t);
  printf("size = %d\n", tree_size(t));


  return 0;
}
