/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_atoi.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: dpuccion <dpuccion@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/07 14:58:48 by dpuccion     #+#   ##    ##    #+#       */
/*   Updated: 2019/11/08 14:04:06 by dpuccion    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "libasm.h"

int main(int argc, char **argv) {
    printf("len = %ld\n", ft_strlen(argv[1]));

    char s[] = "";
    ft_strcpy(s, argv[1]);
    printf("dest =%s\n", s);
}