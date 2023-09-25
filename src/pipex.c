/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 14:13:31 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	wait_children(int cmd_i)
{
	int	ret;
	int	i;

	i = 0;
	while (i < cmd_i)
	{
		ret = wait(NULL);
		check_wait(ret);
		i++;
	}
}

static void	set_input_fd(t_data *data, int cmd_i)
{
	if (cmd_i == 0)
	{
		if (dup2(data->infile_fd, STDIN_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_fd(data->infile_fd);
	}
	else
	{
		if (dup2(data->pipe_fd[cmd_i - 1][0], STDIN_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_pipe(data->pipe_fd[cmd_i - 1]);
	}
}

static void	set_output_fd(t_data *data, int cmd_i)
{
	if (data->cmd_absolute_path[cmd_i + 1] != NULL)
	{
		if (dup2(data->pipe_fd[cmd_i][1], STDOUT_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_pipe(data->pipe_fd[cmd_i]);
	}
	else
	{
		if (dup2(data->outfile_fd, STDOUT_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_fd(data->outfile_fd);
	}
}

static void	exec_child(char **envp, t_data *data, int cmd_i)
{
	char	**cmd;

	cmd = ft_split(data->cmd_absolute_path_with_option[cmd_i], ' ');
	check_malloc(cmd);
	set_input_fd(data, cmd_i);
	set_output_fd(data, cmd_i);
	execve(data->cmd_absolute_path[cmd_i], cmd, envp);
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
