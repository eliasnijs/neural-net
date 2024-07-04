struct Node {
	i32	size;
	Value	*w;
	Value	b;
};

#define MAX_LAYER_SIZE 10
struct Layer {
	i32	size;
	Node	nodes[MAX_LAYER_SIZE];
};

#define MAX_LAYER_COUNT 100
struct Net {
	i32	size;
	Layer	layers[MAX_LAYER_COUNT];
};


////////////////////////////////////////////////////////////////////////////////
//// Initialization

internal void
initialize_node(Node *n, i32 size, VStack *s) {
	n->size = size;
	n->w = vstack_claim(s, size);
	for (i32 i = 0; i < size; ++i) {
		n->w[i] = vrand_uniform(-1.0f, 1.0f);
	}
	n->b = vrand_uniform(-1.0f, 1.0f);
}

internal void
initialize_layer(Layer *l, i32 size, i32 nodesize, VStack *s) {
	l->size = size;
	for (i32 i = 0; i < size; ++i) {
		initialize_node(&l->nodes[i], nodesize, s);
	}
}

internal void
initialize_net(Net *n, i32 size, i32 *layer_sizes, i32 input_size, VStack *s) {
	n->size = size;
	initialize_layer(&n->layers[0], layer_sizes[0], input_size, s);
	for (i32 i = 1; i < size; ++i) {
		initialize_layer(&n->layers[i], layer_sizes[i], layer_sizes[i-1], s);
	}
}

////////////////////////////////////////////////////////////////////////////////
//// Forward pass

internal Value
node_forward(Node *n, Value *xs, VStack *s) {
	Value *sum = value(0.0f, s);
	for (i32 i = 0; i < n->size; ++i) {
		Value *x_ = vmul(&n->w[i], &xs[i], s);
		sum = vadd(sum, x_, s);
	}
	return vtanh(vadd(sum, &n->b, s));
}

internal Value *
layer_forward(Layer *l, Value *input, VStack *s) {
	Value *o = vstack_claim(s, l->size);
	for (i32 i = 0; i < l->size; ++i) {
		o[i] = node_forward(&l->nodes[i], input, s);
	}
	return o;
}

internal Value *
net_forward(Net *n, Value *xs, VStack *s) {
	Value *input = xs;
	for (i32 i = 0; i < n->size; ++i) {
		input = layer_forward(&n->layers[i], input, s);
	}
	return input;
}

////////////////////////////////////////////////////////////////////////////////
//// Backward pass

internal void
backward_(Value *y, i32 step) {
	if (!y || y->op == OP_NONE) {
		return;
	}

	Value *x1 = y->p1;
	Value *x2 = y->p2;

	switch (y->op) {
		case OP_ADD: {
			x1->grad += 1.0f * y->grad;
			x2->grad += 1.0f * y->grad;
		} break;
		case OP_SUB: {
			x1->grad += 1.0f * y->grad;
			x2->grad += -1.0f * y->grad;
		} break;
		case OP_MUL: {
			x1->grad += x2->data * y->grad;
			x2->grad += x1->data * y->grad;
		} break;
		case OP_DIV: {
			x1->grad += 1.0f / x2->data * y->grad;
			x2->grad += (-x1->data / (x2->data * x2->data)) * y->grad;
		} break;
		case OP_TANH: {
			x1->grad += (1.0f - y->data * y->data) * y->grad;
		} break;
		default: {
			printf("[Backward: Error]: Unknown operation\n");
		} break;
	}

	// ask for input

	backward_(x1, step+1);
	backward_(x2, step+1);
}

internal void
backward(Value *y) {
	y->grad = 1.0f;
	backward_(y, 1);
}

internal void
net_get_params(Net *net, VStack *s, Value **params, i32 *n_param) {
	*n_param = 0;
	*params = &s->values[s->count];
	for (i32 i_layer = 0; i_layer < net->size; ++i_layer) {
		Layer *layer = &net->layers[i_layer];
		for (i32 i_node = 0; i_node < layer->size; ++i_node) {
			Node *node = &layer->nodes[i_node];
			*n_param += node->size + 1;
			for (i32 i_w = 0; i_w < node->size; ++i_w) {
				vstack_push(s, node->w[i_w]);
			}
			vstack_push(s, node->b);
		}
	}
	printf("n_param: %d\n", *n_param);
	for (i32 i = 0; i < *n_param; ++i) {
		VPRINT_DEBUG(params[i]);
	}
}

internal void
update_params(Value *params, i32 n_param, f32 lr) {
	for (i32 i = 0; i < n_param; ++i) {
		params[i].data += -lr * params[i].grad;
		params[i].grad = 0.0f;
	}
}

