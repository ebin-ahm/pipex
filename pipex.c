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

static void	free_args(char **args)
{
	int	index;

	index = 0;
	if (!args)
		return ;
	while (args[index])
		free(args[index++]);
	free(args);
}

static void	check_fds(t_pipex pipex, int fd)
{
	if (fd == -1)
	{
		close(pipex.fd[0]);
		close(pipex.fd[1]);
		exit(1);
	}
}

static void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;

	args = ft_split(cmd, ' ');
	if (!args || !args[0])
	{
		free_args(args);
		write(2, "command not found\n", 18);
		exit(127);
	}
	path = find_path(args[0], envp);
	if (!path)
	{
		free_args(args);
		write(2, "command not found\n", 18);
		exit(127);
	}
	execve(path, args, envp);
	perror("execve");
	exit(127);
}

void	child_one(t_pipex pipex, char *cmd)
{
	check_fds(pipex, pipex.infile);
	dup2(pipex.infile, 0);
	close(pipex.infile);
	dup2(pipex.fd[1], 1);
	close(pipex.fd[1]);
	close(pipex.fd[0]);
	exec_cmd(cmd, pipex.envp);
}

void	child_two(t_pipex pipex, char *cmd)
{
	check_fds(pipex, pipex.outfile);
	dup2(pipex.fd[0], 0);
	close(pipex.fd[0]);
	dup2(pipex.outfile, 1);
	close(pipex.outfile);
	close(pipex.fd[1]);
	exec_cmd(cmd, pipex.envp);
}
