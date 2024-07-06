#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <omp.h>

#include "utils.cpp"
#include "arena.cpp"

#include "value.cpp"
#include "dataloader.cpp"
#include "net.cpp"
#include "loss.cpp"


I32
main() {
	srand(time(0x0));

	/* Hyperparameters */
	char	*datapath	= "resources/data.mldata";
	I32  	layer_conf[]	= {4, 2};

	F32  	lr		= 0.05f;

	I32	n_epochs	= 40;
	F32	loss_threshold	= 0.0001f;

	I32	epochs_per_log	= 5;

	I32     n_main_arena	= 100000;
	I32     n_temp_arena	= 100000;


	/* Initialize memory arenas */
	Arena main_arena, inter_arena;
	arena_init(&main_arena, (U8 *)malloc(n_main_arena), n_main_arena);
	arena_init(&inter_arena, (U8 *)malloc(n_temp_arena), n_temp_arena);


	/* Load the data */
	Dataset data = load_data(datapath, &main_arena);
	if (!data.success) {
		printf("error: failed to load dataset\n");
		free(main_arena.buf);
		free(inter_arena.buf);
		return 1;
	}

	/* Initialize a new model */
	Net net = {};
	initialize_net(&net, ArrayCount(layer_conf), layer_conf, data.n_in,
		       &main_arena);

	/* Train the model */
	B32 running = true;
	for (I32 i = 0; i < n_epochs + 1 && running; ++i) {
		Value **os = (Value **)arena_alloc(&inter_arena, data.n_samples * sizeof(Value *));

		for (I32 i = 0; i < data.n_samples; ++i) {
			os[i] = net_forward(&net, &data.xs[i * data.n_in],
					    &inter_arena);
		}

		Value loss = MSE(os, data.ys, data.n_samples, data.n_out,
				 &inter_arena);

		if (i % epochs_per_log == 0) {
			printf("Epoch %6d: loss: %f\n", i, loss.data);
		}


		if (loss.data < loss_threshold) {
			running = false;
			printf("--------------------\n");
			printf("Reached loss treshold\n");
			printf("Final loss: %f in %d epochs\n",
			       loss.data, i);
		} else if (i == n_epochs) {
			running = false;
			printf("--------------------\n");
			printf("Reached max epochs\n");
			printf("Final loss: %f in %d epochs\n",
			       loss.data, i);
		} else {
			backward(&loss);
			net_update_params(&net, lr);
		}

		arena_reset(&inter_arena);
	}

	free(main_arena.buf);
	free(inter_arena.buf);
	return 0;
}
