#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#include "utils.cpp"
#include "value.cpp"
#include "dataloader.cpp"
#include "net.cpp"
#include "loss.cpp"


i32
main() {
	srand(time(0x0));

	/* Hyperparameters */
	char	*datapath	= "data.mldata";
	i32  	layer_conf[]	= {2};
	f32  	lr		= 0.5f;
	i32	n_epochs	= 10;

	i32     n_main_stack	= 10000;
	i32     n_temp_stack	= 10000;


	VStack main_stack = vstack(n_main_stack);

	/* Load the data */
	Dataset data = load_data(datapath, &main_stack);

	/* Initialize a new model */
	Net net = {};
	initialize_net(&net, ArrayCount(layer_conf), layer_conf,
		       data.input_size, &main_stack);

	/* Train the model */
	printf("\n");
	printf("Training:\n");
	for (i32 i = 0; i < n_epochs; ++i) {
		VStack temp_stack = vstack(n_temp_stack);

		Value *os[1000] = {}; /* TODO: Use an arena for the temp stack */
		for (i32 i = 0; i < data.samples; ++i) {
			os[i] = net_forward(&net, &data.xs[i * data.input_size],
					    &temp_stack);
		}

		Value loss = MSE_1d(os, data.ys, data.samples, data.output_size,
				    &temp_stack);

		printf("%05d loss: %9f\n", i + 1, loss.data);

		backward(&loss);

		Value	*params  = 0x0;
		i32	n_params = 0;
		net_get_params(&net, &temp_stack, &params, &n_params);
		for (i32 i = 0; i < n_params; ++i) {
			VPRINT_DEBUG(params + i*data.input_size);
		}

		update_params(params, n_params, lr);

		vstack_die(&temp_stack);
	}


	vstack_die(&main_stack);
	return 0;
}
