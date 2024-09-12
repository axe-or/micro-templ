#define ARENA_IMPLEMENTATION 1
#pragma region Arena
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define uintptr uintptr_t
#define isize ptrdiff_t
#define byte uint8_t

struct Arena {
	isize offset;
	isize capacity;
	byte* data;
};

void arena_init(struct Arena* a, byte* data, isize len);

void *arena_alloc(struct Arena* a, isize size, isize align);

void arena_reset(struct Arena* a);

void arena_destroy(struct Arena* a);

#ifdef ARENA_IMPLEMENTATION
static
uintptr arena_align_forward_ptr(uintptr p, uintptr a){
	uintptr mod = p % a;
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}

void arena_init(struct Arena* a, byte* data, isize len){
	a->capacity = len;
	a->data = data;
	a->offset = 0;
}

void arena_reset(struct Arena* a){
	a->offset = 0;
}

void arena_destroy(struct Arena* a){
	arena_reset(a);
	a->capacity = 0;
	a->data = NULL;
}

static
uintptr arena_required_mem(uintptr cur, isize nbytes, isize align){
	bool valid_alignment = (align & (align - 1)) == 0 && align > 0;
	if(!valid_alignment){
		return ~((uintptr_t)(0));
	}
	uintptr_t aligned  = arena_align_forward_ptr(cur, align);
	uintptr_t padding  = (uintptr)(aligned - cur);
	uintptr_t required = padding + nbytes;
	return required;
}

void* arena_alloc(struct Arena* a, isize size, isize align){
	uintptr base = (uintptr)a->data;
	uintptr current = (uintptr)base + (uintptr)a->offset;

	uintptr available = (uintptr)a->capacity - (current - base);
	uintptr required = arena_required_mem(current, size, align);

	if(required > available){
		return NULL;
	}

	a->offset += required;
	void* allocation = &a->data[a->offset - size];
	return allocation;
}

#undef uintptr
#undef isize
#undef byte
#endif
#pragma endregion

#define ARENA_MEMORY_SIZE (4 * 1024 * 1024)

struct GlobalState {
	struct Arena arena;
	uint8_t arena_memory[ARENA_MEMORY_SIZE];
};

typedef struct {
	char const* data;
	int len;
} String;

static int cstring_len(char const* s){
	int i = 0;
	while(s[i] != 0){ i += 1; }
	return i;
}

#define DICT_KEY_COUNT 512

struct Dict {
	String keys[DICT_KEY_COUNT];
	String values[DICT_KEY_COUNT];
};

void dict_init(Dict* d){
	__builtin_memset(&d.keys[0], 0, sizeof(String) * DICT_KEY_COUNT);
	__builtin_memset(&d.values[0], 0, sizeof(String) * DICT_KEY_COUNT);
}

bool str_eq(String a, String b){
	if(a.len != b.len){ return false; }
	for(int i = 0; i < a.len; i++){
		if(a.data[i] != b.data[i]){ return false; }
	}
	return true;
}

String dict_get(Dict const* d, String key){}

String dict_set(Dict* d, String key, String value){}

bool dict_has_key(Dict const* d, String key){}


static struct GlobalState global = {0};

int main(){
	return 0;
}
