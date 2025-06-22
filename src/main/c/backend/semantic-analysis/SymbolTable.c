#include "SymbolTable.h"

/* Hash-table entry */
typedef struct Entry {
    char * name;          // identifier name
    SymbolInfo info;      // metadata
    struct Entry * next;  // collision list
} Entry;

/* A scope frame  (hash table + pointer to enclosing frame) */
typedef struct Frame {
    Entry * bucket[BUCKET_SIZE];
    struct Frame *parent;
} Frame;

/* Pointer to the current stack top */
struct SymbolTable {
    Frame *top;
};

static Frame * _frameCreate(Frame * parent) {
    Frame *f = ecalloc(1, sizeof *f);
    f->parent = parent;
    return f;
}

static void _frameDestroy(Frame * f) {
    for (size_t i = 0; i < BUCKET_SIZE; ++i) {
        Entry *e = f->bucket[i];
        while (e) {
            Entry *next = e->next;
            free(e->name);
            free(e);
            e = next;
        }
    }
    free(f);
}

static const SymbolInfo * _lookupEntry(Entry * entry, const char * name) {
    if (!entry) return NULL;

    return (strcmp(entry->name, name) == 0)
           ? &entry->info
           : _lookupEntry(entry->next, name);
}

static const SymbolInfo * _lookupFrame(const Frame * frame, uint32_t hash, const char * name) {
    if (!frame) return NULL;

    const SymbolInfo * info = _lookupEntry(frame->bucket[hash], name);
    return info ? info : _lookupFrame(frame->parent, hash, name);
}

static bool _entryExists(Entry * entry, const char * name) {
    if (!entry) return false;
    if (strcmp(entry->name, name) == 0) return true;

    return _entryExists(entry->next, name);
}

/* Public Functions */

SymbolTable * stCreate(void) {
    SymbolTable *st = ecalloc(1, sizeof * st);
    st->top = _frameCreate(NULL); // global scope
    return st;
}

void stDestroy(SymbolTable * st) {
    while (st->top) {
        Frame *tmp = st->top->parent;
        _frameDestroy(st->top);
        st->top = tmp;
    }
    free(st);
}

void stEnterScope(SymbolTable * st) {
    st->top = _frameCreate(st->top);
}

void stExitScope(SymbolTable * st) {
    if (!st->top->parent) {
        // Trying to pop the global scope – ignore
        return;
    }
    Frame * tmp = st->top->parent;
    _frameDestroy(st->top);
    st->top = tmp;
}

bool stInsert(SymbolTable * st, const char * name, SymKind kind) {
    uint32_t h = hash(name);
    // reject duplicates in current scope
    if (_entryExists(st->top->bucket[h], name))
        return false;

    Entry * newEntry = ecalloc(1, sizeof *newEntry);
    newEntry->name = strdup(name);
    newEntry->info.kind = kind;
    newEntry->next = st->top->bucket[h];
    st->top->bucket[h] = newEntry;

    return true;
}

const SymbolInfo * stLookup(const SymbolTable * st, const char *name) {
    uint32_t h = hash(name);
    return _lookupFrame(st->top, h, name);
}
