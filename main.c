// main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "libasm.h"

#define GREEN  "\x1b[32m"
#define RED    "\x1b[31m"
#define RESET  "\x1b[0m"
#define OK(b)  ((b) ? GREEN "OK" RESET : RED "KO" RESET)

static void test_strlen(void) {
    const char *v[] = {"", "a", "hello", "with spaces", "0123456789", "line1\nline2"};
    size_t n = sizeof v / sizeof *v;
    puts("== ft_strlen ==");
    for (size_t i = 0; i < n; ++i) {
        size_t a = ft_strlen(v[i]), b = strlen(v[i]);
        printf("[%zu] \"%s\" -> ft=%zu std=%zu : %s\n", i, v[i], a, b, OK(a==b));
    }
    puts("");
}

static void test_strcmp(void) {
    struct { const char *s1,*s2; } v[] = {
        {"",""},{"","a"},{"a",""},{"a","a"},{"abc","abc"},{"abc","abd"},
        {"abcd","abc"},{"abc","abcd"},{"Hello","hello"},{"same","same!"}
    };
    size_t n = sizeof v / sizeof *v;
    puts("== ft_strcmp ==");
    for (size_t i = 0; i < n; ++i) {
        int a = ft_strcmp(v[i].s1, v[i].s2), b = strcmp(v[i].s1, v[i].s2);
        int sa=(a>0)-(a<0), sb=(b>0)-(b<0);
        printf("[%zu] \"%s\" vs \"%s\" -> ft=%d std=%d : %s\n",
               i, v[i].s1, v[i].s2, a, b, OK(sa==sb));
    }
    puts("");
}

static void test_strcpy(void) {
    const char *v[] = {"", "a", "hello", "with spaces", "0123456789"};
    size_t n = sizeof v / sizeof *v;
    puts("== ft_strcpy ==");
    for (size_t i = 0; i < n; ++i) {
        char d1[128]={0}, d2[128]={0};
        char *r1 = ft_strcpy(d1, v[i]);
        char *r2 = strcpy   (d2, v[i]);
        int ok = (strcmp(d1,d2)==0) && (r1==d1) && (r2==d2);
        printf("[%zu] \"%s\" -> %s\n", i, v[i], OK(ok));
    }
    puts("");
}

static void test_strdup(void) {
    const char *v[] = {"", "a", "hello", "with spaces", "0123456789"};
    size_t n = sizeof v / sizeof *v;
    puts("== ft_strdup ==");
    for (size_t i = 0; i < n; ++i) {
        char *a = ft_strdup(v[i]);
        char *b = strdup   (v[i]);
        int ok = (a && b) ? (strcmp(a,b)==0) : (a==b);
        printf("[%zu] \"%s\" -> %s\n", i, v[i], OK(ok));
        free(a); free(b);
    }
    puts("");
}

static void test_write(void) {
    const char *v[] = {"", "A", "hello", "with\nnewline"};
    size_t n = sizeof v / sizeof *v;
    puts("== ft_write ==");
    for (size_t i = 0; i < n; ++i) {
        int p1[2], p2[2];
        if (pipe(p1)==-1 || pipe(p2)==-1) { perror("pipe"); exit(1); }
        size_t len = strlen(v[i]);

        errno=0; ssize_t w1 = write   (p1[1], v[i], len); int e1=errno;
        errno=0; ssize_t w2 = ft_write(p2[1], v[i], len); int e2=errno;

        close(p1[1]); close(p2[1]);      // critical to avoid blocking on ""

        char b1[128]={0}, b2[128]={0};
        ssize_t r1 = read(p1[0], b1, sizeof(b1)-1);
        ssize_t r2 = read(p2[0], b2, sizeof(b2)-1);
        close(p1[0]); close(p2[0]);

        int ok = (w1==w2) && (e1==e2) && (r1==r2) && (r1==(ssize_t)len) && (strcmp(b1,b2)==0);
        printf("[%zu] \"%s\" -> %s\n", i, v[i], OK(ok));
    }
    // error cases
    errno=0; ssize_t ew1 = write(-1, "abc", 3); int ee1=errno;
    errno=0; ssize_t ew2 = ft_write(-1, "abc", 3); int ee2=errno;
    printf("[write err fd] std=%zd/%d ft=%zd/%d : %s\n", ew1,ee1,ew2,ee2, OK(ew1==-1 && ew2==-1 && ee1==ee2));

    errno=0; ew1 = write(STDOUT_FILENO, (const void*)0, 3); ee1=errno;
    errno=0; ew2 = ft_write(STDOUT_FILENO, (const void*)0, 3); ee2=errno;
    printf("[write err buf] std=%zd/%d ft=%zd/%d : %s\n\n", ew1,ee1,ew2,ee2, OK(ew1==-1 && ew2==-1 && ee1==ee2));
}

static void test_read(void) {
    const char *v[] = {"", "X", "hello", "0123456789"};
    size_t n = sizeof v / sizeof *v;
    puts("== ft_read ==");

    // Success via temp file (no pipe semantics)
    for (size_t i = 0; i < n; ++i) {
        char templ[] = "/tmp/libasm_test.XXXXXX";
        int fd = mkstemp(templ);
        if (fd < 0) { perror("mkstemp"); exit(1); }
        size_t len = strlen(v[i]);
        if (write(fd, v[i], len) != (ssize_t)len) { perror("write(tmp)"); exit(1); }
        if (lseek(fd, 0, SEEK_SET) == (off_t)-1) { perror("lseek"); exit(1); }

        char b1[128]={0}, b2[128]={0};
        errno=0; ssize_t r1 = read   (fd, b1, sizeof(b1)-1); int e1=errno;
        if (lseek(fd, 0, SEEK_SET) == (off_t)-1) { perror("lseek"); exit(1); }
        errno=0; ssize_t r2 = ft_read(fd, b2, sizeof(b2)-1); int e2=errno;

        close(fd); unlink(templ);

        int ok = (r1==r2) && (e1==e2) && (r1==(ssize_t)len) && (memcmp(b1,b2,len)==0);
        printf("[%zu] \"%s\" -> %s\n", i, v[i], OK(ok));
    }

    // Error: bad FD
    char tmp[8];
    errno=0; ssize_t er1 = read(-1, tmp, sizeof tmp); int ee1=errno;
    errno=0; ssize_t er2 = ft_read(-1, tmp, sizeof tmp); int ee2=errno;
    printf("[read err fd] std=%zd/%d ft=%zd/%d : %s\n", er1,ee1,er2,ee2, OK(er1==-1 && er2==-1 && ee1==ee2));
}

int main(void) {
    test_strlen();
    test_strcmp();
    test_strcpy();
    test_strdup();
    test_write();
    test_read();
    return 0;
}
