/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: toshota <toshota@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 17:32:48 by toshota           #+#    #+#             */
/*   Updated: 2023/09/17 09:47:59 by toshota          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// gcc main.c ../libft/libft.a && ./a.out
#include "../libft/libft.h"
#include "../pipex.h"

void put_error(char *err_msg)
{
	write(STDERR_FILENO, err_msg, ft_strlen(err_msg));
	exit(1);
}

void check_argc(int argc)
{
	if(argc < 5)
		put_error(TOO_FEW_ARGC_ERROR);
}

// ファイルが適切であるかを確かめる
	// 入力用ファイルは読み取り可能であり，かつ，ディレクトリでないかを確かめる
	// 出力用ファイルは書き込み可能であり，また存在しており，かつ，ディレクトリでないかを確かめる
void check_argv(int argc, char **argv)
{
	char *infile_fd;
	char *outfile_fd;

	infile_fd = argv[1];
	outfile_fd = argv[argc - 1];

}

void check_arg(int argc, char **argv)
{
	check_argc(argc);
	// check_argv(argc, argv);
}

// char **envpによって環境変数を受け取ることができる
// ./pipex infile cmd1 cmd2 outfile
int main(int argc, char **argv, char **envp)
{
	char **bin_path;

	// コマンドライン引数が適切であるかを確かめる
	check_arg(argc, argv);
		// コマンドライン引数の数は5個以上あるかを確かめる
		// ファイルが適切であるかを確かめる
			// 入力用ファイルは読み取り可能であり，かつ，ディレクトリでないかを確かめる
			// 出力用ファイルは書き込み可能であり，かつ，ディレクトリでないかを確かめる
	// 環境変数のポインタenvpからbin_pathを取得する
	// get_bin_path(&bin_path, envp);
	// pipexとしての処理をする
	// pipex(argc, argv, envp, bin_path);
}

/* やる

1．使用可能な関数それぞれの内容を理解する
execve，dup2，pipe，

2．パイプについて理解する


3．パイプを実装する？

access		ファイルに対する実ユーザーでのアクセス権をチェックする
dup
dup2		1（STD_OUT：標準出力）であるfd（第1引数？）を任意のfd（第2引数？）に変える．
execve		プログラムを実行する．たとえば，ls | catでは，execveがlsとcatで2回呼ばれる，第1引数にコマンドの絶対パス（/bin/lsなど），第2引数に実行するコマンド全部（ls -a (null)など），第3引数は環境変数．
exit
fork		子プロセスを生成する
pipe		保存領域を確保する．	パイプ関数が成功したら，引数として渡したpipefd[0]に読み込み用のfdが，pipefd[1]に書き込み用のfdが返される．
unlink		filenameで指定されたファイル（今回の場合ヒアドク<<時に生成したファイル）を削除する．　rm -f と同義
wait		親プロセスで実行するもの．親プロセスに対するwaitは子プロセスの終了を待つために行われる．PIDは指定できない．waitしているものと親子関係にあるPIDのいずれか一つが終了するまで処理を中断する．
waitpid		親プロセスで実行するもの．親プロセスに対するwaitは子プロセスの終了を待つために行われる．PIDを指定できる．指定したPIDが終了するまで処理を中断する．　◀︎━PIDを指定する必要がなければwaitで良い．

a.out──────────────
	 	├fork ━▶︎ execve
		├fork ━▶︎ execve
		├fork ━▶︎ execve
	 	├fork ━▶︎ execve
ヒアドク	<<
$ <<a cat
heredoc> hello		//	ファイルを生成し，コマンドを実行する上での引数（入力内容）をファイルに保存する
heredoc> a			//	<<の後に来る文字と同様の文字が来たらファイルの書き込みを終了する
hello				//	cat ファイルに書き込んだデータ が実行される



 */

/* access ファイルに対する実ユーザーでのアクセス権をチェックする

■プロトタイプ
#include <unistd.h>
int access(const char *filepath, int amode);
＊第1引数　ファイル名
＊第2引数　チェックするモード
			├ R_OK	読み出し許可と，ファイルの存在をチェックする
			├ W_OK	書き込み許可と，ファイルの存在をチェックする
			├ X_OK	実行許可と，ファイルの存在をチェックする
			└ F_OK	ファイルの存在をチェックする

■返り値
0	アクセス権あり
-1	アクセス権なし

 */

/* unlink filenameで指定されたファイル（今回の場合ヒアドク<<時に生成したファイル）を削除する．　rm -f と同義

■プロトタイプ
#include <unistd.h>
int unlink(const char *filename);

■返り値
0	filenameの削除に成功した
-1	filenameの削除に失敗した

 */



/* waitpid	プロセスIDの状態を変化を待つ．
特定の子プロセスを待つ機能を提供し，戻り値のトリガ動作を変更する．
子プロセスが終了した場合に加えて，子プロセスが停止した場合や継続した場合にも返すことができる．
子プロセスが異常終了した場合はstatus_ptrが0以外の値を返す．

■プロトタイプ
#include <sys/wait.h>
pid_t waitpid(pid_t pid, int *status_ptr, int options);


第1引数　呼び出し元が待機する必要がある子プロセス



 */
