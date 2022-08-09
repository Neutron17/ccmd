#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>

/*
 * TODO
 * make highlight work for other files from $LS_COLORS
 * other options (-l)
*/

#define ESC "\033[%sm"
#define C_SZ 3

enum ColType {
	C_EX = 0,
	C_DIR,
	C_LN
};
char colors[C_SZ][6] = {
	"01;32","01;34","01;36"
};
const char cident[C_SZ][4] = {
	"ex=","di=","ln="
};

int main(int argc, char *argv[]) {
	int shift = 1;
	int printAll = 0;
	DIR *dp;
	struct dirent *ep;
	if(argc <= 1) {
		goto def;
	} else if(argc > 3) {
		fprintf(stderr, "ERROR: Too much arguments for '%s'\n", argv[0]);
		return -1;
	}

	if(*argv[1] == '-') {
		if(strncmp(argv[1], "-a", 2) != 0) {
			fprintf(stderr, "ERROR: Unknown option: '%s', '%s'\n", argv[1], strerror(errno));
			return -2;
		} else {
			printAll = 1;
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


	if(argv[shift] == NULL)
		goto def;
	dp = opendir(argv[shift]);
	chdir(argv[shift]);
	goto aft;
def:
	dp = opendir(".");
aft:
	if(!dp) {
		if(errno == 20) {
			printf("%s\n", argv[shift]);
			return 0;
		}
		fprintf(stderr, "ERROR: Couldn't open directory: '%s', '%s'\n", argv[shift], strerror(errno));
		return -1;
	}
	char *LS_COLORS = getenv("LS_COLORS");
	for(char *col = strtok(LS_COLORS, ":");
	    col != NULL;
	    col = strtok(NULL, ":")) {
		for(int i = 0; i < C_SZ; i++) {
			if(strncmp(col, cident[i], 3) == 0) {
				col += 3;
				strncpy(colors[i], col, 6);
			}
		}
	}

	struct stat stats;
	while((ep = readdir(dp))) {
		if(!printAll && ep->d_name[0] == '.')
			continue;
		stat(ep->d_name, &stats);
		if(S_ISDIR(stats.st_mode)) {
			printf(ESC, colors[C_DIR]);
		} else if((stats.st_mode) & X_OK) {
			printf(ESC, colors[C_EX]);
		} else if(S_ISLNK(stats.st_mode)) {
			printf(ESC, colors[C_LN]);
		} else {
			printf(ESC, "0");
		}

		printf("%s\033[m ", ep->d_name);
	}
	puts("");
	closedir(dp);
	return 0;
}

