# ifndef __mul__h
# define __mul__h
# include <mdlint.h>
# define incrp(__mul) __mul->cur++
typedef struct bucket {
	mdl_u8_t sort;
	mdl_u8_t val;
	mdl_u8_t *beg, *end;
	char *p;
	mdl_u16_t len;
	struct bucket *next;
} *bucketp;

typedef struct entry {
	struct entry *next, *fd;
	mdl_u8_t const* key;
	mdl_uint_t len;
	void *p;
} *entryp;

typedef struct hash {
	entryp *table;
	entryp head;
} *hashp;

typedef struct brick {
	struct brick *next;
	mdl_u8_t *p, *end;

} *brickp;

enum {
	_quote,
	_comma
};

enum {
	_unknown,
	_ident,
	_keywd,
	_chr,
	_str,
	_brick
};

typedef struct mul {
	mdl_u8_t *p, *end;
	mdl_u8_t *cur;
	brickp top;
	struct hash bricks;
} *mulp;

mdl_u8_t at_eof(mulp);

void to_free(void*);

void hash_init(hashp);
void hash_put(hashp, mdl_u8_t const*, mdl_uint_t, void*);
void* hash_get(hashp, mdl_u8_t const*, mdl_uint_t);
void hash_free(hashp);

void lex(mulp, bucketp*);

void mul_prepare(mulp);
void mul_ld(mulp, char const*);
void mul_process(mulp);

brickp get_brick(mulp, char const*);
mdl_uint_t mul_bricklen(brickp);
void mul_brickw(void*, mdl_uint_t, mdl_uint_t, brickp);
void mul_brickr(void*, mdl_uint_t, mdl_uint_t, brickp);

void mul_oust(mulp, char const*);

void mul_cleanup(mulp);
void lexer_cleanup();
# endif /*__mul__h*/
