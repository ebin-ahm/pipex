/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:27:06 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/04/04 16:54:03 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	get_length(char *str)
{
	int	length;

	length = 0;
	while (str[length])
		length++;
	return (length);
}

char	*ft_strjoin(char *str1, char *str2)
{
	char	*result;
	int		length1;
	int		length2;
	int		index;

	length1 = get_length(str1);
	length2 = get_length(str2);
	result = malloc(sizeof(char) * (length1 + length2 + 1));
	if (!result)
		return (NULL);
	index = 0;
	while (index < length1)
	{
		result[index] = str1[index];
		index++;
	}
	while (index < length1 + length2)
	{
		result[index] = str2[index - length1];
		index++;
	}
	result[index] = '\0';
	return (result);
}

int	ft_strncmp(char *str1, char *str2, int length)
{
	int	index;

	index = 0;
	while (index < length)
	{
		if (str1[index] != str2[index])
			return (str1[index] - str2[index]);
		index++;
	}
	return (0);
}
