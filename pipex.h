/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 09:29:26 by toshota           #+#    #+#             */
/*   Updated: 2023/09/26 11:06:54 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/types.h>

typedef struct s_data
{
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

int			open_file(char *file, int file_type);
void		close_pipe(int *pipe_fd);
void		add_slash_eos(char ***path);
void		all_free_tab(char **ptr);
void		put_error(char *err_msg);
char		*ft_strrnchr(const char *s, int c, int n);
size_t		strlen_until_c(char *str, char c);
void		check_malloc(void *ptr);
void		check_open(int ret);
void		check_close(int ret);
void		check_pipe(int ret);
void		check_fork(pid_t child_pid);
void		check_dup(int ret);
void		check_execve(int ret);
void		check_wait(int ret);
void		check_unlink(int ret);
void		check_arg(int argc, char **argv);
void		check_is_path_found(char *path);
int			get_cmd_count(int argc, char **argv);
int			is_cmd(char *str);
void		get_cmd_option(int argc, char **argv, char ***cmd_absolute_path,
				char ***cmd_option);
void		get_cmd_absolute_path(int argc, char **argv, char **envp,
				t_data *data);
void		add_absolute_path_to_cmd_name(char ***cmd_absolute_path,
				char **env_path, char **envp);
void		get_env_path(char ***env_path, char **envp);
void		get_pwd(char ***pwd_path, char **envp);
int			get_down_count_from_pwd(char *relative_path);
char		*get_pwd_for_relative_path(char ***pwd_path,
				int down_count_from_pwd);
void		get_pipe(t_data *data, int cmd_i);
char		*get_infile(char **argv);
char		*get_outfile(int argc, char **argv);
int			is_specified_here_doc(char **argv);
void		proc_here_doc(char *limitter, int infile_fd);
int			is_cmd_relative_path(char ***cmd_absolute_path, int cmd_i);
int			is_cmd_alreadly_absollute_path(char ***cmd_absolute_path,
				int cmd_i);
void		pipex(char **envp, t_data *data);
void		get_data(int argc, char **argv, char **envp, t_data *data);
void		end_pipex(char **argv, t_data *data);

#endif
