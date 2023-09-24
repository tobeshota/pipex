/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:29:26 by toshota           #+#    #+#             */
/*   Updated: 2023/09/24 12:07:53 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/types.h>

typedef struct s_data
{
	pid_t	child_pid;
	int		infile_fd;
	int		outfile_fd;
	char	**cmd_absolute_path;
	char	**cmd_absolute_path_with_option;
	int		**pipe_fd;
}			t_data;

# define TRUE 1
# define FALSE 0
# define HERE_DOC_FILE_PATH "/tmp/here_doc"
# define INFILE 0
# define INFILE_HERE_DOC 1
# define OUTFILE 2
# define OUTFILE_HERE_DOC 3
# define INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC 0

// utils2.c
int			open_file(char *file, int file_type);
void		close_fd(int fd);
void		add_slash_eos(char ***path);
void		malloc_multiple_pipe(int argc, char **argv, t_data *data);

// utils1.c
void		all_free_tab(char **ptr);
void		all_free_int(int **ptr);
void		put_error(char *err_msg);
char		*ft_strrnchr(const char *s, int c, int n);
size_t		strlen_until_c(char *str, char c);

// check_func_is_worked.c
void		check_malloc(void *ptr);
void		check_pipe(int ret);
void		check_fork(pid_t child_pid);
void		check_close(int ret);
void		check_wait(int ret);

// get_cmd_absolute_path.c
int			get_cmd_count(int argc, char **argv);
void		get_cmd_name_from_arg(int argc, char **argv,
				char ***cmd_absolute_path);
void		get_cmd_option(int argc, char **argv, char ***cmd_absolute_path,
				char ***cmd_option);
void		get_cmd_absolute_path(int argc, char **argv, char **envp,
				t_data *data);
void		get_cmd_absolute_path_with_option(int argc, char **argv,
				char ***cmd_option, t_data *data);

// get_cmd_absolute_path_utils.c
void		add_absolute_path_to_cmd_name(char ***cmd_absolute_path,
				char **env_path, char **envp);
void		add_absolute_path_to_cmd_name_from_env_path(char ***cmd_absolute_path,
				char **env_path, int cmd_i);
void		convert_relative_path_to_absolute_path(char ***cmd_absolute_path,
				int cmd_i, char **envp);
void		delete_relative_path(char ***cmd_absolute_path, int cmd_i);
void		check_cmd_exist(char *env_path);

// get_utils.c
void		get_env_path(char ***env_path, char **envp);
void		get_pwd(char ***pwd_path, char **envp);
int			get_down_count_from_pwd(char *relative_path);
char		*get_pwd_for_relative_path(char ***pwd_path,
				int down_count_from_pwd);
void		get_pipe(t_data *data, int *cmd_i);

// get_file.c
char		*get_infile(char **argv);
char		*get_outfile(int argc, char **argv);
void		get_infile_fd(char **argv, int *infile_fd);
void		get_outfile_fd(int argc, char **argv, int *outfile_fd);

// is_arg_valid.c
int			is_argc_valid(int argc, char **argv);
int			is_argv_valid(int argc, char **argv);
int			is_infile_valid(char *infile);
int			is_outfile_valid(char *infile, char *outfile);

// here_doc.c
int			is_specified_here_doc(char **argv);
void		proc_here_doc(char *limitter, int infile_fd);

// is_utils.c
int			is_cmd_relative_path(char ***cmd_absolute_path, int cmd_i);
int			is_cmd_alreadly_absollute_path(char ***cmd_absolute_path,
				int cmd_i);

// pipex.c
void		create_child(t_data *data);
void		wait_children(int *cmd_i);
void		exec_child(char **envp, t_data *data, int *cmd_i);
void		set_input_fd(t_data *data, int *cmd_i);
void		set_output_fd(t_data *data, int *cmd_i);

// main.c
void		check_arg(int argc, char **argv);
void		get_data(int argc, char **argv, char **envp, t_data *data);
void		pipex(char **envp, t_data *data);
void		end_pipex(char **argv, t_data *data);

#endif
