// main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>   // read, write, pipe, close
#include <errno.h>
#include <fcntl.h>    // open
#include "libasm.h"   // your header

#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define RESET  "\033[0m"

static const char *ok(int cond) { return cond ? GREEN "OK" RESET : RED "KO" RESET; }

/* ------------------------- strlen / strcpy / strcmp ------------------------ */

static void test_strlen(void)
{
    const char *tests[] = {
        "",
        "a",
        "hello",
        "with spaces",
        "1234567890",
        "line1\nline2",
        "tabs\tand\tsuch",
        "long string long string long string long string!",
    };
    size_t nb = sizeof(tests) / sizeof(tests[0]);

    printf("=== ft_strlen vs strlen ===\n");
    for (size_t i = 0; i < nb; ++i)
    {
        size_t std = strlen(tests[i]);
        size_t mine = ft_strlen(tests[i]);
        int okv = (std == mine);
        printf("Test %zu: \"%s\"\n", i, tests[i]);
        printf("  strlen   = %zu\n", std);
        printf("  ft_strlen= %zu  -> %s\n\n", mine, ok(okv));
    }
}

static void test_strcpy(void)
{
    const char *tests[] = {
        "",
        "a",
        "hello",
        "with spaces",
        "1234567890",
        "line1\nline2",
        "tabs\tand\tsuch",
        "long string long string long string long string!",
    };
    size_t nb = sizeof(tests) / sizeof(tests[0]);

    printf("=== ft_strcpy vs strcpy ===\n");
    for (size_t i = 0; i < nb; ++i)
    {
        const char *src = tests[i];
        size_t len = strlen(src);

        char *dst_std = malloc(len + 10);
        char *dst_ft  = malloc(len + 10);
        if (!dst_std || !dst_ft) { perror("malloc"); exit(1); }

        memset(dst_std, 'X', len + 9);
        memset(dst_ft,  'Y', len + 9);
        dst_std[len + 9] = '\0';
        dst_ft[len + 9]  = '\0';

        char *ret_std = strcpy(dst_std, src);
        char *ret_ft  = ft_strcpy(dst_ft, src);

        int same_content    = (strcmp(dst_std, dst_ft) == 0);
        int correct_ret_std = (ret_std == dst_std);
        int correct_ret_ft  = (ret_ft == dst_ft);
        int okv = same_content && correct_ret_std && correct_ret_ft;

        printf("Test %zu: \"%s\"\n", i, src);
        printf("  strcpy   -> \"%s\" (ret==dst: %s)\n", dst_std, correct_ret_std ? "yes" : "no");
        printf("  ft_strcpy-> \"%s\" (ret==dst: %s)\n", dst_ft,  correct_ret_ft  ? "yes" : "no");
        printf("  -> %s\n\n", ok(okv));

        free(dst_std);
        free(dst_ft);
    }
}

static void test_strcmp(void)
{
    struct { const char *s1, *s2; } tests[] = {
        { "", "" }, { "", "a" }, { "a", "" }, { "a", "a" },
        { "abc", "abc" }, { "abc", "abd" },
        { "abcd", "abc" }, { "abc", "abcd" },
        { "hello", "hellp" }, { "Hello", "hello" },
        { "same prefix", "same prefix!" },
        { "short", "short\0hidden" },
        { "\xff", "\x7f" },  // unsigned-char behavior
    };
    size_t nb = sizeof(tests) / sizeof(tests[0]);

    printf("=== ft_strcmp vs strcmp ===\n");
    for (size_t i = 0; i < nb; ++i)
    {
        int std  = strcmp(tests[i].s1, tests[i].s2);
        int mine = ft_strcmp(tests[i].s1, tests[i].s2);
        int std_sign  = (std > 0) - (std < 0);
        int mine_sign = (mine > 0) - (mine < 0);
        int okv = (std_sign == mine_sign);

        printf("Test %zu: \"%s\" vs \"%s\"\n", i, tests[i].s1, tests[i].s2);
        printf("  strcmp   = %d (sign %d)\n", std, std_sign);
        printf("  ft_strcmp= %d (sign %d)\n", mine, mine_sign);
        printf("  -> %s\n\n", ok(okv));
    }
}

/* ------------------------------ ft_write tests ----------------------------- */

static void test_write_success_cases(void)
{
    printf("=== ft_write vs write (success cases) ===\n");

    const char *payloads[] = { "", "A", "hello", "with\nnewline", "0123456789" };
    size_t nb = sizeof(payloads) / sizeof(payloads[0]);

    for (size_t i = 0; i < nb; ++i)
    {
        const char *msg = payloads[i];
        size_t len = strlen(msg);

        int p_std[2], p_ft[2];
        if (pipe(p_std) == -1 || pipe(p_ft) == -1) { perror("pipe"); exit(1); }

        errno = 0;
        ssize_t r_std = write(p_std[1], msg, len);
        int errno_std = errno;

        errno = 0;
        ssize_t r_ft  = ft_write(p_ft[1], msg, len);
        int errno_ft  = errno;

        // Read back contents to compare
        char buf_std[256] = {0}, buf_ft[256] = {0};
        ssize_t rd1 = read(p_std[0], buf_std, sizeof(buf_std)-1);
        ssize_t rd2 = read(p_ft[0],  buf_ft, sizeof(buf_ft)-1);

        int same_ret    = (r_std == r_ft);
        int same_errno  = (errno_std == errno_ft);   // on success errno should remain unchanged; both often 0
        int same_bytes  = (rd1 == rd2) && (rd1 == (ssize_t)len);
        int same_buffer = (strcmp(buf_std, buf_ft) == 0);

        int okv = same_ret && same_errno && same_bytes && same_buffer;

        printf("Test %zu: write \"%s\"\n", i, msg);
        printf("  write:    ret=%zd errno=%d, readback=\"%s\"\n", r_std, errno_std, buf_std);
        printf("  ft_write: ret=%zd errno=%d, readback=\"%s\"\n", r_ft,  errno_ft,  buf_ft);
        printf("  -> %s\n\n", ok(okv));

        close(p_std[0]); close(p_std[1]);
        close(p_ft[0]);  close(p_ft[1]);
    }
}

static void test_write_error_cases(void)
{
    printf("=== ft_write vs write (error cases) ===\n");

    const char *msg = "abc";
    size_t len = strlen(msg);

    // Invalid fd
    errno = 0;
    ssize_t r_std = write(-1, msg, len);
    int errno_std = errno;

    errno = 0;
    ssize_t r_ft  = ft_write(-1, msg, len);
    int errno_ft  = errno;

    int okv = (r_std == -1) && (r_ft == -1) && (errno_std == errno_ft);
    printf("Invalid fd (-1):\n");
    printf("  write:    ret=%zd errno=%d\n", r_std, errno_std);
    printf("  ft_write: ret=%zd errno=%d\n", r_ft,  errno_ft);
    printf("  -> %s\n\n", ok(okv));

    // Bad pointer (may crash if library doesn’t handle; but both should set EFAULT)
    // We’ll try a clearly invalid pointer (NULL with len>0).
    errno = 0;
    r_std = write(STDOUT_FILENO, (const void *)0x0, 3);
    errno_std = errno;

    errno = 0;
    r_ft  = ft_write(STDOUT_FILENO, (const void *)0x0, 3);
    errno_ft  = errno;

    okv = (r_std == -1) && (r_ft == -1) && (errno_std == errno_ft);
    printf("NULL buffer:\n");
    printf("  write:    ret=%zd errno=%d\n", r_std, errno_std);
    printf("  ft_write: ret=%zd errno=%d\n", r_ft,  errno_ft);
    printf("  -> %s\n\n", ok(okv));
}

/* ------------------------------- ft_read tests ----------------------------- */

static void test_read_success_cases(void)
{
    printf("=== ft_read vs read (success cases) ===\n");

    const char *payloads[] = { "", "X", "hello", "with\nnewline", "0123456789" };
    size_t nb = sizeof(payloads) / sizeof(payloads[0]);

    for (size_t i = 0; i < nb; ++i)
    {
        const char *src = payloads[i];
        size_t len = strlen(src);

        int p[2];
        if (pipe(p) == -1) { perror("pipe"); exit(1); }

        // Fill pipe with test data
        if (write(p[1], src, len) != (ssize_t)len) { perror("write(pipe)"); exit(1); }
        close(p[1]);

        char bstd[256] = {0}, bft[256] = {0};

        errno = 0;
        ssize_t r_std = read(p[0], bstd, sizeof(bstd)-1);
        int errno_std = errno;

        // Refill for ft_read since the first read drained it
        close(p[0]);
        if (pipe(p) == -1) { perror("pipe"); exit(1); }
        if (write(p[1], src, len) != (ssize_t)len) { perror("write(pipe)"); exit(1); }
        close(p[1]);

        errno = 0;
        ssize_t r_ft  = ft_read(p[0], bft, sizeof(bft)-1);
        int errno_ft  = errno;

        int same_ret    = (r_std == r_ft);
        int same_errno  = (errno_std == errno_ft);
        int same_buffer = (r_std >= 0) ? (memcmp(bstd, bft, r_std) == 0) : 1;

        int okv = same_ret && same_errno && same_buffer;

        printf("Test %zu: payload \"%s\"\n", i, src);
        printf("  read:    ret=%zd errno=%d buf=\"%.*s\"\n", r_std, errno_std, (int)((r_std>0)?r_std:0), bstd);
        printf("  ft_read: ret=%zd errno=%d buf=\"%.*s\"\n", r_ft,  errno_ft,  (int)((r_ft>0)?r_ft:0),  bft);
        printf("  -> %s\n\n", ok(okv));

        close(p[0]);
    }
}

static void test_read_error_cases(void)
{
    printf("=== ft_read vs read (error cases) ===\n");

    char buf[8];

    // Invalid fd
    errno = 0;
    ssize_t r_std = read(-1, buf, sizeof(buf));
    int errno_std = errno;

    errno = 0;
    ssize_t r_ft  = ft_read(-1, buf, sizeof(buf));
    int errno_ft  = errno;

    int okv = (r_std == -1) && (r_ft == -1) && (errno_std == errno_ft);
    printf("Invalid fd (-1):\n");
    printf("  read:    ret=%zd errno=%d\n", r_std, errno_std);
    printf("  ft_read: ret=%zd errno=%d\n", r_ft,  errno_ft);
    printf("  -> %s\n\n", ok(okv));

    // NULL buffer
    errno = 0;
    r_std = read(STDIN_FILENO, (void *)0x0, 3);
    errno_std = errno;

    errno = 0;
    r_ft  = ft_read(STDIN_FILENO, (void *)0x0, 3);
    errno_ft  = errno;

    okv = (r_std == -1) && (r_ft == -1) && (errno_std == errno_ft);
    printf("NULL buffer:\n");
    printf("  read:    ret=%zd errno=%d\n", r_std, errno_std);
    printf("  ft_read: ret=%zd errno=%d\n", r_ft,  errno_ft);
    printf("  -> %s\n\n", ok(okv));
}

/* ------------------------------ ft_strdup tests ---------------------------- */

static void test_strdup(void)
{
    printf("=== ft_strdup vs strdup ===\n");

    const char *tests[] = {
        "",
        "a",
        "hello",
        "with spaces",
        "0123456789",
        "line1\nline2",
        "tabs\tand\tsuch",
        "long string long string long string long string!",
        "\xff\x01\x00\x7f",  // includes a NUL; only first 2 bytes should be copied
    };
    size_t nb = sizeof(tests) / sizeof(tests[0]);

    for (size_t i = 0; i < nb; ++i)
    {
        const char *s = tests[i];

        char *std = strdup(s);
        char *ft  = ft_strdup(s);

        int oklen = (std && ft) ? (strlen(std) == strlen(ft)) : (std == ft);
        int okcmp = (std && ft) ? (strcmp(std, ft) == 0) : (std == ft);

        printf("Test %zu: input -> \"%s\"\n", i, s);
        printf("  strdup:    %s\n", std ? std : "(null)");
        printf("  ft_strdup: %s\n", ft  ? ft  : "(null)");
        printf("  -> %s\n\n", ok(oklen && okcmp));

        free(std);
        free(ft);
    }

    // Large string stress
    size_t big = 1 << 16;
    char *bigsrc = malloc(big + 1);
    if (!bigsrc) { perror("malloc"); exit(1); }
    for (size_t i = 0; i < big; ++i) bigsrc[i] = 'A' + (i % 26);
    bigsrc[big] = '\0';

    char *std = strdup(bigsrc);
    char *ft  = ft_strdup(bigsrc);

    int okv = (std && ft) && (strcmp(std, ft) == 0);
    printf("Large string (%zu bytes): %s\n\n", big, ok(okv));

    free(std);
    free(ft);
    free(bigsrc);
}

/* ----------------------------------- main ---------------------------------- */

int main(void)
{
    test_strlen();
    test_strcpy();
    test_strcmp();

    test_write_success_cases();
    test_write_error_cases();

    test_read_success_cases();
    test_read_error_cases();

    test_strdup();

    return 0;
}
