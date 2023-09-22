/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/22 10:01:44 by toshota          ###   ########.fr       */
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

int is_argc_valid(int argc, char **argv)
{
	if (argc < 5)
	{
		put_error(TOO_FEW_ARGC_ERROR);
		return FALSE;
	}
	if (is_specified_here_doc(argv) && argc < 6)
	{
		put_error(TOO_FEW_ARGC_ERROR_IN_BONUS);
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

int	open_file(char *file, int file_type)
{
	int	fd;

	if (file_type == INFILE)
		fd = open(file, O_RDONLY);
	else if (file_type == INFILE_HERE_DOC)
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
	else if (file_type == OUTFILE)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	else if (file_type == OUTFILE_HERE_DOC)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
	else
		fd = -1;
	if (fd == -1)
	{
		put_error(FILE_OPEN_ERROR);
		exit(1);
	}
	return fd;
}

int	is_infile_valid(char *infile)
{
	int fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(HERE_DOC_FILE_NAME, INFILE_HERE_DOC);
	else
		fd = open_file(infile, INFILE);
	close(fd);
	if (fd == -1)
		return FALSE;
	return TRUE;
}

int	is_outfile_valid(char *infile, char *outfile)
{
	int fd;

	if (infile == INFILE_NOT_SPECIFIED_BECAUSE_OF_HERE_DOC)
		fd = open_file(outfile, OUTFILE_HERE_DOC);
	else
		fd = open_file(outfile, OUTFILE);
	close(fd);
	if(fd == -1)
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

char *get_outfile(int argc, char **argv)
{
	return argv[argc - 1];
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
	// "/"が文頭にあり，かつ，"/"が文中にあるならば，それは絶対パスである
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

void	get_pwd(char ***pwd_path, char **envp)
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
		// argv[i]は存在する読み取りファイル，存在する書き込みファイル，存在する実行ファイルのいずれかである．
		// argv[i]が実行可能であるとき，それは実行ファイルである．
		// また，パスが省略されうる（絶対パスや相対パスで指定されるという意味ではない）ファイルは実行ファイルのみである．
		// ゆえに，パスが省略されたために存在していないと評価されたファイルは，実行ファイルでなければならない
		// したがって，argv[i]が実行ファイルであるかを調べるには，それが実行可能であるか，パスが省略されたために存在しないと評価されるかを調べれば良い．
		if (!access(argv[i], X_OK) || access(argv[i], F_OK))
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

// 文字列 s 中に n 番目に文字 c が現れた位置へのポインターを返す
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

// PWDから何段下がるかの数ぶんPWDを変更する
// 文末からn個'/'が来るまでpwd_pathを消す
char *get_pwd_for_relative_path(char ***pwd_path, int down_count_from_pwd)
{
	int delete_len;

	delete_len = ft_strlen(ft_strrnchr(pwd_path[0][0], '/', down_count_from_pwd) + 1);
	return ft_substr(pwd_path[0][0], 0, ft_strlen(pwd_path[0][0]) - delete_len);
}

void delete_relative_path(char ***cmd_absolute_path, int cmd_i)
{
	char *tmp;
	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strtrim(cmd_absolute_path[0][cmd_i], "../");
	free(tmp);
	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strtrim(cmd_absolute_path[0][cmd_i], "./");
	free(tmp);
}

/* 相対パスから絶対パスに変換する
 * ・PWDを取得する
 * ・PWDを相対パスで指定された段数ぶん変更する
 * ・cmd_absolute_pathから相対パスの部分（"../"や"./"）を削除する
 * ・cmd_absolute_pathにpathを加える
 */
void convert_relative_path_to_absolute_path(char ***cmd_absolute_path, int cmd_i, char **envp)
{
	char	*tmp;
	char	**pwd;
	char	*pwd_for_relative_path;

	get_pwd(&pwd, envp);
	pwd_for_relative_path = get_pwd_for_relative_path(&pwd, get_down_count_from_pwd(cmd_absolute_path[0][cmd_i]));
	delete_relative_path(cmd_absolute_path, cmd_i);
	tmp = cmd_absolute_path[0][cmd_i];
	cmd_absolute_path[0][cmd_i] = ft_strjoin(pwd_for_relative_path, cmd_absolute_path[0][cmd_i]);
	free(tmp);
	all_free(pwd);
	free(pwd_for_relative_path);
}

void add_absolute_path_to_cmd_name_from_env_path(char ***cmd_absolute_path, char **env_path, int cmd_i)
{
	int env_i;
	char *tmp;

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
}

/* cmdにパスを加える
 * cmd一つ一つに対して，にenv_pathを一つずつ結合していく．
 * 結合したものが実行可能であるならば，それをコマンドの絶対パスとして返す.
 * 実行可能なenv_pathが見つからなければ，エラーとする．
 */
void add_absolute_path_to_cmd_name(char ***cmd_absolute_path, char **env_path, char **envp)
{
	int cmd_i;

	cmd_i = -1;
	while(cmd_absolute_path[0][++cmd_i])
	{
		if (is_cmd_relative_path(cmd_absolute_path, cmd_i))
		{
			convert_relative_path_to_absolute_path(cmd_absolute_path, cmd_i, envp);
			continue;
		}
		if (is_cmd_alreadly_absollute_path(cmd_absolute_path, cmd_i))
			continue;
		add_absolute_path_to_cmd_name_from_env_path(cmd_absolute_path, env_path, cmd_i);
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

void proc_here_doc(char *limitter, int infile_fd)
{
	char *line;

	line = get_next_line(STDIN_FILENO);
	while(ft_strncmp(line, limitter, ft_strlen(limitter)))
	{
		ft_putstr_fd(line, infile_fd);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
}


// p_fd[1]を用いるためにpipeを開く．パイプの書き込み側に書き込まれたデータはパイプの読み出し側から読み出されるまでカーネルでバッファリグされる．
	// pipeを用いるためにfork()で現在のプロセス（親プロセス）を複製して新しいプロセス（子プロセス）を生成する．
	// LIMITTERが来るまでhere_docの内容をgnlで読み取り，それをp_fd[1]（パイプの書き込み側．データの一時保存領域）に代入する．
	// here_docファイルをclose();する．
	// here_docファイルをunlinkで消す．




/* ■ファイルおよびコマンドは適切なものであるかを確かめる
 * 	・入力用ファイルは読み取り可能であり，かつ，ディレクトリでないかを確かめる
 * 	・出力用ファイルは書き込み可能であり，かつ，ディレクトリでないかを確かめる
 * 	・コマンドが存在するかを確かめる（command not foundとならないかを調べる）
 */
int	is_argv_valid(int argc, char **argv)
{
	if (is_infile_valid(get_infile(argv)) == FALSE)
		return FALSE;
	if (is_outfile_valid(get_infile(argv), get_outfile(argc, argv)) == FALSE)
		return FALSE;
	return TRUE;
}

void get_infile_fd(int argc, char **argv, int *infile_fd)
{
	if (is_specified_here_doc(argv))
	{
		*infile_fd = open_file(HERE_DOC_FILE_NAME, INFILE_HERE_DOC);
		proc_here_doc(argv[2], *infile_fd);
	}
	else
		*infile_fd = open_file(get_infile(argv), INFILE);
}

void get_outfile_fd(int argc, char **argv, int *outfile_fd)
{
	if (is_specified_here_doc(argv))
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE_HERE_DOC);
	else
		*outfile_fd = open_file(get_outfile(argc, argv), OUTFILE);
}

/* コマンドライン引数が適切であるかを確かめる
 * ・コマンドライン引数の数は5個以上あるかを確かめる
 * ・コマンドライン引数の要素は適切なものであるかを確かめる
 */
void check_arg(int argc, char **argv)
{
	if (is_argc_valid(argc, argv) == FALSE)
		exit(1);
	if (is_argv_valid(argc, argv) == FALSE)
		exit(1);
}

void	get_data(int argc, char **argv, t_data *data, char **envp)
{
	get_infile_fd(argc, argv, &data->infile_fd);
	get_outfile_fd(argc, argv, &data->outfile_fd);
	get_cmd_absolute_path(&data->cmd_absolute_path, argc, argv, envp);
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
	int p_fd[2];

	execve(cmd_absolute_path[1], argv, envp);
	// p_fd[0]およびp_fd[1]を用いるためにpipeを開く．
	// pipeおよびexecveを用いるためにforkで現在のプロセス（親プロセス）を複製して新しいプロセス（子プロセス）を生成する．
	// <>forkの出力値が0より小さい数だったら（子プロセスの生成に失敗したら），エラー終了する．
	// <>forkの出力値が0だったら（子プロセスのpidが渡されたら），子プロセスを実行する．
		// コマンドの入力先を指定する（コマンドの入力先fdを標準入力に変える）．
			// はじめ　infile_fd					を標準入力に変える
			// いつも　p_fd[0]（パイプの読み取り側）	を標準入力に変える
		// コマンドの出力先fdを指定する（コマンドの出力先fdを標準出力に変える）
			// いつも　p_fd[1]（パイプの書き込み側）	を標準出力に変える
			// おわり　outfile_fd					を標準出力に変える
		// execveでコマンドを実行する．
	// <>forkの出力値が0より大きい数だったら（親プロセスのpidが渡されたら），親プロセスを実行する．
}

void end_pipex(char **argv, t_data *data)
{
	all_free(data->cmd_absolute_path);
	if (is_specified_here_doc(argv))
		unlink(HERE_DOC_FILE_NAME);
}

int	main(int argc, char **argv, char **envp)
{
	t_data data;

// argv = ft_split("./pipex infile ls cat brew outfile", ' ');
// argc = 6;

	check_arg(argc, argv);
	get_data(argc, argv, &data, envp);

for (int i = 0; data.cmd_absolute_path[i]; i++)
	ft_printf(">>> %s\n", data.cmd_absolute_path[i]);

	// pipexとしての処理をする
	// pipex(argc, argv, envp, data.cmd_absolute_path);
	// 終了する
	end_pipex(argv, &data);
// all_free(argv);
}
