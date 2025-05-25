#include "Environment.h"

/* PUBLIC FUNCTIONS */

const boolean getBooleanOrDefault(const char * name, const boolean defaultValue) {
	const char * value = getStringOrDefault(name, NULL);
	if (value == NULL) {
		return defaultValue;
	}
	else if (strcmp(value, "true") == 0) {
		return true;
	}
	else {
		return false;
	}
}

const char * getStringOrDefault(const char * name, const char * defaultValue) {
	const char * value = getenv(name);
	if (value == NULL) {
		return defaultValue;
	}
	else {
		return value;
	}
}

void setOutputFileName(const char *filename) {
    outputFileName = filename;
}

FILE * getOutputStream() {
    static FILE * cachedFile = NULL;
    static int initialized = 0;
    if (!initialized) {
        if (outputFileName == NULL) {
            cachedFile = stdout;
        } else {
            cachedFile = fopen(outputFileName, "w");
            if (cachedFile == NULL) {
                cachedFile = stdout;
            }
        }
        initialized = 1;
    }
    return cachedFile;
}
