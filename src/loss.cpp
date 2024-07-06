
internal Value
MSE(Value **out, Value *ground, I32 n_samples, I32 n_outputs, Arena *a)
{
	Value *total_loss = value(0.0, a);
	for (I32 i = 0; i < n_samples; ++i) {
		for (I32 j = 0; j < n_outputs; ++j) {
			Value *loss = vsub(&out[i][j], &ground[i * n_outputs + j], a);
			Value *loss_sq = vmul(loss, loss, a);
			total_loss = vadd(total_loss, loss_sq, a);
		}
	}
	return vdiv(total_loss, value((F32)(n_samples * n_outputs), a));
}

