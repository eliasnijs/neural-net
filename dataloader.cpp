/*
we  are building a parser for the following data format "data.mldata", which
looks like:
```
[metadata]
samples=4
input_size=3
output_size=1

[xs]
2.0,  3.0, -1.0
3.0, -1.0,  0.5
0.5,  1.0,  1.0
1.0,  1.0, -1.0

[ys]
1.0
-1.0
-1.0
1.0
```
*/

struct Dataset {
	i32 samples;
	i32 input_size;
	i32 output_size;
	Value *xs;
	Value *ys;
	b32 success;
};

internal Dataset
load_data(char *path, VStack *s) {
	Dataset result = {};

	FILE *file = fopen(path, "r");
	if (!file) {
		printf("error: could not open file %s\n", path);
		exit(1);
	}
	i32 l_file = 0;
	fseek(file, 0, SEEK_END);
	l_file = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *)malloc(l_file);
	fread(buffer, 1, l_file, file);
	fclose(file);

	char *ptr = buffer;

	ptr = strstr(buffer, "[metadata]");
	if (!ptr) {
		printf("error: metadata not found\n");
		result.success = false;
		return result;
	}

	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "input_size=%d\n", &result.samples);
	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "input_size=%d\n", &result.input_size);
	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "output_size=%d\n", &result.output_size);


	ptr = strstr(buffer, "[xs]");
	if (!ptr) {
		printf("error: metadata not found\n");
		result.success = false;
		return result;
	}

	Value *xs = vstack_claim(s, result.input_size * result.samples);
	result.xs = xs;

	for (i32 i = 0; i < result.samples; ++i) {
		ptr = strchr(ptr, '\n') + 1;
		for (i32 j = 0; j < result.input_size; ++j) {
			f32 value_data = MAX_F32;
			sscanf(ptr, "%f", &value_data);
			xs[i * result.input_size + j] = value(value_data);
			if (j < result.input_size - 1) {
				ptr = strchr(ptr, ',') + 1;
			}
		}
	}

	ptr = strstr(buffer, "[ys]");
	if (!ptr) {
		printf("error: metadata not found\n");
		result.success = false;
		return result;
	}

	Value *ys = vstack_claim(s, result.output_size * result.samples);
	result.ys = ys;

	for (i32 i = 0; i < result.samples; ++i) {
		ptr = strchr(ptr, '\n') + 1;
		f32 value_data = MAX_F32;
		sscanf(ptr, "%f", &value_data);
		ys[i] = value(value_data);
	}


	free(buffer);

	return result;
}
