import herss;

int main()
{
    GlobalConfig config;
    if(EnumToString(RESERVOIR)[0] != 'R') {
        return 1;
    }
    if(config.nr_nodes == 0) {
        return 2;
    }
    return 0;
}
