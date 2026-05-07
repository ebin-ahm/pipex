/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 13:28:36 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/04/04 16:54:25 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
		perror(argv[1]);
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		perror(argv[4]);
}

static void	create_pipe(t_pipex *pipex)
{
	if (pipe(pipex->fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
}

static void	wait_children(pid_t pid1, pid_t pid2)
{
	int	status;

	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	exit(WEXITSTATUS(status));
}

static void	fork_children(t_pipex pipex, char **argv)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid1 == 0)
		child_one(pipex, argv[2]);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pid2 == 0)
		child_two(pipex, argv[3]);
	close(pipex.fd[0]);
	close(pipex.fd[1]);
	close(pipex.infile);
	close(pipex.outfile);
	wait_children(pid1, pid2);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
	{
		write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 40);
		exit(1);
	}
	pipex.envp = envp;
	open_files(&pipex, argv);
	create_pipe(&pipex);
	fork_children(pipex, argv);
	return (0);
}
