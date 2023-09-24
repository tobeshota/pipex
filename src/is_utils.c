/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 12:40:03 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	is_cmd_relative_path(char ***cmd_absolute_path, int cmd_i)
{
	if (cmd_absolute_path[0][cmd_i][0] != '/'
		&& ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return (TRUE);
	return (FALSE);
}

int	is_cmd_alreadly_absollute_path(char ***cmd_absolute_path, int cmd_i)
{
	if (cmd_absolute_path[0][cmd_i][0] == '/'
		&& ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return (TRUE);
	return (FALSE);
}
