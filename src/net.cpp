struct Node {
	I32	size;
	Value	*w;
	Value	b;
};

#define MAX_LAYER_SIZE 10
struct Layer {
	I32	size;
	Node	nodes[MAX_LAYER_SIZE];
};

#define MAX_LAYER_COUNT 100
struct Net {
	I32	size;
	Layer	layers[MAX_LAYER_COUNT];
};


////////////////////////////////////////////////////////////////////////////////
//// Initialization

internal void
initialize_node(Node *n, I32 size, Arena *a) {
	n->size = size;
	n->w = (Value *)arena_alloc(a, size * sizeof(Value));
	for (I32 i = 0; i < size; ++i) {
		n->w[i] = vrand_uniform(-1.0f, 1.0f);
	}
	n->b = vrand_uniform(-1.0f, 1.0f);
}

internal void
initialize_layer(Layer *l, I32 size, I32 nodesize, Arena *a) {
	l->size = size;
	for (I32 i = 0; i < size; ++i) {
		initialize_node(&l->nodes[i], nodesize, a);
	}
}

internal void
initialize_net(Net *n, I32 size, I32 *layer_sizes, I32 input_size, Arena *a) {
	n->size = size;
	initialize_layer(&n->layers[0], layer_sizes[0], input_size, a);
	for (I32 i = 1; i < size; ++i) {
		initialize_layer(&n->layers[i], layer_sizes[i], layer_sizes[i-1], a);
	}
}

////////////////////////////////////////////////////////////////////////////////
//// Forward pass

internal Value
node_forward(Node *n, Value *xs, Arena *a) {
	Value *sum = value(0.0f, a);
	for (I32 i = 0; i < n->size; ++i) {
		Value *x_ = vmul(&n->w[i], &xs[i], a);
		sum = vadd(sum, x_, a);
	}
	return vtanh(vadd(sum, &n->b, a));
}

internal Value *
layer_forward(Layer *l, Value *input, Arena *a) {
	Value *o = (Value *)arena_alloc(a, l->size * sizeof(Value));
	for (I32 i = 0; i < l->size; ++i) {
		o[i] = node_forward(&l->nodes[i], input, a);
	}
	return o;
}

internal Value *
net_forward(Net *n, Value *xs, Arena *a) {
	Value *input = xs;
	for (I32 i = 0; i < n->size; ++i) {
		input = layer_forward(&n->layers[i], input, a);
	}
	return input;
}

////////////////////////////////////////////////////////////////////////////////
//// Backward pass

internal void
backward_(Value *y) {
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

	backward_(x1);
	backward_(x2);
}

internal void
backward(Value *y) {
	y->grad = 1.0f;
	backward_(y);
}

internal void
net_update_params(Net *net, F32 lr) {
	for (I32 i_layer = 0; i_layer < net->size; ++i_layer) {
		Layer *layer = &net->layers[i_layer];
		for (I32 i_node = 0; i_node < layer->size; ++i_node) {
			Node *node = &layer->nodes[i_node];
			for (I32 i_w = 0; i_w < node->size; ++i_w) {
				node->w[i_w].data += -lr * node->w[i_w].grad;
				node->w[i_w].grad = 0.0f;
			}
			node->b.data += -lr * node->b.grad;
			node->b.grad = 0.0f;
		}
	}
}

