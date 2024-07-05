#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <omp.h>

#include "utils.cpp"
#include "value.cpp"
#include "dataloader.cpp"
#include "net.cpp"
#include "loss.cpp"


i32
main() {
	srand(time(0x0));

	/* Hyperparameters */
	char	*datapath	= "resources/data.mldata";
	i32  	layer_conf[]	= {4, 1};

	f32  	lr		= 0.005f;

	i32	n_epochs	= 10000000;
	f32	loss_threshold	= 0.0001f;

	i32	epochs_per_log	= 500;

	i32     n_main_stack	= 100000;
	i32     n_temp_stack	= 100000;


	VStack main_stack = vstack(n_main_stack);
	VStack temp_stack = vstack(n_temp_stack);

	/* Load the data */
	Dataset data = load_data(datapath, &main_stack);

	/* Initialize a new model */
	Net net = {};
	initialize_net(&net, ArrayCount(layer_conf), layer_conf,
		       data.input_size, &main_stack);

	/* Train the model */
	b32 running = true;
	for (i32 i = 0; i < n_epochs + 1 && running; ++i) {

		Value *os[1000] = {}; /* TODO: Use an arena for the temp stack */

		for (i32 i = 0; i < data.samples; ++i) {
			os[i] = net_forward(&net, &data.xs[i * data.input_size],
					    &temp_stack);
		}

		Value loss = MSE(os, data.ys, data.samples, data.output_size, &temp_stack);

		if (i % epochs_per_log == 0) {
			printf("Epoch %03d: loss: %f\n", i, loss.data);
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

		vstack_reset(&temp_stack);
	}

	vstack_die(&temp_stack);
	vstack_die(&main_stack);
	return 0;
}
