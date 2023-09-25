/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_func_is_worked.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/25 13:49:54 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	check_open(int fd)
{
	if (fd == -1)
	{
		put_error("failed to open\n");
		exit(1);
	}
}

void	check_close(int ret)
{
	if (ret < 0)
	{
		put_error("failed to close\n");
		exit(1);
	}
}

void	check_pipe(int ret)
{
	if (ret < 0)
	{
		put_error("failed to create pipe\n");
		exit(1);
	}
}

void	check_fork(pid_t child_pid)
{
	if (child_pid < 0)
	{
		put_error("failed to fork\n");
		exit(1);
	}
}

void	check_wait(int ret)
{
	if (ret == -1)
	{
		put_error("failed to wait\n");
		exit(1);
	}
}
