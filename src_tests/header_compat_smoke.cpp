#include "herss.h"

int main()
{
    GlobalConfig config;
    if(EnumToString(PSTATION) != "PSTATION") {
        return 1;
    }
    Dataset* dataset_view = nullptr;
    if(dataset_view != nullptr) {
        return 2;
    }
    return 0;
}
