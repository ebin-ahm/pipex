/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/04 13:17:43 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/04/04 16:54:00 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <stdio.h>
# include <string.h>

typedef struct s_pipex
{
	int		fd[2];
	int		infile;
	int		outfile;
	char	**envp;
}	t_pipex;

void	child_one(t_pipex pipex, char *cmd);
void	child_two(t_pipex pipex, char *cmd);
char	*find_path(char *cmd, char **envp);
char	**ft_split(char *str, char sep);
char	*ft_strjoin(char *str1, char *str2);
int		ft_strncmp(char *str1, char *str2, int length);
void	ft_strlcpy(char *dst, char *src, int size);

#endif
