# include "mul.h"
# include <string.h>
# include <malloc.h>
void* memdup(void const *__p, mdl_uint_t __bc) {
	mdl_u8_t *p = (mdl_u8_t*)malloc(__bc);
	mdl_u8_t *cur = p, *end = p+__bc;
	while(cur != end) {
		*cur = *((mdl_u8_t*)__p+(cur-p));
		cur++;
	}
	return p;
}

mdl_u64_t sum(mdl_u8_t const *__key, mdl_uint_t __len) {
	mdl_u8_t const *end = __key+__len;
	mdl_u8_t const *p = __key;
	mdl_u64_t ret = ~(mdl_u64_t)0;
	while(p != end) {
		ret ^= *p;
		ret = ret<<4|ret>>60;
		p++;
	}
	return ret;
}

void hash_init(hashp __table) {
	__table->table = (entryp*)malloc(0xff*sizeof(entryp));
	entryp *cur = __table->table;
	entryp *end = cur+0xff;
	while(cur != end) *(cur++) = NULL;
	__table->head = NULL;
}

void hash_put(hashp __table, mdl_u8_t const *__key, mdl_uint_t __len, void *__p) {
	mdl_u64_t val = sum(__key, __len);
	entryp *table = __table->table+(val&0xff);

	entryp e = (entryp)malloc(sizeof(struct entry));
	e->next = *table;
	*table = e;

	e->fd = __table->head;
	__table->head = e;

	e->key = (mdl_u8_t const*)memdup(__key, __len);
	e->len = __len;
	e->p = __p;
}

void* hash_get(hashp __table, mdl_u8_t const *__key, mdl_uint_t __len) {
	mdl_u64_t val = sum(__key, __len);
	entryp p;
	if (!(p = *(__table->table+(val&0xff)))) {
		return NULL;
	}

	while(p != NULL) {
		if (p->len == __len)
			if (!memcmp(p->key, __key, __len)) return p->p;
		p = p->next;
	}
	fprintf(stderr, "lookup failed.\n");
	return NULL;
}

void hash_free(hashp __table) {
	return;
	free(__table->table);

	entryp cur = __table->head;
	while(cur != NULL) {
		entryp bk = cur;
		cur = cur->fd;
		free((void*)bk->key);
		free(bk);
	}
}
