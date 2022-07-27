#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*
 * TODO
 * make highlight work for other files from $LS_COLORS
 * make highlight work for executables
 * other options
*/

int main(int argc, char *argv[]) {
	if(argc <= 1) {
		fprintf(stderr, "ERROR: Not enough arguments for '%s'\n", argv[0]);
		return -1;
	} else if(argc > 3) {
		fprintf(stderr, "ERROR: Too much arguments for '%s'\n", argv[0]);
		return -1;
	}
	int shift = 1;
	int printAll = 0;

	if(*argv[1] == '-') {
		if(strncmp(argv[1], "-a", 2) != 0) {
			fprintf(stderr, "ERROR: Unknown option: '%s', '%s'\n", argv[1], strerror(errno));
			return -2;
		}
		shift++;
	}
	if(argc == 3) {
		if(*argv[2] == '-') {
			if(strncmp(argv[2], "-a", 2) != 0) {
				fprintf(stderr, "ERROR: Unknown option: '%s', '%s'\n", argv[2], strerror(errno));
				return -2;
			}
			printAll = 1;
		}
	}

	DIR *dp;
	struct dirent *ep;

	dp = opendir(argv[shift]);
	if(!dp) {
		fprintf(stderr, "ERROR: Couldn't open directory: '%s', '%s'\n", argv[shift], strerror(errno));
		return -1;
	}
	char di[6] = "01;34";
	char ln[6] = "01;36";
	char *LS_COLORS = getenv("LS_COLORS");
	char *col;
	for(col = strtok(LS_COLORS, ":");
	    col != NULL;
	    col = strtok(NULL, ":")) {
		if(strncmp(col, "di=", 3) == 0) {
			col += 3;
			strncpy(di, col, 6);
		} else if(strncmp(col, "ln=", 3) == 0) {
			col += 3;
			strncpy(ln, col, 6);
		}
	}
	while((ep = readdir(dp))) {
		if(!printAll && ep->d_name[0] == '.')
			continue;
		if((ep->d_type) & DT_DIR)
			printf("\033[%sm", di);
		//else if((ep->d_type) & DT_LNK)
		//	printf("\033[%sm", ln);
		else
			printf("\033[0m");

		printf("%s\033[m ", ep->d_name);
	}
	puts("");
	closedir(dp);
	return 0;
}
