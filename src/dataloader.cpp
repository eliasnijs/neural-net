struct Dataset {
	I32 n_samples;
	I32 n_in;
	I32 n_out;
	Value *xs;
	Value *ys;
	B32 success;
};

internal Dataset
load_data(char *path, Arena *a) {
	Dataset dataset = {};

	FILE *file = fopen(path, "r");
	if (!file) {
		printf("error: could not open file %s\n", path);
		exit(1);
	}
	I32 l_file = 0;
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
		return dataset;
	}

	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "n_samples=%d\n", &dataset.n_samples);
	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "n_input=%d\n", &dataset.n_in);
	ptr = strchr(ptr, '\n') + 1;
	sscanf(ptr, "n_output=%d\n", &dataset.n_out);


	Value *xs = (Value *)arena_alloc(a, dataset.n_in * dataset.n_samples * sizeof(Value));
	Value *ys = (Value *)arena_alloc(a, dataset.n_out * dataset.n_samples * sizeof(Value));
	dataset.ys = ys;
	dataset.xs = xs;

	ptr = strstr(buffer, "[data]");
	if (!ptr) {
		printf("error: data not found\n");
		return dataset;
	}

	F32 value_data = MAX_F32;
	for (I32 i = 0; i < dataset.n_samples; ++i) {
		ptr = strchr(ptr, '\n');
		for (I32 j = 0; j < dataset.n_in; ++j) {
			sscanf(ptr, "%f", &value_data);
			xs[i*dataset.n_in + j] = value(value_data);
			ptr = strchr(ptr, ',') + 1;
		}
		for (I32 j = 0; j < dataset.n_out; ++j) {
			sscanf(ptr, "%f", &value_data);
			ys[i * dataset.n_out + j] = value(value_data);
			if (j < dataset.n_out - 1) {
				ptr = strchr(ptr, ',') + 1;
			}
		}
	}

	dataset.success = true;
	free(buffer);
	return dataset;
}
