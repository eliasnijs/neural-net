#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <omp.h>


#define ENABLE_PRINT  1
#define ENABLE_ASSERT 1
#include "utils/utils.h"

#include "value.cpp"
#include "dataloader.cpp"
#include "net.cpp"
#include "loss.cpp"

I32
main() {
	srand(time(0x0));

	/* Hyperparameters */
	char	*path		= "resources/ionosphere.mld";


	I32  	layer_conf[]	= {2, 2, 2, 1};

	F32  	lr		= 0.00001f;

	I32	n_epochs	= 40000000;
	F32	loss_threshold	= 0.01f;

	I32	epochs_per_log	= 1;

	I32     n_main_arena	= Megabytes(1);
	I32     n_temp_arena	= Megabytes(10);





	/* Initialize memory arenas */
	Arena main_arena, temp_arena;
	arena_init(&main_arena, (U8 *)malloc(n_main_arena), n_main_arena);
	arena_init(&temp_arena, (U8 *)malloc(n_temp_arena), n_temp_arena);

	/* Initialize the dataset */
	Dataset data = load_data(path, &main_arena);
	if (!data.success) {
		print_error("error: could not load dataset");
		free(main_arena.buf);
		free(temp_arena.buf);
		return 1;
	}

	/* Initialize a new model */
	Net net = {};
	initialize_net(&net, ArrayCount(layer_conf), layer_conf, data.n_in,
		       &main_arena);

	/* Train the model */
	B32 running = true;
	for (I32 i = 0; i < n_epochs + 1 && running; ++i) {
		Value **os = (Value **)arena_alloc(
		    &temp_arena, data.n_samples*sizeof(Value *));

		for (I32 i = 0; i < data.n_samples; ++i) {
			os[i] = net_forward(&net, &data.xs[i * data.n_in],
					    &temp_arena);
		}

		Value loss = MSE(os, data.ys, data.n_samples, data.n_out,
				 &temp_arena);

		if (i % epochs_per_log == 0) {
			print("epoch %6d: loss: %f", i, loss.data);
		}


		if (loss.data < loss_threshold) {
			running = false;
			print("--------------------");
			print("Reached loss treshold");
			print("Final loss: %f in %d epochs",
				    loss.data, i);
		} else if (i == n_epochs) {
			running = false;
			print("--------------------");
			print("Reached max epochs");
			print("Final loss: %f in %d epochs", loss.data, i);
		} else {
			backward(&loss);
			net_update_params(&net, lr);
		}

		arena_reset(&temp_arena);
	}

	free(main_arena.buf);
	free(temp_arena.buf);
	return 0;
}
