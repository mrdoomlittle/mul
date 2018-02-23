# include "mul.h"
# include <stdio.h>
# include <string.h>
int main(int __argc, char const *__argv[]) {
	char const *src = NULL, *dst = NULL;

	char const **arg = __argv;
	char const **end = arg+__argc;
	arg++; // ignore
	while(arg != end) {
		if (!strcmp(*arg, "-d"))
			dst = *(++arg);
		else if (!strcmp(*arg, "-s"))
			src = *(++arg);
		arg++;
	}

	if (!src || !dst) {
		fprintf(stderr, "usage: -s source -d destination\n");
		return -1;
	}

	struct mul m;
	mul_prepare(&m);
	mul_ld(&m, src);
	mul_process(&m);

	brickp a = get_brick(&m, "d");
	char r = '2';
	mul_brickw(&r, 4, 1, a);

	mul_oust(&m, dst);
	mul_cleanup(&m);
	return 0;
}
