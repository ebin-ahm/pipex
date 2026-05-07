/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 17:52:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/05/07 17:52:52 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*try_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full_path;

	tmp = ft_strjoin(dir, "/");
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static void	free_split(char **dirs)
{
	int	index;

	index = 0;
	while (dirs[index])
		free(dirs[index++]);
	free(dirs);
}

static char	*search_dirs(char **dirs, char *cmd)
{
	char	*result;
	int		index;

	index = 0;
	while (dirs[index])
	{
		result = try_path(dirs[index], cmd);
		if (result)
		{
			free_split(dirs);
			return (result);
		}
		index++;
	}
	free_split(dirs);
	return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	**dirs;
	int		index;

	index = 0;
	while (envp[index] && ft_strncmp(envp[index], "PATH=", 5) != 0)
		index++;
	if (!envp[index])
		return (NULL);
	dirs = ft_split(envp[index] + 5, ':');
	if (!dirs)
		return (NULL);
	return (search_dirs(dirs, cmd));
}
