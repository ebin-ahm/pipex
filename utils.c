/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 15:07:49 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/04/04 16:54:02 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(char *str, char sep)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*str)
	{
		if (*str != sep && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == sep)
			in_word = 0;
		str++;
	}
	return (count);
}

static char	*extract_word2(char *str, int start, int end)
{
	char	*word;
	int		index;

	word = malloc(sizeof(char) * (end - start + 1));
	if (!word)
		return (NULL);
	index = 0;
	while (start < end)
	{
		word[index] = str[start];
		index++;
		start++;
	}
	word[index] = '\0';
	return (word);
}

char	**ft_split(char *str, char sep)
{
	char	**result;
	int		word_count;
	int		index;
	int		start;
	int		result_index;

	word_count = count_words(str, sep);
	result = malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	index = 0;
	result_index = 0;
	while (result_index < word_count)
	{
		while (str[index] && str[index] == sep)
			index++;
		start = index;
		while (str[index] && str[index] != sep)
			index++;
		result[result_index++] = extract_word2(str, start, index);
	}
	result[result_index] = NULL;
	return (result);
}
