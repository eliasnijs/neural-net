enum Operand {OP_NONE, OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_TANH, OP_COUNT};
global_variable const char *OP_REPR_TABLE[] = {
	"NONE", "ADD", "SUB", "MUL", "DIV", "TANH", "COUNT"
};

struct Value {
	f32 data;
	f32 grad;

	Operand op;
	Value *p1;
	Value *p2;
};

struct VStack {
	i32	count;
	i32	capacity;
	Value	*values;
};

internal VStack
vstack(i32 capacity) {
	return {0, capacity, (Value *)calloc(capacity, sizeof(Value))};
}

internal void
vstack_die(VStack *s) {
	free(s->values);
	s->values = 0x0;
}


internal Value *
vstack_push(VStack *s, Value v) {
	s->values[s->count++] = v;
	return &s->values[s->count-1];
}

internal Value *
vstack_claim(VStack *s, i32 size) {
	Value* start = &s->values[s->count];
	s->count += size;
	return start;
}

internal Value
value(f32 value) {
	return {value, 0.0f, OP_NONE, 0x0, 0x0};
}

internal Value *
value(f32 value, VStack *s) {
	return vstack_push(s, {value, 0.0f, OP_NONE, 0x0, 0x0});
}

internal Value
vadd(Value *a1, Value *a2) {
	return {a1->data + a2->data, 0.0f, OP_ADD, a1, a2};
}

internal Value *
vadd(Value *a1, Value *a2, VStack *s) {
	return vstack_push(s, {a1->data + a2->data, 0.0f, OP_ADD, a1, a2});
}

internal Value
vsub(Value *a1, Value *a2) {
	return {a1->data - a2->data, 0.0f, OP_SUB, a1, a2};
}

internal Value *
vsub(Value *a1, Value *a2, VStack *s) {
	return vstack_push(s, {a1->data - a2->data, 0.0f, OP_SUB, a1, a2});
}

internal Value
vmul(Value *a1, Value *a2) {
	return {a1->data * a2->data, 0.0f, OP_MUL, a1, a2};
}

internal Value *
vmul(Value *a1, Value *a2, VStack *s) {
	return vstack_push(s, {a1->data * a2->data, 0.0f, OP_MUL, a1, a2});
}

internal Value
vdiv(Value *a1, Value *a2) {
	return {a1->data / a2->data, 0.0f, OP_DIV, a1, a2};
}

internal Value *
vdiv(Value *a1, Value *a2, VStack *s) {
	return vstack_push(s, {a1->data / a2->data, 0.0f, OP_DIV, a1, a2});
}

internal Value
vtanh(Value *a1) {
	f32 t = exp(2 * a1->data);
	return { (t - 1)/(t + 1), 0.0f, OP_TANH, a1, 0x0 };
}

internal Value *
vtanh(Value *a1, VStack *s) {
	f32 t = exp(2 * a1->data);
	return vstack_push(s, {(t - 1)/(t + 1), 0.0f, OP_TANH, a1, 0x0});
}

internal Value
vrand_uniform(f32 min, f32 max) {
	f32 data = min + (f32)rand() / ((f32)RAND_MAX / (max - min));
	return value(data);
}

internal void
vprint(Value *v) {
	printf("(data=%9.4f, grad=%9.4f, op=%-5s, p1=%p, p2=%p)",
	       v->data, v->grad, OP_REPR_TABLE[v->op],
	       (void *)v->p1, (void *)v->p2);
}
#define VPRINT_DEBUG(v) printf("%10s", #v); vprint(v); printf("\n");
