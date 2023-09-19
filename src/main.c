/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/19 15:55:41 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
gcc -g main.c ../libft/libft.a -o pipex

cc -Wall -Wextra -Werror main.c ../libft/libft.a -o pipex
./pipex infile "ls -a" cat outfile
./pipex here_doc wow ls cat outfile
*/
#include "../libft/libft.h"
#include "../pipex.h"

__attribute__((destructor)) static void destructor()
{
	system("leaks -q pipex");
}

int is_specified_here_doc(char **argv)
{
	return (!ft_strncmp(argv[1], "here_doc", ft_strlen("here_doc")));
}

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

int	is_argc_valid(int argc, char **argv)
{
	if (is_specified_here_doc(argv))
	{
		if (argc < 6)
		{
			put_error(TOO_FEW_ARGC_ERROR_IN_BONUS);
			return FALSE;
		}
	}
	else
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

char *get_infile(char **argv)
{
	char *infile;

	if (is_specified_here_doc(argv))
		infile = INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC;
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
 * 	・コマンドが存在するかを確かめる（command not foundとならないかを調べる）
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
	return TRUE;
}

/* コマンドライン引数が適切であるかを確かめる
 * ・コマンドライン引数の数は5個以上あるかを確かめる
 * ・ファイルおよびコマンドは適切なものであるかを確かめる
 */
void	check_arg(int argc, char **argv)
{
	if(is_argc_valid(argc, argv) == FALSE)
		exit(1);
	if(is_argv_valid(argc, argv) == FALSE)
		exit(1);
}

void	check_malloc(void *ptr)
{
	if (ptr == NULL)
	{
		put_error(MALLOC_ERROR);
		exit(1);
	}
}

// env_pathそれぞれの文字列の終わりに"/"を加える．
void add_slash_eos(char ***env_path)
{
	char *tmp;
	int i;

	i = 0;
	while(env_path[0][i])
	{
		tmp = env_path[0][i];
		env_path[0][i] = ft_strjoin(env_path[0][i], "/");
		free(tmp);
		i++;
	}
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
	add_slash_eos(env_path);
}

// コマンドの数を取得する
int get_cmd_count(int argc, char **argv)
{
	int cmd_count;
	int i;

	cmd_count = 0;
	if (is_specified_here_doc(argv))
		i = 3;
	else
		i = 2;
	while (i < argc - 1)
	{
		if (access(argv[i], F_OK))
			cmd_count++;
		i++;
	}
	return cmd_count;
}

size_t strlen_until_c(char *str, char c)
{
	size_t len;
	len = 0;
	while(str[len] != '\0' && str[len] != c)
		len++;
	return len;
}

/* コマンドライン引数からcmdであるべきものを取得する
 * "here_doc"がない場合，argv[0](実行ファイル)，argv[1](infile)，argv[argc-1](outfile)以外を「cmdであるべきもの」として取得する
 * "here_doc"がある場合，argv[0](実行ファイル)，argv[1](here_doc)，argv[2](LIMITTER)，argv[argc-1](outfile)以外であり，かつ，F_OKに失敗したものを「cmdであるべきもの」として取得する
 */
void get_cmd_name_from_arg(char ***cmd_absolute_path, int argc, char **argv)
{
	int arg_i;
	int cmd_i;

	*cmd_absolute_path = (char **)malloc(sizeof(char *) * (get_cmd_count(argc, argv) + 1));
	check_malloc(*cmd_absolute_path);
	if (is_specified_here_doc(argv))
		arg_i = 3;
	else
		arg_i = 2;
	cmd_i = 0;
	while (arg_i < argc - 1)
	{
		if (access(argv[arg_i], F_OK))
		{
			cmd_absolute_path[0][cmd_i] = ft_substr(argv[arg_i], 0, strlen_until_c(argv[arg_i], ' '));
			check_malloc(cmd_absolute_path[0][cmd_i]);
			cmd_i++;
		}
		arg_i++;
	}
	cmd_absolute_path[0][cmd_i] = NULL;
}

void check_cmd(char *env_path)
{
	if (env_path == NULL)
	{
		put_error(CMD_ERROR);
		exit(1);
	}
}

/* cmdにパスを加える
 * cmd一つ一つに対して，にenv_pathを一つずつ結合していく．
 * 結合したものが実行可能であるならば，それをコマンドの絶対パスとして返す.
 * 実行可能なenv_pathが見つからなければ，エラーとする．
 */
void add_absolute_path_to_cmd_name(char ***cmd_absolute_path, char **env_path)
{
	int cmd_i;
	int env_i;
	char *tmp;

	cmd_i = 0;
	while(cmd_absolute_path[0][cmd_i])
	{
		env_i = 0;
		while (env_path[env_i])
		{
			tmp = ft_strjoin(env_path[env_i], cmd_absolute_path[0][cmd_i]);
			if (!access(tmp, X_OK))
			{
				free(cmd_absolute_path[0][cmd_i]);
				cmd_absolute_path[0][cmd_i] = ft_strdup(tmp);
				free(tmp);
				break;
			}
			free(tmp);
			env_i++;
		}
		check_cmd(env_path[env_i]);
		cmd_i++;
	}
}

/*コマンドライン引数からcmdの絶対パスを取得する
 * ・環境変数のポインタenvpからbin_pathを取得する
 * ・cmdであるべきものを取得する
 * ・cmdにパスを加える
 */
void	get_cmd_absolute_path(char ***cmd_absolute_path, int argc, char **argv, char **envp)
{
	char	**env_path;

	get_env_path(&env_path, envp);
// env_path = ft_split("PATH=/Library/Frameworks/Python.framework/Versions/3.6/bin/:/Users/tobeshota/anaconda3/condabin/:/opt/homebrew/opt/node@18/bin/:/Users/tobeshota/.cargo/bin/:/usr/local/Qt-5.15.10/bin/:/opt/homebrew/opt/pyqt@5/5 5.15.7_2/bin/:/opt/homebrew/opt/qt@5/bin/:/Users/tobeshota/.nodebrew/current/bin/:/Users/tobeshota/.pyenv/shims/:/Users/tobeshota/.pyenv/bin/:/Library/Frameworks/Python.framework/Versions/3.10/bin/:/usr/local/bin/:/usr/bin/:/bin/:/usr/sbin/:/sbin/:/opt/X11/bin/:/Users/tobeshota/workspace/command", ':');
	get_cmd_name_from_arg(cmd_absolute_path, argc, argv);
	add_absolute_path_to_cmd_name(cmd_absolute_path, env_path);
	all_free(env_path);
}

/* 必要となる引数
 * ■execve
 * 	・環境変数
 * 	・コマンドの絶対パス
 * 	・コマンド(+オプション)(+ファイル)
 * ■
 */
void pipex(int argc, char **argv, char **envp, char **cmd_absolute_path)
{
	// p_fd[0]およびp_fd[1]を用いるためにpipeを開く．
	// pipeおよびexecveを用いるためにforkで現在のプロセス（親プロセス）を複製して新しいプロセス（子プロセス）を生成する．
	// forkの出力値が0より小さい数だったら（子プロセスの生成に失敗したら），エラー終了する．
	// forkの出力値が0だったら（子プロセスのpidが渡されたら），子プロセスを実行する．
		// コマンドの入力先を指定する（コマンドの入力先fdを標準入力に変える）．
			// はじめ　infile_fd					を標準入力に変える
			// いつも　p_fd[0]（パイプの読み取り側）	を標準入力に変える
		// コマンドの出力先fdを指定する（コマンドの出力先fdを標準出力に変える）
			// いつも　p_fd[1]（パイプの書き込み側）	を標準出力に変える
			// おわり　outfile_fd					を標準出力に変える
		// execveでコマンドを実行する．
	// forkの出力値が0より大きい数だったら（親プロセスのpidが渡されたら），親プロセスを実行する．
}

void proc_here_doc(char **argv)
{
	// p_fd[1]を用いるためにpipeを開く．パイプの書き込み側に書き込まれたデータはパイプの読み出し側から読み出されるまでカーネルでバッファリグされる．
	// pipeを用いるためにfork()で現在のプロセス（親プロセス）を複製して新しいプロセス（子プロセス）を生成する．
	// LIMITTERが来るまでhere_docの内容をgnlで読み取り，それをp_fd[1]（パイプの書き込み側．データの一時保存領域）に代入する．
}

int	main(int argc, char **argv, char **envp)
{
	char 	**cmd_absolute_path;

// argv = ft_split("./pipex infile ls cat brew outfile", ' ');
// argc = 6;

	check_arg(argc, argv);
	// コマンドライン引数からcmdの絶対パスを取得する
	get_cmd_absolute_path(&cmd_absolute_path, argc, argv, envp);

	// here_docが指定されていたらhere_docの内容をgnlで読み取り，それをp_fd[1]に書き込む
	proc_here_doc(argv);

	// pipexとしての処理をする
	// pipex(argc, argv, envp, cmd_absolute_path);
	all_free(cmd_absolute_path);
// all_free(argv);
}
