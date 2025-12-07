#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "libasm.h"   // <-- use your header

#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define RESET  "\033[0m"

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
        int ok = (std == mine);

        printf("Test %zu: \"%s\"\n", i, tests[i]);
        printf("  strlen   = %zu\n", std);
        printf("  ft_strlen= %zu  -> %s%s%s\n",
               mine,
               ok ? GREEN : RED,
               ok ? "OK" : "KO",
               RESET);
    }
    printf("\n");
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
        if (!dst_std || !dst_ft)
        {
            perror("malloc");
            exit(1);
        }

        memset(dst_std, 'X', len + 9);
        memset(dst_ft,  'Y', len + 9);
        dst_std[len + 9] = '\0';
        dst_ft[len + 9]  = '\0';

        char *ret_std = strcpy(dst_std, src);
        char *ret_ft  = ft_strcpy(dst_ft, src);

        int same_content   = (strcmp(dst_std, dst_ft) == 0);
        int correct_ret_std = (ret_std == dst_std);
        int correct_ret_ft  = (ret_ft == dst_ft);
        int ok = same_content && correct_ret_std && correct_ret_ft;

        printf("Test %zu: \"%s\"\n", i, src);
        printf("  strcpy   -> \"%s\" (ret == dst: %s)\n",
               dst_std, correct_ret_std ? "yes" : "no");
        printf("  ft_strcpy-> \"%s\" (ret == dst: %s)\n",
               dst_ft, correct_ret_ft ? "yes" : "no");
        printf("  -> %s%s%s\n",
               ok ? GREEN : RED,
               ok ? "OK" : "KO",
               RESET);

        free(dst_std);
        free(dst_ft);
    }
    printf("\n");
}

static void test_strcmp(void)
{
    struct {
        const char *s1;
        const char *s2;
    } tests[] = {
        { "", "" },
        { "", "a" },
        { "a", "" },
        { "a", "a" },
        { "abc", "abc" },
        { "abc", "abd" },
        { "abcd", "abc" },
        { "abc", "abcd" },
        { "hello", "hellp" },
        { "Hello", "hello" },
        { "same prefix", "same prefix!" },
        { "short", "short\0hidden" },
        { "\xff", "\x7f" },
    };
    size_t nb = sizeof(tests) / sizeof(tests[0]);

    printf("=== ft_strcmp vs strcmp ===\n");
    for (size_t i = 0; i < nb; ++i)
    {
        const char *s1 = tests[i].s1;
        const char *s2 = tests[i].s2;

        int std  = strcmp(s1, s2);
        int mine = ft_strcmp(s1, s2);

        int std_sign  = (std > 0) - (std < 0);
        int mine_sign = (mine > 0) - (mine < 0);
        int ok = (std_sign == mine_sign);

        printf("Test %zu: \"%s\" vs \"%s\"\n", i, s1, s2);
        printf("  strcmp   = %d (sign %d)\n", std, std_sign);
        printf("  ft_strcmp= %d (sign %d)\n", mine, mine_sign);
        printf("  -> %s%s%s\n",
               ok ? GREEN : RED,
               ok ? "OK" : "KO",
               RESET);
    }
    printf("\n");
}

int main(void)
{
    test_strlen();
    test_strcpy();
    test_strcmp();
    return 0;
}