
internal Value *
mean_squared_error(Net *net, Value *n, Value **xs, Value *ys, VStack *s) {
	Value *total_loss = value(0.0, s);
	for (i32 i = 0; i < n->data; ++i) {
		Value *o = net_forward(net, xs[i], s);

		Value *loss = vsub(o, &ys[i], s);
		loss = vmul(loss, loss, s);

		total_loss = vadd(total_loss, loss, s);
	}
	return vdiv(total_loss, n, s);
}

