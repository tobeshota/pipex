/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/21 10:04:10 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
gcc -g main.c ../libft/libft.a -o pipex

cc -Wall -Wextra -Werror main.c ../libft/libft.a -o pipex
./pipex infile "ls -a" cat ../../a.out outfile
./pipex infile "ls -a" brew /bin/cat ../../a.out fuga/a.out outfile
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

int is_file_exist(char *file)
{
	if (access(file, F_OK))
	{
		put_error(FILE_EXIST_ERROR);
		return FALSE;
	}
	return TRUE;
}

int	is_file_openable(char *file, int file_type)
{
	int	fd;

	if (file_type == INFILE)
		fd = open(file, O_RDONLY);
	else if (file_type == OUTFILE)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	else if (file_type == OUTFILE_HERE_DOC)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
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
	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		return TRUE;
	if (is_file_openable(infile, INFILE) == FALSE)
		return FALSE;
	return TRUE;
}

int	is_outfile_valid(char *infile, char *outfile)
{
	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC && is_file_openable(outfile, OUTFILE_HERE_DOC) == FALSE)
		return FALSE;
	if (infile != INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC && is_file_openable(outfile, OUTFILE) == FALSE)
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
	outfile = argv[argc - 1];
	if (is_infile_valid(infile) == FALSE)
		return FALSE;
	if (is_outfile_valid(infile, outfile) == FALSE)
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

// pathそれぞれの文字列の終わりに"/"を加える．
void add_slash_eos(char ***path)
{
	char *tmp;
	int i;

	i = 0;
	while(path[0][i])
	{
		tmp = path[0][i];
		path[0][i] = ft_strjoin(path[0][i], "/");
		free(tmp);
		i++;
	}
}

int is_cmd_alreadly_absollute_path(char ***cmd_absolute_path, int cmd_i)
{
	// "/"が文頭になく，かつ，"/"が文中にあるならば，それは絶対パスである
	if (cmd_absolute_path[0][cmd_i][0] == '/' && ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return TRUE;
	return FALSE;
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

void	get_pwd_path(char ***pwd_path, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PWD=", ft_strlen("PWD=")))
		i++;
	if (envp[i] == NULL)
		put_error(PATH_EXIST_ERROR);
	*pwd_path = ft_split(envp[i] + ft_strlen("PWD="), ':');
	check_malloc(pwd_path);
	add_slash_eos(pwd_path);
}

// コマンドであるべきコマンドライン引数の数を取得する
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
		ft_printf("argv[%d]:\t%s\t%d\n", i, argv[i], (!access(argv[i], X_OK) || access(argv[i], F_OK)));
		// argv[i]は存在する読み取りファイル，存在する書き込みファイル，存在する実行ファイルのいずれかである．
		// argv[i]が実行可能であるとき，それは実行ファイルである．
		// また，パスが省略されうる（絶対パスや相対パスで指定されるという意味ではない）ファイルは実行ファイルのみである．
		// ゆえに，パスが省略されたために存在していないと評価されたファイルは，実行ファイルでなければならない
		// したがって，argv[i]が実行ファイルであるかを調べるには，それが実行可能であるか，パスが省略されたために存在しないと評価されるかを調べれば良い．
		if (!access(argv[i], X_OK) || access(argv[i], F_OK))
			cmd_count++;
		i++;
	}
	ft_printf("cmd_count:\t%d\n", cmd_count);
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
		if (!access(argv[arg_i], X_OK) || access(argv[arg_i], F_OK))
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

int is_cmd_relative_path(char ***cmd_absolute_path, int cmd_i)
{
	// "/"が文頭になく，かつ，"/"が文中にあるならば，それは相対パスである．
	if (cmd_absolute_path[0][cmd_i][0] != '/' && ft_strchr(&cmd_absolute_path[0][cmd_i][1], '/'))
		return TRUE;
	return FALSE;
}

int get_down_count_from_pwd(char *relative_path)
{
	int down_count_from_pwd;
	down_count_from_pwd = 0;

	while (ft_strnstr(relative_path, "../", ft_strlen(relative_path)))
	{
		down_count_from_pwd++;
		relative_path += ft_strlen("../");
	}
	return down_count_from_pwd;
}

void convert_relative_path_to_absolute_path(char *relative_path, char **envp)
{
	int		down_count_from_pwd;
	char	**pwd_path;
	// PWDを取得する
	get_pwd_path(&pwd_path, envp);

	// PWDから何段下がるかの数を調べる（相対パスにいくつ"../"が含まれるのかを調べる）
	down_count_from_pwd = get_down_count_from_pwd(relative_path);
	// PWDから何段下がるかの数ぶんPWDを変更し，それをpathに加える

	// cmd_absolute_pathにpathを加える

	// PWDをfreeする
	all_free(pwd_path);
}

/* cmdにパスを加える
 * cmd一つ一つに対して，にenv_pathを一つずつ結合していく．
 * 結合したものが実行可能であるならば，それをコマンドの絶対パスとして返す.
 * 実行可能なenv_pathが見つからなければ，エラーとする．
 */
void add_absolute_path_to_cmd_name(char ***cmd_absolute_path, char **env_path, char **envp)
{
	int cmd_i;
	int env_i;
	char *tmp;

	cmd_i = -1;
	while(cmd_absolute_path[0][++cmd_i])
	{
		env_i = 0;
		if (is_cmd_relative_path(cmd_absolute_path, cmd_i))
		{
			// 相対パスから絶対パスに変換する
			convert_relative_path_to_absolute_path(cmd_absolute_path[0][cmd_i], envp);
			continue;
		}
		if (is_cmd_alreadly_absollute_path(cmd_absolute_path, cmd_i))
			continue;
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
	add_absolute_path_to_cmd_name(cmd_absolute_path, env_path, envp);
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

for (int i = 0; cmd_absolute_path[i]; i++)
	ft_printf(">>> %s\n", cmd_absolute_path[i]);

	// here_docが指定されていたらhere_docの内容をgnlで読み取り，それをp_fd[1]に書き込む
	proc_here_doc(argv);

	// pipexとしての処理をする
	// pipex(argc, argv, envp, cmd_absolute_path);
	all_free(cmd_absolute_path);
// all_free(argv);
}
