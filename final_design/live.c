
#include "live.h"

int in_data[1024], out_data[1024];

void live_var_analysis() {
	int i, j;
	devide_bb();
	memset(in_data, 0, 1024 * sizeof(int));
	for (i = 0; i < in_bb_top; i++)
		if (bb_map[in_bb_top][i]) {
			printf("entry->%d\n", i);
			break;
		}
	for (i = 0; i < in_bb_top; i++)
		for (j = 0; j < in_bb_top; j++)
			if (bb_map[i][j])
				printf("%d->%d\n", i, j);
	for (i = 0; i < in_bb_top; i++)
		if (bb_map[i][in_bb_top + 1])
			printf("%d->exit\n", i);
}