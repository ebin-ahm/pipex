/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 13:17:17 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/04/04 16:55:39 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_pipex pipex, char *cmd)
{
	char	**args;
	char	*path;

	dup2(pipex.infile, 0);
	close(pipex.infile);
	dup2(pipex.fd[1], 1);
	close(pipex.fd[1]);
	close(pipex.fd[0]);
	args = ft_split(cmd, ' ');
	path = find_path(args[0], pipex.envp);
	if (path == NULL)
	{
		write(2, "command not found\n", 18);
		exit(127);
	}
	execve(path, args, pipex.envp);
	perror("execve");
	exit(127);
}

void	child_two(t_pipex pipex, char *cmd)
{
	char	**args;
	char	*path;

	dup2(pipex.fd[0], 0);
	close(pipex.fd[0]);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	close(pipex.fd[1]);
	args = ft_split(cmd, ' ');
	path = find_path(args[0], pipex.envp);
	if (path == NULL)
	{
		write(2, "command not found\n", 18);
		exit(127);
	}
	execve(path, args, pipex.envp);
	perror("execve");
	exit(127);
}
