#include <stdio.h>
#include <time.h>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#include "utils.cpp"
#include "value.cpp"
#include "net.cpp"



i32
main() {
	srand(time(0x0));

	/* data */
	Value n = value(4);
	Value xs[][3] = {
		{value(2.0), value( 3.0), value(-1.0)},
		{value(3.0), value(-1.0), value( 0.5)},
		{value(0.5), value( 1.0), value( 1.0)},
		{value(1.0), value( 1.0), value(-1.0)},
	};

	Value ys_ground[4] = {
		value( 1.0),
		value(-1.0),
		value(-1.0),
		value( 1.0)
	};

	/* feature selection */
	/* None */

	/* feature scaling */
	/* None */

	/* initialize net */
	VStack param_stack = vstack(1000);

	Net net = {};
	i32 layer_conf[] = {2};
	initialize_net(&net, 1, layer_conf, 3, &param_stack);


	f32 lr = 0.05f;

	VStack inter_stack = vstack(10000);

	Value *total_loss = value(0.0, &inter_stack);
	for (i32 i = 0; i < n.data; ++i) {
		Value *input = xs[i];
		Value *ground = &ys_ground[i];

		Value *o = net_forward(&net, input, &inter_stack);
		printf("output: %9.4f ", o->data);

		/* using quadratic loss */
		Value *loss = vsub(o, ground, &inter_stack);
		loss = vmul(loss, loss, &inter_stack);
		printf(">>> loss: %9.4f\n", loss->data);

		total_loss = vadd(total_loss, loss, &inter_stack);
	}

	/* average quadratic loss */
	Value avg_loss = vdiv(total_loss, &n);
	printf("avg loss: %9.4f\n", avg_loss.data);

	backward(&avg_loss);

	/*Value *params = 0x0;*/
	/*i32 n_params = 0;*/
	/*net_get_params(&net, &inter_stack, &params, &n_params);*/
	/*update_params(params, n_params, lr);*/

	vstack_die(&inter_stack);

	vstack_die(&param_stack);
	return 0;
}
