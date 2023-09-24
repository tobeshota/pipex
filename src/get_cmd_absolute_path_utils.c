/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_absolute_path_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 12:57:22 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

static void	check_cmd_exist(char *env_path)
{
	if (env_path == NULL)
	{
		put_error("command not found\n");
		exit(1);
	}
}

static void	add_absolute_from_env_path(char ***cmd_absolute_path,
		char **env_path, int cmd_i)
{
	int		env_i;
	char	*tmp;

	env_i = 0;
	while (env_path[env_i])
	{
		tmp = ft_strjoin(env_path[env_i], cmd_absolute_path[0][cmd_i]);
		if (!access(tmp, X_OK))
		{
			free(cmd_absolute_path[0][cmd_i]);
			cmd_absolute_path[0][cmd_i] = ft_strdup(tmp);
			free(tmp);
			break ;
		}
		free(tmp);
		env_i++;
	}
	check_cmd_exist(env_path[env_i]);
}

static void	delete_relative_path(char ***cmd_absolute_path, int cmd_i)
{
	char	*tmp;

	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strtrim(cmd_absolute_path[0][cmd_i],
			"../");
	free(tmp);
	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strtrim(cmd_absolute_path[0][cmd_i], "./");
	free(tmp);
}

static void	convert_relative_path_to_absolute_path(char ***cmd_absolute_path,
		int cmd_i, char **envp)
{
	char	*tmp;
	char	**pwd;
	char	*pwd_for_relative_path;

	get_pwd(&pwd, envp);
	pwd_for_relative_path = get_pwd_for_relative_path(&pwd,
			get_down_count_from_pwd(cmd_absolute_path[0][cmd_i]));
	delete_relative_path(cmd_absolute_path, cmd_i);
	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strjoin(pwd_for_relative_path,
			cmd_absolute_path[0][cmd_i]);
	free(tmp);
	all_free_tab(pwd);
	free(pwd_for_relative_path);
}

void	add_absolute_path_to_cmd_name(char ***cmd_absolute_path,
		char **env_path, char **envp)
{
	int	cmd_i;

	cmd_i = -1;
	while (cmd_absolute_path[0][++cmd_i])
	{
		if (is_cmd_relative_path(cmd_absolute_path, cmd_i))
		{
			convert_relative_path_to_absolute_path(cmd_absolute_path, cmd_i,
				envp);
			continue ;
		}
		if (is_cmd_alreadly_absollute_path(cmd_absolute_path, cmd_i))
			continue ;
		add_absolute_from_env_path(cmd_absolute_path, env_path,
			cmd_i);
	}
}
