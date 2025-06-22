/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouknan <abouknan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 03:07:32 by abouknan          #+#    #+#             */
/*   Updated: 2025/06/21 21:03:56 by abouknan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

char	*ft_strjoin(char *s1, char *s2);

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	**ft_split(char *str)
{
	int		i;
	int		k;
	char	**out;
	int		j;

	i = 0;
	k = 0;
	out = malloc(sizeof(char *) * 1000);
	if (!out)
		return (NULL);
	while (str[i])
	{
		while (str[i] && (str[i] == ' '))
			i++;
		if (str[i])
		{
			j = 0;
			out[k] = malloc(1000);
			while (str[i] && !(str[i] == ' '))
				out[k][j++] = str[i++];
			out[k++][j] = '\0';
		}
	}
	out[k] = NULL;
	return (out);
}

char	*join_function(int ac, char **av)
{
	char	*tmp;
	char	*str;
	int		a;

	a = 1;
	str = NULL;
	while (a < ac)
	{
		tmp = str;
		str = ft_strjoin(tmp, av[a]);
		if (!str)
			return (NULL);
		if (a < ac - 1)
		{
			tmp = str;
			str = ft_strjoin(tmp, " ");
			if (!str)
				return (NULL);
		}
		a++;
	}
	return (str);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		total_len;
	char	*str;
	int		i;
	int		j;

	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		return (NULL);
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	str = (char *)malloc(total_len * sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	return (str);
}

int	valid_arguments(int ac, char **av)
{
	int	a;
	int	i;

	a = 0;
	while (a < ac - 1)
	{
		i = 0;
		if (av[a][i] == '+')
			i++;
		if (av[a][i] == '\0')
			return (0);
		while (av[a][i])
		{
			if (av[a][i] < '0' || av[a][i] > '9')
				return (0);
			i++;
		}
		a++;
	}
	return (1);
}
