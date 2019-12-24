#ifdef __APPLE__
/* Copied from glib */
char *
strchrnul (const char *s, int c)
{
        char *p = (char *) s;
        while (*p && (*p != c))
                ++p;

        return p;
}
#endif
