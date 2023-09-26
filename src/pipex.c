/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/26 09:57:21 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	wait_children(int cmd_i)
{
	int	i;

	i = 0;
	while (i < cmd_i)
	{
		check_wait(wait(NULL));
		i++;
	}
}

static void	set_input_fd(t_data *data, int cmd_i)
{
	if (cmd_i == 0)
	{
		check_dup(dup2(data->infile_fd, STDIN_FILENO));
		check_close(close(data->infile_fd));
	}
	else
	{
		check_dup(dup2(data->pipe_fd[cmd_i - 1][0], STDIN_FILENO));
		close_pipe(data->pipe_fd[cmd_i - 1]);
	}
}

static void	set_output_fd(t_data *data, int cmd_i)
{
	if (data->cmd_absolute_path[cmd_i + 1] != NULL)
	{
		check_dup(dup2(data->pipe_fd[cmd_i][1], STDOUT_FILENO));
		close_pipe(data->pipe_fd[cmd_i]);
	}
	else
	{
		check_dup(dup2(data->outfile_fd, STDOUT_FILENO));
		check_close(close(data->outfile_fd));
	}
}

static void	exec_child(char **envp, t_data *data, int cmd_i)
{
	char	**cmd;

	cmd = ft_split(data->cmd_absolute_path_with_option[cmd_i], ' ');
	check_malloc(cmd);
	set_input_fd(data, cmd_i);
	set_output_fd(data, cmd_i);
	check_execve(execve(data->cmd_absolute_path[cmd_i], cmd, envp));
}

void	pipex(char **envp, t_data *data)
{
	int		cmd_i;
	pid_t	child_pid;

	cmd_i = 0;
	while (data->cmd_absolute_path[cmd_i])
	{
		get_pipe(data, cmd_i);
		child_pid = fork();
		check_fork(child_pid);
		if (child_pid == 0)
			exec_child(envp, data, cmd_i);
		if (cmd_i > 0)
			close_pipe(data->pipe_fd[cmd_i - 1]);
		cmd_i++;
	}
	wait_children(cmd_i);
}
