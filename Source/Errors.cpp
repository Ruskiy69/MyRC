#include "Errors.h"

void handleError(const char* errorMessage, const bool quit)
{
#ifdef _WIN32
    MessageBoxA(NULL, errorMessage, "Error!", MB_ICONEXCLAMATION | MB_OK | MB_ICONERROR);
#else
    fprintf(stderr, "[-] %s", errorMessage);
#endif // _WIN32

    if(quit)
        exit(EXIT_FAILURE);
}