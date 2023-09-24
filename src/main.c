/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 11:29:32 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

// __attribute__((destructor)) static void destructor()
// {
// 	system("leaks -q pipex");
// }

void	put_error(char *err_msg)
{
	write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
}

void	check_close(int ret)
{
	if (ret < 0)
	{
		put_error("failed to close\n");
		exit(1);
	}
}

void	close_fd(int fd)
{
	int	ret;

	ret = close(fd);
	check_close(ret);
}

int	is_specified_here_doc(char **argv)
{
	return (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")));
}

void	all_free_tab(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

void	all_free_int(int **ptr)
{
	int	i;

	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
	ptr = NULL;
}

int	is_argc_valid(int argc, char **argv)
{
	if (argc < 5)
	{
		put_error("argc at least 5 as follows:\n./pipex infile cmd1 cmd2 outfile\n");
		return (FALSE);
	}
	if (is_specified_here_doc(argv) && argc < 6)
	{
		put_error("argc at least 6 as follows:\n./pipex here_doc LIMITTER cmd1 cmd2 outfile\n");
		return (FALSE);
	}
	return (TRUE);
}

int	open_file(char *file, int file_type)
{
	int	fd;

	if (file_type == INFILE)
		fd = open(file, O_RDONLY);
	else if (file_type == INFILE_HERE_DOC)
		fd = open(file, O_RDWR | O_CREAT | O_APPEND,
				S_IRWXU | S_IRWXG | S_IRWXO);
	else if (file_type == OUTFILE)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
				S_IRWXU | S_IRWXG | S_IRWXO);
	else if (file_type == OUTFILE_HERE_DOC)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND,
				S_IRWXU | S_IRWXG | S_IRWXO);
	else
		fd = -1;
	if (fd == -1)
	{
		put_error("unable to open file\n");
		exit(1);
	}
	return (fd);
}

int	is_infile_valid(char *infile)
{
	int	fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
	else
		fd = open_file(infile, INFILE);
	close_fd(fd);
	if (fd == -1)
		return (FALSE);
	return (TRUE);
}

int	is_outfile_valid(char *infile, char *outfile)
{
	int	fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(outfile, OUTFILE_HERE_DOC);
	else
		fd = open_file(outfile, OUTFILE);
	close_fd(fd);
	if (fd == -1)
		return (FALSE);
	return (TRUE);
}

char	*get_infile(char **argv)
{
	char	*infile;

	if (is_specified_here_doc(argv))
		infile = INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC;
	else
		infile = argv[1];
	return (infile);
}

char	*get_outfile(int argc, char **argv)
{
	return (argv[argc - 1]);
}

void	check_malloc(void *ptr)
{
	if (ptr == NULL)
	{
		put_error("failed to malloc\n");
		exit(1);
	}
}

void	add_slash_eos(char ***path)
{
	char	*tmp;
	int		i;

	i = 0;
	while (path[0][i])
	{
		tmp = path[0][i];
		path[0][i] = ft_strjoin(path[0][i], "/");
		free(tmp);
		i++;
	}
}

int	is_cmd_alreadly_absollute_path(char ***cmd_absolute_path, int cmd_i)
{
	if (cmd_absolute_path[0][cmd_i][0] == '/'
		&& ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return (TRUE);
	return (FALSE);
}

void	get_env_path(char ***env_path, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", ft_strlen("PATH=")))
		i++;
	if (envp[i] == NULL)
		put_error("PATH not found\n");
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
	if (envp[i] == NULL)
		put_error("PATH not found\n");
	*pwd_path = ft_split(envp[i] + ft_strlen("PWD="), ':');
	check_malloc(pwd_path);
	add_slash_eos(pwd_path);
}

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

size_t	strlen_until_c(char *str, char c)
{
	size_t	len;

	len = 0;
	while (str[len] != '\0' && str[len] != c)
		len++;
	return (len);
}

void	get_cmd_name_from_arg(int argc, char **argv, char ***cmd_absolute_path)
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

void	check_cmd(char *env_path)
{
	if (env_path == NULL)
	{
		put_error("command not found\n");
		exit(1);
	}
}

int	is_cmd_relative_path(char ***cmd_absolute_path, int cmd_i)
{
	if (cmd_absolute_path[0][cmd_i][0] != '/'
		&& ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return (TRUE);
	return (FALSE);
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
	return down_count_from_pwd;
}

char	*ft_strrnchr(const char *s, int c, int n)
{
	char	ch;
	int		i;
	int		count;

	ch = (char)c;
	i = ft_strlen(s);
	count = 0;
	while (i >= 0)
	{
		if (s[i] == ch)
		{
			if (count == n)
				return ((char *)&s[i]);
			else
				count++;
		}
		i--;
	}
	return (0);
}

char	*get_pwd_for_relative_path(char ***pwd_path, int down_count_from_pwd)
{
	int	delete_len;

	delete_len = ft_strlen(ft_strrnchr(pwd_path[0][0], '/', down_count_from_pwd)
			+ 1);
	return ft_substr(pwd_path[0][0], 0, ft_strlen(pwd_path[0][0]) - delete_len);
}

void	delete_relative_path(char ***cmd_absolute_path, int cmd_i)
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

void	convert_relative_path_to_absolute_path(char ***cmd_absolute_path,
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

void	add_absolute_path_to_cmd_name_from_env_path(char ***cmd_absolute_path,
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
	check_cmd(env_path[env_i]);
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
		add_absolute_path_to_cmd_name_from_env_path(cmd_absolute_path, env_path,
			cmd_i);
	}
}

void	get_cmd_absolute_path_with_option(int argc, char **argv,
		char ***cmd_option, t_data *data)
{
	int	cmd_i;

	data->cmd_absolute_path_with_option = (char **)malloc(sizeof(char *)
			* (get_cmd_count(argc, argv) + 1));
	check_malloc(data->cmd_absolute_path_with_option);
	cmd_i = 0;
	while (data->cmd_absolute_path[cmd_i])
	{
		data->cmd_absolute_path_with_option[cmd_i] = ft_strjoin(data->cmd_absolute_path[cmd_i],
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

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	proc_here_doc(char *limitter, int infile_fd)
{
	char	*line;
	char	*limitter_endl;

	line = get_next_line(STDIN_FILENO);
	limitter_endl = ft_strjoin(limitter, "\n");
	check_malloc(limitter_endl);
	while (ft_strncmp(line, limitter_endl, ft_strlen(line)))
	{
		ft_putstr_fd(line, infile_fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	free(limitter_endl);
	close_fd(infile_fd);
	infile_fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
}

int	is_argv_valid(int argc, char **argv)
{
	if (is_infile_valid(get_infile(argv)) == FALSE)
		return FALSE;
	if (is_outfile_valid(get_infile(argv), get_outfile(argc, argv)) == FALSE)
		return FALSE;
	return TRUE;
}

void	get_infile_fd(char **argv, int *infile_fd)
{
	if (is_specified_here_doc(argv))
	{
		*infile_fd = open_file(HERE_DOC_FILE_PATH, INFILE_HERE_DOC);
		proc_here_doc(argv[2], *infile_fd);
	}
	else
		*infile_fd = open_file(get_infile(argv), INFILE);
}

void	get_outfile_fd(int argc, char **argv, int *outfile_fd)
{
	if (is_specified_here_doc(argv))
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE_HERE_DOC);
	else
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE);
}

void	check_arg(int argc, char **argv)
{
	if (is_argc_valid(argc, argv) == FALSE)
		exit(1);
	if (is_argv_valid(argc, argv) == FALSE)
		exit(1);
}

void	check_pipe(int ret)
{
	if (ret < 0)
	{
		put_error("failed to create pipe\n");
		exit(1);
	}
}

void	get_pipe(t_data *data, int *cmd_i)
{
	int	ret;

	ret = pipe(data->pipe_fd[*cmd_i]);
	check_pipe(ret);
}

void	malloc_multiple_pipe(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	data->pipe_fd = (int **)malloc(sizeof(int *) * (get_cmd_count(argc, argv)
				+ 1));
	check_malloc(data->pipe_fd);
	while (i < get_cmd_count(argc, argv))
	{
		data->pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		check_malloc(data->pipe_fd);
		i++;
	}
	data->pipe_fd[i] = NULL;
}

void	get_data(int argc, char **argv, char **envp, t_data *data)
{
	get_cmd_absolute_path(argc, argv, envp, data);
	get_infile_fd(argv, &data->infile_fd);
	get_outfile_fd(argc, argv, &data->outfile_fd);
	malloc_multiple_pipe(argc, argv, data);
}

void	set_input_fd(t_data *data, int *cmd_i)
{
	if (*cmd_i == 0)
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
		if (dup2(data->pipe_fd[*cmd_i - 1][0], STDIN_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_fd(data->pipe_fd[*cmd_i - 1][0]);
		close_fd(data->pipe_fd[*cmd_i - 1][1]);
	}
}

void	set_output_fd(t_data *data, int *cmd_i)
{
	if (data->cmd_absolute_path[*cmd_i + 1] != NULL)
	{
		if (dup2(data->pipe_fd[*cmd_i][1], STDOUT_FILENO) == -1)
		{
			put_error("failed to dup\n");
			exit(1);
		}
		close_fd(data->pipe_fd[*cmd_i][0]);
		close_fd(data->pipe_fd[*cmd_i][1]);
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

void	exec_child(char **envp, t_data *data, int *cmd_i)
{
	char	**cmd;

	cmd = ft_split(data->cmd_absolute_path_with_option[*cmd_i], ' ');
	check_malloc(cmd);
	set_input_fd(data, cmd_i);
	set_output_fd(data, cmd_i);
	execve(data->cmd_absolute_path[*cmd_i], cmd, envp);
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

void	wait_children(int *cmd_i)
{
	int	ret;
	int	i;

	i = 0;
	while (i < *cmd_i)
	{
		ret = wait(NULL);
		check_wait(ret);
		i++;
	}
}

void	create_child(t_data *data)
{
	data->child_pid = fork();
	check_fork(data->child_pid);
}

void	pipex(char **envp, t_data *data)
{
	int	cmd_i;

	cmd_i = 0;
	while (data->cmd_absolute_path[cmd_i])
	{
		get_pipe(data, &cmd_i);
		create_child(data);
		if (data->child_pid == 0)
			exec_child(envp, data, &cmd_i);
		if (cmd_i > 0)
		{
			close_fd(data->pipe_fd[cmd_i - 1][0]);
			close_fd(data->pipe_fd[cmd_i - 1][1]);
		}
		cmd_i++;
	}
	wait_children(&cmd_i);
}

void	end_pipex(char **argv, t_data *data)
{
	all_free_tab(data->cmd_absolute_path);
	all_free_tab(data->cmd_absolute_path_with_option);
	all_free_int(data->pipe_fd);
	if (is_specified_here_doc(argv))
		unlink(HERE_DOC_FILE_PATH);
	else
		close_fd(data->infile_fd);
	close_fd(data->outfile_fd);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	check_arg(argc, argv);
	get_data(argc, argv, envp, &data);
	pipex(envp, &data);
	end_pipex(argv, &data);
}
