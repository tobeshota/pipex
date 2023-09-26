/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_func2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 10:03:29 by toshota           #+#    #+#             */
/*   Updated: 2023/09/26 11:32:15 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	check_dup(int ret)
{
	if (ret == -1)
	{
		put_error("failed to dup\n");
		exit(1);
	}
}

void	check_execve(int ret)
{
	if (ret == -1)
	{
		put_error("failed to execve\n");
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

void	check_unlink(int ret)
{
	if (ret == -1)
	{
		put_error("failed to unlink\n");
		exit(1);
	}
}
