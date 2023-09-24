/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_absolute_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 12:14:49 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 13:01:02 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

int	get_cmd_count(int argc, char **argv)
{
	int	cmd_count;
	int	i;

	cmd_count = 0;
	if (is_specified_here_doc(argv))
		i = 3;
	else
		i = 2;
	while (i < argc - 1)
	{
		if (!access(argv[i], X_OK) || access(argv[i], F_OK))
			cmd_count++;
		i++;
	}
	return (cmd_count);
}

static void	get_cmd_name_from_arg(int argc, char **argv,
		char ***cmd_absolute_path)
{
	int	arg_i;
	int	cmd_i;

	*cmd_absolute_path = (char **)malloc(sizeof(char *) * (get_cmd_count(argc,
					argv) + 1));
	check_malloc(*cmd_absolute_path);
	if (is_specified_here_doc(argv))
		arg_i = 3;
	else
		arg_i = 2;
	cmd_i = 0;
	while (arg_i < argc - 1)
	{
		if (!access(argv[arg_i], X_OK) || access(argv[arg_i], F_OK))
		{
			cmd_absolute_path[0][cmd_i] = ft_substr(argv[arg_i], 0,
					strlen_until_c(argv[arg_i], ' '));
			check_malloc(cmd_absolute_path[0][cmd_i]);
			cmd_i++;
		}
		arg_i++;
	}
	cmd_absolute_path[0][cmd_i] = NULL;
}

void	get_cmd_option(int argc, char **argv, char ***cmd_absolute_path,
		char ***cmd_option)
{
	int	arg_i;
	int	cmd_i;

	*cmd_option = (char **)malloc(sizeof(char *) * (get_cmd_count(argc, argv)
				+ 1));
	check_malloc(*cmd_option);
	if (is_specified_here_doc(argv))
		arg_i = 3;
	else
		arg_i = 2;
	cmd_i = 0;
	while (arg_i < argc - 1)
	{
		if (!access(argv[arg_i], X_OK) || access(argv[arg_i], F_OK))
		{
			cmd_option[0][cmd_i] = ft_substr(argv[arg_i],
					ft_strlen(cmd_absolute_path[0][cmd_i]),
					ft_strlen(argv[arg_i]));
			check_malloc(cmd_option[0][cmd_i]);
			cmd_i++;
		}
		arg_i++;
	}
	cmd_option[0][cmd_i] = NULL;
}

static void	get_cmd_absolute_path_with_option(int argc, char **argv,
		char ***cmd_option, t_data *data)
{
	int	cmd_i;

	data->cmd_absolute_path_with_option = (char **)malloc(sizeof(char *)
			* (get_cmd_count(argc, argv) + 1));
	check_malloc(data->cmd_absolute_path_with_option);
	cmd_i = 0;
	while (data->cmd_absolute_path[cmd_i])
	{
		data->cmd_absolute_path_with_option[cmd_i] = \
		ft_strjoin(data->cmd_absolute_path[cmd_i],
				cmd_option[0][cmd_i]);
		cmd_i++;
	}
	data->cmd_absolute_path_with_option[cmd_i] = NULL;
}

void	get_cmd_absolute_path(int argc, char **argv, char **envp, t_data *data)
{
	char	**env_path;
	char	**cmd_option;

	get_env_path(&env_path, envp);
	get_cmd_name_from_arg(argc, argv, &data->cmd_absolute_path);
	get_cmd_option(argc, argv, &data->cmd_absolute_path, &cmd_option);
	add_absolute_path_to_cmd_name(&data->cmd_absolute_path, env_path, envp);
	get_cmd_absolute_path_with_option(argc, argv, &cmd_option, data);
	all_free_tab(env_path);
	all_free_tab(cmd_option);
}
