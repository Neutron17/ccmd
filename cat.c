#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <strings.h>

int main(int argc, char *argv[]) {
	if(argc <= 1) {
		fprintf(stderr, "ERROR: Not enough arguments for '%s'\n", argv[0]);
		return -1;
	} else if(argc > 3) {
		fprintf(stderr, "ERROR: Too much arguments for '%s'\n", argv[0]);
		return -1;
	}
	int shift = 1;
	int isNum = 0;
	if(argc >= 1) {
		if(argv[1][0] == '-') {
			if(strncasecmp(argv[1], "-n", 2) != 0) {
				fprintf(stderr, "ERROR: unknown option: '%s'\n", argv[1]);
				return -2;
			}
			isNum = 1;
			shift++;
		}
		if(argc == 3) {
			if(argv[2][0] == '-') {
				if(strncasecmp(argv[2], "-n", 2) != 0) {
					fprintf(stderr, "ERROR: unknown option: '%s'\n", argv[1]);
					return -2;
				}
				isNum = 1;
			}
		}
	}
	FILE *file = fopen(argv[shift], "r");
	if(!file) {
		fprintf(stderr, "ERROR: Couldn't open file: '%s', '%s'\n", argv[shift], strerror(errno));
		return -3;
	}
	char line[248];
	int i = 0;
	while((fgets(line, 248, file)) != NULL) {
		i++;
		if(isNum)
			printf("    %d ", i);
		printf("%s", line);
	}
	fclose(file);
	return 0;
}
