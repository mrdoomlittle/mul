# include "mul.h"
# include <malloc.h>
# include <string.h>
# define is_no(__c) (__c >= '0' && __c <= '9')
# define is_ident(__c) (__c >= 'a' && __c <= 'z')
char fetchc(mulp __mul) {
	return *__mul->cur;
}

mdl_u8_t is_space(char __c) {
	return (__c == ' ' || __c == '\t' || __c == '\n');
}

char* read_ident(mulp __mul, mdl_u16_t *__len) {
	char buf[1024];
	char *bufp = buf;
	while((fetchc(__mul) >= 'a' && fetchc(__mul) <= 'z') || fetchc(__mul) == '_') {
		*(bufp++) = fetchc(__mul);
		incrp(__mul);
	}
	*bufp = '\0';
	mdl_u16_t len;
	char *p = (char*)malloc((len = (bufp-buf))+1);
	to_free(p);
	memcpy(p, buf, len+1);
	*__len = len;
	return p;
}

# define BACK 20
bucketp static head = NULL;
bucketp static back = NULL;
mdl_uint_t static len = 0;

void lex(mulp __mul, bucketp *__tok) {
	if (len >= BACK) {
		len--;
		bucketp bk = head;
		head = head->next;
		free(bk);
	}

	if (at_eof(__mul)) {

		return;
	}

	while(is_space(fetchc(__mul)) && !at_eof(__mul))
		incrp(__mul);

	bucketp tok;
	
	if ((tok = (bucketp)malloc(sizeof(struct bucket))) == NULL) {
		// fault
	}

	tok->next = NULL;
	if (!head)
		head = tok;
	
	if (back != NULL)
		back->next = tok;
	back = tok;

	tok->beg = __mul->cur;

	char c = fetchc(__mul);
	if (is_ident(c)) {
		tok->sort = _ident;
		tok->p = read_ident(__mul, &tok->len);
	} else {
		if (c == 0x27) {
			incrp(__mul);
			while(fetchc(__mul) != 0x27)
				incrp(__mul);
			incrp(__mul);
			tok->sort = _brick;
		} else {
			switch(c) {
				case ',':
					tok->sort = _chr;
					tok->val = _comma;
				break;
				default:
					tok->sort = _unknown;
			}
			incrp(__mul);
		}
	}
	len++;

	tok->end = __mul->cur;
	*__tok = tok;	
}

void lexer_cleanup() {
	bucketp cur = head;
	while(cur != NULL) {
		bucketp bk = cur;
		cur = cur->next;
		free(bk);
	}
}
