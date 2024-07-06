enum Operand {OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_TANH, OP_COUNT};
global_variable const char *OP_REPR_TABLE[] = {
	"NONE", "ADD", "SUB", "MUL", "DIV", "TANH", "COUNT"
};

struct Value {
	F32 data;
	F32 grad;

	Operand op;
	Value *p1;
	Value *p2;
};

internal Value
value(F32 value) {
	return {value, 0.0f, OP_NONE, 0x0, 0x0};
}

internal Value *
value(F32 value, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	*v = {value, 0.0f, OP_NONE, 0x0, 0x0};
	return v;

}

internal Value
vadd(Value *a1, Value *a2) {
	return {a1->data + a2->data, 0.0f, OP_ADD, a1, a2};
}

internal Value *
vadd(Value *a1, Value *a2, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	*v = {a1->data + a2->data, 0.0f, OP_ADD, a1, a2};
	return v;
}

internal Value
vsub(Value *a1, Value *a2) {
	return {a1->data - a2->data, 0.0f, OP_SUB, a1, a2};
}

internal Value *
vsub(Value *a1, Value *a2, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	*v = {a1->data - a2->data, 0.0f, OP_SUB, a1, a2};
	return v;
}

internal Value
vmul(Value *a1, Value *a2) {
	return {a1->data * a2->data, 0.0f, OP_MUL, a1, a2};
}

internal Value *
vmul(Value *a1, Value *a2, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	*v = {a1->data * a2->data, 0.0f, OP_MUL, a1, a2};
	return v;
}

internal Value
vdiv(Value *a1, Value *a2) {
	return {a1->data / a2->data, 0.0f, OP_DIV, a1, a2};
}

internal Value *
vdiv(Value *a1, Value *a2, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	*v = {a1->data / a2->data, 0.0f, OP_DIV, a1, a2};
	return v;
}

internal Value
vtanh(Value *a1) {
	F32 t = exp(2 * a1->data);
	return { (t - 1)/(t + 1), 0.0f, OP_TANH, a1, 0x0 };
}

internal Value *
vtanh(Value *a1, Arena *a) {
	Value *v = (Value *)arena_alloc(a, sizeof(Value));
	F32 t = exp(2 * a1->data);
	*v = { (t - 1)/(t + 1), 0.0f, OP_TANH, a1, 0x0 };
	return v;
}

internal Value
vrand_uniform(F32 min, F32 max) {
	F32 data = min + (F32)rand() / ((F32)RAND_MAX / (max - min));
	return value(data);
}

internal void
vprint(Value *v) {
	printf("(data=%9.4f, grad=%9.4f, op=%-5s, p1=%p, p2=%p)",
	       v->data, v->grad, OP_REPR_TABLE[v->op],
	       (void *)v->p1, (void *)v->p2);
}
#define VPRINT_DEBUG(v) printf("%10s", #v); vprint(v); printf("\n");

