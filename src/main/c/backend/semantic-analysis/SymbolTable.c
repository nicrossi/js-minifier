#include "SymbolTable.h"

#define MIN_NAMES 32

/* Hash-table entry */
typedef struct Entry {
    char * name;          // identifier name
    SymbolInfo info;      // metadata
    struct Entry * next;  // collision list
} Entry;

/* A scope frame  (hash table + pointer to enclosing frame) */
typedef struct Frame {
    Entry * bucket[BUCKET_SIZE];
    struct Frame * parent;
    struct Frame * nextZombie;
} Frame;

// List of frames that have been pooped but not yet destroyed.
static Frame * zombies = NULL;

/* Pointer to the current stack top */
struct SymbolTable {
    Frame * top;
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
            free(e->info.minifiedName);
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

static const SymbolInfo * _lookupChain(const Frame * chain, uint32_t hash, const char * name) {
    while (chain) {
        const SymbolInfo * info = _lookupEntry(chain->bucket[hash], name);
        if (info) return info;
        chain = chain->nextZombie;
    }
    return NULL;
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

/*
 * The function _nextMinifiedName generates a unique,
 * incrementing string identifier using a base-26 encoding scheme.
 * starting with "a", "b", ..., "z", then "aa", "ab", and so on.
 *
 * Note: Because each scope has its own frame, two different functions
 * can both obtain the name a for their private local variables
 * */
static char * _nextMinifiedName(void) {
    static unsigned long counter = 0;
    char buf[MIN_NAMES];
    unsigned long n = counter++;
    size_t i = 0;
    do { // base-26
        buf[i++] = 'a' + (char)(n % 26);
        n /= 26;
    } while (n);
    buf[i] = '\0';
    return strdup(buf);
}

/*
 * Because the entries are freed when a scope ends, by the time code-generation
 * tries to look up some identifier, it can only find those that lived in the global frame.
 *
 * Prepend child's bucket-chain onto parent
 * */
static void _adoptEntries(Frame * parent, Frame * child) {
    for (size_t i = 0; i < BUCKET_SIZE; ++i) {
        if (!child->bucket[i]) continue;

        Entry * tail = child->bucket[i];
        while (tail->next) {
            tail = tail->next;
        }
        tail->next = parent->bucket[i];
        parent->bucket[i] = child->bucket[i];
        child->bucket[i] = NULL;
    }
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

    Frame * dead = st->top;
    st->top = dead->parent;
    dead->nextZombie = zombies;
    zombies = dead;
}

void stPurge(void) {
    while (zombies) {
        Frame * next = zombies->nextZombie;
        _frameDestroy(zombies);
        zombies = next;
    }
}

bool stInsert(SymbolTable * st, const char * name, SymKind kind) {
    uint32_t h = hash(name);
    // reject duplicates in current scope
    if (_entryExists(st->top->bucket[h], name))
        return false;

    Entry * newEntry = ecalloc(1, sizeof *newEntry);
    newEntry->name = strdup(name);
    newEntry->info.minifiedName = _nextMinifiedName();
    newEntry->info.kind = kind;
    newEntry->next = st->top->bucket[h];
    st->top->bucket[h] = newEntry;

    return true;
}

const SymbolInfo * stLookup(const SymbolTable * st, const char *name) {
    uint32_t h = hash(name);
    const SymbolInfo * info = _lookupFrame(st->top, h, name);
    if (info) return info;

    return _lookupChain(zombies, h, name);
}
