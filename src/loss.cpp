
internal Value
MSE(Value **out, Value *ground, i32 n_samples, i32 n_outputs, VStack *s)
{
	Value *total_loss = value(0.0, s);
	for (i32 i = 0; i < n_samples; ++i) {
		for (i32 j = 0; j < n_outputs; ++j) {
			Value *loss = vsub(&out[i][j], &ground[i * n_outputs + j], s);
			Value *loss_sq = vmul(loss, loss, s);
			total_loss = vadd(total_loss, loss_sq, s);
		}
	}
	return vdiv(total_loss, value((f32)(n_samples * n_outputs), s));
}

