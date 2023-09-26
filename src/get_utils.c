/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/26 11:09:57 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	get_env_path(char ***env_path, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", ft_strlen("PATH=")))
		i++;
	check_is_path_found(envp[i]);
	*env_path = ft_split(envp[i] + ft_strlen("PATH="), ':');
	check_malloc(env_path);
	add_slash_eos(env_path);
}

void	get_pwd(char ***pwd_path, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PWD=", ft_strlen("PWD=")))
		i++;
	check_is_path_found(envp[i]);
	*pwd_path = ft_split(envp[i] + ft_strlen("PWD="), ':');
	check_malloc(pwd_path);
	add_slash_eos(pwd_path);
}

int	get_down_count_from_pwd(char *relative_path)
{
	int	down_count_from_pwd;

	down_count_from_pwd = 0;
	while (ft_strnstr(relative_path, "../", ft_strlen(relative_path)))
	{
		down_count_from_pwd++;
		relative_path += ft_strlen("../");
	}
	return (down_count_from_pwd);
}

char	*get_pwd_for_relative_path(char ***pwd_path, int down_count_from_pwd)
{
	int	delete_len;

	delete_len = ft_strlen(ft_strrnchr(pwd_path[0][0], '/', down_count_from_pwd)
			+ 1);
	return (ft_substr(pwd_path[0][0], 0, ft_strlen(pwd_path[0][0])
			- delete_len));
}

void	get_pipe(t_data *data, int cmd_i)
{
	int	ret;

	ret = pipe(data->pipe_fd[cmd_i]);
	check_pipe(ret);
}
