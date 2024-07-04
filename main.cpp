#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#include "utils.cpp"
#include "value.cpp"
#include "net.cpp"
#include "loss.cpp"
#include "dataloader.cpp"


i32
main() {
	srand(time(0x0));

	char *datapath		= "data.mldata";
	i32  layer_conf[]	= {2};
	f32  lr			= 0.05f;
	i32  n_epochs		= 100;


	VStack input_stack = vstack(1000);
	Dataset data = load_data(datapath, &input_stack);

	VStack param_stack = vstack(1000);
	Net net = {};
	initialize_net(&net, ArrayCount(layer_conf), layer_conf,
		       data.input_size, &param_stack);

	Value n = value(data.samples);
	for (i32 i = 0; i < n_epochs; ++i) {
		VStack inter_stack = vstack(10000);

		Value loss = mean_squared_error(&net, &n, data.xs, data.ys,
						&inter_stack);

		printf("%3d loss: %9f\n", i, loss.data);

		backward(&loss);

		Value *params = 0x0;
		i32 n_params = 0;

		net_get_params(&net, &inter_stack, &params, &n_params);
		update_params(params, n_params, lr);

		vstack_die(&inter_stack);
	}


	vstack_die(&param_stack);
	return 0;
}
