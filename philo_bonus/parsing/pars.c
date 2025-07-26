/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macbookpro <macbookpro@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:31:21 by macbookpro        #+#    #+#             */
/*   Updated: 2025/07/14 01:48:52 by macbookpro       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo_bonus.h"

int is_valid_arg(int ac, char **av)
{
    int i;
    int a;

    a = 1;
    while (a < ac)
    {
        i = 0;
        if (av[a][i] == '+')
            i++;
        if (av[a][i] == '\0')
            return (0);
        while (av[a][i])
        {
            if (av[a][i] > '9' || av[a][i] < '0')
                return (0);
            i++;
        }
        a++;
    }
    return (1);
}
