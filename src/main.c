/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/17 15:07:24 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
cc -Wall -Wextra -Werror main.c ../libft/libft.a -o pipex
./pipex infile cmd1 cmd2 outfile
*/
#include "../libft/libft.h"
#include "../pipex.h"

void	all_free(char **ptr)
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

void	put_error(char *err_msg)
{
	write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
}

int	is_argc_valid(int argc)
{
	if (argc < 5)
	{
		put_error(TOO_FEW_ARGC_ERROR);
		return FALSE;
	}
	return TRUE;
}

int	is_file_readable(char *file)
{
	if (access(file, R_OK))
	{
		put_error(PERMISSION_DENIED_ERROR);
		return FALSE;
	}
	return TRUE;
}

int	is_file_writable(char *file)
{
	if (access(file, W_OK))
	{
		put_error(PERMISSION_DENIED_ERROR);
		return FALSE;
	}
	return TRUE;
}

int is_file_exist(char *file)
{
	if (access(file, F_OK))
	{
		put_error(FILE_EXIST_ERROR);
		return FALSE;
	}
	return TRUE;
}

int	is_file_openable(char *file)
{
	int	fd;

	fd = open(file, O_RDWR);
	close(fd);
	if (fd == -1)
	{
		put_error(FILE_OPEN_ERROR);
		return FALSE;
	}
	return TRUE;
}

int	is_infile_valid(char *infile)
{
	if(is_file_openable(infile) == FALSE)
		return FALSE;
	if(is_file_readable(infile) == FALSE)
		return FALSE;
	return TRUE;
}

int	is_outfile_valid(char *outfile)
{
	if(is_file_openable(outfile) == FALSE)
		return FALSE;
	if(is_file_writable(outfile) == FALSE)
		return FALSE;
	return TRUE;
}

int is_specified_here_doc(char **argv)
{
	return (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")));
}


char *get_infile(char **argv)
{
	char *infile;

	if (is_specified_here_doc(argv))
	{
		if(is_file_exist(argv[3]))
			infile = argv[3];
		else
			infile = INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC;
	}
	else
		infile = argv[1];
	return infile;
}

char *get_outfile(char argc, char **argv)
{
	char *outfile;

	outfile = argv[argc - 1];
	return outfile;
}

/* ■ファイルおよびコマンドは適切なものであるかを確かめる
 * 	・入力用ファイルは読み取り可能であり，かつ，ディレクトリでないかを確かめる
 * 	・出力用ファイルは書き込み可能であり，かつ，ディレクトリでないかを確かめる
 * 	・コマンドが存在するかを確かめる（command not foundとならないかを調べる）　◀︎━ここからやる！
 */
int	is_argv_valid(int argc, char **argv)
{
	char *infile;
	char *outfile;

	infile = get_infile(argv);
	outfile = get_outfile(argc, argv);

	if (infile != INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		if(is_infile_valid(infile) == FALSE)
			return FALSE;
	if(is_outfile_valid(outfile) == FALSE)
		return FALSE;
	// if(is_valid_cmd() == FALSE)
	// 	return FALSE;
	return TRUE;
}

/* コマンドライン引数が適切であるかを確かめる
 * ・コマンドライン引数の数は5個以上あるかを確かめる
 * ・ファイルおよびコマンドは適切なものであるかを確かめる
 */
void	check_arg(int argc, char **argv)
{
	if(is_argc_valid(argc) == FALSE)
		exit(1);
	if(is_argv_valid(argc, argv) == FALSE)
		exit(1);
}

void	check_malloc(void *ptr)
{
	if (ptr == NULL)
		put_error(MALLOC_ERROR);
}

void	get_env_path(char ***env_path, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", ft_strlen("PATH=")))
		i++;
	if (envp[i] == NULL)
		put_error(PATH_EXIST_ERROR);
	*env_path = ft_split(envp[i] + ft_strlen("PATH="), ':');
	check_malloc(env_path);
}

// char **envpによって環境変数を受け取ることができる
int	main(int argc, char **argv, char **envp)
{
	char	**env_path;

	// 環境変数のポインタenvpからbin_pathを取得する
	get_env_path(&env_path, envp);
	check_arg(argc, argv);
// int i;
// i = 0;
// while(env_path[i])
// {
// 	ft_printf("%s\n", env_path[i]);
// 	i++;
// }
	// pipexとしての処理をする
	// pipex(argc, argv, envp, bin_path);
	all_free(env_path);
}

__attribute__((destructor)) static void destructor()
{
	system("leaks -q pipex");
}
