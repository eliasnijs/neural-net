
internal Value
MSE_1d(Value **out, Value *ground, i32 n_samples, i32 n_outputs, VStack *s)
{
	Value *n = value((f32)n_samples, s);
	Value *total_loss = value(0.0, s);
	for (i32 i = 0; i < n_samples; ++i) {
		Value *loss = vsub(out[i * n_outputs], &ground[i * n_outputs], s);
		Value *loss_sq = vmul(loss, loss, s);
		total_loss = vadd(total_loss, loss_sq, s);
	}
	return vdiv(total_loss, n);
}

